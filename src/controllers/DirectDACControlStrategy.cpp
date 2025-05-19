/**
 * @file DirectDACControlStrategy.cpp
 * @brief 直接DAC控制策略实现
 * @author 重构
 * @date 2025-05-15
 * @details 实现直接设置DAC值的控制策略
 */
#include "DirectDACControlStrategy.h"
#include <Arduino.h>

DirectDACControlStrategy::DirectDACControlStrategy(double scale, double offset, double outputMin, double outputMax)
    : scale_(scale), offset_(offset), 
      outputMin_(outputMin), outputMax_(outputMax) {
}

double DirectDACControlStrategy::compute(double target, double measured) {
    // 直接将目标值按线性映射转换为输出值
    double output = target * scale_ + offset_;
    
    // 输出限幅
    if (output > outputMax_) {
        output = outputMax_;
    } else if (output < outputMin_) {
        output = outputMin_;
    }
    
    return output;
}

void DirectDACControlStrategy::reset() {
    // 无状态控制，无需重置
}

const char* DirectDACControlStrategy::getName() const {
    return "直接DAC控制";
}

void DirectDACControlStrategy::setScale(double scale) {
    scale_ = scale;
}

void DirectDACControlStrategy::setOffset(double offset) {
    offset_ = offset;
}

void DirectDACControlStrategy::setOutputLimits(double min, double max) {
    if (min < max) {
        outputMin_ = min;
        outputMax_ = max;
    }
}