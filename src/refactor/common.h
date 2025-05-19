/**
 * @file common.h
 * @brief 通用类型和定义
 * @author skyswordx
 * @date 2025-05-19
 * @details 提供系统的核心定义和通用功能
 */

#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/event_groups.h>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <functional>
#include <type_traits>
#include "our_queque.hpp"

namespace OUR {

/**
 * @brief 服务请求/响应对结构体模板
 * @tparam ReqT 请求数据类型
 * @tparam RespT 响应数据类型
 */
template <typename ReqT, typename RespT>
struct ServicePair {
    ReqT request;              // 请求数据
    RespT response;            // 响应数据
    SemaphoreHandle_t done;    // 完成信号量
    
    // 构造函数
    ServicePair() : request(), response(), done(nullptr) {
        done = xSemaphoreCreateBinary();
    }
    
    // 析构函数
    ~ServicePair() {
        if (done) {
            vSemaphoreDelete(done);
            done = nullptr;
        }
    }
    
    // 禁止拷贝构造和赋值
    ServicePair(const ServicePair&) = delete;
    ServicePair& operator=(const ServicePair&) = delete;
};

// 辅助函数，复制任意类型数据到内存块
template<typename T>
void copyToBuffer(const T& data, void* buffer, size_t bufferSize) {
    size_t dataSize = sizeof(T);
    if (dataSize <= bufferSize) {
        memcpy(buffer, &data, dataSize);
    }
}

// 辅助函数，从内存块复制数据到任意类型
template<typename T>
void copyFromBuffer(const void* buffer, T& data) {
    memcpy(&data, buffer, sizeof(T));
}

/**
 * @brief 队列配置结构体，用于配置话题的队列属性
 */
struct QueueConfig {
    UBaseType_t queueSize;      // 队列长度，最大可以缓存的消息数量
    size_t maxItemSize;         // 队列元素的最大大小（字节）
    
    // 默认构造函数
    QueueConfig(UBaseType_t size = 5, size_t itemSize = 64) : 
        queueSize(size), maxItemSize(itemSize) {}
};

// 全局名称到队列句柄的映射
typedef std::map<std::string, QueueHandle_t> TopicQueueMap;
typedef std::map<std::string, SemaphoreHandle_t> ServiceSemaphoreMap;

// 声明存储队列和信号量的全局映射表
extern TopicQueueMap g_topicQueues;
extern ServiceSemaphoreMap g_serviceSemaphores;

// 声明用于保护全局映射表的互斥锁
extern SemaphoreHandle_t g_topicQueuesMutex;
extern SemaphoreHandle_t g_serviceSemaphoresMutex;

} // namespace OUR

#endif // COMMON_H
