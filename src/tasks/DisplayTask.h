/**
 * @file DisplayTask.h
 * @brief 显示任务类定义
 * @author Triwalt
 * @date 2025-05-16
 * @details 基于LVGL的显示处理任务，负责界面更新和处理
 */

#pragma once

#include "../core/Task.h"
#include "../core/EventListener.h"
#include "../controllers/ProtectionController.h" // 添加保护控制器头文件
#include <Arduino.h>

// 显示模式枚举，定义不同的显示界面模式
enum class DisplayMode {
    MAIN,       // 主界面，显示基本测量和控制
    SETTINGS,   // 设置界面
    CHART,      // 图表界面，显示历史数据
    PROTECTION, // 保护设置界面
    CALIBRATION // 校准界面
};
class DisplayTask : public Task, public EventListener {
public:
    /**
     * @brief 构造函数
     * @param name 任务名称
     * @param stackSize 任务栈大小
     * @param priority 任务优先级
     * @param coreId 运行核心ID
     */
    DisplayTask(const String& name = "DisplayTask",
              uint32_t stackSize = 8192, // LVGL需要较大的栈空间
              UBaseType_t priority = 3,
              BaseType_t coreId = 1);
    
    virtual ~DisplayTask();
    
    /**
     * @brief 初始化显示
     * @return 初始化成功返回true，失败返回false
     */
    bool initDisplay();
    
    /**
     * @brief 事件处理函数，用于处理监听到的事件
     * @param event 事件对象
     */
    virtual void onEvent(const Event& event) override;
      /**
     * @brief 加载主界面
     */
    void loadMainScreen();
    
    /**
     * @brief 设置电流传感器
     * @param sensor 电流传感器指针
     */
    void setCurrentSensor(INA226Device* sensor) { currentSensor_ = sensor; }
    
    /**
     * @brief 设置保护控制器
     * @param controller 保护控制器指针
     */
    void setProtectionController(ProtectionController* controller) { protectionController_ = controller; }
    
    /**
     * @brief 切换显示模式
     * @param mode 目标显示模式
     */
    void switchDisplayMode(DisplayMode mode);
    
protected:
    /**
     * @brief 任务运行函数
     */
    virtual void run() override;
    
private:
    bool displayInitialized_;            // 显示是否已初始化
    unsigned long lastUpdateTime_;       // 上次更新时间
    const unsigned long updateInterval_; // 更新间隔 (ms)
    DisplayMode currentMode_;            // 当前显示模式
    INA226Device* currentSensor_;        // 电流传感器引用
    ProtectionController* protectionController_; // 保护控制器引用
    
    /**
     * @brief 更新显示内容
     */
    void updateDisplay();
    
    /**
     * @brief 注册事件监听
     */
    void registerEventListeners();
      /**
     * @brief 处理编码器值变化事件
     * @param event 编码器事件
     */
    void handleEncoderEvent(const Event& event);
    
    /**
     * @brief 处理按钮事件
     * @param event 按钮事件
     */
    void handleButtonEvent(const Event& event);
    
    /**
     * @brief 处理测量事件
     * @param event 测量事件（电流、电压、功率、电阻）
     */
    void handleMeasurementEvent(const Event& event);
    
    /**
     * @brief 处理保护事件
     * @param event 保护事件（过压、过流、过功率）
     */
    void handleProtectionEvent(const Event& event);
    
    /**
     * @brief 处理控制事件
     * @param event 控制事件（目标电流变化等）
     */
    void handleControlEvent(const Event& event);
    
    /**
     * @brief 处理UI事件
     * @param event UI事件（页面切换等）
     */
    void handleUIEvent(const Event& event);
};
