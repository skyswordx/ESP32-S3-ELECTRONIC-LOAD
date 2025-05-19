/**
 * @file subscriber.h
 * @brief 话题订阅者接口
 * @author skyswordx
 * @date 2025-05-19
 * @details 提供系统的话题订阅功能
 */

#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "common.h"
#include <functional>

namespace OUR {

/**
 * @brief 话题订阅者类
 * @tparam T 订阅的数据类型
 */
template <typename T>
class Subscriber {
private:
    std::string topicName;       // 话题名称
    QueueHandle_t queueHandle;   // 队列句柄

public:
    /**
     * @brief 构造函数
     * @param topic 话题名称
     * @param handle 队列句柄
     */
    Subscriber(const std::string& topic, QueueHandle_t handle)
        : topicName(topic), queueHandle(handle) {}
    
    /**
     * @brief 析构函数
     */
    ~Subscriber() {}
    
    /**
     * @brief 获取话题名称
     * @return 话题名称
     */
    std::string getTopic() const {
        return topicName;
    }
    
    /**
     * @brief 阻塞式接收一条消息
     * @param element 接收到的队列元素
     * @param timeout 超时时间（毫秒）
     * @return 是否接收成功
     */
    bool receive(QueueElement_t<T>& element, uint32_t timeout = portMAX_DELAY) {
        BaseType_t result = xQueueReceive(queueHandle, &element, pdMS_TO_TICKS(timeout));
        return (result == pdTRUE);
    }
    
    /**
     * @brief 检查指定任务ID的消息
     * @param data 数据保存位置
     * @param taskId 目标任务ID
     * @param timeout 超时时间（毫秒）
     * @return 是否接收成功
     */
    bool receiveWithTaskId(T& data, task_id_t taskId, uint32_t timeout = portMAX_DELAY) {
        QueueElement_t<T> element;
        TickType_t startTime = xTaskGetTickCount();
        TickType_t waitTime = pdMS_TO_TICKS(timeout);
        TickType_t endTime = startTime + waitTime;
        
        while (xTaskGetTickCount() < endTime || timeout == portMAX_DELAY) {
            TickType_t remainingTime = (endTime > xTaskGetTickCount()) ? 
                                     (endTime - xTaskGetTickCount()) : 0;
            
            if (timeout == portMAX_DELAY) {
                remainingTime = portMAX_DELAY;
            }
            
            if (xQueueReceive(queueHandle, &element, remainingTime) == pdTRUE) {
                if (element.task_id == taskId) {
                    data = element.data;
                    return true;
                }
                
                // 不是目标任务ID，继续等待
                continue;
            }
            
            // 超时退出
            break;
        }
        
        return false;
    }
    
    /**
     * @brief 检查指定数据描述的消息
     * @param data 数据保存位置
     * @param dataDesc 目标数据描述
     * @param timeout 超时时间（毫秒）
     * @return 是否接收成功
     */
    bool receiveWithDataDesc(T& data, data_description_t dataDesc, uint32_t timeout = portMAX_DELAY) {
        QueueElement_t<T> element;
        TickType_t startTime = xTaskGetTickCount();
        TickType_t waitTime = pdMS_TO_TICKS(timeout);
        TickType_t endTime = startTime + waitTime;
        
        while (xTaskGetTickCount() < endTime || timeout == portMAX_DELAY) {
            TickType_t remainingTime = (endTime > xTaskGetTickCount()) ? 
                                     (endTime - xTaskGetTickCount()) : 0;
            
            if (timeout == portMAX_DELAY) {
                remainingTime = portMAX_DELAY;
            }
            
            if (xQueueReceive(queueHandle, &element, remainingTime) == pdTRUE) {
                if (element.data_description == dataDesc) {
                    data = element.data;
                    return true;
                }
                
                // 不是目标数据描述，继续等待
                continue;
            }
            
            // 超时退出
            break;
        }
        
        return false;
    }
};

/**
 * @brief 创建订阅者
 * @tparam T 订阅的数据类型
 * @param topicName 话题名称
 * @return 订阅者对象指针
 */
template <typename T>
std::shared_ptr<Subscriber<T>> createSubscriber(const std::string& topicName) {
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(g_topicQueuesMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Serial.println("警告: 无法获取话题互斥锁");
        return nullptr;
    }
    
    // 检查话题是否存在
    QueueHandle_t queueHandle = nullptr;
    auto it = g_topicQueues.find(topicName);
    if (it == g_topicQueues.end()) {
        Serial.printf("错误: 话题'%s'不存在，无法订阅\n", topicName.c_str());
        xSemaphoreGive(g_topicQueuesMutex);
        return nullptr;
    }
    
    queueHandle = it->second;
    xSemaphoreGive(g_topicQueuesMutex);
    
    // 创建订阅者
    auto subscriber = std::make_shared<Subscriber<T>>(topicName, queueHandle);
    
    return subscriber;
}

} // namespace OUR

#endif // SUBSCRIBER_H
