/**
 * @file ControlStrategy.h
 * @brief 控制策略接口定义
 * @author 重构
 * @date 2025-05-15
 * @details 定义控制策略的接口，用于实现不同的控制算法
 */
#ifndef CONTROL_STRATEGY_H
#define CONTROL_STRATEGY_H

/**
 * @brief 控制策略接口
 * @details 所有控制策略都应该实现这个接口
 */
class ControlStrategy {
public:
    /**
     * @brief 虚析构函数
     */
    virtual ~ControlStrategy() = default;
    
    /**
     * @brief 计算控制输出值
     * @param target 目标值
     * @param measured 测量值
     * @return 计算后的控制输出
     */
    virtual double compute(double target, double measured) = 0;
    
    /**
     * @brief 重置控制策略的状态
     */
    virtual void reset() = 0;
    
    /**
     * @brief 获取控制策略名称
     * @return 控制策略名称
     */
    virtual const char* getName() const = 0;
};

#endif // CONTROL_STRATEGY_H