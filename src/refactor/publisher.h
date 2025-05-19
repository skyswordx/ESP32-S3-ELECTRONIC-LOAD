/**
 * @file publisher.h
 * @brief 话题发布者接口
 * @author skyswordx
 * @date 2025-05-19
 * @details 提供系统的话题发布功能
 */

#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "common.h"
#include <string>

namespace OUR {

/**
 * @brief 话题发布者类
 */
class Publisher {
private:
    std::string topicName;    // 话题名称
    QueueHandle_t queueHandle; // 队列句柄
    size_t maxItemSize;       // 最大消息大小

public:
    /**
     * @brief 构造函数
     * @param topic 话题名称
     * @param handle 队列句柄
     * @param size 最大消息大小
     */
    Publisher(const std::string& topic, QueueHandle_t handle, size_t size)
        : topicName(topic), queueHandle(handle), maxItemSize(size) {}
    
    /**
     * @brief 获取话题名称
     * @return 话题名称
     */
    std::string getTopic() const {
        return topicName;
    }
    
    /**
     * @brief 发布数据
     * @tparam T 数据类型
     * @param msg 要发布的数据
     * @param taskId 任务ID
     * @param dataDesc 数据描述
     * @return 是否发布成功
     */    
    template <typename T>
    bool publish(const T& data, task_id_t taskId = TASK_UNKNOWN, data_description_t dataDesc = DATA_DESCRIPTION_NONE) {
        // 检查队列句柄是否有效
        if (queueHandle == nullptr) {
            Serial.printf("错误: 话题'%s'的队列句柄无效，无法发布消息\n", topicName.c_str());
            return false;
        }

        // 检查消息大小是否超过队列允许的最大大小
        if (sizeof(QueueElement_t<T>) > maxItemSize) {
            Serial.printf("错误: 话题'%s'中消息大小(%u字节)超过队列容量(%u字节)\n", 
                          topicName.c_str(), sizeof(QueueElement_t<T>), maxItemSize);
            return false;
        }
        
        // 创建队列元素
        QueueElement_t<T> element;
        element.task_id = taskId;
        element.data = data;
        element.data_description = dataDesc;
        
        // 从队尾发送消息，超时设置为0表示不阻塞
        BaseType_t result = xQueueSendToBack(queueHandle, &element, 0);
        
        // 检查发送结果
        if (result != pdPASS) {
            UBaseType_t messagesWaiting = uxQueueMessagesWaiting(queueHandle);
            UBaseType_t spacesAvailable = uxQueueSpacesAvailable(queueHandle);
            
            Serial.printf("警告: 话题'%s'发送消息失败 [任务ID:%d, 数据类型:%d]\n", 
                          topicName.c_str(), taskId, dataDesc);
            Serial.printf("队列状态: 已使用:%u, 可用空间:%u\n", 
                          messagesWaiting, spacesAvailable);
            
            // 如果队列已满，提供更明确的提示
            if (spacesAvailable == 0) {
                Serial.println("原因: 队列已满，请考虑增加队列长度或加快消费速度");
            }
            
            return false;
        }
        
        return true;
    }
    
    /**
     * @brief advertise方法作为publish的别名
     * @tparam T 数据类型
     * @param data 要发布的数据
     * @param taskId 任务ID
     * @param dataDesc 数据描述
     * @return 是否发布成功
     */
    template <typename T>
    bool advertise(const T& data, task_id_t taskId = TASK_UNKNOWN, data_description_t dataDesc = DATA_DESCRIPTION_NONE) {
        return publish(data, taskId, dataDesc);
    }
};

/**
 * @brief 创建发布者
 * @param topicName 话题名称
 * @param config 队列配置
 * @return 发布者对象
 */
Publisher createPublisher(const std::string& topicName, const QueueConfig& config = QueueConfig());

} // namespace OUR

#endif // PUBLISHER_H
