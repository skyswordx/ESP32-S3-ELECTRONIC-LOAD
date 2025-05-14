/**
 * @file TaskManager.cpp
 * @brief 任务管理器实现
 * @author Triwalt
 * @date 2025-05-06
 * @details 实现管理FreeRTOS任务的创建、调度和销毁的具体功能
 */

#include "TaskManager.h"

TaskManager& TaskManager::getInstance() {
    static TaskManager instance;
    return instance;
}

TaskManager::TaskManager() {
    // 创建互斥锁
    mutex_ = xSemaphoreCreateMutex();
    if (mutex_ == NULL) {
        Serial.println("Failed to create TaskManager mutex!");
    }
}

TaskManager::~TaskManager() {
    // 停止所有任务
    std::vector<TaskHandle_t> taskHandles;
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) == pdTRUE) {
        for (const auto& pair : tasks_) {
            taskHandles.push_back(pair.first);
        }
        xSemaphoreGive(mutex_);
    }
    
    // 停止所有任务
    for (const auto& handle : taskHandles) {
        stopTask(handle);
    }
    
    // 释放互斥锁
    if (mutex_ != NULL) {
        vSemaphoreDelete(mutex_);
        mutex_ = NULL;
    }
}

TaskHandle_t TaskManager::startTask(
    const String& name,
    TaskFunction_t function,
    void* params,
    uint32_t stackSize,
    UBaseType_t priority,
    BaseType_t coreId
) {
    TaskHandle_t handle = NULL;
    
    // 创建任务
    BaseType_t result = xTaskCreatePinnedToCore(
        function,
        name.c_str(),
        stackSize,
        params,
        priority,
        &handle,
        coreId
    );
    
    if (result != pdPASS || handle == NULL) {
        Serial.printf("Failed to create task: %s\n", name.c_str());
        return NULL;
    }
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) == pdTRUE) {
        tasks_[handle] = name;
        Serial.printf("Task created: %s (handle=%p)\n", name.c_str(), handle);
        xSemaphoreGive(mutex_);
    } else {
        Serial.println("Failed to take TaskManager mutex!");
        vTaskDelete(handle);
        return NULL;
    }
    
    return handle;
}

bool TaskManager::stopTask(TaskHandle_t handle) {
    if (handle == NULL) {
        return false;
    }
    
    String taskName = "";
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) == pdTRUE) {
        auto it = tasks_.find(handle);
        if (it != tasks_.end()) {
            taskName = it->second;
            tasks_.erase(it);
        }
        xSemaphoreGive(mutex_);
    } else {
        Serial.println("Failed to take TaskManager mutex!");
        return false;
    }
    
    if (!taskName.isEmpty()) {
        Serial.printf("Stopping task: %s (handle=%p)\n", taskName.c_str(), handle);
        vTaskDelete(handle);
        return true;
    }
    
    return false;
}

bool TaskManager::suspendTask(TaskHandle_t handle) {
    if (handle == NULL) {
        return false;
    }
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) == pdTRUE) {
        auto it = tasks_.find(handle);
        if (it != tasks_.end()) {
            xSemaphoreGive(mutex_);
            Serial.printf("Suspending task: %s (handle=%p)\n", it->second.c_str(), handle);
            vTaskSuspend(handle);
            return true;
        }
        xSemaphoreGive(mutex_);
    } else {
        Serial.println("Failed to take TaskManager mutex!");
    }
    
    return false;
}

bool TaskManager::resumeTask(TaskHandle_t handle) {
    if (handle == NULL) {
        return false;
    }
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) == pdTRUE) {
        auto it = tasks_.find(handle);
        if (it != tasks_.end()) {
            xSemaphoreGive(mutex_);
            Serial.printf("Resuming task: %s (handle=%p)\n", it->second.c_str(), handle);
            vTaskResume(handle);
            return true;
        }
        xSemaphoreGive(mutex_);
    } else {
        Serial.println("Failed to take TaskManager mutex!");
    }
    
    return false;
}

TaskStats TaskManager::getTaskStats(TaskHandle_t handle) {
    TaskStats stats;
    stats.name = "";
    
    if (handle == NULL) {
        return stats;
    }
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) == pdTRUE) {
        auto it = tasks_.find(handle);
        if (it != tasks_.end()) {
            stats.name = it->second;
        }
        xSemaphoreGive(mutex_);
    } else {
        Serial.println("Failed to take TaskManager mutex!");
        return stats;
    }
    
    // 获取任务信息
    if (!stats.name.isEmpty()) {
        // 使用单独的API替代vTaskGetInfo
        stats.priority = uxTaskPriorityGet(handle);
        stats.taskState = eTaskGetState(handle);
        stats.stackHighWater = uxTaskGetStackHighWaterMark(handle);
        
        // ESP32专用：获取运行此任务的核心ID
        stats.coreId = xTaskGetAffinity(handle);
        
        // 获取任务栈大小（FreeRTOS API未提供直接获取方式，这里使用默认值）
        stats.stackSize = 1024 * 4; // 默认值
    }
    
    return stats;
}

TaskHandle_t TaskManager::getTaskHandleByName(const String& name) {
    TaskHandle_t handle = NULL;
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) == pdTRUE) {
        for (const auto& pair : tasks_) {
            if (pair.second == name) {
                handle = pair.first;
                break;
            }
        }
        xSemaphoreGive(mutex_);
    } else {
        Serial.println("Failed to take TaskManager mutex!");
    }
    
    return handle;
}

std::vector<TaskStats> TaskManager::getAllTaskStats() {
    std::vector<TaskStats> allStats;
    std::vector<TaskHandle_t> taskHandles;
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) == pdTRUE) {
        for (const auto& pair : tasks_) {
            taskHandles.push_back(pair.first);
        }
        xSemaphoreGive(mutex_);
    } else {
        Serial.println("Failed to take TaskManager mutex!");
        return allStats;
    }
    
    // 获取每个任务的统计信息
    for (const auto& handle : taskHandles) {
        TaskStats stats = getTaskStats(handle);
        if (!stats.name.isEmpty()) {
            allStats.push_back(stats);
        }
    }
    
    return allStats;
}

void TaskManager::removeTaskRecord(TaskHandle_t handle) {
    if (handle == NULL) {
        return;
    }
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) == pdTRUE) {
        tasks_.erase(handle);
        xSemaphoreGive(mutex_);
    } else {
        Serial.println("Failed to take TaskManager mutex!");
    }
}