/**
 * @file common.cpp
 * @brief 通用功能实现
 * @author skyswordx
 * @date 2025-05-19
 * @details 实现系统的全局变量和初始化函数
 */

#include "common.h"

namespace OUR {

// 定义全局映射表
TopicQueueMap g_topicQueues;
ServiceSemaphoreMap g_serviceSemaphores;

// 定义全局互斥锁
SemaphoreHandle_t g_topicQueuesMutex = nullptr;
SemaphoreHandle_t g_serviceSemaphoresMutex = nullptr;

} // namespace OUR
