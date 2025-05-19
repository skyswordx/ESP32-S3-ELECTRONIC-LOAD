/**
 * @file client.h
 * @brief 客户端接口
 * @author skyswordx
 * @date 2025-05-19
 * @details 提供系统的服务客户端功能
 */

#ifndef CLIENT_H
#define CLIENT_H

#include "common.h"

namespace OUR {

/**
 * @brief 服务客户端类模板
 * @tparam ReqT 请求数据类型
 * @tparam RespT 响应数据类型
 */
template <typename ReqT, typename RespT>
class Client {
private:
    std::string serviceName;     // 服务名称
    QueueHandle_t requestQueue;  // 请求队列
    SemaphoreHandle_t serviceSemaphore; // 服务是否可用的信号量

public:
    /**
     * @brief 构造函数
     * @param name 服务名称
     */
    Client(const std::string& name) : serviceName(name), requestQueue(nullptr), serviceSemaphore(nullptr) {
        // 尝试获取互斥锁
        if (xSemaphoreTake(g_serviceSemaphoresMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            // 查找服务
            auto it = g_serviceSemaphores.find(serviceName);
            if (it != g_serviceSemaphores.end()) {
                serviceSemaphore = it->second;
            }
            xSemaphoreGive(g_serviceSemaphoresMutex);
        }
    }
    
    /**
     * @brief 析构函数
     */
    ~Client() {
        // 客户端不负责清理资源，只是使用
    }
    
    /**
     * @brief 调用服务
     * @param request 请求数据
     * @param response 响应数据
     * @param timeout 超时时间（毫秒）
     * @return 是否调用成功
     */
    bool call(const ReqT& request, RespT& response, uint32_t timeout = 5000) {
        // 检查服务是否可用
        if (!serviceSemaphore) {
            Serial.printf("错误: 服务'%s'不可用\n", serviceName.c_str());
            return false;
        }
        
        // 查找服务的请求队列
        if (!requestQueue) {
            // 尝试获取互斥锁
            if (xSemaphoreTake(g_serviceSemaphoresMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                // 查找服务相关的请求队列
                auto it = g_serviceSemaphores.find(serviceName);
                if (it != g_serviceSemaphores.end()) {
                    serviceSemaphore = it->second;
                }
                xSemaphoreGive(g_serviceSemaphoresMutex);
            }
            
            // 如果仍然找不到服务
            if (!serviceSemaphore) {
                Serial.printf("错误: 服务'%s'不存在\n", serviceName.c_str());
                return false;
            }
        }
        
        // 创建服务对
        auto servicePair = new ServicePair<ReqT, RespT>();
        servicePair->request = request;
        
        // 发送请求
        if (xQueueSend(requestQueue, &servicePair, pdMS_TO_TICKS(100)) != pdPASS) {
            Serial.printf("错误: 发送服务'%s'请求失败\n", serviceName.c_str());
            delete servicePair;
            return false;
        }
        
        // 等待响应
        bool success = (xSemaphoreTake(servicePair->done, pdMS_TO_TICKS(timeout)) == pdTRUE);
        
        if (success) {
            // 复制响应
            response = servicePair->response;
        }
        
        // 清理资源
        delete servicePair;
        
        return success;
    }
    
    /**
     * @brief 异步调用服务，不等待响应
     * @param request 请求数据
     * @return 是否发送请求成功
     */
    bool callAsync(const ReqT& request) {
        // 检查服务是否可用
        if (!serviceSemaphore) {
            return false;
        }
        
        // 创建服务对
        auto servicePair = new ServicePair<ReqT, RespT>();
        servicePair->request = request;
        
        // 发送请求
        if (xQueueSend(requestQueue, &servicePair, pdMS_TO_TICKS(100)) != pdPASS) {
            delete servicePair;
            return false;
        }
        
        // 不等待响应，由调用者自行处理
        return true;
    }
    
    /**
     * @brief 获取服务名称
     * @return 服务名称
     */
    std::string getName() const {
        return serviceName;
    }
};

/**
 * @brief 创建客户端
 * @tparam ReqT 请求数据类型
 * @tparam RespT 响应数据类型
 * @param serviceName 服务名称
 * @return 客户端对象指针
 */
template <typename ReqT, typename RespT>
std::shared_ptr<Client<ReqT, RespT>> createClient(const std::string& serviceName) {
    return std::make_shared<Client<ReqT, RespT>>(serviceName);
}

} // namespace OUR

#endif // CLIENT_H
