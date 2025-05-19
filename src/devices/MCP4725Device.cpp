/**
 * @file MCP4725Device.cpp
 * @brief MCP4725 DAC设备类实现
 * @author 重构
 * @date 2025-05-15
 * @details 基于MCP4725库的DAC设备实现
 */

#include "MCP4725Device.h"

// MCP4725状态代码
#define MCP4725_OK              0
#define MCP4725_VALUE_ERROR     -999
#define MCP4725_REG_ERROR       -998
#define MCP4725_NOT_CONNECTED   -997
#define MCP4725_MAXVALUE        4095

MCP4725Device::MCP4725Device(const String& name, uint8_t address, float maxVoltage, TwoWire* wire)
    : Device(name),
      mcp4725_(address, wire),
      address_(address),
      maxVoltage_(maxVoltage),
      lastValue_(0)
{
}

bool MCP4725Device::init() {
    // 检查连接状态
    if (!mcp4725_.begin()) {
        Serial.printf("[MCP4725Device] Error: Could not connect to MCP4725 at address 0x%02X\n", address_);
        return false;
    }
    
    // 配置最大电压
    mcp4725_.setMaxVoltage(maxVoltage_);
    
    // 设置初始输出为0V
    mcp4725_.setValue(0);
    lastValue_ = 0;
    
    Serial.printf("[MCP4725Device] MCP4725 initialized successfully at address 0x%02X\n", address_);
    Serial.printf("[MCP4725Device] Maximum output voltage: %.3f V\n", maxVoltage_);
    
    return true;
}

void MCP4725Device::update() {
    // 读取当前DAC值，更新缓存
    lastValue_ = mcp4725_.getValue();
}

const char* MCP4725Device::getType() const {
    return "MCP4725";
}

int MCP4725Device::setVoltage(float voltage) {
    // 限制电压范围
    if (voltage < 0) {
        voltage = 0;
    } else if (voltage > maxVoltage_) {
        voltage = maxVoltage_;
    }
    
    // 设置电压
    int result = mcp4725_.setVoltage(voltage);
    if (result == MCP4725_OK) {
        // 更新缓存
        lastValue_ = mcp4725_.getValue();
    }
    
    return result;
}

float MCP4725Device::getVoltage() const {
    // 因为原库中getVoltage()不是const，我们返回基于最大电压和缓存的DAC值计算的电压
    return (lastValue_ * maxVoltage_) / MCP4725_MAXVALUE;
}

void MCP4725Device::setMaxVoltage(float maxVoltage) {
    maxVoltage_ = maxVoltage;
    mcp4725_.setMaxVoltage(maxVoltage);
}

float MCP4725Device::getMaxVoltage() const {
    return maxVoltage_;
}

int MCP4725Device::setValue(uint16_t value) {
    // 设置DAC值
    int result = mcp4725_.setValue(value);
    if (result == MCP4725_OK) {
        // 更新缓存
        lastValue_ = value;
    }
    
    return result;
}

uint16_t MCP4725Device::getValue() const {
    // 返回缓存的DAC值，避免调用非const方法
    return lastValue_;
}

int MCP4725Device::setPercentage(float percentage) {
    // 设置百分比
    int result = mcp4725_.setPercentage(percentage);
    if (result == MCP4725_OK) {
        // 更新缓存
        lastValue_ = mcp4725_.getValue();
    }
    
    return result;
}

float MCP4725Device::getPercentage() const {
    // 基于缓存的DAC值计算百分比
    return lastValue_ * (100.0f / MCP4725_MAXVALUE);
}

int MCP4725Device::writeDAC(uint16_t value, bool EEPROM) {
    // 写入DAC和EEPROM
    int result = mcp4725_.writeDAC(value, EEPROM);
    if (result == MCP4725_OK) {
        // 更新缓存
        lastValue_ = value;
    }
    
    return result;
}

uint16_t MCP4725Device::readEEPROM() {
    return mcp4725_.readEEPROM();
}

bool MCP4725Device::ready() const {
    // 简单返回true，真正的写入操作会在非const方法中处理
    // 在我们的架构中，const方法不应该修改设备状态，所以这里假设设备总是就绪的
    return true;
}

int MCP4725Device::reset() {
    // 复位设备
    int result = mcp4725_.powerOnReset();
    if (result == MCP4725_OK) {
        // 更新缓存
        lastValue_ = mcp4725_.getValue();
    }
    
    return result;
}

MCP4725& MCP4725Device::getRawDevice() {
    return mcp4725_;
}