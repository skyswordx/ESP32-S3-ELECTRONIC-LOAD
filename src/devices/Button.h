/**
 * @file Button.h
 * @brief 按键设备类定义
 * @author Triwalt
 * @date 2025-05-16
 * @details 封装了对按键输入的处理，包括去抖动和事件触发
 */

#pragma once

#include "Device.h"
#include "../core/EventBus.h"
#include <Arduino.h>

// 按键事件类型
enum class ButtonEventType {
    PRESSED,    // 按下
    RELEASED,   // 释放
    CLICKED,    // 短按
    LONG_PRESS, // 长按
    DOUBLE_CLICK // 双击
};

// 按键事件数据
class ButtonEvent : public Event {
public:
    ButtonEvent(const String& buttonName, ButtonEventType type) 
        : Event(EventType::BUTTON), buttonName_(buttonName), type_(type) {}
    
    virtual ~ButtonEvent() {}
    
    const String& getButtonName() const { return buttonName_; }
    ButtonEventType getEventType() const { return type_; }
    
private:
    String buttonName_;        // 按键名称
    ButtonEventType type_;     // 事件类型
};

// 按键配置选项
struct ButtonConfig {
    uint16_t debounceTime = 50;       // 去抖动时间(ms)
    uint16_t clickTime = 500;         // 短按判定时间(ms)
    uint16_t longPressTime = 1000;    // 长按判定时间(ms)
    uint16_t doubleClickTime = 300;   // 双击间隔判定时间(ms)
    bool enableDoubleClick = false;   // 是否启用双击检测
    bool enableLongPress = true;      // 是否启用长按检测
    bool activeLow = true;            // 按键逻辑，true为低电平有效，false为高电平有效
};

// 按键设备类
class Button : public Device {
public:
    /**
     * @brief 构造函数
     * @param name 按键名称
     * @param pin 按键引脚
     * @param config 按键配置
     */
    Button(const String& name, uint8_t pin, const ButtonConfig& config = ButtonConfig());
    
    virtual ~Button();
    
    /**
     * @brief 初始化按键
     * @return 初始化成功返回true，失败返回false
     */
    virtual bool init() override;
    
    /**
     * @brief 更新按键状态，检测事件
     * @details 需要定期调用此函数以检测按键事件
     */
    virtual void update() override;
    
    /**
     * @brief 获取按键当前状态
     * @return 按键按下返回true，否则返回false
     */
    bool isPressed() const;
    
    /**
     * @brief 设置按键配置
     * @param config 按键配置
     */
    void setConfig(const ButtonConfig& config);
    
private:
    uint8_t pin_;              // 按键引脚
    ButtonConfig config_;      // 按键配置
    
    bool buttonState_;         // 当前按键状态（已去抖）
    bool lastButtonState_;     // 上次按键状态
    bool lastRawState_;        // 上次原始按键状态（未去抖）
    
    unsigned long pressStartTime_;    // 按下开始时间
    unsigned long lastReleaseTime_;   // 上次释放时间
    unsigned long lastDebounceTime_;  // 上次去抖时间
    
    bool longPressDetected_;   // 标记是否已触发长按
    bool clickDetected_;       // 标记是否已触发点击
    
    /**
     * @brief 读取原始按键状态
     * @return 按键按下返回true，否则返回false
     */
    bool readRawState() const;
    
    /**
     * @brief 发布按键事件
     * @param type 事件类型
     */
    void publishEvent(ButtonEventType type);
};
