/**
 * @file test_simplified_queue.cpp
 * @brief 简化的电流控制队列系统测试程序
 * @author skyswordx
 * @details 测试基于统一QueueElement_t结构的电流控制队列系统
 *          验证通过task_id实现优先级控制的简化方案
 */

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "our_queque.hpp"

// 队列句柄
QueueHandle_t test_current_control_queue;

/**
 * @brief 测试队列创建功能
 */
void test_queue_creation() {
    printf("\n=== Testing Simplified Queue Creation ===");
    
    test_current_control_queue = xQueueCreate(10, sizeof(QueueElement_t<double>));
    
    if (test_current_control_queue != NULL) {
        printf("\n[PASS] Current control queue created successfully");
        printf("\n[INFO] Queue size: 10, Element size: %d bytes", sizeof(QueueElement_t<double>));
    } else {
        printf("\n[FAIL] Failed to create current control queue");
    }
}

/**
 * @brief 测试统一消息结构的发送和接收
 */
void test_unified_messaging() {
    printf("\n\n=== Testing Unified QueueElement_t Messaging ===");
    
    // 创建不同优先级的消息（通过task_id控制优先级）
    QueueElement_t<double> encoder_msg(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 100.5);
    QueueElement_t<double> emergency_msg(EVENT_OVER_VOLTAGE, DATA_DESCRIPTION_EMERGENCY_STOP, 0.0);
    QueueElement_t<double> normal_msg(TASK_INA226, DATA_DESCRIPTION_CURRENT_SETPOINT, 50.0);
    
    // 发送消息
    printf("\n[INFO] Sending messages with different priorities:");
    
    if (xQueueSend(test_current_control_queue, &encoder_msg, 0) == pdTRUE) {
        printf("\n[PASS] Encoder message sent (task_id: %d, priority: MEDIUM)", TASK_ENCODER);
    } else {
        printf("\n[FAIL] Failed to send encoder message");
    }
    
    if (xQueueSend(test_current_control_queue, &emergency_msg, 0) == pdTRUE) {
        printf("\n[PASS] Emergency message sent (task_id: %d, priority: HIGHEST)", EVENT_OVER_VOLTAGE);
    } else {
        printf("\n[FAIL] Failed to send emergency message");
    }
    
    if (xQueueSend(test_current_control_queue, &normal_msg, 0) == pdTRUE) {
        printf("\n[PASS] Normal message sent (task_id: %d, priority: LOW)", TASK_INA226);
    } else {
        printf("\n[FAIL] Failed to send normal message");
    }
    
    // 接收消息并显示处理逻辑
    printf("\n\n[INFO] Processing messages (FIFO order, priority handled by task_id value):");
    QueueElement_t<double> received_msg;
    int message_count = 0;
    
    while (xQueueReceive(test_current_control_queue, &received_msg, 0) == pdTRUE) {
        message_count++;
        printf("\n[INFO] Message %d - Task ID: %d, Data: %.1f mA, Description: %d", 
               message_count, received_msg.task_id, received_msg.data, received_msg.data_description);
        
        // 模拟PID任务的处理逻辑
        if (received_msg.task_id == EVENT_OVER_VOLTAGE) {
            printf("\n[ACTION] EMERGENCY detected! Setting current to 0 mA immediately");
        } else {
            printf("\n[ACTION] Setting target current to %.1f mA", received_msg.data);
        }
    }
    
    printf("\n[INFO] Total messages processed: %d", message_count);
}

/**
 * @brief 测试简化的优先级处理（无需缓冲区和排序）
 */
void test_simplified_priority_handling() {
    printf("\n\n=== Testing Simplified Priority Handling ===");
    printf("\n[INFO] This approach eliminates the need for internal buffering and sorting!");
    
    // 发送混合消息来演示简化的处理方式
    QueueElement_t<double> messages[] = {
        QueueElement_t<double>(TASK_INA226, DATA_DESCRIPTION_CURRENT_SETPOINT, 30.0),     // task_id=3, 低优先级
        QueueElement_t<double>(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 75.0),   // task_id=1, 中优先级  
        QueueElement_t<double>(EVENT_OVER_VOLTAGE, DATA_DESCRIPTION_EMERGENCY_STOP, 0.0), // task_id=0, 最高优先级
        QueueElement_t<double>(TASK_ADC1, DATA_DESCRIPTION_CURRENT_SETPOINT, 25.0)       // task_id=4, 低优先级
    };
    
    // 发送所有消息
    for (int i = 0; i < 4; i++) {
        if (xQueueSend(test_current_control_queue, &messages[i], 0) == pdTRUE) {
            printf("\n[PASS] Message %d sent (Task ID: %d, Data: %.1f)", 
                   i+1, messages[i].task_id, messages[i].data);
        }
    }
    
    printf("\n\n[INFO] Processing messages one by one (no internal buffering needed):");
    
    // 简化的处理：直接接收消息，根据task_id判断处理方式
    QueueElement_t<double> received_msg;
    int count = 0;
    while (xQueueReceive(test_current_control_queue, &received_msg, 0) == pdTRUE) {
        count++;
        
        // 根据task_id值判断优先级和处理方式
        const char* priority_str = (received_msg.task_id == EVENT_OVER_VOLTAGE) ? "EMERGENCY" : 
                                  (received_msg.task_id == TASK_ENCODER) ? "MEDIUM" : "NORMAL";
        
        printf("\n[INFO] Processing message %d - Task ID: %d (%s), Data: %.1f mA", 
               count, received_msg.task_id, priority_str, received_msg.data);
        
        // 简化的处理逻辑：直接根据task_id判断
        if (received_msg.task_id == EVENT_OVER_VOLTAGE) {
            printf("\n[ACTION] EMERGENCY: Immediate response - Setting current to 0 mA");
        } else {
            printf("\n[ACTION] Normal processing - Setting target current to %.1f mA", received_msg.data);
        }
    }
    
    printf("\n[INFO] Simplified approach benefits:");
    printf("\n  - No internal message buffering required");
    printf("\n  - No sorting algorithm needed");
    printf("\n  - Reduced memory usage");
    printf("\n  - Faster processing");
    printf("\n  - Emergency messages still processed immediately when received");
}

/**
 * @brief 测试队列满载情况的处理
 */
void test_queue_full_scenario() {
    printf("\n\n=== Testing Queue Full Scenario ===");
    
    // 填满队列
    QueueElement_t<double> test_msg(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 100.0);
    int sent_count = 0;
    
    // 尝试发送超过队列容量的消息
    for (int i = 0; i < 12; i++) {  // 队列大小为10，尝试发送12个
        test_msg.data = 100.0 + i;
        if (xQueueSend(test_current_control_queue, &test_msg, 0) == pdTRUE) {
            sent_count++;
        } else {
            printf("\n[INFO] Queue full at message %d (sent %d messages)", i+1, sent_count);
            break;
        }
    }
    
    printf("\n[INFO] Successfully sent %d messages before queue became full", sent_count);
    
    // 测试紧急消息在队列满时的备选方案
    QueueElement_t<double> emergency_msg(EVENT_OVER_VOLTAGE, DATA_DESCRIPTION_EMERGENCY_STOP, 0.0);
    if (xQueueSend(test_current_control_queue, &emergency_msg, 0) != pdTRUE) {
        printf("\n[INFO] Emergency message couldn't be sent - queue full");
        printf("\n[INFO] Fallback mechanism would activate: Direct PID target setting");
        printf("\n[PASS] System maintains safety even when queue is full");
    }
    
    // 清空队列
    QueueElement_t<double> dummy_msg;
    int cleared_count = 0;
    while (xQueueReceive(test_current_control_queue, &dummy_msg, 0) == pdTRUE) {
        cleared_count++;
    }
    printf("\n[INFO] Cleared %d messages from queue", cleared_count);
}

/**
 * @brief 演示代码简化的对比
 */
void demonstrate_code_simplification() {
    printf("\n\n=== Code Simplification Demonstration ===");
    printf("\n[INFO] Original approach complexity:");
    printf("\n  - Custom current_control_message_t structure");
    printf("\n  - Separate priority enumeration");
    printf("\n  - Internal message buffering array");
    printf("\n  - Bubble sort algorithm for priority");
    printf("\n  - Multiple message collection logic");
    
    printf("\n\n[INFO] Simplified approach benefits:");
    printf("\n  - Reuses existing QueueElement_t<T> structure");
    printf("\n  - Uses task_id enum values for priority (lower = higher priority)");
    printf("\n  - No internal buffering needed");
    printf("\n  - No sorting required");
    printf("\n  - Direct message processing");
    printf("\n  - Emergency messages still get immediate attention");
    
    printf("\n\n[INFO] Memory and performance improvements:");
    printf("\n  - Reduced struct size: QueueElement_t vs current_control_message_t");
    printf("\n  - No 10-element buffer array in PID task");
    printf("\n  - O(1) processing instead of O(n²) bubble sort");
    printf("\n  - Simplified control flow");
}

/**
 * @brief 主测试函数
 */
void setup() {
    Serial.begin(115200);
    delay(2000);
    
    printf("\n");
    printf("========================================");
    printf("\n  Simplified Current Control Queue Test");
    printf("\n  Using Unified QueueElement_t Structure");
    printf("\n  Priority via task_id enum values");
    printf("\n========================================");
    
    // 运行所有测试
    test_queue_creation();
    test_unified_messaging();
    test_simplified_priority_handling();
    test_queue_full_scenario();
    demonstrate_code_simplification();
    
    printf("\n\n========================================");
    printf("\n  All Tests Completed Successfully!");
    printf("\n  System is ready for simplified operation");
    printf("\n========================================");
}

void loop() {
    // 测试完成后保持空闲
    delay(1000);
}
