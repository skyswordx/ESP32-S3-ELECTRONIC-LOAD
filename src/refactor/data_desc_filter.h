/**
 * @file data_desc_filter.h
 * @brief 基于数据描述的消息过滤器
 * @author AI Assistant
 * @date 2025-05-19
 * @details 提供按数据描述过滤消息并分发到不同子队列的功能
 */

#ifndef DATA_DESC_FILTER_H
#define DATA_DESC_FILTER_H

#include "filter_queue.h"
#include <unordered_map>

namespace OUR {

/**
 * @brief 基于数据描述的消息过滤器
 * @tparam T 消息数据类型
 */
template<typename T>
class DataDescFilter : public FilterQueueBase<T> {
private:
    // 数据描述到子队列名称的映射
    std::unordered_map<data_description_t, std::string> dataDescToQueueMap;
    SemaphoreHandle_t mapMutex;  // 保护映射表的互斥锁
    
    /**
     * @brief 根据数据描述分发消息到对应子队列
     * @param element 接收到的消息
     * @return 是否成功分发
     */
    bool dispatchMessage(const QueueElement_t<T>& element) override {
        if (xSemaphoreTake(mapMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取数据描述过滤器互斥锁");
            return false;
        }
        
        // 查找该数据描述对应的子队列
        auto it = dataDescToQueueMap.find(element.data_description);
        if (it == dataDescToQueueMap.end()) {
            // 没有为该数据描述配置子队列
            xSemaphoreGive(mapMutex);
            return false;
        }
        
        std::string queueName = it->second;
        xSemaphoreGive(mapMutex);
        
        // 获取子队列互斥锁
        if (xSemaphoreTake(this->queuesMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取过滤队列互斥锁");
            return false;
        }
        
        // 获取子队列句柄
        auto queueIt = this->subQueues.find(queueName);
        if (queueIt == this->subQueues.end()) {
            Serial.printf("错误: 子队列'%s'不存在\n", queueName.c_str());
            xSemaphoreGive(this->queuesMutex);
            return false;
        }
        
        QueueHandle_t targetQueue = queueIt->second.handle;
        xSemaphoreGive(this->queuesMutex);
        
        // 将消息发送到目标队列
        if (xQueueSend(targetQueue, &element, 0) != pdPASS) {
            Serial.printf("警告: 无法将消息发送到子队列'%s'\n", queueName.c_str());
            return false;
        }
        
        return true;
    }
    
public:
    /**
     * @brief 构造函数
     * @param topic 源话题名称
     * @param interval 处理间隔(毫秒)
     */
    DataDescFilter(const std::string& topic, uint32_t interval = 10)
        : FilterQueueBase<T>(topic, interval) {
        // 创建互斥锁
        mapMutex = xSemaphoreCreateMutex();
        if (!mapMutex) {
            Serial.println("错误: 无法创建数据描述过滤器互斥锁");
        }
    }
    
    /**
     * @brief 析构函数
     */
    ~DataDescFilter() {
        // 删除互斥锁
        if (mapMutex) {
            vSemaphoreDelete(mapMutex);
            mapMutex = nullptr;
        }
    }
    
    /**
     * @brief 添加数据描述过滤规则和对应的子队列
     * @param dataDesc 要过滤的数据描述
     * @param queueName 子队列名称
     * @param queueSize 队列长度
     * @param maxMsgSize 最大消息大小，默认为sizeof(QueueElement_t<T>)
     * @return 是否成功添加
     */
    bool addFilterQueue(data_description_t dataDesc, const std::string& queueName, UBaseType_t queueSize, size_t maxMsgSize = 0) {
        // 创建子队列
        if (!this->createSubQueue(queueName, queueSize, maxMsgSize)) {
            return false;
        }
        
        // 添加数据描述到队列名称的映射
        if (xSemaphoreTake(mapMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取数据描述过滤器互斥锁");
            return false;
        }
        
        dataDescToQueueMap[dataDesc] = queueName;
        
        xSemaphoreGive(mapMutex);
        
        Serial.printf("已添加数据描述 %d 的过滤规则，对应子队列'%s'\n", dataDesc, queueName.c_str());
        return true;
    }
    
    /**
     * @brief 移除数据描述过滤规则
     * @param dataDesc 要移除的数据描述
     * @return 是否成功移除
     */
    bool removeFilter(data_description_t dataDesc) {
        if (xSemaphoreTake(mapMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取数据描述过滤器互斥锁");
            return false;
        }
        
        auto it = dataDescToQueueMap.find(dataDesc);
        if (it == dataDescToQueueMap.end()) {
            xSemaphoreGive(mapMutex);
            return false;
        }
        
        dataDescToQueueMap.erase(it);
        xSemaphoreGive(mapMutex);
        
        Serial.printf("已移除数据描述 %d 的过滤规则\n", dataDesc);
        return true;
    }
    
    /**
     * @brief 获取为指定数据描述配置的子队列名称
     * @param dataDesc 数据描述
     * @return 子队列名称，如果未配置则返回空字符串
     */
    std::string getQueueForDataDesc(data_description_t dataDesc) {
        if (xSemaphoreTake(mapMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            return "";
        }
        
        auto it = dataDescToQueueMap.find(dataDesc);
        std::string result = (it != dataDescToQueueMap.end()) ? it->second : "";
        
        xSemaphoreGive(mapMutex);
        
        return result;
    }
};

/**
 * @brief 创建基于数据描述的过滤器
 * @tparam T 消息数据类型
 * @param topicName 源话题名称
 * @param interval 处理间隔(毫秒)
 * @param autoStart 是否自动启动
 * @return 过滤器指针
 */
template<typename T>
std::shared_ptr<DataDescFilter<T>> createDataDescFilter(
    const std::string& topicName,
    uint32_t interval = 10,
    bool autoStart = true
) {
    // 创建过滤器
    auto filter = std::make_shared<DataDescFilter<T>>(topicName, interval);
    
    // 初始化
    if (!filter->init()) {
        Serial.printf("错误: 初始化数据描述过滤器失败，源话题'%s'\n", topicName.c_str());
        return nullptr;
    }
    
    // 如果需要自动启动
    if (autoStart) {
        if (!filter->start()) {
            Serial.printf("错误: 启动数据描述过滤器失败，源话题'%s'\n", topicName.c_str());
            return nullptr;
        }
    }
    
    return filter;
}

} // namespace OUR

#endif // DATA_DESC_FILTER_H
