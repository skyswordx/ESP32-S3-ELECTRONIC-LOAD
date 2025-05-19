/**
 * @file Event.h
 * @brief 事件基类定义
 * @author Triwalt
 * @date 2025-05-06
 * @details 定义了系统中所有事件的基类和事件类型枚举
 */

#ifndef EVENT_H
#define EVENT_H

#include <Arduino.h>

/**
 * @brief 事件类型枚举
 */
enum class EventType {
    // 系统事件
    SYSTEM_STARTUP,
    SYSTEM_SHUTDOWN,
    
    // 按键事件
    BUTTON_PRESSED,
    BUTTON_RELEASED,
    BUTTON_LONG_PRESSED,
    
    // 编码器事件
    ENCODER_CHANGED,
    ENCODER_BUTTON_PRESSED,

    // 测量事件
    CURRENT_CHANGED,
    VOLTAGE_CHANGED,
    POWER_CHANGED,
    RESISTANCE_CHANGED,

    // 控制事件
    TARGET_CURRENT_CHANGED,
    
    // 警报事件
    OVER_VOLTAGE,
    OVER_CURRENT,
    OVER_POWER,
    
    // 测试事件
    LOAD_REGULATION_TEST,

    // UI事件
    UI_PAGE_CHANGED,
    
    // 通用事件类型
    BUTTON,     // 按钮通用事件
    CONTROL,    // 控制通用事件
    
    // 未知事件
    UNKNOWN
};

/**
 * @brief 事件基类
 * @details 所有事件类型都应该继承此基类
 */
class Event {
public:
    /**
     * @brief 构造函数
     * @param type 事件类型
     */
    explicit Event(EventType type) : type_(type) {}
    
    /**
     * @brief 析构函数
     */
    virtual ~Event() = default;
    
    /**
     * @brief 获取事件类型
     * @return 事件类型
     */
    EventType getType() const { return type_; }
    
    /**
     * @brief 获取事件名称（用于调试）
     * @return 事件的字符串表示
     */
    virtual String toString() const {
        return "Event[type=" + String(static_cast<int>(type_)) + "]";
    }
    
protected:
    EventType type_; ///< 事件类型
};

#endif // EVENT_H