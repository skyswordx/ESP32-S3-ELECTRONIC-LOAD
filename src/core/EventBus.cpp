/**
 * @file EventBus.cpp
 * @brief 事件总线实现
 * @author Triwalt
 * @date 2025-05-06
 * @details 事件总线的具体实现，包含事件分发、订阅和取消订阅功能
 */

#include "EventBus.h"
#include <Arduino.h>

EventBus& EventBus::getInstance() {
    static EventBus instance;
    return instance;
}

EventBus::EventBus() {
    // 创建互斥锁
    mutex_ = xSemaphoreCreateMutex();
    if (mutex_ == NULL) {
        Serial.println("Failed to create EventBus mutex!");
    }
}

EventBus::~EventBus() {
    // 释放互斥锁
    if (mutex_ != NULL) {
        vSemaphoreDelete(mutex_);
        mutex_ = NULL;
    }
}

void EventBus::publish(const Event& event) {
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE) {
        Serial.println("Failed to take EventBus mutex!");
        return;
    }
    
    // 获取事件类型对应的监听器列表
    EventType type = event.getType();
    auto it = listeners_.find(type);
    
    if (it != listeners_.end()) {
        // 创建一个临时副本，以便在通知过程中可以安全地修改原始列表
        std::vector<EventListener*> listenersCopy = it->second;
        xSemaphoreGive(mutex_); // 在遍历前释放互斥锁，允许在回调中注册或注销监听器
        
        // 通知所有监听器
        for (auto listener : listenersCopy) {
            if (listener) {
                listener->onEvent(event);
            }
        }    } else {
        xSemaphoreGive(mutex_); // 如果没有监听器，释放互斥锁
    }
}

void EventBus::publish(const std::shared_ptr<Event>& event) {
    if (!event) {
        return;
    }
    publish(*event);
}

void EventBus::subscribe(EventType type, EventListener* listener) {
    if (!listener) {
        return;
    }
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE) {
        Serial.println("Failed to take EventBus mutex!");
        return;
    }
    
    // 检查监听器是否已经订阅
    auto it = listeners_.find(type);
    if (it != listeners_.end()) {
        auto& listeners = it->second;
        bool alreadySubscribed = false;
        
        for (auto existingListener : listeners) {
            if (existingListener == listener) {
                alreadySubscribed = true;
                break;
            }
        }
        
        if (!alreadySubscribed) {
            listeners.push_back(listener);
            Serial.printf("Listener %s subscribed to event type %d\n", 
                          listener->getListenerId().c_str(), static_cast<int>(type));
        }
    } else {
        // 如果该事件类型没有监听器列表，创建一个新的
        listeners_[type] = {listener};
        Serial.printf("Listener %s subscribed to event type %d (new list)\n", 
                      listener->getListenerId().c_str(), static_cast<int>(type));
    }
    
    xSemaphoreGive(mutex_); // 释放互斥锁
}

void EventBus::unsubscribe(EventType type, EventListener* listener) {
    if (!listener) {
        return;
    }
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE) {
        Serial.println("Failed to take EventBus mutex!");
        return;
    }
    
    // 查找该事件类型的监听器列表
    auto it = listeners_.find(type);
    if (it != listeners_.end()) {
        auto& listeners = it->second;
        
        // 移除指定的监听器
        listeners.erase(
            std::remove(listeners.begin(), listeners.end(), listener),
            listeners.end()
        );
        
        Serial.printf("Listener %s unsubscribed from event type %d\n", 
                     listener->getListenerId().c_str(), static_cast<int>(type));
        
        // 如果列表为空，删除该事件类型的条目
        if (listeners.empty()) {
            listeners_.erase(it);
        }
    }
    
    xSemaphoreGive(mutex_); // 释放互斥锁
}

void EventBus::unsubscribeAll(EventListener* listener) {
    if (!listener) {
        return;
    }
    
    // 尝试获取互斥锁
    if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE) {
        Serial.println("Failed to take EventBus mutex!");
        return;
    }
    
    // 遍历所有事件类型
    for (auto it = listeners_.begin(); it != listeners_.end();) {
        auto& listeners = it->second;
        
        // 移除指定的监听器
        listeners.erase(
            std::remove(listeners.begin(), listeners.end(), listener),
            listeners.end()
        );
        
        // 如果列表为空，删除该事件类型的条目
        if (listeners.empty()) {
            it = listeners_.erase(it);
        } else {
            ++it;
        }
    }
    
    Serial.printf("Listener %s unsubscribed from all events\n", listener->getListenerId().c_str());
    
    xSemaphoreGive(mutex_); // 释放互斥锁
}

void EventBus::registerListener(EventListener* listener) {
    if (!listener) {
        return;
    }
    
    // 为ALL_EVENTS类型注册监听器，表示监听所有事件
    subscribe(ALL_EVENTS, listener);
    
    Serial.printf("Listener %s registered for all events\n", listener->getListenerId().c_str());
}

void EventBus::unregisterListener(EventListener* listener) {
    if (!listener) {
        return;
    }
    
    // 取消所有事件的监听
    unsubscribeAll(listener);
    
    Serial.printf("Listener %s unregistered from all events\n", listener->getListenerId().c_str());
}