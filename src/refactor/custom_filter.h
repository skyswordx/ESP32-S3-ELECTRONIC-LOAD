/**
 * @file custom_filter.h
 * @brief 自定义过滤逻辑的消息过滤器
 * @author AI Assistant
 * @date 2025-05-19
 * @details 提供基于自定义函数的消息过滤和分发功能
 */

#ifndef CUSTOM_FILTER_H
#define CUSTOM_FILTER_H

#include "filter_queue.h"
#include <functional>
#include <unordered_map>
#include <string>

namespace OUR {

/**
 * @brief 自定义过滤器
 * @tparam T 消息数据类型
 */
template<typename T>
class CustomFilter : public FilterQueueBase<T> {
public:
    // 过滤函数类型，接收消息并返回是否匹配
    using FilterFunction = std::function<bool(const QueueElement_t<T>&)>;
    
private:
    // 过滤条件到子队列名称的映射
    struct FilterInfo {
        std::string queueName;      // 子队列名称
        FilterFunction filterFunc;  // 过滤函数
    };
    
    std::vector<FilterInfo> filters;    // 过滤器列表
    SemaphoreHandle_t filtersMutex;     // 保护过滤器列表的互斥锁
    
    /**
     * @brief 根据自定义过滤函数分发消息到对应子队列
     * @param element 接收到的消息
     * @return 是否成功分发
     */
    bool dispatchMessage(const QueueElement_t<T>& element) override {
        if (xSemaphoreTake(filtersMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取自定义过滤器互斥锁");
            return false;
        }
        
        // 检查每个过滤条件
        bool dispatched = false;
        for (const auto& filter : filters) {
            // 如果消息匹配过滤条件
            if (filter.filterFunc(element)) {
                dispatched = true;
                
                // 获取子队列互斥锁
                if (xSemaphoreTake(this->queuesMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
                    Serial.println("警告: 无法获取过滤队列互斥锁");
                    continue;
                }
                
                // 获取子队列句柄
                auto queueIt = this->subQueues.find(filter.queueName);
                if (queueIt == this->subQueues.end()) {
                    Serial.printf("错误: 子队列'%s'不存在\n", filter.queueName.c_str());
                    xSemaphoreGive(this->queuesMutex);
                    continue;
                }
                
                QueueHandle_t targetQueue = queueIt->second.handle;
                xSemaphoreGive(this->queuesMutex);
                
                // 将消息发送到目标队列
                if (xQueueSend(targetQueue, &element, 0) != pdPASS) {
                    Serial.printf("警告: 无法将消息发送到子队列'%s'\n", filter.queueName.c_str());
                    continue;
                }
            }
        }
        
        xSemaphoreGive(filtersMutex);
        return dispatched;
    }
    
public:
    /**
     * @brief 构造函数
     * @param topic 源话题名称
     * @param interval 处理间隔(毫秒)
     */
    CustomFilter(const std::string& topic, uint32_t interval = 10)
        : FilterQueueBase<T>(topic, interval) {
        // 创建互斥锁
        filtersMutex = xSemaphoreCreateMutex();
        if (!filtersMutex) {
            Serial.println("错误: 无法创建自定义过滤器互斥锁");
        }
    }
    
    /**
     * @brief 析构函数
     */
    ~CustomFilter() {
        // 删除互斥锁
        if (filtersMutex) {
            vSemaphoreDelete(filtersMutex);
            filtersMutex = nullptr;
        }
    }
    
    /**
     * @brief 添加自定义过滤条件和对应的子队列
     * @param filterFunc 过滤函数
     * @param queueName 子队列名称
     * @param queueSize 队列长度
     * @param maxMsgSize 最大消息大小，默认为sizeof(QueueElement_t<T>)
     * @return 是否成功添加
     */
    bool addFilterQueue(FilterFunction filterFunc, const std::string& queueName, UBaseType_t queueSize, size_t maxMsgSize = 0) {
        // 创建子队列
        if (!this->createSubQueue(queueName, queueSize, maxMsgSize)) {
            return false;
        }
        
        // 添加过滤条件
        if (xSemaphoreTake(filtersMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取自定义过滤器互斥锁");
            return false;
        }
        
        FilterInfo info;
        info.queueName = queueName;
        info.filterFunc = filterFunc;
        filters.push_back(info);
        
        xSemaphoreGive(filtersMutex);
        
        Serial.printf("已添加自定义过滤规则，对应子队列'%s'\n", queueName.c_str());
        return true;
    }
    
    /**
     * @brief 移除指定子队列的过滤条件
     * @param queueName 子队列名称
     * @return 是否成功移除
     */
    bool removeFilter(const std::string& queueName) {
        if (xSemaphoreTake(filtersMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取自定义过滤器互斥锁");
            return false;
        }
        
        bool found = false;
        for (auto it = filters.begin(); it != filters.end(); ) {
            if (it->queueName == queueName) {
                it = filters.erase(it);
                found = true;
            } else {
                ++it;
            }
        }
        
        xSemaphoreGive(filtersMutex);
        
        if (found) {
            Serial.printf("已移除子队列'%s'的过滤规则\n", queueName.c_str());
        }
        
        return found;
    }
    
    /**
     * @brief 获取过滤条件数量
     * @return 过滤条件数量
     */
    size_t getFilterCount() const {
        if (xSemaphoreTake(filtersMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            return 0;
        }
        
        size_t count = filters.size();
        xSemaphoreGive(filtersMutex);
        
        return count;
    }
};

/**
 * @brief 创建自定义过滤器
 * @tparam T 消息数据类型
 * @param topicName 源话题名称
 * @param interval 处理间隔(毫秒)
 * @param autoStart 是否自动启动
 * @return 过滤器指针
 */
template<typename T>
std::shared_ptr<CustomFilter<T>> createCustomFilter(
    const std::string& topicName,
    uint32_t interval = 10,
    bool autoStart = true
) {
    // 创建过滤器
    auto filter = std::make_shared<CustomFilter<T>>(topicName, interval);
    
    // 初始化
    if (!filter->init()) {
        Serial.printf("错误: 初始化自定义过滤器失败，源话题'%s'\n", topicName.c_str());
        return nullptr;
    }
    
    // 如果需要自动启动
    if (autoStart) {
        if (!filter->start()) {
            Serial.printf("错误: 启动自定义过滤器失败，源话题'%s'\n", topicName.c_str());
            return nullptr;
        }
    }
    
    return filter;
}

} // namespace OUR

#endif // CUSTOM_FILTER_H
