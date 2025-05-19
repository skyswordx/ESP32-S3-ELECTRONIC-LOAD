/**
 * @file our.h
 * @brief 系统总头文件
 * @author skyswordx
 * @date 2025-05-19
 * @details 提供系统的所有组件，基于FreeRTOS实现
 */

#ifndef OUR_H
#define OUR_H

#include "common.h"
#include "publisher.h"
#include "subscriber.h"
#include "server.h"
#include "client.h"
#include "callback_subscriber.h"
// 添加过滤队列相关头文件
#include "filter_queue.h"
#include "task_id_filter.h"
#include "data_desc_filter.h"
#include "custom_filter.h"

namespace OUR {

/**
 * @brief 创建并获取发布者对象
 * @tparam T 消息数据类型
 * @param topicName 话题名称
 * @param config 队列配置
 * @return 发布者对象
 */
template <typename T>
Publisher pub(const std::string& topicName, const QueueConfig& config = QueueConfig()) {
    // 尝试获取互斥锁
    if (xSemaphoreTake(g_topicQueuesMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Serial.println("警告: 无法获取话题互斥锁");
        // 假设没有互斥锁，创建一个不可用的发布者
        return Publisher(topicName, nullptr, 0);
    }
    
    // 检查话题是否已经存在
    QueueHandle_t queueHandle = nullptr;
    size_t itemSize = config.maxItemSize;
    
    auto it = g_topicQueues.find(topicName);
    if (it == g_topicQueues.end()) {
        // 话题不存在，创建新队列
        // 确保队列元素足够大以容纳数据类型
        size_t dataSize = sizeof(QueueElement_t<T>);
        if (dataSize > itemSize) {
            itemSize = dataSize;
        }
        
        queueHandle = xQueueCreate(config.queueSize, itemSize);
        if (!queueHandle) {
            Serial.printf("错误: 无法为话题'%s'创建队列\n", topicName.c_str());
            xSemaphoreGive(g_topicQueuesMutex);
            return Publisher(topicName, nullptr, 0);
        }
        
        // 保存队列句柄
        g_topicQueues[topicName] = queueHandle;
    } else {
        // 话题已存在，获取队列句柄
        queueHandle = it->second;
    }
    
    xSemaphoreGive(g_topicQueuesMutex);
    
    // 创建并返回发布者
    return Publisher(topicName, queueHandle, itemSize);
}

/**
 * @brief 创建并获取订阅者对象
 * @tparam T 数据类型
 * @param topicName 话题名称
 * @return 订阅者对象指针
 */
template <typename T>
std::shared_ptr<Subscriber<T>> sub(const std::string& topicName) {
    return createSubscriber<T>(topicName);
}

/**
 * @brief 创建并获取回调订阅者对象
 * @tparam T 数据类型
 * @param topicName 话题名称
 * @param interval 处理间隔(毫秒)
 * @param autoStart 是否自动启动处理任务
 * @return 回调订阅者对象指针
 */
template <typename T>
std::shared_ptr<CallbackSubscriber<T>> callbackSub(
    const std::string& topicName,
    uint32_t interval = 10,
    bool autoStart = true
) {
    return createCallbackSubscriber<T>(topicName, interval, autoStart);
}

/**
 * @brief 创建并获取服务器对象
 * @tparam ReqT 请求数据类型
 * @tparam RespT 响应数据类型
 * @param serviceName 服务名称
 * @param callback 服务回调函数
 * @param autoStart 是否自动启动服务器
 * @return 服务器对象指针
 */
template <typename ReqT, typename RespT>
std::shared_ptr<Server<ReqT, RespT>> advertiseService(
    const std::string& serviceName,
    typename Server<ReqT, RespT>::ServiceCallbackType callback,
    bool autoStart = true) {
        
    return createServer<ReqT, RespT>(serviceName, callback, autoStart);
}

/**
 * @brief 创建并获取客户端对象
 * @tparam ReqT 请求数据类型
 * @tparam RespT 响应数据类型
 * @param serviceName 服务名称
 * @return 客户端对象指针
 */
template <typename ReqT, typename RespT>
std::shared_ptr<Client<ReqT, RespT>> serviceClient(const std::string& serviceName) {
    return createClient<ReqT, RespT>(serviceName);
}

/**
 * @brief 初始化系统通信组件
 * @return 是否初始化成功
 */
bool init() {
    // 创建互斥锁
    if (!g_topicQueuesMutex) {
        g_topicQueuesMutex = xSemaphoreCreateMutex();
        if (!g_topicQueuesMutex) {
            Serial.println("错误: 无法创建话题互斥锁");
            return false;
        }
    }
    
    if (!g_serviceSemaphoresMutex) {
        g_serviceSemaphoresMutex = xSemaphoreCreateMutex();
        if (!g_serviceSemaphoresMutex) {
            Serial.println("错误: 无法创建服务互斥锁");
            return false;
        }
    }
    
    return true;
}

/**
 * @brief 检查系统是否就绪
 * @return 是否就绪
 */
bool ok() {
    return (g_topicQueuesMutex != nullptr) && (g_serviceSemaphoresMutex != nullptr);
}

} // namespace OUR

#endif // OUR_H
