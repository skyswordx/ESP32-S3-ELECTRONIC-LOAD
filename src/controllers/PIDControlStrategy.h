/**
 * @file PIDControlStrategy.h
 * @brief PID控制策略实现
 * @author 重构
 * @date 2025-05-15
 * @details 实现基于PID算法的控制策略
 */
#ifndef PID_CONTROL_STRATEGY_H
#define PID_CONTROL_STRATEGY_H

#include "ControlStrategy.h"

/**
 * @brief PID控制策略类
 * @details 基于PID算法实现的控制策略
 */
class PIDControlStrategy : public ControlStrategy {
public:
    /**
     * @brief 构造函数
     * @param kp 比例系数
     * @param ki 积分系数
     * @param kd 微分系数
     * @param outputMin 输出最小值
     * @param outputMax 输出最大值
     */
    PIDControlStrategy(double kp, double ki, double kd, double outputMin, double outputMax);
    
    /**
     * @brief 计算PID控制输出
     * @param target 目标值
     * @param measured 测量值
     * @return PID控制器输出
     */
    double compute(double target, double measured) override;
    
    /**
     * @brief 重置PID控制状态
     */
    void reset() override;
    
    /**
     * @brief 获取控制策略名称
     * @return 控制策略名称
     */
    const char* getName() const override;
    
    /**
     * @brief 设置PID参数
     * @param kp 比例系数
     * @param ki 积分系数
     * @param kd 微分系数
     */
    void setTunings(double kp, double ki, double kd);
    
    /**
     * @brief 设置输出限制
     * @param min 最小输出
     * @param max 最大输出
     */
    void setOutputLimits(double min, double max);

private:
    double kp_;              ///< 比例系数
    double ki_;              ///< 积分系数
    double kd_;              ///< 微分系数
    
    double outputMin_;       ///< 输出最小值
    double outputMax_;       ///< 输出最大值
    
    double lastMeasured_;    ///< 上次测量值
    double lastOutput_;      ///< 上次输出值
    double iTerm_;           ///< 积分项
    
    bool isFirstCompute_;    ///< 是否是首次计算标志
};

#endif // PID_CONTROL_STRATEGY_H