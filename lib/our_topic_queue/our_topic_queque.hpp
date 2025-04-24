#ifndef OUR_TOPIC_QUEUE_HPP
#define OUR_TOPIC_QUEUE_HPP

#include <Arduino.h>

// GUI 更新使用的消息队列
extern QueueHandle_t sensor_queue_handle; // 消息队列句柄
extern const int queue_element_size; // 消息队列元素大小
extern QueueHandle_t button_queue_handle; // 按键消息队列句柄

#endif