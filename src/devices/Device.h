/**
 * @file Device.h
 * @brief 设备基类定义
 * @author Triwalt
 * @date 2025-05-15
 * @details 定义所有硬件设备的公共接口
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>

/**
 * @brief 设备基类
 * @details 所有硬件设备类的公共接口
 */
class Device {
public:
    /**
     * @brief 构造函数
     * @param name 设备名称
     */
    explicit Device(const String& name) : name_(name), initialized_(false) {}
    
    /**
     * @brief 析构函数
     */
    virtual ~Device() = default;
    
    /**
     * @brief 初始化设备
     * @return 如果初始化成功返回true，否则返回false
     */
    virtual bool init() = 0;
    
    /**
     * @brief 获取设备名称
     * @return 设备名称
     */
    String getName() const { return name_; }
    
    /**
     * @brief 检查设备是否已初始化
     * @return 如果设备已初始化返回true，否则返回false
     */
    bool isInitialized() const { return initialized_; }
    
protected:
    String name_;        ///< 设备名称
    bool initialized_;   ///< 设备初始化状态
};

#endif // DEVICE_H