/**
 * @file refactor_test.cpp
 * @brief 重构后的系统测试程序
 * @author Triwalt
 * @date 2025-05-16
 * @details 用于测试重构后的系统功能
 */

#include <Arduino.h>
#include "src/core/TaskManager.h"
#include "src/devices/DeviceManager.h"
#include "src/devices/Encoder.h"
#include "src/devices/Button.h"
#include "src/devices/INA226Device.h"
#include "src/devices/MCP4725Device.h"
#include "src/controllers/CurrentController.h"
#include "src/controllers/PIDControlStrategy.h"
#include "src/tasks/EncoderTask.h"
#include "src/tasks/ButtonTask.h"
#include "src/tasks/DisplayTask.h"
#include "src/tasks/CurrentControlTask.h"

// 引脚定义
#define ENCODER_PIN_A 3
#define ENCODER_PIN_B 4
#define ENCODER_BUTTON_PIN 5

#define BUTTON1_PIN 6
#define BUTTON2_PIN 7

#define INA226_ADDRESS 0x40
#define MCP4725_ADDRESS 0x60

// 全局变量
TaskManager& taskManager = TaskManager::getInstance();
DeviceManager& deviceManager = DeviceManager::getInstance();

// 设备实例
Encoder* encoder = nullptr;
Button* button1 = nullptr;
Button* button2 = nullptr;
INA226Device* currentSensor = nullptr;
MCP4725Device* dacDevice = nullptr;

// 控制器
CurrentController* currentController = nullptr;

// 任务实例
EncoderTask* encoderTask = nullptr;
ButtonTask* buttonTask = nullptr;
DisplayTask* displayTask = nullptr;
CurrentControlTask* currentControlTask = nullptr;

/**
 * @brief 初始化系统
 */
void setup() {
    // 初始化串口
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n--- ESP32-S3 Electronic Load (Refactored) ---");
    
    // 初始化I2C
    Wire.begin();
    
    // 创建设备
    encoder = new Encoder("MainEncoder", ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_BUTTON_PIN, 0, 5000, 10, 0);
    button1 = new Button("Button1", BUTTON1_PIN);
    button2 = new Button("Button2", BUTTON2_PIN);
    currentSensor = new INA226Device("CurrentSensor", INA226_ADDRESS, 0.02);
    dacDevice = new MCP4725Device("DAC", MCP4725_ADDRESS, 5.0);
    
    // 添加设备到设备管理器
    deviceManager.addDevice(encoder);
    deviceManager.addDevice(button1);
    deviceManager.addDevice(button2);
    deviceManager.addDevice(currentSensor);
    deviceManager.addDevice(dacDevice);
    
    // 初始化控制策略和控制器
    ControlStrategy* pidStrategy = new PIDControlStrategy(0.5, 0.2, 0.1); // Kp, Ki, Kd
    currentController = new CurrentController("CurrentController", pidStrategy);
    
    // 创建任务
    encoderTask = new EncoderTask("EncoderTask");
    encoderTask->addEncoder(encoder);
    
    buttonTask = new ButtonTask("ButtonTask");
    buttonTask->addButton(button1);
    buttonTask->addButton(button2);
    
    displayTask = new DisplayTask("DisplayTask");
    
    currentControlTask = new CurrentControlTask("CurrentControlTask");
    currentControlTask->setCurrentController(currentController);
    currentControlTask->setCurrentSensor(currentSensor);
    currentControlTask->setDAC(dacDevice);
    currentControlTask->setTargetCurrent(0);
    currentControlTask->setEnabled(false);
    
    // 启动任务
    taskManager.startTask(encoderTask);
    taskManager.startTask(buttonTask);
    taskManager.startTask(displayTask);
    taskManager.startTask(currentControlTask);
    
    Serial.println("System initialization completed");
}

/**
 * @brief 主循环（不需要做什么，任务已在RTOS中运行）
 */
void loop() {
    delay(1000);
    
    // 打印一些系统信息
    Serial.println("\n--- System Status ---");
    
    // 打印编码器值
    Serial.printf("Encoder: %.2f\n", encoder->getValue());
    
    // 打印按钮状态
    Serial.printf("Button1: %s\n", button1->isPressed() ? "Pressed" : "Released");
    Serial.printf("Button2: %s\n", button2->isPressed() ? "Released" : "Pressed");
    
    // 打印传感器值
    Serial.printf("Current: %.2f mA\n", currentSensor->getCurrent());
    Serial.printf("Voltage: %.2f V\n", currentSensor->getVoltage());
    Serial.printf("Power: %.2f W\n", currentSensor->getPower());
    
    // 打印控制器状态
    Serial.printf("Current Control: %s\n", currentControlTask->isEnabled() ? "Enabled" : "Disabled");
    Serial.printf("Target Current: %.2f mA\n", currentControlTask->getTargetCurrent());
    Serial.printf("Actual Current: %.2f mA\n", currentControlTask->getActualCurrent());
    
    // 打印任务状态
    std::vector<TaskStats> taskStats = taskManager.getAllTaskStats();
    Serial.println("Running Tasks:");
    for (const auto& stats : taskStats) {
        Serial.printf("  - %s: Pri=%d, Stack=%d\n", 
                     stats.name.c_str(), stats.priority, stats.stackHighWater);
    }
}
