/**
 * @file Controller.cpp
 * @brief 控制器基类实现
 * @author 重构
 * @date 2025-05-15
 * @details 实现控制器基类的功能
 */
#include "Controller.h"

Controller::Controller(const String& name, std::shared_ptr<ControlStrategy> strategy)
    : name_(name), target_(0.0), strategy_(strategy) {
}

void Controller::setTarget(double target) {
    target_ = target;
}

double Controller::getTarget() const {
    return target_;
}

double Controller::compute(double measured) {
    if (strategy_) {
        return strategy_->compute(target_, measured);
    }
    return 0.0; // 没有策略时返回默认值
}

void Controller::setStrategy(std::shared_ptr<ControlStrategy> strategy) {
    if (strategy) {
        strategy_ = strategy;
    }
}

std::shared_ptr<ControlStrategy> Controller::getStrategy() const {
    return strategy_;
}

const String& Controller::getName() const {
    return name_;
}

void Controller::reset() {
    if (strategy_) {
        strategy_->reset();
    }
}