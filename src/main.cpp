/**
 * @file main.cpp
 * @brief RTOS事件驱动系统示例 - 编码器值变化监测
 * @author skyswordx
 * @date 2025-05-07
 * @details 演示如何使用事件驱动型RTOS监听和处理编码器值变化事件
 */

#include <Arduino.h>
#include "core/EventBus.h"
#include "core/TaskManager.h"
#include "devices/EncoderEvent.h"


// 编码器值处理任务
void encoderProcessTask(void* params) {
    // 创建编码器事件监听器
    class EncoderValueListener : public EventListener {
    public:
        bool onEvent(const Event& event) override {
            if (event.getType() == EventType::ENCODER_CHANGED) {
                const auto& encoderEvent = static_cast<const EncoderValueChangedEvent&>(event);
                double value = encoderEvent.getValue();
                Serial.printf("编码器值更新: %.2f\n", value);
                return true;
            }
            return false;
        }
        
        String getListenerId() const override {
            return "EncoderValueListener";
        }
    };
    
    // 创建静态监听器实例并订阅编码器事件
    static EncoderValueListener listener;
    EventBus::getInstance().subscribe(EventType::ENCODER_CHANGED, &listener);
    
    Serial.println("编码器处理任务已启动，等待编码器事件...");
    
    // 任务永久循环
    while (true) {
        // 任务以阻塞方式等待，事件通过EventBus回调处理
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// 编码器模拟任务，模拟编码器值的变化
void encoderSimulationTask(void* params) {
    double encoderValue = 0.0;
    int direction = 1; // 1: 增加, -1: 减少
    
    Serial.println("编码器模拟任务已启动，开始生成编码器值变化事件...");
    
    // 任务永久循环
    while (true) {
        // 更新编码器值
        encoderValue += direction * 1.0;
        
        // 在一定范围内变化
        if (encoderValue > 50.0) {
            direction = -1;
        } else if (encoderValue < 0.0) {
            direction = 1;
        }
        
        // 创建并发布编码器值变化事件
        EncoderValueChangedEvent event(encoderValue);
        bool handled = EventBus::getInstance().publish(event);
        
        if (!handled) {
            Serial.println("警告: 没有监听器处理编码器事件");
        }
        
        // 每秒更新一次编码器值
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// 监控任务，用于显示系统状态
void monitoringTask(void* params) {
    Serial.println("监控任务已启动，将定期显示系统状态...");
    
    while (true) {
        // 获取任务管理器
        TaskManager& taskManager = TaskManager::getInstance();
        
        // 获取所有任务的统计信息
        Serial.println("\n--- 系统状态 ---");
        auto allStats = taskManager.getAllTaskStats();
        for (const auto& stats : allStats) {
            Serial.printf("任务: %s, 状态: %d, 优先级: %d, 栈高水位: %d\n", 
                         stats.name.c_str(), 
                         stats.taskState,
                         stats.priority, 
                         stats.stackHighWater);
        }
        Serial.println("---------------\n");
        
        // 每10秒显示一次系统状态
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

// 主函数
void setup() {
    // 初始化串口通信
    Serial.begin(115200);
    delay(1000); // 等待串口稳定
    Serial.println("\n\n===== 编码器RTOS事件驱动示例 =====");
    
    // 获取任务管理器
    TaskManager& taskManager = TaskManager::getInstance();
    
    // 创建并启动编码器处理任务
    TaskHandle_t encoderProcessHandle = taskManager.startTask(
        "EncoderProcess",     // 任务名称
        encoderProcessTask,   // 任务函数
        nullptr,              // 参数
        4096,                 // 栈大小
        2,                    // 优先级
        1                     // 运行在核心1
    );
    
    // 创建并启动编码器模拟任务
    TaskHandle_t encoderSimulationHandle = taskManager.startTask(
        "EncoderSimulation",  // 任务名称
        encoderSimulationTask,// 任务函数
        nullptr,              // 参数
        4096,                 // 栈大小
        1,                    // 优先级
        0                     // 运行在核心0
    );
    
    // 创建并启动监控任务
    TaskHandle_t monitoringHandle = taskManager.startTask(
        "Monitoring",         // 任务名称
        monitoringTask,       // 任务函数
        nullptr,              // 参数
        4096,                 // 栈大小
        1,                    // 优先级
        0                     // 运行在核心0
    );
    
    // 检查所有任务是否成功启动
    if (encoderProcessHandle && encoderSimulationHandle && monitoringHandle) {
        Serial.println("所有任务已成功启动");
    } else {
        Serial.println("错误: 部分任务启动失败");
    }
    
    Serial.println("系统初始化完成\n");
}

void loop() {
    // 主循环中不需要做任何事情，所有功能都在RTOS任务中运行
    vTaskDelay(pdMS_TO_TICKS(1000));
}