/**
 * @file INA226Device.h
 * @brief INA226电流传感器设备类
 * @author 重构
 * @date 2025-05-15
 * @details 基于INA226库的电流传感器设备实现
 */
#ifndef INA226_DEVICE_H
#define INA226_DEVICE_H

#include "Device.h"
#include <Arduino.h>
#include <Wire.h>
#include <INA226.h>

/**
 * @brief INA226电流传感器设备类
 * @details 封装INA226电流传感器，提供电流、电压、功率测量功能
 */
class INA226Device : public Device {
public:
    /**
     * @brief 构造函数
     * @param name 设备名称
     * @param address INA226的I2C地址
     * @param shunt 分流电阻值(欧姆)
     * @param wire I2C通信实例
     */
    INA226Device(const String& name, uint8_t address = 0x40, float shunt = 0.020, TwoWire* wire = &Wire);
    
    /**
     * @brief 析构函数
     */
    virtual ~INA226Device() = default;
    
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
     * @brief 校准传感器
     * @param current_LSB_mA 电流LSB值(毫安)
     * @param current_zero_offset_mA 电流零点偏移(毫安)
     * @param bus_V_scaling_e4 电压缩放系数
     * @return 校准是否成功
     */
    bool calibrate(float current_LSB_mA = 0.05, float current_zero_offset_mA = -0.525, uint16_t bus_V_scaling_e4 = 9847);
    
    /**
     * @brief 设置电流校准系数
     * @param a 系数A (斜率)
     * @param b 系数B (偏移)
     */
    void setCurrentCalibration(double a, double b);
    
    /**
     * @brief 设置电压校准系数
     * @param a 系数A (斜率)
     * @param b 系数B (偏移)
     */
    void setVoltageCalibration(double a, double b);
    
    /**
     * @brief 设置转换时间
     * @param samples 采样次数
     */
    void setConversionTime(uint8_t samples);
    
    /**
     * @brief 获取电流值(毫安)
     * @return 校准后的电流值
     */
    double getCurrent();
    
    /**
     * @brief 获取电压值(伏特)
     * @return 校准后的电压值
     */
    double getVoltage();
    
    /**
     * @brief 获取功率值(瓦特)
     * @return 计算的功率值
     */
    double getPower();
    
    /**
     * @brief 获取电阻值(欧姆)
     * @return 计算的电阻值
     */
    double getResistance();
    
    /**
     * @brief 获取原始INA226对象引用
     * @return INA226对象的引用
     */
    INA226& getRawDevice();
    
    /**
     * @brief 检查是否已校准
     * @return 是否已校准
     */
    bool isCalibrated() const;
    
    /**
     * @brief 重置设备
     * @return 是否重置成功
     */
    bool reset();
    
private:
    INA226 ina226_;                ///< INA226实例
    uint8_t address_;              ///< I2C地址
    float shunt_;                  ///< 分流电阻值(欧姆)
    bool calibrated_;              ///< 是否已校准
    double currentCalibrationA_;   ///< 电流校准系数A
    double currentCalibrationB_;   ///< 电流校准系数B
    double voltageCalibrationA_;   ///< 电压校准系数A
    double voltageCalibrationB_;   ///< 电压校准系数B
    double lastCurrent_;           ///< 上次电流值
    double lastVoltage_;           ///< 上次电压值
    double lastPower_;             ///< 上次功率值
    double lastResistance_;        ///< 上次电阻值
};

#endif // INA226_DEVICE_H