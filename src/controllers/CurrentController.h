/**
 * @file CurrentController.h
 * @brief 电流控制器类定义
 * @author 重构
 * @date 2025-05-15
 * @details 专用于电子负载的电流控制器
 */
#ifndef CURRENT_CONTROLLER_H
#define CURRENT_CONTROLLER_H

#include "Controller.h"
#include <functional>

/**
 * @brief 电流控制器类
 * @details 专门用于控制电子负载中的电流
 */
class CurrentController : public Controller {
public:
    /**
     * @brief 构造函数
     * @param name 控制器名称
     * @param strategy 初始控制策略
     * @param outputCallback 输出回调函数，用于设置DAC输出
     */
    CurrentController(
        const String& name, 
        std::shared_ptr<ControlStrategy> strategy,
        std::function<void(double)> outputCallback = nullptr
    );
    
    /**
     * @brief 设置DAC输出回调函数
     * @param outputCallback 输出回调函数
     */
    void setOutputCallback(std::function<void(double)> outputCallback);
      /**
     * @brief 计算并设置控制输出
     * @param measured 当前电流测量值
     * @return 控制器输出电压值
     */
    double compute(double measured) override;
    
    /**
     * @brief 设置目标电流值
     * @param setpoint 目标电流(mA)
     */
    void setSetpoint(double setpoint) { setTarget(setpoint); }
    
    /**
     * @brief 获取目标电流值
     * @return 目标电流(mA)
     */
    double getSetpoint() const { return getTarget(); }

    /**
     * @brief 设置限流保护值
     * @param limit 电流限制值(mA)
     */
    void setCurrentLimit(double limit);
    
    /**
     * @brief 获取限流保护值
     * @return 电流限制值(mA)
     */
    double getCurrentLimit() const;
    
    /**
     * @brief 检查是否超出限流保护
     * @param measured 当前电流测量值
     * @return true:已超出限流保护, false:未超出
     */
    bool isCurrentLimitExceeded(double measured) const;
    
    /**
     * @brief 执行控制服务
     * @param measured 当前电流测量值(mA)
     */
    void service(double measured);

private:
    std::function<void(double)> outputCallback_; ///< 输出回调函数
    double currentLimit_; ///< 电流限制值(mA)
    bool limitExceeded_; ///< 是否超出限流保护
};

#endif // CURRENT_CONTROLLER_H