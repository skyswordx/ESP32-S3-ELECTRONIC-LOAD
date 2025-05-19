/**
 * @file our_queue_helpers.hpp
 * @brief 队列操作辅助函数
 * @author AI Assistant
 * @date 2025-05-19
 * @details 提供队列发送和接收的辅助函数，增强错误检测和处理
 */

#ifndef OUR_QUEUE_HELPERS_HPP
#define OUR_QUEUE_HELPERS_HPP

#include <Arduino.h>
#include "our_queque.hpp"

/**
 * @brief 安全发送消息到队列
 * 
 * @tparam T 数据类型
 * @param queue 队列句柄
 * @param element 队列元素
 * @param queueName 队列名称（用于错误提示）
 * @param timeout 超时时间（毫秒，0表示不阻塞）
 * @param logLevel 日志级别（0=关闭日志，1=只记录错误，2=记录错误和警告，3=记录全部）
 * @return true 发送成功
 * @return false 发送失败
 */
template<typename T>
bool safeSendToQueue(QueueHandle_t queue, QueueElement_t<T>& element, 
                    const char* queueName, TickType_t timeout = 0, uint8_t logLevel = 1) {
    // 检查队列是否有效
    if (queue == nullptr) {
        if (logLevel >= 1) {
            Serial.printf("[ERROR] 队列'%s'无效，无法发送消息\n", queueName);
        }
        return false;
    }
    
    // 获取队列状态
    UBaseType_t messagesWaiting = uxQueueMessagesWaiting(queue);
    UBaseType_t spacesAvailable = uxQueueSpacesAvailable(queue);
    
    // 检查队列是否接近满
    if (spacesAvailable <= 2 && logLevel >= 2) {
        Serial.printf("[WARNING] 队列'%s'接近满: 已有%u条消息，仅剩%u个空间\n", 
                     queueName, messagesWaiting, spacesAvailable);
    }
    
    // 发送消息到队列
    BaseType_t result = xQueueSend(queue, (void *)&element, pdMS_TO_TICKS(timeout));
    
    // 检查发送结果
    if (result != pdTRUE) {
        if (logLevel >= 1) {
            Serial.printf("[ERROR] 发送到队列'%s'失败 [任务ID:%d, 数据描述:%d]\n", 
                         queueName, element.task_id, element.data_description);
            
            if (result == errQUEUE_FULL) {
                Serial.printf("       原因: 队列已满 (已有%u条消息)\n", messagesWaiting);
            } else {
                Serial.printf("       错误代码: %d\n", result);
            }
        }
        return false;
    } else if (logLevel >= 3) {
        Serial.printf("[INFO] 成功发送消息到队列'%s' [任务ID:%d, 数据描述:%d]\n", 
                     queueName, element.task_id, element.data_description);
    }
    
    return true;
}

/**
 * @brief 批量发送消息到队列
 * 
 * @tparam T 数据类型
 * @param queue 队列句柄
 * @param elements 队列元素数组
 * @param count 元素数量
 * @param queueName 队列名称（用于错误提示）
 * @param timeout 超时时间（毫秒，0表示不阻塞）
 * @param logLevel 日志级别
 * @return int 成功发送的消息数量
 */
template<typename T>
int batchSendToQueue(QueueHandle_t queue, QueueElement_t<T>* elements, size_t count, 
                    const char* queueName, TickType_t timeout = 0, uint8_t logLevel = 1) {
    int successCount = 0;
    
    // 检查队列是否有效
    if (queue == nullptr) {
        if (logLevel >= 1) {
            Serial.printf("[ERROR] 队列'%s'无效，无法发送批量消息\n", queueName);
        }
        return 0;
    }
    
    // 获取队列状态
    UBaseType_t spacesAvailable = uxQueueSpacesAvailable(queue);
    
    // 检查队列空间是否足够
    if (spacesAvailable < count && logLevel >= 2) {
        Serial.printf("[WARNING] 队列'%s'空间不足: 需要%u个空间，但只有%u个可用\n", 
                     queueName, count, spacesAvailable);
    }
    
    // 发送所有消息
    for (size_t i = 0; i < count; i++) {
        if (safeSendToQueue(queue, elements[i], queueName, timeout, (logLevel > 0) ? 1 : 0)) {
            successCount++;
        }
    }
    
    // 报告结果
    if (logLevel >= 2) {
        if (successCount == count) {
            Serial.printf("[INFO] 成功发送全部%d条消息到队列'%s'\n", count, queueName);
        } else {
            Serial.printf("[WARNING] 批量发送到队列'%s'部分失败: %d/%d条成功\n", 
                         queueName, successCount, count);
        }
    }
    
    return successCount;
}

/**
 * @brief 安全接收队列消息
 * 
 * @tparam T 数据类型
 * @param queue 队列句柄
 * @param element 接收消息的变量
 * @param queueName 队列名称
 * @param timeout 超时时间（毫秒）
 * @param logLevel 日志级别
 * @return true 接收成功
 * @return false 接收失败
 */
template<typename T>
bool safeReceiveFromQueue(QueueHandle_t queue, QueueElement_t<T>& element, 
                         const char* queueName, TickType_t timeout = portMAX_DELAY, uint8_t logLevel = 1) {
    // 检查队列是否有效
    if (queue == nullptr) {
        if (logLevel >= 1) {
            Serial.printf("[ERROR] 接收失败: 队列'%s'无效\n", queueName);
        }
        return false;
    }
    
    // 接收消息
    BaseType_t result = xQueueReceive(queue, &element, pdMS_TO_TICKS(timeout));
    
    // 检查接收结果
    if (result != pdTRUE) {
        if (logLevel >= 1 && timeout > 0) {  // 只在非零超时时记录超时错误
            Serial.printf("[ERROR] 从队列'%s'接收消息超时\n", queueName);
        }
        return false;
    } else if (logLevel >= 3) {
        Serial.printf("[INFO] 成功从队列'%s'接收消息 [任务ID:%d, 数据描述:%d]\n", 
                     queueName, element.task_id, element.data_description);
    }
    
    return true;
}

/**
 * @brief 清空队列
 * 
 * @param queue 队列句柄
 * @param queueName 队列名称
 * @param logLevel 日志级别
 * @return int 清空的消息数量
 */
inline int purgeQueue(QueueHandle_t queue, const char* queueName, uint8_t logLevel = 1) {
    // 检查队列是否有效
    if (queue == nullptr) {
        if (logLevel >= 1) {
            Serial.printf("[ERROR] 无法清空: 队列'%s'无效\n", queueName);
        }
        return 0;
    }
    
    UBaseType_t messagesWaiting = uxQueueMessagesWaiting(queue);
    
    // 使用xQueueReset清空队列
    xQueueReset(queue);
    
    if (logLevel >= 2 && messagesWaiting > 0) {
        Serial.printf("[INFO] 已清空队列'%s', 删除了%u条消息\n", queueName, messagesWaiting);
    }
    
    return messagesWaiting;
}

/**
 * @brief 获取队列使用率
 * 
 * @param queue 队列句柄
 * @return float 使用率（0-1.0）
 */
inline float getQueueUsageRate(QueueHandle_t queue) {
    if (queue == nullptr) {
        return 0.0f;
    }
    
    UBaseType_t messagesWaiting = uxQueueMessagesWaiting(queue);
    UBaseType_t spacesAvailable = uxQueueSpacesAvailable(queue);
    UBaseType_t totalSize = messagesWaiting + spacesAvailable;
    
    if (totalSize == 0) {
        return 0.0f;
    }
    
    return static_cast<float>(messagesWaiting) / totalSize;
}

#endif // OUR_QUEUE_HELPERS_HPP
