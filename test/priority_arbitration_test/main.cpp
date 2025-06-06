/**
 * @file main.cpp
 * @brief 电流队列优先级仲裁机制测试程序
 * @author GitHub Copilot
 * @details 测试统一的优先级仲裁机制，验证不同优先级消息的处理逻辑
 */

#include <Arduino.h>
#include "our_tasks_config.hpp"

void setup() {
    Serial.begin(115200);
    printf("\n[Priority Arbitration Test] Starting...");
    
    // 初始化队列
    current_control_queue = xQueueCreate(10, sizeof(QueueElement_t<double>));
    if (current_control_queue == NULL) {
        printf("\n[Test] Failed to create current_control_queue");
        return;
    }
    
    // 创建电流控制任务
    xTaskCreatePinnedToCore(
        set_current_task,
        "set_current_task", 
        4096,
        NULL,
        3,  // 高优先级
        NULL,
        1   // 核心1
    );
    
    printf("\n[Test] Priority arbitration test setup completed");
}

void loop() {
    static int test_phase = 0;
    static TickType_t last_test_time = 0;
    TickType_t current_time = xTaskGetTickCount();
    
    // 每2秒执行一个测试阶段
    if (current_time - last_test_time > pdMS_TO_TICKS(2000)) {
        last_test_time = current_time;
        
        printf("\n\n[Test] ========== Phase %d ==========", test_phase);
        printf("\n[Test] Current priority state: %d", get_current_priority_state());
        
        switch (test_phase) {
            case 0: {
                // 测试正常状态下的编码器消息
                printf("\n[Test] Testing normal encoder message");
                QueueElement_t<double> encoder_msg(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 500.0);
                xQueueSend(current_control_queue, &encoder_msg, 0);
                break;
            }
            
            case 1: {
                // 测试INA226消息
                printf("\n[Test] Testing INA226 message");
                QueueElement_t<double> ina_msg(TASK_INA226, DATA_DESCRIPTION_CURRENT_SETPOINT, 800.0);
                xQueueSend(current_control_queue, &ina_msg, 0);
                break;
            }
            
            case 2: {
                // 测试过压保护消息（应该立即处理）
                printf("\n[Test] Testing over-voltage protection message");
                QueueElement_t<double> ovp_msg(EVENT_OVER_VOLTAGE, DATA_DESCRIPTION_EMERGENCY_STOP, 0.0);
                xQueueSend(current_control_queue, &ovp_msg, 0);
                break;
            }
            
            case 3: {
                // 在过压状态下测试编码器消息（应该被拒绝）
                printf("\n[Test] Testing encoder message during over-voltage state (should be rejected)");
                QueueElement_t<double> encoder_msg(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 600.0);
                xQueueSend(current_control_queue, &encoder_msg, 0);
                break;
            }
            
            case 4: {
                // 测试负载测试消息
                printf("\n[Test] Testing load rate test message");
                QueueElement_t<double> test_msg(EVENT_TESING_LOAD_RATE, DATA_DESCRIPTION_CURRENT_SETPOINT, 300.0);
                xQueueSend(current_control_queue, &test_msg, 0);
                break;
            }
            
            case 5: {
                // 强制退出过压状态，测试状态恢复
                printf("\n[Test] Force exit over-voltage state");
                force_priority_state(PRIORITY_STATE_NORMAL);
                break;
            }
            
            case 6: {
                // 测试状态恢复后的编码器消息
                printf("\n[Test] Testing encoder message after state recovery");
                QueueElement_t<double> encoder_msg(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 700.0);
                xQueueSend(current_control_queue, &encoder_msg, 0);
                break;
            }
            
            case 7: {
                // 测试负载测试状态
                printf("\n[Test] Testing load test state transition");
                QueueElement_t<double> load_test_msg(EVENT_TESING_LOAD_RATE, DATA_DESCRIPTION_CURRENT_SETPOINT, 400.0);
                xQueueSend(current_control_queue, &load_test_msg, 0);
                break;
            }
            
            case 8: {
                // 在负载测试状态下测试编码器消息（应该被拒绝）
                printf("\n[Test] Testing encoder message during load test state (should be rejected)");
                QueueElement_t<double> encoder_msg(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 900.0);
                xQueueSend(current_control_queue, &encoder_msg, 0);
                break;
            }
            
            case 9: {
                // 用编码器消息退出负载测试状态
                printf("\n[Test] Exit load test state with encoder message");
                force_priority_state(PRIORITY_STATE_NORMAL);
                QueueElement_t<double> encoder_msg(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 1000.0);
                xQueueSend(current_control_queue, &encoder_msg, 0);
                break;
            }
            
            default: {
                // 测试完成，重新开始
                printf("\n[Test] All tests completed, restarting...");
                test_phase = -1; // 下一次循环将变为0
                break;
            }
        }
        
        test_phase++;
    }
    
    // 短暂延时
    vTaskDelay(pdMS_TO_TICKS(100));
}
