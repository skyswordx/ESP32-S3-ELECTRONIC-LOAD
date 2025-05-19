/**
 * @file ProtectionController.h
 * @brief 保护控制器类定义
 * @author Triwalt
 * @date 2025-05-16
 * @details 定义电子负载的保护控制器，处理过压过流等保护功能
 */

#pragma once

#include "../core/EventListener.h"
#include "../devices/INA226Device.h"
#include <Arduino.h>

// 保护类型枚举
enum class ProtectionType {
    OVER_VOLTAGE,    // 过压保护
    OVER_CURRENT,    // 过流保护
    OVER_POWER,      // 过功率保护
    OVER_TEMPERATURE, // 过温保护
    REVERSE_VOLTAGE  // 反接保护
};

// 保护事件类
class ProtectionEvent : public Event {
public:
    ProtectionEvent(ProtectionType type, double value, double threshold) 
        : type_(type), value_(value), threshold_(threshold) {}
    
    ProtectionType getType() const { return type_; }
    double getValue() const { return value_; }
    double getThreshold() const { return threshold_; }
    
private:
    ProtectionType type_;   // 保护类型
    double value_;          // 触发值
    double threshold_;      // 阈值
};

// 保护配置结构体
struct ProtectionConfig {
    double overVoltageThreshold = 30.0;    // 过压保护阈值(V)
    double overCurrentThreshold = 5000.0;  // 过流保护阈值(mA)
    double overPowerThreshold = 60.0;      // 过功率保护阈值(W)
    double overTempThreshold = 85.0;       // 过温保护阈值(℃)
    double reverseVoltageThreshold = -0.5; // 反接保护阈值(V)
    
    bool enableOverVoltage = true;         // 启用过压保护
    bool enableOverCurrent = true;         // 启用过流保护
    bool enableOverPower = true;           // 启用过功率保护
    bool enableOverTemp = true;            // 启用过温保护
    bool enableReverseVoltage = true;      // 启用反接保护
};

// 保护控制器类
class ProtectionController : public EventListener {
public:
    /**
     * @brief 获取保护控制器单例
     * @return 保护控制器实例引用
     */
    static ProtectionController& getInstance();
    
    /**
     * @brief 设置电流传感器
     * @param sensor 电流传感器指针
     */
    void setCurrentSensor(INA226Device* sensor);
    
    /**
     * @brief 设置温度传感器
     * @param tempPin 温度传感器引脚
     */
    void setTemperatureSensor(uint8_t tempPin);
    
    /**
     * @brief 设置保护配置
     * @param config 保护配置
     */
    void setConfig(const ProtectionConfig& config);
    
    /**
     * @brief 获取当前保护配置
     * @return 保护配置
     */
    const ProtectionConfig& getConfig() const;
    
    /**
     * @brief 检查保护状态
     * @return 如果有保护触发，返回true
     */
    bool checkProtection();
    
    /**
     * @brief 获取最后触发的保护类型
     * @return 保护类型枚举值
     */
    ProtectionType getLastProtectionType() const;
    
    /**
     * @brief 获取最后触发的保护值
     * @return 保护触发值
     */
    double getLastProtectionValue() const;
    
    /**
     * @brief 重置保护状态
     */
    void resetProtection();
    
    /**
     * @brief 处理事件
     * @param event 事件对象
     */
    virtual void onEvent(const Event& event) override;
    
private:
    ProtectionController();
    virtual ~ProtectionController();
    
    // 禁止复制和赋值
    ProtectionController(const ProtectionController&) = delete;
    ProtectionController& operator=(const ProtectionController&) = delete;
    
    INA226Device* currentSensor_;      // 电流传感器
    uint8_t temperaturePin_;           // 温度传感器引脚
    
    ProtectionConfig config_;          // 保护配置
    bool protectionTriggered_;         // 保护是否已触发
    ProtectionType lastProtectionType_; // 最后触发的保护类型
    double lastProtectionValue_;       // 最后触发的保护值
    
    /**
     * @brief 检查过压保护
     * @return 如果触发过压保护，返回true
     */
    bool checkOverVoltage();
    
    /**
     * @brief 检查过流保护
     * @return 如果触发过流保护，返回true
     */
    bool checkOverCurrent();
    
    /**
     * @brief 检查过功率保护
     * @return 如果触发过功率保护，返回true
     */
    bool checkOverPower();
    
    /**
     * @brief 检查过温保护
     * @return 如果触发过温保护，返回true
     */
    bool checkOverTemperature();
    
    /**
     * @brief 检查反接保护
     * @return 如果触发反接保护，返回true
     */
    bool checkReverseVoltage();
    
    /**
     * @brief 触发保护事件
     * @param type 保护类型
     * @param value 触发值
     * @param threshold 阈值
     */
    void triggerProtection(ProtectionType type, double value, double threshold);
    
    /**
     * @brief 读取温度传感器
     * @return 温度值(℃)
     */
    double readTemperature();
};
