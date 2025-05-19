/**
 * @file DirectDACControlStrategy.h
 * @brief 直接DAC控制策略实现
 * @author 重构
 * @date 2025-05-15
 * @details 实现直接设置DAC值的控制策略
 */
#ifndef DIRECT_DAC_CONTROL_STRATEGY_H
#define DIRECT_DAC_CONTROL_STRATEGY_H

#include "ControlStrategy.h"

/**
 * @brief 直接DAC控制策略类
 * @details 无反馈控制，直接将设定值转换为DAC输出
 */
class DirectDACControlStrategy : public ControlStrategy {
public:
    /**
     * @brief 构造函数
     * @param scale 缩放系数
     * @param offset 偏移量
     * @param outputMin 输出最小值
     * @param outputMax 输出最大值
     */
    DirectDACControlStrategy(double scale, double offset, double outputMin, double outputMax);
    
    /**
     * @brief 计算控制输出
     * @param target 目标值
     * @param measured 测量值（在直接控制中不使用）
     * @return 控制器输出
     */
    double compute(double target, double measured) override;
    
    /**
     * @brief 重置控制状态
     */
    void reset() override;
    
    /**
     * @brief 获取控制策略名称
     * @return 控制策略名称
     */
    const char* getName() const override;
    
    /**
     * @brief 设置缩放系数
     * @param scale 缩放系数
     */
    void setScale(double scale);
    
    /**
     * @brief 设置偏移量
     * @param offset 偏移量
     */
    void setOffset(double offset);
    
    /**
     * @brief 设置输出限制
     * @param min 最小输出
     * @param max 最大输出
     */
    void setOutputLimits(double min, double max);

private:
    double scale_;           ///< 缩放系数
    double offset_;          ///< 偏移量
    double outputMin_;       ///< 输出最小值
    double outputMax_;       ///< 输出最大值
};

#endif // DIRECT_DAC_CONTROL_STRATEGY_H