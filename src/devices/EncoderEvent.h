/**
 * @file EncoderEvent.h
 * @brief 编码器事件定义
 * @author Triwalt
 * @date 2025-05-06
 * @details 定义了与编码器相关的事件类型
 */

#ifndef ENCODER_EVENT_H
#define ENCODER_EVENT_H

#include "../core/Event.h"

/**
 * @brief 编码器值改变事件
 */
class EncoderValueChangedEvent : public Event {
public:
    /**
     * @brief 构造函数
     * @param value 新的编码器值
     */
    explicit EncoderValueChangedEvent(double value)
        : Event(EventType::ENCODER_CHANGED), value_(value) {}
    
    /**
     * @brief 获取编码器值
     * @return 编码器值
     */
    double getValue() const { return value_; }
    
    /**
     * @brief 获取事件名称（用于调试）
     * @return 事件的字符串表示
     */
    String toString() const override {
        return "EncoderValueChangedEvent[value=" + String(value_) + "]";
    }
    
private:
    double value_; ///< 编码器值
};

/**
 * @brief 编码器按键事件
 */
class EncoderButtonEvent : public Event {
public:
    /**
     * @brief 构造函数
     * @param pressed 按键是否被按下
     */
    explicit EncoderButtonEvent(bool pressed)
        : Event(pressed ? EventType::BUTTON_PRESSED : EventType::BUTTON_RELEASED),
          pressed_(pressed) {}
    
    /**
     * @brief 判断按键是否被按下
     * @return 如果按下返回true，否则返回false
     */
    bool isPressed() const { return pressed_; }
    
    /**
     * @brief 获取事件名称（用于调试）
     * @return 事件的字符串表示
     */
    String toString() const override {
        return "EncoderButtonEvent[pressed=" + String(pressed_ ? "true" : "false") + "]";
    }
    
private:
    bool pressed_; ///< 按键是否被按下
};

#endif // ENCODER_EVENT_H