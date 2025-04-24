#include "our_topic_queque.hpp"

// GUI 更新使用的消息队列
QueueHandle_t sensor_queue_handle; // 消息队列句柄
const int queue_element_size = 10; // 消息队列元素大小

QueueHandle_t button_queue_handle; // 按键消息队列句柄