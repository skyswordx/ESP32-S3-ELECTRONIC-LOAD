/**
 * @file Task.h
 * @brief 任务基类定义
 * @author Triwalt
 * @date 2025-05-15
 * @details 所有任务类的公共接口
 */

#ifndef TASK_H
#define TASK_H

#include <Arduino.h>
#include "TaskManager.h"

/**
 * @brief 任务基类
 * @details 所有任务类的公共接口
 */
class Task {
public:
    /**
     * @brief 构造函数
     * @param name 任务名称
     * @param stackSize 栈大小
     * @param priority 优先级
     * @param coreId 运行的核心ID
     */
    Task(const String& name, 
         uint32_t stackSize = 1024 * 4, 
         UBaseType_t priority = 1, 
         BaseType_t coreId = tskNO_AFFINITY);
    
    /**
     * @brief 析构函数
     */
    virtual ~Task();
    
    /**
     * @brief 启动任务
     * @return 如果成功启动返回true，否则返回false
     */
    bool start();
    
    /**
     * @brief 停止任务
     * @return 如果成功停止返回true，否则返回false
     */
    bool stop();
    
    /**
     * @brief 暂停任务
     * @return 如果成功暂停返回true，否则返回false
     */
    bool suspend();
    
    /**
     * @brief 恢复任务
     * @return 如果成功恢复返回true，否则返回false
     */
    bool resume();
    
    /**
     * @brief 获取任务名称
     * @return 任务名称
     */
    String getName() const { return name_; }
    
    /**
     * @brief 获取任务句柄
     * @return 任务句柄
     */
    TaskHandle_t getHandle() const { return handle_; }
    
protected:
    /**
     * @brief 任务函数，由派生类实现
     */
    virtual void run() = 0;
    
    /**
     * @brief 静态任务包装函数
     * @param param 传递给任务的参数
     */
    static void taskFunction(void* param);
    
protected:
    String name_;             ///< 任务名称
    uint32_t stackSize_;      ///< 任务栈大小
    UBaseType_t priority_;    ///< 任务优先级
    BaseType_t coreId_;       ///< 任务运行的核心ID
    TaskHandle_t handle_;     ///< 任务句柄
};

#endif // TASK_H