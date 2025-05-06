/**
 * @file TaskManager.h
 * @brief 任务管理器定义
 * @author Triwalt
 * @date 2025-05-06
 * @details 管理FreeRTOS任务的创建、调度和销毁
 */

#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <map>
#include <string>
#include <vector>

/**
 * @brief 任务统计信息结构体
 */
struct TaskStats {
    String name;              ///< 任务名称
    UBaseType_t priority;     ///< 任务优先级
    uint32_t stackSize;       ///< 任务栈大小
    uint32_t stackHighWater;  ///< 任务栈高水位
    eTaskState taskState;     ///< 任务状态
    BaseType_t coreId;        ///< 任务运行的核心ID
};

/**
 * @brief 任务管理器类
 */
class TaskManager {
public:
    /**
     * @brief 获取TaskManager单例
     * @return TaskManager单例引用
     */
    static TaskManager& getInstance();
    
    /**
     * @brief 创建并启动一个任务
     * @param name 任务名称
     * @param function 任务函数指针
     * @param params 任务参数
     * @param stackSize 任务栈大小
     * @param priority 任务优先级
     * @param coreId 任务运行的核心ID（0或1）
     * @return 任务句柄，如果创建失败则返回NULL
     */
    TaskHandle_t startTask(
        const String& name,
        TaskFunction_t function,
        void* params,
        uint32_t stackSize = 1024 * 4,
        UBaseType_t priority = 1,
        BaseType_t coreId = tskNO_AFFINITY
    );
    
    /**
     * @brief 停止并删除一个任务
     * @param handle 任务句柄
     * @return 如果成功删除返回true，否则返回false
     */
    bool stopTask(TaskHandle_t handle);
    
    /**
     * @brief 暂停一个任务
     * @param handle 任务句柄
     * @return 如果成功暂停返回true，否则返回false
     */
    bool suspendTask(TaskHandle_t handle);
    
    /**
     * @brief 恢复一个任务
     * @param handle 任务句柄
     * @return 如果成功恢复返回true，否则返回false
     */
    bool resumeTask(TaskHandle_t handle);
    
    /**
     * @brief 根据任务句柄获取任务统计信息
     * @param handle 任务句柄
     * @return 任务统计信息
     */
    TaskStats getTaskStats(TaskHandle_t handle);
    
    /**
     * @brief 根据任务名称获取任务句柄
     * @param name 任务名称
     * @return 任务句柄，如果未找到则返回NULL
     */
    TaskHandle_t getTaskHandleByName(const String& name);
    
    /**
     * @brief 获取所有任务的统计信息
     * @return 包含所有任务统计信息的vector
     */
    std::vector<TaskStats> getAllTaskStats();
    
private:
    /**
     * @brief 构造函数(私有)
     */
    TaskManager();
    
    /**
     * @brief 析构函数(私有)
     */
    ~TaskManager();
    
    /**
     * @brief 禁用拷贝构造函数
     */
    TaskManager(const TaskManager&) = delete;
    
    /**
     * @brief 禁用赋值操作符
     */
    TaskManager& operator=(const TaskManager&) = delete;
    
    /**
     * @brief 从任务管理器中移除一个任务记录
     * @param handle 任务句柄
     */
    void removeTaskRecord(TaskHandle_t handle);

private:
    std::map<TaskHandle_t, String> tasks_; ///< 任务句柄到名称的映射
    SemaphoreHandle_t mutex_;             ///< 互斥锁，保护任务列表的线程安全
};

#endif // TASK_MANAGER_H