/**
 * @file Encoder.h
 * @brief 编码器设备类定义
 * @author Triwalt
 * @date 2025-05-15
 * @details 基于ESP32Encoder库实现的编码器设备
 */

#ifndef ENCODER_H
#define ENCODER_H

#include "Device.h"
#include "../core/EventBus.h"
#include "EncoderEvent.h"
#include <ESP32Encoder.h>

/**
 * @brief 编码器模式
 */
enum class EncoderMode {
    SINGLE = 0,  ///< 单倍频模式
    HALF = 1,    ///< 半倍频模式
    FULL = 2     ///< 全倍频模式（四倍频）
};

/**
 * @brief 编码器设备类
 */
class Encoder : public Device {
public:
    /**
     * @brief 构造函数
     * @param name 设备名称
     * @param pinA 编码器A相引脚
     * @param pinB 编码器B相引脚
     * @param pinBtn 编码器按钮引脚（如无按钮则为-1）
     * @param minValue 最小值
     * @param maxValue 最大值
     * @param step 步进值
     * @param initialValue 初始值
     */
    Encoder(const String& name, 
           uint8_t pinA, uint8_t pinB, int8_t pinBtn = -1,
           double minValue = 0.0, double maxValue = 1000.0, 
           double step = 1.0, double initialValue = 0.0);
    
    /**
     * @brief 析构函数
     */
    virtual ~Encoder();
    
    /**
     * @brief 初始化设备
     * @return 如果初始化成功返回true，否则返回false
     */
    bool init() override;
    
    /**
     * @brief 设置编码器模式
     * @param mode 编码器模式
     */
    void setMode(EncoderMode mode);
    
    /**
     * @brief 更新编码器状态，应在任务中周期性调用
     */
    void update();
    
    /**
     * @brief 获取当前值
     * @return 当前编码器值
     */
    double getValue() const;
    
    /**
     * @brief 设置当前值
     * @param value 要设置的值
     */
    void setValue(double value);
    
    /**
     * @brief 设置最小值
     * @param value 最小值
     */
    void setMinValue(double value);
    
    /**
     * @brief 设置最大值
     * @param value 最大值
     */
    void setMaxValue(double value);
    
    /**
     * @brief 设置步进值
     * @param value 步进值
     */
    void setStep(double value);
    
    /**
     * @brief 获取按钮状态
     * @return 如果按钮被按下返回true，否则返回false
     */
    bool isButtonPressed() const;
    
private:
    /**
     * @brief 读取并清除编码器计数
     * @return 编码器增量
     */
    int64_t readAndClearCount();
    
    /**
     * @brief 读取按钮状态
     */
    void readButtonState();
    
private:
    ESP32Encoder encoder_;      ///< ESP32编码器对象
    uint8_t pinA_;              ///< A相引脚
    uint8_t pinB_;              ///< B相引脚
    int8_t pinBtn_;             ///< 按钮引脚
    EncoderMode mode_;          ///< 编码器模式
    double currentValue_;       ///< 当前值
    double minValue_;           ///< 最小值
    double maxValue_;           ///< 最大值
    double step_;               ///< 步进值
    int64_t lastCount_;         ///< 上次的编码器计数
    bool buttonState_;          ///< 按钮当前状态
    bool lastButtonState_;      ///< 按钮上次状态
    unsigned long lastDebounceTime_; ///< 上次去抖动时间
};

#endif // ENCODER_H