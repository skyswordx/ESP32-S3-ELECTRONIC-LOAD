/**
 * @file EventListener.h
 * @brief 事件监听器接口定义
 * @author Triwalt
 * @date 2025-05-06
 * @details 定义了事件监听器接口，任何需要接收事件的类都应实现此接口
 */

#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include "Event.h"

/**
 * @brief 事件监听器接口
 * @details 任何需要接收事件的类都应实现此接口
 */
class EventListener {
public:
    /**
     * @brief 析构函数
     */
    virtual ~EventListener() = default;
      /**
     * @brief 接收事件的接口方法
     * @param event 接收到的事件对象
     */
    virtual void onEvent(const Event& event) = 0;
    
    /**
     * @brief 获取监听器的标识符(用于调试)
     * @return 监听器的字符串标识符
     */
    virtual String getListenerId() const = 0;
};

#endif // EVENT_LISTENER_H