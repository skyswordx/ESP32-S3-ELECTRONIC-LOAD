/**
 * @file callback_subscriber.h
 * @brief 基于回调的话题订阅者接口
 * @author AI Assistant
 * @date 2025-05-19
 * @details 提供基于回调函数的话题订阅机制
 */

#ifndef CALLBACK_SUBSCRIBER_H
#define CALLBACK_SUBSCRIBER_H

#include "common.h"
#include <functional>
#include <vector>

namespace OUR {

// 定义回调函数类型
template<typename T>
using MessageCallback = std::function<void(const T&, task_id_t, data_description_t)>;

// 定义过滤器类型
enum class FilterType {
    NONE,           // 不过滤，接收所有消息
    TASK_ID,        // 按任务ID过滤
    DATA_DESC,      // 按数据描述过滤
    CUSTOM          // 自定义过滤器
};

/**
 * @brief 回调订阅者类模板
 * @tparam T 订阅的数据类型
 */
template<typename T>
class CallbackSubscriber {
public:
    // 自定义过滤器函数类型
    using FilterFunction = std::function<bool(const T&, task_id_t, data_description_t)>;
    
private:
    std::string topicName;                  // 话题名称
    QueueHandle_t queueHandle;              // 队列句柄
    TaskHandle_t processorTaskHandle;       // 处理任务句柄
    
    // 替换 std::mutex 为 FreeRTOS 互斥信号量
    SemaphoreHandle_t subscriptionMutex;    // 保护订阅列表的互斥锁
    
    bool isRunning;                                    // 是否运行中
    uint32_t processingInterval;                       // 处理间隔(毫秒)
    
    /**
     * @brief 处理器任务函数
     * @param param CallbackSubscriber对象指针
     */
    static void processorTask(void* param) {
        CallbackSubscriber<T>* self = static_cast<CallbackSubscriber<T>*>(param);
        
        QueueElement_t<T> element;
        TickType_t lastWakeTime = xTaskGetTickCount();
        
        // 每隔processingInterval毫秒检查一次队列
        while (self->isRunning) {
            // 非阻塞方式检查队列
            while (xQueueReceive(self->queueHandle, &element, 0) == pdTRUE) {
                self->dispatchMessage(element);
            }
            
            // 精确延时，确保CPU可以被其他任务使用
            vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(self->processingInterval));
        }
        
        vTaskDelete(NULL);
    }
    
    /**
     * @brief 分发消息到匹配的回调函数
     * @param element 接收的队列元素
     */
    void dispatchMessage(const QueueElement_t<T>& element) {
        // 替换 std::lock_guard 为手动获取/释放互斥锁
        if (xSemaphoreTake(subscriptionMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取订阅互斥锁");
            return;
        }
        
        for (const auto& sub : subscriptions) {
            if (!sub.active) continue;
            
            bool shouldCall = false;
            
            // 根据过滤类型决定是否调用回调
            switch (sub.filterType) {
                case FilterType::NONE:
                    shouldCall = true;
                    break;
                    
                case FilterType::TASK_ID:
                    shouldCall = (element.task_id == sub.taskIdFilter);
                    break;
                    
                case FilterType::DATA_DESC:
                    shouldCall = (element.data_description == sub.dataDescFilter);
                    break;
                    
                case FilterType::CUSTOM:
                    if (sub.customFilter) {
                        shouldCall = sub.customFilter(
                            element.data, 
                            element.task_id, 
                            element.data_description
                        );
                    }
                    break;
            }
            
            if (shouldCall && sub.callback) {
                // 调用回调函数
                sub.callback(element.data, element.task_id, element.data_description);
            }
        }
        
        // 释放互斥锁
        xSemaphoreGive(subscriptionMutex);
    }
    
public:
    /**
     * @brief 构造函数
     * @param topic 话题名称
     * @param handle 队列句柄
     * @param interval 处理间隔(毫秒)
     */
    CallbackSubscriber(const std::string& topic, QueueHandle_t handle, uint32_t interval = 10)
        : topicName(topic), queueHandle(handle), processorTaskHandle(nullptr),
          isRunning(false), processingInterval(interval) {
        // 创建互斥信号量
        subscriptionMutex = xSemaphoreCreateMutex();
        if (!subscriptionMutex) {
            Serial.println("错误: 无法创建订阅互斥锁");
        }
    }
    
    /**
     * @brief 析构函数
     */
    ~CallbackSubscriber() {
        stop();
        // 删除互斥信号量
        if (subscriptionMutex) {
            vSemaphoreDelete(subscriptionMutex);
            subscriptionMutex = nullptr;
        }
    }
    
    /**
     * @brief 开始处理消息
     * @param stackSize 任务栈大小
     * @param priority 任务优先级
     * @param core 运行的核心
     * @return 是否成功启动
     */
    bool start(uint32_t stackSize = 4096, UBaseType_t priority = 1, BaseType_t core = tskNO_AFFINITY) {
        if (isRunning) return true;
        
        isRunning = true;
        
        // 创建处理任务
        BaseType_t result = xTaskCreatePinnedToCore(
            processorTask,
            ("SubProc_" + topicName).c_str(),
            stackSize,
            this,
            priority,
            &processorTaskHandle,
            core
        );
        
        if (result != pdPASS) {
            isRunning = false;
            return false;
        }
        
        return true;
    }
    
    /**
     * @brief 停止处理消息
     */
    void stop() {
        isRunning = false;
        
        // 等待任务退出
        if (processorTaskHandle) {
            vTaskDelay(pdMS_TO_TICKS(processingInterval * 2));
            processorTaskHandle = nullptr;
        }
    }
    
    /**
     * @brief 添加不带过滤器的订阅
     * @param callback 回调函数
     * @return 订阅ID(用于后续修改或删除)
     */
    int subscribe(MessageCallback<T> callback) {
        // 替换 std::lock_guard 为手动获取/释放互斥锁
        if (xSemaphoreTake(subscriptionMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取订阅互斥锁");
            return -1;
        }
        
        subscriptions.emplace_back(callback);
        int result = subscriptions.size() - 1;
        
        // 释放互斥锁
        xSemaphoreGive(subscriptionMutex);
        
        return result;
    }
    
    /**
     * @brief 添加按任务ID过滤的订阅
     * @param callback 回调函数
     * @param taskId 过滤的任务ID
     * @return 订阅ID
     */
    int subscribeTaskId(MessageCallback<T> callback, task_id_t taskId) {
        if (xSemaphoreTake(subscriptionMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取订阅互斥锁");
            return -1;
        }
        
        SubscriptionInfo info(callback);
        info.filterType = FilterType::TASK_ID;
        info.taskIdFilter = taskId;
        
        subscriptions.push_back(info);
        int result = subscriptions.size() - 1;
        
        xSemaphoreGive(subscriptionMutex);
        
        return result;
    }
    
    /**
     * @brief 添加按数据描述过滤的订阅
     * @param callback 回调函数
     * @param dataDesc 过滤的数据描述
     * @return 订阅ID
     */
    int subscribeDataDesc(MessageCallback<T> callback, data_description_t dataDesc) {
        if (xSemaphoreTake(subscriptionMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取订阅互斥锁");
            return -1;
        }
        
        SubscriptionInfo info(callback);
        info.filterType = FilterType::DATA_DESC;
        info.dataDescFilter = dataDesc;
        
        subscriptions.push_back(info);
        int result = subscriptions.size() - 1;
        
        xSemaphoreGive(subscriptionMutex);
        
        return result;
    }
    
    /**
     * @brief 添加自定义过滤器的订阅
     * @param callback 回调函数
     * @param filter 自定义过滤器函数
     * @return 订阅ID
     */
    int subscribeCustom(MessageCallback<T> callback, FilterFunction filter) {
        if (xSemaphoreTake(subscriptionMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取订阅互斥锁");
            return -1;
        }
        
        SubscriptionInfo info(callback);
        info.filterType = FilterType::CUSTOM;
        info.customFilter = filter;
        
        subscriptions.push_back(info);
        int result = subscriptions.size() - 1;
        
        xSemaphoreGive(subscriptionMutex);
        
        return result;
    }
    
    /**
     * @brief 取消订阅
     * @param subscriptionId 订阅ID
     * @return 是否成功取消
     */
    bool unsubscribe(int subscriptionId) {
        if (xSemaphoreTake(subscriptionMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取订阅互斥锁");
            return false;
        }
        
        bool result = false;
        if (subscriptionId >= 0 && subscriptionId < subscriptions.size()) {
            subscriptions[subscriptionId].active = false;
            result = true;
        }
        
        xSemaphoreGive(subscriptionMutex);
        
        return result;
    }
    
    /**
     * @brief 获取话题名称
     * @return 话题名称
     */
    std::string getTopic() const {
        return topicName;
    }
    
    /**
     * @brief 获取活跃订阅数量
     * @return 活跃订阅数量
     */
    size_t getActiveSubscriptionsCount() const {
        if (xSemaphoreTake(subscriptionMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取订阅互斥锁");
            return 0;
        }
        
        size_t count = 0;
        for (const auto& sub : subscriptions) {
            if (sub.active) count++;
        }
        
        xSemaphoreGive(subscriptionMutex);
        
        return count;
    }
    
    /**
     * @brief 设置处理间隔
     * @param interval 处理间隔(毫秒)
     */
    void setProcessingInterval(uint32_t interval) {
        processingInterval = interval;
    }
};

/**
 * @brief 创建回调订阅者对象
 * @tparam T 订阅的数据类型
 * @param topicName 话题名称
 * @param interval 处理间隔(毫秒)
 * @param autoStart 是否自动启动处理任务
 * @return 回调订阅者对象指针
 */
template<typename T>
std::shared_ptr<CallbackSubscriber<T>> createCallbackSubscriber(
    const std::string& topicName,
    uint32_t interval = 10,
    bool autoStart = true
) {
    // 尝试获取互斥锁
    if (xSemaphoreTake(g_topicQueuesMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Serial.println("警告: 无法获取话题互斥锁");
        return nullptr;
    }
    
    // 检查话题是否存在
    QueueHandle_t queueHandle = nullptr;
    auto it = g_topicQueues.find(topicName);
    if (it == g_topicQueues.end()) {
        Serial.printf("错误: 话题'%s'不存在，无法订阅\n", topicName.c_str());
        xSemaphoreGive(g_topicQueuesMutex);
        return nullptr;
    }
    
    queueHandle = it->second;
    xSemaphoreGive(g_topicQueuesMutex);
    
    // 创建回调订阅者
    auto subscriber = std::make_shared<CallbackSubscriber<T>>(topicName, queueHandle, interval);
    
    // 如果需要自动启动
    if (autoStart) {
        subscriber->start();
    }
    
    return subscriber;
}

} // namespace OUR

#endif // CALLBACK_SUBSCRIBER_H
