#include "our_queque.hpp"


QueueHandle_t button_queue; // 按键消息队列句柄

QueueHandle_t LVGL_queue;

QueueHandle_t current_control_queue; // 电流控制器队列句柄
