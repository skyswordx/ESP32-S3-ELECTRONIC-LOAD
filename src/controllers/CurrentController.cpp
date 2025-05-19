/**
 * @file CurrentController.cpp
 * @brief 电流控制器类实现
 * @author 重构
 * @date 2025-05-15
 * @details 专用于电子负载的电流控制器实现
 */
#include "CurrentController.h"
#include <Arduino.h>

CurrentController::CurrentController(
    const String& name, 
    std::shared_ptr<ControlStrategy> strategy,
    std::function<void(double)> outputCallback
) : Controller(name, strategy), 
    outputCallback_(outputCallback),
    currentLimit_(5000.0), // 默认设置5A限流
    limitExceeded_(false) {
}

void CurrentController::setOutputCallback(std::function<void(double)> outputCallback) {
    outputCallback_ = outputCallback;
}

double CurrentController::compute(double measured) {
    // 首先检查电流是否超限
    if (isCurrentLimitExceeded(measured)) {
        // 超出限流时，输出设为0以保护电路
        if (outputCallback_) {
            outputCallback_(0.0);
        }
        return 0.0;
    }
    
    // 正常情况下调用父类计算
    double output = Controller::compute(measured);
    
    // 如果有输出回调，设置输出值
    if (outputCallback_) {
        outputCallback_(output);
    }
    
    return output;
}

void CurrentController::setCurrentLimit(double limit) {
    // 设置限流保护值，确保为正值
    currentLimit_ = limit > 0.0 ? limit : 0.0;
}

double CurrentController::getCurrentLimit() const {
    return currentLimit_;
}

bool CurrentController::isCurrentLimitExceeded(double measured) const {
    return measured > currentLimit_;
}

void CurrentController::service(double measured) {
    // 检查是否超出限流
    limitExceeded_ = isCurrentLimitExceeded(measured);
    
    // 计算并设置输出
    compute(measured);
}

// setSetpoint 方法已在头文件中定义