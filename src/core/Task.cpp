/**
 * @file Task.cpp
 * @brief 任务基类实现
 * @author Triwalt
 * @date 2025-05-15
 * @details 实现所有任务类的公共功能
 */

#include "Task.h"

Task::Task(const String& name, 
           uint32_t stackSize, 
           UBaseType_t priority, 
           BaseType_t coreId)
    : name_(name),
      stackSize_(stackSize),
      priority_(priority),
      coreId_(coreId),
      handle_(NULL)
{
}

Task::~Task() {
    // 如果任务还在运行，先停止它
    if (handle_ != NULL) {
        stop();
    }
}

bool Task::start() {
    // 如果任务已经在运行，直接返回true
    if (handle_ != NULL) {
        return true;
    }
    
    // 创建任务
    BaseType_t result = xTaskCreatePinnedToCore(
        taskFunction,       // 任务函数
        name_.c_str(),      // 任务名称
        stackSize_,         // 栈大小
        this,               // 传递给任务函数的参数
        priority_,          // 任务优先级
        &handle_,           // 任务句柄
        coreId_             // 运行的核心ID
    );
    
    if (result != pdPASS || handle_ == NULL) {
        Serial.printf("Failed to create task: %s\n", name_.c_str());
        return false;
    }
    
    Serial.printf("Task '%s' started successfully (handle=%p)\n", name_.c_str(), handle_);
    return true;
}

bool Task::stop() {
    if (handle_ == NULL) {
        return false;
    }
    
    // 删除任务
    vTaskDelete(handle_);
    handle_ = NULL;
    
    Serial.printf("Task '%s' stopped\n", name_.c_str());
    return true;
}

bool Task::suspend() {
    if (handle_ == NULL) {
        return false;
    }
    
    // 暂停任务
    vTaskSuspend(handle_);
    Serial.printf("Task '%s' suspended\n", name_.c_str());
    return true;
}

bool Task::resume() {
    if (handle_ == NULL) {
        return false;
    }
    
    // 恢复任务
    vTaskResume(handle_);
    Serial.printf("Task '%s' resumed\n", name_.c_str());
    return true;
}

void Task::taskFunction(void* param) {
    // 将参数转换为Task对象
    Task* task = static_cast<Task*>(param);
    if (task) {
        // 调用任务的run方法
        task->run();
    }
    // 如果任务函数退出，自动删除任务
    vTaskDelete(NULL);
}