/**
 * @file filter_queue.h
 * @brief 消息过滤队列基础类，用于创建消息过滤和分发机制
 * @author AI Assistant
 * @date 2025-05-19
 * @details 提供将消息根据条件分发到不同子队列的基础框架
 */

#ifndef FILTER_QUEUE_H
#define FILTER_QUEUE_H

#include "common.h"
#include <map>
#include <string>

namespace OUR {

/**
 * @brief 消息过滤器基类模板
 * @tparam T 消息数据类型
 */
template<typename T>
class FilterQueueBase {
protected:
    std::string sourceTopic;              // 源话题名称
    QueueHandle_t sourceQueue;           // 源队列句柄
    TaskHandle_t filterTaskHandle;        // 过滤任务句柄
    
    bool isRunning;                       // 是否运行中
    uint32_t processingInterval;          // 处理间隔(毫秒)
    
    // 子队列映射表，保存创建的子队列
    struct QueueInfo {
        QueueHandle_t handle;            // 队列句柄
        size_t maxItemSize;              // 最大消息大小
        UBaseType_t queueLength;         // 队列长度
    };
    
    std::map<std::string, QueueInfo> subQueues;     // 子队列信息
    SemaphoreHandle_t queuesMutex;                 // 保护子队列映射表的互斥锁
    
    /**
     * @brief 子类必须实现的分发逻辑
     * @param element 接收到的消息
     * @return 是否成功分发
     */
    virtual bool dispatchMessage(const QueueElement_t<T>& element) = 0;
    
    /**
     * @brief 过滤器任务函数
     * @param param FilterQueueBase对象指针
     */
    static void filterTask(void* param) {
        FilterQueueBase<T>* self = static_cast<FilterQueueBase<T>*>(param);
        
        QueueElement_t<T> element;
        TickType_t lastWakeTime = xTaskGetTickCount();
        
        // 每隔processingInterval毫秒检查一次队列
        while (self->isRunning) {
            // 非阻塞方式检查队列
            while (xQueueReceive(self->sourceQueue, &element, 0) == pdTRUE) {
                self->dispatchMessage(element);
            }
            
            // 精确延时，确保CPU可以被其他任务使用
            vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(self->processingInterval));
        }
        
        vTaskDelete(NULL);
    }
    
public:
    /**
     * @brief 构造函数
     * @param topic 源话题名称
     * @param interval 处理间隔(毫秒)
     */
    FilterQueueBase(const std::string& topic, uint32_t interval = 10)
        : sourceTopic(topic), sourceQueue(nullptr), filterTaskHandle(nullptr),
          isRunning(false), processingInterval(interval) {
        // 创建互斥锁
        queuesMutex = xSemaphoreCreateMutex();
        if (!queuesMutex) {
            Serial.println("错误: 无法创建过滤队列互斥锁");
        }
    }
    
    /**
     * @brief 析构函数
     */
    virtual ~FilterQueueBase() {
        stop();
        
        // 删除所有子队列
        if (xSemaphoreTake(queuesMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            for (auto& pair : subQueues) {
                vQueueDelete(pair.second.handle);
            }
            subQueues.clear();
            xSemaphoreGive(queuesMutex);
        }
        
        // 删除互斥锁
        if (queuesMutex) {
            vSemaphoreDelete(queuesMutex);
            queuesMutex = nullptr;
        }
    }
    
    /**
     * @brief 初始化过滤器，连接到源话题队列
     * @return 是否成功初始化
     */
    bool init() {
        // 尝试获取互斥锁
        if (xSemaphoreTake(g_topicQueuesMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取话题互斥锁");
            return false;
        }
        
        // 检查源话题是否存在
        auto it = g_topicQueues.find(sourceTopic);
        if (it == g_topicQueues.end()) {
            Serial.printf("错误: 源话题'%s'不存在\n", sourceTopic.c_str());
            xSemaphoreGive(g_topicQueuesMutex);
            return false;
        }
        
        sourceQueue = it->second;
        xSemaphoreGive(g_topicQueuesMutex);
        
        return sourceQueue != nullptr;
    }
    
    /**
     * @brief 创建子队列
     * @param queueName 子队列名称
     * @param queueSize 队列长度
     * @param maxMsgSize 最大消息大小，默认为sizeof(QueueElement_t<T>)
     * @return 是否成功创建
     */
    bool createSubQueue(const std::string& queueName, UBaseType_t queueSize, size_t maxMsgSize = 0) {
        // 如果没有指定最大消息大小，使用默认大小
        if (maxMsgSize == 0) {
            maxMsgSize = sizeof(QueueElement_t<T>);
        }
        
        // 尝试获取互斥锁
        if (xSemaphoreTake(queuesMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取过滤队列互斥锁");
            return false;
        }
        
        // 检查子队列是否已存在
        if (subQueues.find(queueName) != subQueues.end()) {
            Serial.printf("警告: 子队列'%s'已存在\n", queueName.c_str());
            xSemaphoreGive(queuesMutex);
            return false;
        }
        
        // 创建新队列
        QueueHandle_t newQueue = xQueueCreate(queueSize, maxMsgSize);
        if (newQueue == nullptr) {
            Serial.printf("错误: 无法创建子队列'%s'\n", queueName.c_str());
            xSemaphoreGive(queuesMutex);
            return false;
        }
        
        // 添加到子队列映射表
        QueueInfo info = {
            .handle = newQueue,
            .maxItemSize = maxMsgSize,
            .queueLength = queueSize
        };
        subQueues[queueName] = info;
        
        // 尝试获取话题互斥锁
        if (xSemaphoreTake(g_topicQueuesMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            Serial.println("警告: 无法获取话题互斥锁");
            xSemaphoreGive(queuesMutex);
            return false;
        }
        
        // 将子队列添加到全局话题映射表
        g_topicQueues[queueName] = newQueue;
        
        xSemaphoreGive(g_topicQueuesMutex);
        xSemaphoreGive(queuesMutex);
        
        Serial.printf("成功创建子队列'%s'\n", queueName.c_str());
        return true;
    }
    
    /**
     * @brief 启动过滤任务
     * @param stackSize 任务栈大小
     * @param priority 任务优先级
     * @param core 运行的核心
     * @return 是否成功启动
     */
    bool start(uint32_t stackSize = 4096, UBaseType_t priority = 2, BaseType_t core = tskNO_AFFINITY) {
        // 检查是否已初始化
        if (sourceQueue == nullptr) {
            if (!init()) {
                Serial.println("错误: 过滤器初始化失败");
                return false;
            }
        }
        
        // 检查是否已在运行
        if (isRunning) {
            return true;
        }
        
        isRunning = true;
        
        // 创建过滤任务
        BaseType_t result = xTaskCreatePinnedToCore(
            filterTask,
            ("Filter_" + sourceTopic).c_str(),
            stackSize,
            this,
            priority,
            &filterTaskHandle,
            core
        );
        
        if (result != pdPASS) {
            isRunning = false;
            Serial.println("错误: 无法创建过滤任务");
            return false;
        }
        
        Serial.printf("已启动'%s'的过滤任务\n", sourceTopic.c_str());
        return true;
    }
    
    /**
     * @brief 停止过滤任务
     */
    void stop() {
        isRunning = false;
        
        // 等待任务退出
        if (filterTaskHandle) {
            vTaskDelay(pdMS_TO_TICKS(processingInterval * 2));
            filterTaskHandle = nullptr;
        }
    }
    
    /**
     * @brief 设置处理间隔
     * @param interval 处理间隔(毫秒)
     */
    void setProcessingInterval(uint32_t interval) {
        processingInterval = interval;
    }
    
    /**
     * @brief 获取源话题名称
     * @return 源话题名称
     */
    std::string getSourceTopic() const {
        return sourceTopic;
    }
    
    /**
     * @brief 获取子队列数量
     * @return 子队列数量
     */
    size_t getSubQueueCount() const {
        if (xSemaphoreTake(queuesMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            return 0;
        }
        
        size_t count = subQueues.size();
        xSemaphoreGive(queuesMutex);
        
        return count;
    }
};

} // namespace OUR

#endif // FILTER_QUEUE_H
