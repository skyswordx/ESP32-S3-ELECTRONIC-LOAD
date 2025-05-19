/**
 * @file MCP4725Device.h
 * @brief MCP4725 DAC设备类
 * @author 重构
 * @date 2025-05-15
 * @details 基于MCP4725库的DAC设备实现
 */
#ifndef MCP4725_DEVICE_H
#define MCP4725_DEVICE_H

#include "Device.h"
#include <Arduino.h>
#include <Wire.h>
#include <MCP4725.h>

/**
 * @brief MCP4725 DAC设备类
 * @details 封装MCP4725 DAC，提供电压输出控制功能
 */
class MCP4725Device : public Device {
public:
    /**
     * @brief 构造函数
     * @param name 设备名称
     * @param address I2C地址
     * @param maxVoltage 最大输出电压
     * @param wire I2C通信实例
     */
    MCP4725Device(const String& name, uint8_t address = 0x60, float maxVoltage = 5.0, TwoWire* wire = &Wire);
    
    /**
     * @brief 析构函数
     */
    virtual ~MCP4725Device() = default;
    
    /**
     * @brief 初始化设备
     * @return 是否初始化成功
     */
    bool init() override;
    
    /**
     * @brief 更新设备状态
     */
    void update() override;
    
    /**
     * @brief 获取设备类型
     * @return 设备类型字符串
     */
    const char* getType() const override;
    
    /**
     * @brief 设置DAC输出电压
     * @param voltage 输出电压(V)
     * @return 操作是否成功
     */
    int setVoltage(float voltage);
    
    /**
     * @brief 获取当前设置的输出电压
     * @return 当前输出电压(V)
     */
    float getVoltage() const;
    
    /**
     * @brief 设置最大输出电压
     * @param maxVoltage 最大电压值
     */
    void setMaxVoltage(float maxVoltage);
    
    /**
     * @brief 获取设置的最大输出电压
     * @return 最大电压值
     */
    float getMaxVoltage() const;
    
    /**
     * @brief 设置原始DAC值(0-4095)
     * @param value 12位DAC值
     * @return 操作是否成功
     */
    int setValue(uint16_t value);
    
    /**
     * @brief 获取当前原始DAC值
     * @return 12位DAC值
     */
    uint16_t getValue() const;
    
    /**
     * @brief 设置输出为百分比值(0-100%)
     * @param percentage 百分比值
     * @return 操作是否成功
     */
    int setPercentage(float percentage);
    
    /**
     * @brief 获取当前输出百分比
     * @return 百分比值
     */
    float getPercentage() const;
    
    /**
     * @brief 将输出保存到EEPROM
     * @param value 要保存的值
     * @return 操作是否成功
     */
    int writeDAC(uint16_t value, bool EEPROM = true);
    
    /**
     * @brief 从EEPROM读取值
     * @return EEPROM中的值
     */
    uint16_t readEEPROM();
    
    /**
     * @brief 检查设备是否已准备好写入
     * @return 设备是否准备好
     */
    bool ready() const;
    
    /**
     * @brief 复位设备
     * @return 操作是否成功
     */
    int reset();
    
    /**
     * @brief 获取原始MCP4725对象引用
     * @return MCP4725对象的引用
     */
    MCP4725& getRawDevice();
    
private:
    MCP4725 mcp4725_;          ///< MCP4725实例
    uint8_t address_;          ///< I2C地址
    float maxVoltage_;         ///< 最大输出电压
    uint16_t lastValue_;       ///< 上次设置的DAC值
};

#endif // MCP4725_DEVICE_H