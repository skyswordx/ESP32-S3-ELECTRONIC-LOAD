/**
 * @file CurrentControlTask.cpp
 * @brief 电流控制任务类实现
 * @author Triwalt
 * @date 2025-05-16
 * @details 实现处理电流控制的任务
 */

#include "CurrentControlTask.h"
#include "../core/EventBus.h"

// 定义一个电流控制状态事件类
class CurrentControlEvent : public Event {
public:
    CurrentControlEvent(double targetCurrent, double actualCurrent, double outputValue, bool isEnabled)
        : Event(EventType::CONTROL),
          targetCurrent_(targetCurrent),
          actualCurrent_(actualCurrent),
          outputValue_(outputValue),
          isEnabled_(isEnabled) {}
    
    double getTargetCurrent() const { return targetCurrent_; }
    double getActualCurrent() const { return actualCurrent_; }
    double getOutputValue() const { return outputValue_; }
    bool isEnabled() const { return isEnabled_; }
    
private:
    double targetCurrent_;    // 目标电流
    double actualCurrent_;    // 实际电流
    double outputValue_;      // 输出值
    bool isEnabled_;          // 是否启用
};

CurrentControlTask::CurrentControlTask(const String& name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId)
    : Task(name, stackSize, priority, coreId),
      controller_(nullptr),
      currentSensor_(nullptr),
      dac_(nullptr),
      targetCurrent_(0),
      actualCurrent_(0),
      lastDacValue_(0),
      enabled_(false),
      lastControlTime_(0)
{
}

CurrentControlTask::~CurrentControlTask() {
    // 取消事件监听
    EventBus::getInstance().unregisterListener(this);
}

void CurrentControlTask::setCurrentController(CurrentController* controller) {
    controller_ = controller;
}

void CurrentControlTask::setCurrentSensor(INA226Device* sensor) {
    currentSensor_ = sensor;
}

void CurrentControlTask::setDAC(MCP4725Device* dac) {
    dac_ = dac;
}

void CurrentControlTask::setTargetCurrent(double current) {
    // 设置新的目标电流
    targetCurrent_ = current;
    
    // 如果有控制器，更新控制器目标值
    if (controller_ != nullptr) {
        controller_->setSetpoint(targetCurrent_);
    }
}

double CurrentControlTask::getTargetCurrent() const {
    return targetCurrent_;
}

double CurrentControlTask::getActualCurrent() const {
    return actualCurrent_;
}

void CurrentControlTask::setEnabled(bool enabled) {
    // 检查状态是否改变
    if (enabled_ != enabled) {
        enabled_ = enabled;
        
        // 如果禁用，将输出设置为0
        if (!enabled_ && dac_ != nullptr) {
            dac_->setValue(0);
            lastDacValue_ = 0;
        }
    }
}

bool CurrentControlTask::isEnabled() const {
    return enabled_;
}

void CurrentControlTask::onEvent(const Event& event) {
    // 这里可以处理与电流控制相关的事件
    // 例如，处理来自UI的目标电流更改事件等
}

void CurrentControlTask::run() {
    // 注册事件监听
    registerEventListeners();
    
    // 控制循环
    unsigned long controlInterval = 1; // 控制间隔(ms)
    
    // 检查所有必需的组件是否可用
    if (controller_ == nullptr || currentSensor_ == nullptr || dac_ == nullptr) {
        Serial.println("[CurrentControlTask] Error: Missing required components. Task cannot run.");
        return;
    }
    
    // 初始化设备
    if (!currentSensor_->isInitialized() && !currentSensor_->init()) {
        Serial.println("[CurrentControlTask] Error: Failed to initialize current sensor");
        return;
    }
    
    if (!dac_->isInitialized() && !dac_->init()) {
        Serial.println("[CurrentControlTask] Error: Failed to initialize DAC");
        return;
    }
    
    // 初始化控制器
    controller_->setSetpoint(targetCurrent_);
    controller_->reset();
    
    Serial.println("[CurrentControlTask] Current control task started");
    
    // 任务主循环
    while (isRunning()) {
        unsigned long currentTime = millis();
        
        // 以指定的间隔执行控制操作
        if (currentTime - lastControlTime_ >= controlInterval) {
            // 更新传感器读数
            currentSensor_->update();
            
            // 读取实际电流
            actualCurrent_ = currentSensor_->getCurrent();
            
            // 如果电流控制已启用，执行控制
            if (enabled_) {
                // 计算控制输出
                double output = controller_->compute(actualCurrent_);
                
                // 将输出转换为DAC值
                uint16_t dacValue = static_cast<uint16_t>(constrain(output, 0, 4095));
                
                // 设置DAC输出
                dac_->setValue(dacValue);
                lastDacValue_ = dacValue;
            }
            
            // 发布状态事件
            publishStatusEvent();
            
            lastControlTime_ = currentTime;
        }
        
        // 适当延时
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void CurrentControlTask::registerEventListeners() {
    // 注册为事件监听器
    EventBus::getInstance().registerListener(this);
}

void CurrentControlTask::publishStatusEvent() {
    // 创建并发布电流控制状态事件
    CurrentControlEvent event(targetCurrent_, actualCurrent_, lastDacValue_, enabled_);
    EventBus::getInstance().publish(event);
}
