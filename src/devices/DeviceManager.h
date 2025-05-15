/**
 * @file DeviceManager.h
 * @brief 设备管理器定义
 * @author Triwalt
 * @date 2025-05-15
 * @details 管理系统中所有设备的注册和访问
 */

#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include "Device.h"
#include <map>
#include <memory>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

/**
 * @brief 设备管理器类
 * @details 管理系统中所有设备的注册和访问，采用单例模式
 */
class DeviceManager {
public:
    /**
     * @brief 获取单例实例
     * @return DeviceManager单例引用
     */
    static DeviceManager& getInstance();
    
    /**
     * @brief 注册设备
     * @param device 设备指针
     * @return 如果注册成功返回true，否则返回false
     */
    bool registerDevice(std::shared_ptr<Device> device);
    
    /**
     * @brief 根据名称获取设备
     * @param name 设备名称
     * @return 设备指针，如果未找到返回nullptr
     */
    std::shared_ptr<Device> getDevice(const String& name);
    
    /**
     * @brief 初始化所有设备
     * @return 如果所有设备初始化成功返回true，否则返回false
     */
    bool initializeAll();
    
private:
    /**
     * @brief 构造函数(私有)
     */
    DeviceManager();
    
    /**
     * @brief 析构函数(私有)
     */
    ~DeviceManager();
    
    /**
     * @brief 禁用拷贝构造函数
     */
    DeviceManager(const DeviceManager&) = delete;
    
    /**
     * @brief 禁用赋值操作符
     */
    DeviceManager& operator=(const DeviceManager&) = delete;
    
private:
    std::map<String, std::shared_ptr<Device>> devices_; ///< 名称到设备的映射
    SemaphoreHandle_t mutex_; ///< 互斥锁
};

#endif // DEVICE_MANAGER_H