/**
 * @file PIDControlStrategy.cpp
 * @brief PID控制策略实现
 * @author 重构
 * @date 2025-05-15
 * @details 实现基于PID算法的控制策略
 */
#include "PIDControlStrategy.h"
#include <Arduino.h>

PIDControlStrategy::PIDControlStrategy(double kp, double ki, double kd, double outputMin, double outputMax)
    : kp_(kp), ki_(ki), kd_(kd), 
      outputMin_(outputMin), outputMax_(outputMax),
      lastMeasured_(0.0), lastOutput_(0.0), iTerm_(0.0),
      isFirstCompute_(true) {
}

double PIDControlStrategy::compute(double target, double measured) {
    // 初始化首次计算
    if (isFirstCompute_) {
        lastMeasured_ = measured;
        lastOutput_ = 0.0;
        iTerm_ = 0.0;
        isFirstCompute_ = false;
    }
    
    // 计算误差
    double error = target - measured;
    
    // 计算PID各项
    double pTerm = kp_ * error;
    iTerm_ += ki_ * error;
    double dTerm = kd_ * (lastMeasured_ - measured); // 使用微分项来防止微分冲击
    
    // 计算输出
    double output = lastOutput_ + pTerm + iTerm_ + dTerm;
    
    // 输出限幅
    if (output > outputMax_) {
        // 积分限幅处理
        iTerm_ -= output - outputMax_;
        output = outputMax_;
    } else if (output < outputMin_) {
        // 积分限幅处理
        iTerm_ += outputMin_ - output;
        output = outputMin_;
    }
    
    // 更新历史数据
    lastMeasured_ = measured;
    lastOutput_ = output;
    
    return output;
}

void PIDControlStrategy::reset() {
    lastMeasured_ = 0.0;
    lastOutput_ = 0.0;
    iTerm_ = 0.0;
    isFirstCompute_ = true;
}

const char* PIDControlStrategy::getName() const {
    return "PID控制";
}

void PIDControlStrategy::setTunings(double kp, double ki, double kd) {
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
}

void PIDControlStrategy::setOutputLimits(double min, double max) {
    if (min < max) {
        outputMin_ = min;
        outputMax_ = max;
    }
}