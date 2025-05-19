/**
 * @file CurrentControlTask.h
 * @brief 电流控制任务类定义
 * @author Triwalt
 * @date 2025-05-16
 * @details 定义处理电流控制的任务
 */

#pragma once

#include "../core/Task.h"
#include "../core/EventListener.h"
#include "../controllers/CurrentController.h"
#include "../devices/INA226Device.h"
#include "../devices/MCP4725Device.h"

// 电流控制任务类，同时也是事件监听器
class CurrentControlTask : public Task, public EventListener {
public:
    /**
     * @brief 构造函数
     * @param name 任务名称
     * @param stackSize 任务栈大小
     * @param priority 任务优先级
     * @param coreId 运行核心ID
     */
    CurrentControlTask(const String& name = "CurrentControlTask",
                     uint32_t stackSize = 4096,
                     UBaseType_t priority = 4,
                     BaseType_t coreId = 0);
    
    virtual ~CurrentControlTask();
    
    /**
     * @brief 设置电流控制器
     * @param controller 电流控制器指针
     */
    void setCurrentController(CurrentController* controller);
    
    /**
     * @brief 设置电流传感器
     * @param sensor 电流传感器指针
     */
    void setCurrentSensor(INA226Device* sensor);
    
    /**
     * @brief 设置DAC设备
     * @param dac DAC设备指针
     */
    void setDAC(MCP4725Device* dac);
    
    /**
     * @brief 设置目标电流
     * @param current 目标电流(mA)
     */
    void setTargetCurrent(double current);
    
    /**
     * @brief 获取当前目标电流
     * @return 目标电流(mA)
     */
    double getTargetCurrent() const;
    
    /**
     * @brief 获取当前实际电流
     * @return 实际电流(mA)
     */
    double getActualCurrent() const;
    
    /**
     * @brief 启用/禁用电流控制
     * @param enabled 是否启用
     */
    void setEnabled(bool enabled);
    
    /**
     * @brief 检查电流控制是否启用
     * @return 是否启用
     */
    bool isEnabled() const;
      /**
     * @brief 事件处理函数
     * @param event 事件对象
     */
    virtual void onEvent(const Event& event) override;
    
protected:
    /**
     * @brief 任务运行函数
     */
    virtual void run() override;
    
private:
    CurrentController* controller_;   // 电流控制器
    INA226Device* currentSensor_;     // 电流传感器
    MCP4725Device* dac_;              // DAC设备
    double targetCurrent_;            // 目标电流
    double actualCurrent_;            // 实际电流
    double lastDacValue_;             // 上次输出的DAC值
    bool enabled_;                    // 是否启用
    unsigned long lastControlTime_;   // 上次控制时间
    
    /**
     * @brief 注册事件监听
     */
    void registerEventListeners();
    
    /**
     * @brief 发布状态事件
     */
    void publishStatusEvent();
};
