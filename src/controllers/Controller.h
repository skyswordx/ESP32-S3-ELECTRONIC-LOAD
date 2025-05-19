/**
 * @file Controller.h
 * @brief 控制器基类定义
 * @author 重构
 * @date 2025-05-15
 * @details 定义所有控制器的基本接口和功能
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ControlStrategy.h"
#include <memory>
#include <Arduino.h>

/**
 * @brief 控制器基类
 * @details 所有具体控制器应该继承此类
 */
class Controller {
public:
    /**
     * @brief 构造函数
     * @param name 控制器名称
     * @param strategy 初始控制策略
     */
    Controller(const String& name, std::shared_ptr<ControlStrategy> strategy = nullptr);
    
    /**
     * @brief 虚析构函数
     */
    virtual ~Controller() = default;
    
    /**
     * @brief 设置目标值
     * @param target 目标值
     */
    void setTarget(double target);
    
    /**
     * @brief 获取目标值
     * @return 当前目标值
     */
    double getTarget() const;
    
    /**
     * @brief 计算并返回控制输出
     * @param measured 当前测量值
     * @return 控制器输出值
     */
    virtual double compute(double measured);
    
    /**
     * @brief 设置控制策略
     * @param strategy 新的控制策略
     */
    void setStrategy(std::shared_ptr<ControlStrategy> strategy);
    
    /**
     * @brief 获取当前控制策略
     * @return 当前控制策略
     */
    std::shared_ptr<ControlStrategy> getStrategy() const;
    
    /**
     * @brief 获取控制器名称
     * @return 控制器名称
     */
    const String& getName() const;

    /**
     * @brief 重置控制器状态
     */
    virtual void reset();

protected:
    String name_;                            ///< 控制器名称
    double target_;                          ///< 目标值
    std::shared_ptr<ControlStrategy> strategy_; ///< 控制策略
};

#endif // CONTROLLER_H