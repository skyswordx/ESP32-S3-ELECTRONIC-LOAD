/**
 * @file task_id_filter.h
 * @brief 基于任务ID的消息过滤器
 * @author AI Assistant
 * @date 2025-05-19
 * @details 提供按任务ID过滤消息并分发到不同子队列的功能
 */

#ifndef TASK_ID_FILTER_H
#define TASK_ID_FILTER_H

#include "filter_queue.h"
#include <unordered_map>

namespace OUR {

/**
 * @brief 基于任务ID的消息过滤器
 * @tparam T 消息数据类型
 */
template<typename T>
class TaskIdFilter : public FilterQueueBase<T> {
private:
    // 任务ID到子队列名称的映射
    std::unordered_map<task_id_t, std::string> taskIdToQueueMap;
    SemaphoreHandle_t mapMutex;  // 保护映射表的互斥锁
    
    /**
     * @brief 根据任务ID分发消息到对应子队列
     * @param element 接收到的消息
     * @return 是否成功分发
     */
    bool dispatchMessage(const QueueElement_t<T>& element) override {
        if (xSemaphoreTake(mapMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取任务ID过滤器互斥锁");
            return false;
        }
        
        // 查找该任务ID对应的子队列
        auto it = taskIdToQueueMap.find(element.task_id);
        if (it == taskIdToQueueMap.end()) {
            // 没有为该任务ID配置子队列
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
    TaskIdFilter(const std::string& topic, uint32_t interval = 10)
        : FilterQueueBase<T>(topic, interval) {
        // 创建互斥锁
        mapMutex = xSemaphoreCreateMutex();
        if (!mapMutex) {
            Serial.println("错误: 无法创建任务ID过滤器互斥锁");
        }
    }
    
    /**
     * @brief 析构函数
     */
    ~TaskIdFilter() {
        // 删除互斥锁
        if (mapMutex) {
            vSemaphoreDelete(mapMutex);
            mapMutex = nullptr;
        }
    }
    
    /**
     * @brief 添加任务ID过滤规则和对应的子队列
     * @param taskId 要过滤的任务ID
     * @param queueName 子队列名称
     * @param queueSize 队列长度
     * @param maxMsgSize 最大消息大小，默认为sizeof(QueueElement_t<T>)
     * @return 是否成功添加
     */
    bool addFilterQueue(task_id_t taskId, const std::string& queueName, UBaseType_t queueSize, size_t maxMsgSize = 0) {
        // 创建子队列
        if (!this->createSubQueue(queueName, queueSize, maxMsgSize)) {
            return false;
        }
        
        // 添加任务ID到队列名称的映射
        if (xSemaphoreTake(mapMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取任务ID过滤器互斥锁");
            return false;
        }
        
        taskIdToQueueMap[taskId] = queueName;
        
        xSemaphoreGive(mapMutex);
        
        Serial.printf("已添加任务ID %d 的过滤规则，对应子队列'%s'\n", taskId, queueName.c_str());
        return true;
    }
    
    /**
     * @brief 移除任务ID过滤规则
     * @param taskId 要移除的任务ID
     * @return 是否成功移除
     */
    bool removeFilter(task_id_t taskId) {
        if (xSemaphoreTake(mapMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取任务ID过滤器互斥锁");
            return false;
        }
        
        auto it = taskIdToQueueMap.find(taskId);
        if (it == taskIdToQueueMap.end()) {
            xSemaphoreGive(mapMutex);
            return false;
        }
        
        taskIdToQueueMap.erase(it);
        xSemaphoreGive(mapMutex);
        
        Serial.printf("已移除任务ID %d 的过滤规则\n", taskId);
        return true;
    }
    
    /**
     * @brief 获取为指定任务ID配置的子队列名称
     * @param taskId 任务ID
     * @return 子队列名称，如果未配置则返回空字符串
     */
    std::string getQueueForTaskId(task_id_t taskId) {
        if (xSemaphoreTake(mapMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            return "";
        }
        
        auto it = taskIdToQueueMap.find(taskId);
        std::string result = (it != taskIdToQueueMap.end()) ? it->second : "";
        
        xSemaphoreGive(mapMutex);
        
        return result;
    }
};

/**
 * @brief 创建基于任务ID的过滤器
 * @tparam T 消息数据类型
 * @param topicName 源话题名称
 * @param interval 处理间隔(毫秒)
 * @param autoStart 是否自动启动
 * @return 过滤器指针
 */
template<typename T>
std::shared_ptr<TaskIdFilter<T>> createTaskIdFilter(
    const std::string& topicName,
    uint32_t interval = 10,
    bool autoStart = true
) {
    // 创建过滤器
    auto filter = std::make_shared<TaskIdFilter<T>>(topicName, interval);
    
    // 初始化
    if (!filter->init()) {
        Serial.printf("错误: 初始化任务ID过滤器失败，源话题'%s'\n", topicName.c_str());
        return nullptr;
    }
    
    // 如果需要自动启动
    if (autoStart) {
        if (!filter->start()) {
            Serial.printf("错误: 启动任务ID过滤器失败，源话题'%s'\n", topicName.c_str());
            return nullptr;
        }
    }
    
    return filter;
}

} // namespace OUR

#endif // TASK_ID_FILTER_H
