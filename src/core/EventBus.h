/**
 * @file EventBus.h
 * @brief 事件总线定义
 * @author Triwalt
 * @date 2025-05-06
 * @details 实现事件分发机制，连接事件发布者和订阅者
 */

#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include "Event.h"
#include "EventListener.h"
#include <vector>
#include <map>
#include <memory>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/**
 * @brief 事件总线类
 * @details 实现了观察者模式，用于事件的发布和订阅
 */
class EventBus {
public:
    // 监听所有类型事件的特殊值
    static const EventType ALL_EVENTS = static_cast<EventType>(-1);
    
    /**
     * @brief 获取EventBus单例
     * @return EventBus单例引用
     */
    static EventBus& getInstance();

    /**
     * @brief 发布事件
     * @param event 要发布的事件
     */
    void publish(const Event& event);

    /**
     * @brief 发布事件(智能指针版本)
     * @param event 要发布的事件的智能指针
     */
    void publish(const std::shared_ptr<Event>& event);
    
    /**
     * @brief 订阅特定类型的事件
     * @param type 要订阅的事件类型
     * @param listener 事件监听器指针
     */
    void subscribe(EventType type, EventListener* listener);
    
    /**
     * @brief 取消订阅特定类型的事件
     * @param type 要取消订阅的事件类型
     * @param listener 事件监听器指针
     */
    void unsubscribe(EventType type, EventListener* listener);
      /**
     * @brief 取消特定监听器的所有订阅
     * @param listener 要取消订阅的监听器
     */
    void unsubscribeAll(EventListener* listener);
    
    /**
     * @brief 注册监听器（监听所有事件）
     * @param listener 监听器指针
     */
    void registerListener(EventListener* listener);
    
    /**
     * @brief 取消注册监听器
     * @param listener 监听器指针
     */
    void unregisterListener(EventListener* listener);

private:
    /**
     * @brief 构造函数(私有)
     */
    EventBus();
    
    /**
     * @brief 析构函数(私有)
     */
    ~EventBus();

    /**
     * @brief 禁用拷贝构造函数
     */
    EventBus(const EventBus&) = delete;
    
    /**
     * @brief 禁用赋值操作符
     */
    EventBus& operator=(const EventBus&) = delete;

private:
    std::map<EventType, std::vector<EventListener*>> listeners_; ///< 事件类型到监听器列表的映射
    SemaphoreHandle_t mutex_; ///< 互斥锁，保护监听器列表的线程安全
};

#endif // EVENT_BUS_H