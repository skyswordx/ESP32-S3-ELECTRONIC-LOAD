/**
 * @file Encoder.cpp
 * @brief 编码器设备类实现
 * @author Triwalt
 * @date 2025-05-15
 * @details 基于ESP32Encoder库实现的编码器设备
 */

#include "Encoder.h"
#include <Arduino.h>

// 按钮去抖动时间（毫秒）
#define ENCODER_BUTTON_DEBOUNCE_DELAY 50

Encoder::Encoder(const String& name, 
                uint8_t pinA, uint8_t pinB, int8_t pinBtn,
                double minValue, double maxValue, 
                double step, double initialValue)
    : Device(name),
      pinA_(pinA),
      pinB_(pinB),
      pinBtn_(pinBtn),
      mode_(EncoderMode::SINGLE),
      currentValue_(initialValue),
      minValue_(minValue),
      maxValue_(maxValue),
      step_(step),
      lastCount_(0),
      buttonState_(false),
      lastButtonState_(false),
      lastDebounceTime_(0)
{
}

Encoder::~Encoder() {
    // ESP32Encoder的析构函数会处理资源清理
}

bool Encoder::init() {
    // 如果已经初始化过，直接返回
    if (initialized_) {
        return true;
    }
    
    // 配置ESP32编码器的上拉电阻
    ESP32Encoder::useInternalWeakPullResistors = puType::up;
    
    // 根据模式配置编码器
    setMode(mode_);
    
    // 配置按钮引脚（如果有）
    if (pinBtn_ >= 0) {
        pinMode(pinBtn_, INPUT_PULLUP);
    }
    
    // 设置初始值
    encoder_.setCount(0);
    lastCount_ = 0;
    
    Serial.printf("Encoder '%s' initialized with pins A:%d B:%d Btn:%d\n", 
                 name_.c_str(), pinA_, pinB_, pinBtn_);
    
    initialized_ = true;
    return true;
}

void Encoder::setMode(EncoderMode mode) {
    mode_ = mode;
    
    // 如果还没初始化，只设置模式不配置硬件
    if (!initialized_) {
        return;
    }
    
    // 先分离当前配置
    encoder_.detach();
    
    // 根据模式重新配置编码器
    switch (mode_) {
        case EncoderMode::SINGLE:
            encoder_.attachSingleEdge(pinA_, pinB_);
            break;
        case EncoderMode::HALF:
            encoder_.attachHalfQuad(pinA_, pinB_);
            break;
        case EncoderMode::FULL:
            encoder_.attachFullQuad(pinA_, pinB_);
            break;
    }
    
    // 为了减少抖动，设置最大滤波值
    encoder_.setFilter(1023);
    
    // 重设计数值
    encoder_.setCount(0);
    lastCount_ = 0;
}

void Encoder::update() {
    if (!initialized_) {
        return;
    }
    
    // 读取并计算新的值
    int64_t countDiff = readAndClearCount();
    if (countDiff != 0) {
        // 根据编码器变化更新当前值
        double newValue = currentValue_ + countDiff * step_;
        
        // 限制在最小最大值范围内
        newValue = constrain(newValue, minValue_, maxValue_);
        
        // 如果值有变化，发布事件
        if (newValue != currentValue_) {
            currentValue_ = newValue;
            
            // 创建并发布编码器值变化事件
            EncoderValueChangedEvent event(currentValue_);
            EventBus::getInstance().publish(event);
        }
    }
    
    // 处理按钮
    if (pinBtn_ >= 0) {
        readButtonState();
    }
}

int64_t Encoder::readAndClearCount() {
    int64_t currentCount = encoder_.getCount();
    int64_t diff = currentCount - lastCount_;
    
    // 如果是全倍频模式，需要除以4，因为每个位置会计数4次
    if (mode_ == EncoderMode::FULL) {
        diff /= 4;
    }
    // 如果是半倍频模式，需要除以2
    else if (mode_ == EncoderMode::HALF) {
        diff /= 2;
    }
    
    encoder_.clearCount();
    lastCount_ = 0;
    
    return diff;
}

void Encoder::readButtonState() {
    // 读取当前按钮状态（上拉，按下为LOW）
    bool reading = (digitalRead(pinBtn_) == LOW);
    
    // 检查是否需要去抖动
    if (reading != lastButtonState_) {
        lastDebounceTime_ = millis();
    }
    
    // 去抖动处理
    if ((millis() - lastDebounceTime_) > ENCODER_BUTTON_DEBOUNCE_DELAY) {
        // 如果按钮状态改变
        if (reading != buttonState_) {
            buttonState_ = reading;
            
            // 发布按钮事件
            EncoderButtonEvent event(buttonState_);
            EventBus::getInstance().publish(event);
        }
    }
    
    lastButtonState_ = reading;
}

double Encoder::getValue() const {
    return currentValue_;
}

void Encoder::setValue(double value) {
    // 限制在最小最大值范围内
    currentValue_ = constrain(value, minValue_, maxValue_);
    
    // 发布编码器值变化事件
    EncoderValueChangedEvent event(currentValue_);
    EventBus::getInstance().publish(event);
}

void Encoder::setMinValue(double value) {
    if (value < maxValue_) {
        minValue_ = value;
        // 确保当前值在新范围内
        if (currentValue_ < minValue_) {
            setValue(minValue_);
        }
    }
}

void Encoder::setMaxValue(double value) {
    if (value > minValue_) {
        maxValue_ = value;
        // 确保当前值在新范围内
        if (currentValue_ > maxValue_) {
            setValue(maxValue_);
        }
    }
}

void Encoder::setStep(double value) {
    if (value > 0) {
        step_ = value;
    }
}

bool Encoder::isButtonPressed() const {
    return buttonState_;
}