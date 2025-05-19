/**
 * @file Button.cpp
 * @brief 按键设备类实现
 * @author Triwalt
 * @date 2025-05-16
 * @details 实现对按键输入的处理，包括去抖动和事件触发
 */

#include "Button.h"

Button::Button(const String& name, uint8_t pin, const ButtonConfig& config)
    : Device(name),
      pin_(pin),
      config_(config),
      buttonState_(false),
      lastButtonState_(false),
      lastRawState_(false),
      pressStartTime_(0),
      lastReleaseTime_(0),
      lastDebounceTime_(0),
      longPressDetected_(false),
      clickDetected_(false)
{
}

Button::~Button() {
    // 无需额外清理
}

bool Button::init() {
    // 如果已经初始化，直接返回
    if (initialized_) {
        return true;
    }
    
    // 设置按键引脚为输入模式，根据配置添加上拉或下拉电阻
    if (config_.activeLow) {
        pinMode(pin_, INPUT_PULLUP); // 低电平有效，使用上拉电阻
    } else {
        pinMode(pin_, INPUT_PULLDOWN); // 高电平有效，使用下拉电阻
    }
    
    // 初始化状态
    lastRawState_ = readRawState();
    buttonState_ = lastRawState_;
    lastButtonState_ = buttonState_;
    
    Serial.printf("Button '%s' initialized on pin %d\n", name_.c_str(), pin_);
    
    initialized_ = true;
    return true;
}

void Button::update() {
    if (!initialized_) {
        return;
    }
    
    // 读取当前按钮状态
    bool currentReading = readRawState();
    unsigned long currentTime = millis();
    
    // 检查按钮状态是否改变
    if (currentReading != lastRawState_) {
        lastDebounceTime_ = currentTime;
    }
    
    // 去抖动处理
    if ((currentTime - lastDebounceTime_) > config_.debounceTime) {
        // 如果按钮状态在去抖动时间后保持稳定，认为是有效的状态变化
        if (currentReading != buttonState_) {
            buttonState_ = currentReading;
            
            if (buttonState_) {
                // 按钮按下
                pressStartTime_ = currentTime;
                longPressDetected_ = false;
                clickDetected_ = false;
                publishEvent(ButtonEventType::PRESSED);
            } else {
                // 按钮释放
                unsigned long pressDuration = currentTime - pressStartTime_;
                publishEvent(ButtonEventType::RELEASED);
                
                // 处理长按
                if (config_.enableLongPress && pressDuration >= config_.longPressTime) {
                    // 已经在长按期间触发过长按事件，不需要再处理点击事件
                } 
                // 处理点击
                else if (pressDuration < config_.clickTime) {
                    // 检查是否为双击
                    if (config_.enableDoubleClick && 
                        (currentTime - lastReleaseTime_) < config_.doubleClickTime) {
                        publishEvent(ButtonEventType::DOUBLE_CLICK);
                    } else {
                        publishEvent(ButtonEventType::CLICKED);
                    }
                }
                
                lastReleaseTime_ = currentTime;
            }
        }
    }
    
    // 检查长按事件
    if (config_.enableLongPress && buttonState_ && !longPressDetected_) {
        if ((currentTime - pressStartTime_) >= config_.longPressTime) {
            longPressDetected_ = true;
            publishEvent(ButtonEventType::LONG_PRESS);
        }
    }
    
    lastRawState_ = currentReading;
    lastButtonState_ = buttonState_;
}

bool Button::isPressed() const {
    return buttonState_;
}

void Button::setConfig(const ButtonConfig& config) {
    config_ = config;
    
    // 如果引脚模式改变，需要重新初始化
    if (initialized_) {
        if (config_.activeLow) {
            pinMode(pin_, INPUT_PULLUP);
        } else {
            pinMode(pin_, INPUT_PULLDOWN);
        }
    }
}

bool Button::readRawState() const {
    // 读取引脚状态并根据activeLow配置进行转换
    bool physicalState = digitalRead(pin_);
    return config_.activeLow ? !physicalState : physicalState;
}

void Button::publishEvent(ButtonEventType type) {
    ButtonEvent event(name_, type);
    EventBus::getInstance().publish(event);
}
