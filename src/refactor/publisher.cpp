/**
 * @file publisher.cpp
 * @brief 话题发布者接口实现
 * @author skyswordx
 * @date 2025-05-19
 * @details 提供系统的话题发布功能实现
 */

#include "publisher.h"

namespace OUR {

/**
 * @brief 创建发布者
 * @param topicName 话题名称
 * @param config 队列配置
 * @return 发布者对象
 */
Publisher createPublisher(const std::string& topicName, const QueueConfig& config) {
    
    if (xSemaphoreTake(g_topicQueuesMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Serial.println("警告: 无法获取话题互斥锁");
        // 假设没有互斥锁，创建一个不可用的发布者
        return Publisher(topicName, nullptr, 0);
    }
    
    // 检查话题是否已经存在
    QueueHandle_t queueHandle = nullptr;
    auto it = g_topicQueues.find(topicName);
    if (it == g_topicQueues.end()) {
        // 话题不存在，创建新队列
        queueHandle = xQueueCreate(config.queueSize, config.maxItemSize);
        if (!queueHandle) {
            Serial.printf("错误: 无法为话题'%s'创建队列\n", topicName.c_str());
            xSemaphoreGive(g_topicQueuesMutex);
            return Publisher(topicName, nullptr, 0);
        }
        
        // 保存队列句柄
        g_topicQueues[topicName] = queueHandle;
    } else {
        // 话题已存在，获取队列句柄
        queueHandle = it->second;
    }
    
    xSemaphoreGive(g_topicQueuesMutex);
    
    // 创建并返回发布者
    return Publisher(topicName, queueHandle, config.maxItemSize);
}


Publisher createPublisher(const std::string& topicName, const QueueConfig& config) {
    if (xSemaphoreTake(g_topicQueuesMutex, pdMS_TO_TICKS(100))) {
        
        QueueHandle_t queueHandle = nullptr;
        
        auto it = g_topicQueues.find(topicName);
        if (it == g_topicQueues.end()) {
            // 话题不存在，创建新队列
            queueHandle = xQueueCreate(config.queueSize, config.maxItemSize);
            if (!queueHandle) {
                Serial.printf("错误: 无法为话题'%s'创建队列\n", topicName.c_str());
                xSemaphoreGive(g_topicQueuesMutex);
                return Publisher(topicName, nullptr, 0);
            }
            
            // 保存队列句柄
            g_topicQueues[topicName] = queueHandle;
        } else {
            // 话题已存在，获取队列句柄
            queueHandle = it->second;
        }


        xSemaphoreGive(g_topicQueuesMutex);
    }
}

} // namespace OUR
