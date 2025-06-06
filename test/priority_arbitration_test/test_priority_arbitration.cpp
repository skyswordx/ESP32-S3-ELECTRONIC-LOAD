/**
 * @file test_priority_arbitration.cpp
 * @brief 电流队列优先级仲裁机制测试
 * @author Assistant
 * @details 测试优先级仲裁机制的各种场景，确保系统按照预期工作
 */

#include <stdio.h>
#include <unity.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "our_queque.hpp"

// 模拟全局变量
static priority_state_t current_priority_state = PRIORITY_STATE_NORMAL;
static double last_normal_current = 0.0;
static TickType_t over_voltage_start_time = 0;

// 模拟PID控制器
struct MockPIDController {
    struct {
        double target;
    } process_variable;
} mock_current_ctrl;

// 模拟队列
QueueHandle_t test_current_control_queue;

// 复制核心函数用于测试
bool test_should_accept_message(const QueueElement_t<double>& message) {
    switch (current_priority_state) {
        case PRIORITY_STATE_NORMAL:
            return true;
            
        case PRIORITY_STATE_OVER_VOLTAGE:
            return (message.task_id == EVENT_OVER_VOLTAGE || 
                    message.task_id == EVENT_OVER_VOLTAGE_CLEAR);
            
        case PRIORITY_STATE_TESTING:
            return (message.task_id == EVENT_OVER_VOLTAGE || 
                    message.task_id == EVENT_TESING_LOAD_RATE ||
                    message.task_id == EVENT_TESTING_COMPLETE);
            
        case PRIORITY_STATE_EMERGENCY:
            return (message.task_id == EVENT_OVER_VOLTAGE);
            
        default:
            return false;
    }
}

void test_handle_state_transition(const QueueElement_t<double>& message) {
    switch (message.task_id) {
        case EVENT_OVER_VOLTAGE:
            if (current_priority_state != PRIORITY_STATE_OVER_VOLTAGE) {
                if (current_priority_state == PRIORITY_STATE_NORMAL) {
                    last_normal_current = mock_current_ctrl.process_variable.target;
                }
                current_priority_state = PRIORITY_STATE_OVER_VOLTAGE;
                over_voltage_start_time = xTaskGetTickCount();
                printf("\n[TEST] State transition to OVER_VOLTAGE protection");
            }
            break;
            
        case EVENT_OVER_VOLTAGE_CLEAR:
            if (current_priority_state == PRIORITY_STATE_OVER_VOLTAGE) {
                current_priority_state = PRIORITY_STATE_NORMAL;
                printf("\n[TEST] Over-voltage cleared, returned to NORMAL state");
            }
            break;
            
        case EVENT_TESING_LOAD_RATE:
            if (current_priority_state == PRIORITY_STATE_NORMAL) {
                last_normal_current = mock_current_ctrl.process_variable.target;
                current_priority_state = PRIORITY_STATE_TESTING;
                printf("\n[TEST] State transition to TESTING mode");
            }
            break;
            
        case EVENT_TESTING_COMPLETE:
            if (current_priority_state == PRIORITY_STATE_TESTING) {
                current_priority_state = PRIORITY_STATE_NORMAL;
                printf("\n[TEST] Load testing complete, returned to NORMAL state");
            }
            break;
            
        case TASK_ENCODER:
            if (current_priority_state == PRIORITY_STATE_NORMAL) {
                last_normal_current = message.data;
            }
            break;
            
        default:
            break;
    }
}

void test_update_current_by_state(const QueueElement_t<double>& message) {
    switch (current_priority_state) {
        case PRIORITY_STATE_OVER_VOLTAGE:
            mock_current_ctrl.process_variable.target = 0.0;
            printf("\n[TEST] OVER_VOLTAGE state: forcing current to 0 mA");
            break;
            
        case PRIORITY_STATE_TESTING:
            mock_current_ctrl.process_variable.target = message.data;
            printf("\n[TEST] TESTING state: set current to %.3f mA", message.data);
            break;
            
        case PRIORITY_STATE_NORMAL:
            if (message.task_id == EVENT_OVER_VOLTAGE_CLEAR || 
                message.task_id == EVENT_TESTING_COMPLETE) {
                mock_current_ctrl.process_variable.target = last_normal_current;
                printf("\n[TEST] NORMAL state: restored current to %.3f mA", last_normal_current);
            } else {
                mock_current_ctrl.process_variable.target = message.data;
                last_normal_current = message.data;
                printf("\n[TEST] NORMAL state: set current to %.3f mA", message.data);
            }
            break;
            
        case PRIORITY_STATE_EMERGENCY:
            mock_current_ctrl.process_variable.target = 0.0;
            printf("\n[TEST] EMERGENCY state: forcing current to 0 mA");
            break;
            
        default:
            printf("\n[TEST] Unknown priority state, ignoring message");
            break;
    }
}

/**
 * @brief 测试正常状态下的消息接受
 */
void test_normal_state_accepts_all_messages() {
    printf("\n\n=== Testing Normal State Message Acceptance ===");
    
    current_priority_state = PRIORITY_STATE_NORMAL;
    
    QueueElement_t<double> encoder_msg(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 100.0);
    QueueElement_t<double> testing_msg(EVENT_TESING_LOAD_RATE, DATA_DESCRIPTION_CURRENT_SETPOINT, 200.0);
    QueueElement_t<double> overvoltage_msg(EVENT_OVER_VOLTAGE, DATA_DESCRIPTION_EMERGENCY_STOP, 0.0);
    
    TEST_ASSERT_TRUE(test_should_accept_message(encoder_msg));
    TEST_ASSERT_TRUE(test_should_accept_message(testing_msg));
    TEST_ASSERT_TRUE(test_should_accept_message(overvoltage_msg));
    
    printf("\n[PASS] Normal state accepts all message types");
}

/**
 * @brief 测试过压状态下的消息过滤
 */
void test_overvoltage_state_filters_messages() {
    printf("\n\n=== Testing Over-voltage State Message Filtering ===");
    
    current_priority_state = PRIORITY_STATE_OVER_VOLTAGE;
    
    QueueElement_t<double> encoder_msg(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 100.0);
    QueueElement_t<double> testing_msg(EVENT_TESING_LOAD_RATE, DATA_DESCRIPTION_CURRENT_SETPOINT, 200.0);
    QueueElement_t<double> overvoltage_msg(EVENT_OVER_VOLTAGE, DATA_DESCRIPTION_EMERGENCY_STOP, 0.0);
    QueueElement_t<double> clear_msg(EVENT_OVER_VOLTAGE_CLEAR, DATA_DESCRIPTION_CURRENT_SETPOINT, 0.0);
    
    TEST_ASSERT_FALSE(test_should_accept_message(encoder_msg));
    TEST_ASSERT_FALSE(test_should_accept_message(testing_msg));
    TEST_ASSERT_TRUE(test_should_accept_message(overvoltage_msg));
    TEST_ASSERT_TRUE(test_should_accept_message(clear_msg));
    
    printf("\n[PASS] Over-voltage state correctly filters messages");
}

/**
 * @brief 测试负载测试状态下的消息过滤
 */
void test_testing_state_filters_messages() {
    printf("\n\n=== Testing Load Testing State Message Filtering ===");
    
    current_priority_state = PRIORITY_STATE_TESTING;
    
    QueueElement_t<double> encoder_msg(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 100.0);
    QueueElement_t<double> testing_msg(EVENT_TESING_LOAD_RATE, DATA_DESCRIPTION_CURRENT_SETPOINT, 200.0);
    QueueElement_t<double> overvoltage_msg(EVENT_OVER_VOLTAGE, DATA_DESCRIPTION_EMERGENCY_STOP, 0.0);
    QueueElement_t<double> complete_msg(EVENT_TESTING_COMPLETE, DATA_DESCRIPTION_CURRENT_SETPOINT, 0.0);
    
    TEST_ASSERT_FALSE(test_should_accept_message(encoder_msg));
    TEST_ASSERT_TRUE(test_should_accept_message(testing_msg));
    TEST_ASSERT_TRUE(test_should_accept_message(overvoltage_msg));
    TEST_ASSERT_TRUE(test_should_accept_message(complete_msg));
    
    printf("\n[PASS] Testing state correctly filters messages");
}

/**
 * @brief 测试状态转换逻辑
 */
void test_state_transitions() {
    printf("\n\n=== Testing State Transitions ===");
    
    // 初始状态：正常
    current_priority_state = PRIORITY_STATE_NORMAL;
    mock_current_ctrl.process_variable.target = 150.0;
    
    // 测试 1：正常状态转入负载测试
    QueueElement_t<double> testing_msg(EVENT_TESING_LOAD_RATE, DATA_DESCRIPTION_CURRENT_SETPOINT, 200.0);
    test_handle_state_transition(testing_msg);
    TEST_ASSERT_EQUAL(PRIORITY_STATE_TESTING, current_priority_state);
    TEST_ASSERT_EQUAL_DOUBLE(150.0, last_normal_current);
    
    // 测试 2：负载测试状态下编码器消息被忽略（不改变状态）
    QueueElement_t<double> encoder_msg(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 100.0);
    test_handle_state_transition(encoder_msg);
    TEST_ASSERT_EQUAL(PRIORITY_STATE_TESTING, current_priority_state);
    
    // 测试 3：过压消息可以托管测试状态
    QueueElement_t<double> overvoltage_msg(EVENT_OVER_VOLTAGE, DATA_DESCRIPTION_EMERGENCY_STOP, 0.0);
    test_handle_state_transition(overvoltage_msg);
    TEST_ASSERT_EQUAL(PRIORITY_STATE_OVER_VOLTAGE, current_priority_state);
    
    // 测试 4：过压解除，返回正常状态
    QueueElement_t<double> clear_msg(EVENT_OVER_VOLTAGE_CLEAR, DATA_DESCRIPTION_CURRENT_SETPOINT, 0.0);
    test_handle_state_transition(clear_msg);
    TEST_ASSERT_EQUAL(PRIORITY_STATE_NORMAL, current_priority_state);
    
    printf("\n[PASS] State transitions work correctly");
}

/**
 * @brief 测试电流恢复机制
 */
void test_current_restoration() {
    printf("\n\n=== Testing Current Restoration ===");
    
    // 初始状态：正常状态，电流为150mA
    current_priority_state = PRIORITY_STATE_NORMAL;
    mock_current_ctrl.process_variable.target = 150.0;
    last_normal_current = 150.0;
    
    // 进入测试状态
    QueueElement_t<double> testing_msg(EVENT_TESING_LOAD_RATE, DATA_DESCRIPTION_CURRENT_SETPOINT, 200.0);
    test_handle_state_transition(testing_msg);
    test_update_current_by_state(testing_msg);
    TEST_ASSERT_EQUAL_DOUBLE(200.0, mock_current_ctrl.process_variable.target);
    
    // 测试完成，恢复电流
    QueueElement_t<double> complete_msg(EVENT_TESTING_COMPLETE, DATA_DESCRIPTION_CURRENT_SETPOINT, 0.0);
    test_handle_state_transition(complete_msg);
    test_update_current_by_state(complete_msg);
    TEST_ASSERT_EQUAL(PRIORITY_STATE_NORMAL, current_priority_state);
    TEST_ASSERT_EQUAL_DOUBLE(150.0, mock_current_ctrl.process_variable.target);
    
    printf("\n[PASS] Current restoration works correctly");
}

/**
 * @brief 测试过压优先级
 */
void test_overvoltage_priority() {
    printf("\n\n=== Testing Over-voltage Priority ===");
    
    // 初始状态：负载测试中
    current_priority_state = PRIORITY_STATE_TESTING;
    mock_current_ctrl.process_variable.target = 200.0;
    last_normal_current = 150.0;
    
    // 过压事件，应该立即托管
    QueueElement_t<double> overvoltage_msg(EVENT_OVER_VOLTAGE, DATA_DESCRIPTION_EMERGENCY_STOP, 0.0);
    test_handle_state_transition(overvoltage_msg);
    test_update_current_by_state(overvoltage_msg);
    
    TEST_ASSERT_EQUAL(PRIORITY_STATE_OVER_VOLTAGE, current_priority_state);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, mock_current_ctrl.process_variable.target);
    
    // 在过压状态下，编码器和测试消息应该被拒绝
    QueueElement_t<double> encoder_msg(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, 100.0);
    QueueElement_t<double> testing_msg(EVENT_TESING_LOAD_RATE, DATA_DESCRIPTION_CURRENT_SETPOINT, 300.0);
    
    TEST_ASSERT_FALSE(test_should_accept_message(encoder_msg));
    TEST_ASSERT_FALSE(test_should_accept_message(testing_msg));
    
    printf("\n[PASS] Over-voltage has highest priority");
}

/**
 * @brief 运行所有测试
 */
void run_priority_arbitration_tests() {
    printf("\n\n========== PRIORITY ARBITRATION TESTS ==========");
    
    UNITY_BEGIN();
    
    RUN_TEST(test_normal_state_accepts_all_messages);
    RUN_TEST(test_overvoltage_state_filters_messages);
    RUN_TEST(test_testing_state_filters_messages);
    RUN_TEST(test_state_transitions);
    RUN_TEST(test_current_restoration);
    RUN_TEST(test_overvoltage_priority);
    
    UNITY_END();
    
    printf("\n========== ALL TESTS COMPLETED ==========\n");
}

/**
 * @brief 主测试任务
 */
void test_task(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(1000)); // 等待系统初始化
    
    run_priority_arbitration_tests();
    
    vTaskDelete(NULL);
}

void app_main() {
    printf("\nStarting Priority Arbitration Tests...\n");
    
    // 创建测试队列
    test_current_control_queue = xQueueCreate(10, sizeof(QueueElement_t<double>));
    
    if (test_current_control_queue == NULL) {
        printf("Failed to create test queue\n");
        return;
    }
    
    // 创建测试任务
    xTaskCreate(test_task, "test_task", 4096, NULL, 5, NULL);
    
    printf("Tests started. Check output for results.\n");
}
