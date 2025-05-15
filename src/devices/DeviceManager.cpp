/**
 * @file DeviceManager.cpp
 * @brief 设备管理器实现
 * @author Triwalt
 * @date 2025-05-15
 * @details 实现设备管理器的各项功能
 */

#include "DeviceManager.h"
#include <Arduino.h>

DeviceManager& DeviceManager::getInstance() {
    static DeviceManager instance;
    return instance;
}

DeviceManager::DeviceManager() {
    // 创建互斥锁
    mutex_ = xSemaphoreCreateMutex();
    if (mutex_ == NULL) {
        Serial.println("Failed to create DeviceManager mutex!");
    }
}

DeviceManager::~DeviceManager() {
    // 释放互斥锁
    if (mutex_ != NULL) {
        vSemaphoreDelete(mutex_);
        mutex_ = NULL;
    }
    
    // 清空设备列表（智能指针会自动释放内存）
    devices_.clear();
}

bool DeviceManager::registerDevice(std::shared_ptr<Device> device) {
    if (!device) {
        Serial.println("DeviceManager: Cannot register null device!");
        return false;
    }
    
    String deviceName = device->getName();
    if (deviceName.isEmpty()) {
        Serial.println("DeviceManager: Device name cannot be empty!");
        return false;
    }
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE) {
        Serial.println("Failed to take DeviceManager mutex!");
        return false;
    }
    
    // 检查设备名称是否已存在
    auto it = devices_.find(deviceName);
    if (it != devices_.end()) {
        Serial.printf("DeviceManager: Device with name '%s' already exists!\n", deviceName.c_str());
        xSemaphoreGive(mutex_);
        return false;
    }
    
    // 添加设备到列表
    devices_[deviceName] = device;
    Serial.printf("DeviceManager: Device '%s' registered successfully\n", deviceName.c_str());
    xSemaphoreGive(mutex_);
    return true;
}

std::shared_ptr<Device> DeviceManager::getDevice(const String& name) {
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE) {
        Serial.println("Failed to take DeviceManager mutex!");
        return nullptr;
    }
    
    // 查找设备
    auto it = devices_.find(name);
    if (it != devices_.end()) {
        xSemaphoreGive(mutex_);
        return it->second;
    }
    
    xSemaphoreGive(mutex_);
    return nullptr;
}

bool DeviceManager::initializeAll() {
    bool allSuccess = true;
    std::map<String, std::shared_ptr<Device>> devicesCopy;
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE) {
        Serial.println("Failed to take DeviceManager mutex!");
        return false;
    }
    
    // 复制设备列表，以便在不持有锁的情况下初始化设备
    devicesCopy = devices_;
    xSemaphoreGive(mutex_);
    
    // 初始化所有设备
    for (auto& pair : devicesCopy) {
        Serial.printf("DeviceManager: Initializing device '%s'...\n", pair.first.c_str());
        if (!pair.second->init()) {
            Serial.printf("DeviceManager: Failed to initialize device '%s'!\n", pair.first.c_str());
            allSuccess = false;
        } else {
            Serial.printf("DeviceManager: Device '%s' initialized successfully\n", pair.first.c_str());
        }
    }
    
    return allSuccess;
}