/**
 * @file INA226Device.cpp
 * @brief INA226电流传感器设备类实现
 * @author 重构
 * @date 2025-05-15
 * @details 基于INA226库的电流传感器设备实现
 */
#include "INA226Device.h"

// 常见的INA226采样设置枚举值
#define INA226_AVERAGE_1_SAMPLE   0
#define INA226_AVERAGE_4_SAMPLES  1
#define INA226_AVERAGE_16_SAMPLES 2
#define INA226_AVERAGE_64_SAMPLES 3
#define INA226_AVERAGE_128_SAMPLES 4
#define INA226_AVERAGE_256_SAMPLES 5
#define INA226_AVERAGE_512_SAMPLES 6
#define INA226_AVERAGE_1024_SAMPLES 7

// 常见的INA226转换时间枚举值
#define INA226_CONVERSION_140US   0
#define INA226_CONVERSION_204US   1
#define INA226_CONVERSION_332US   2
#define INA226_CONVERSION_588US   3
#define INA226_CONVERSION_1100US  4
#define INA226_CONVERSION_2116US  5
#define INA226_CONVERSION_4156US  6
#define INA226_CONVERSION_8244US  7

INA226Device::INA226Device(const String& name, uint8_t address, float shunt, TwoWire* wire)
    : Device(name),
      ina226_(address, wire),
      address_(address),
      shunt_(shunt),
      calibrated_(false),
      currentCalibrationA_(0.97270047847776),   // 默认电流校准系数A
      currentCalibrationB_(-0.0127991189954173), // 默认电流校准系数B
      voltageCalibrationA_(1.01509505246458),    // 默认电压校准系数A
      voltageCalibrationB_(0.00254773054846081), // 默认电压校准系数B
      lastCurrent_(0.0),
      lastVoltage_(0.0),
      lastPower_(0.0),
      lastResistance_(0.0) {
}

bool INA226Device::init() {
    if (!ina226_.begin()) {
        Serial.printf("[INA226Device] Error: Could not connect to INA226 at address 0x%02X\n", address_);
        return false;
    }
    
    // 默认校准设置 - 使用默认参数
    calibrated_ = calibrate();
    
    // 设置转换时间和采样数
    setConversionTime(INA226_AVERAGE_16_SAMPLES);
    
    // 设置传感器为连续监测模式
    ina226_.setModeShuntBusContinuous();
    
    Serial.printf("[INA226Device] INA226 initialized successfully at address 0x%02X\n", address_);
    return true;
}

void INA226Device::update() {
    // 更新传感器读数
    lastCurrent_ = getCurrent();
    lastVoltage_ = getVoltage();
    lastPower_ = getPower();
    lastResistance_ = getResistance();
}

const char* INA226Device::getType() const {
    return "INA226";
}

bool INA226Device::calibrate(float current_LSB_mA, float current_zero_offset_mA, uint16_t bus_V_scaling_e4) {
    if (ina226_.configure(shunt_, current_LSB_mA, current_zero_offset_mA, bus_V_scaling_e4)) {
        Serial.println("[INA226Device] Error: INA226 calibration failed! Values outside allowed range.");
        calibrated_ = false;
        return false;
    }
    
    // 设置电压和电流转换时间为最长(提供最高精度)
    ina226_.setShuntVoltageConversionTime(INA226_CONVERSION_8244US);
    ina226_.setBusVoltageConversionTime(INA226_CONVERSION_8244US);
    
    calibrated_ = true;
    Serial.println("[INA226Device] INA226 calibrated successfully");
    return true;
}

void INA226Device::setCurrentCalibration(double a, double b) {
    currentCalibrationA_ = a;
    currentCalibrationB_ = b;
}

void INA226Device::setVoltageCalibration(double a, double b) {
    voltageCalibrationA_ = a;
    voltageCalibrationB_ = b;
}

void INA226Device::setConversionTime(uint8_t samples) {
    if (!ina226_.setAverage(samples)) {
        Serial.println("[INA226Device] Error: Invalid sample count value");
    }
}

double INA226Device::getCurrent() {
    if (!calibrated_) {
        return 0.0;
    }
    
    // 获取校准后的电流值(mA)
    double current = currentCalibrationA_ * ina226_.getCurrent_mA() + currentCalibrationB_;
    return current;
}

double INA226Device::getVoltage() {
    // 获取校准后的电压值(V)
    double voltage = voltageCalibrationA_ * ina226_.getBusVoltage() + voltageCalibrationB_;
    return voltage;
}

double INA226Device::getPower() {
    // 从电压和电流计算功率(W)
    double current_mA = getCurrent();
    double voltage_V = getVoltage();
    return (current_mA * voltage_V) / 1000.0;
}

double INA226Device::getResistance() {
    double current_mA = getCurrent();
    double voltage_V = getVoltage();
    
    // 避免除零错误
    if (abs(current_mA) < 0.001) {
        return 0.0;
    }
    
    // 计算电阻(Ω)
    return (voltage_V * 1000.0) / current_mA;
}

INA226& INA226Device::getRawDevice() {
    return ina226_;
}

bool INA226Device::isCalibrated() const {
    // 检查设备是否已经进行了校准
    // 注意：由于原库中isCalibrated()不是const方法，我们这里只能使用自己的标志位
    return calibrated_;
}

bool INA226Device::reset() {
    return ina226_.reset();
}