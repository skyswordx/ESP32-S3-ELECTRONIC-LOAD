/**
 * @file our_lvgl_interaction.hpp
 * @brief LVGL GUI 任务调度接口和 GUI 数据更新接口
 * @author skyswordx
 */

#ifndef OUR_LVGL_INTERACTION_H
#define OUR_LVGL_INTERACTION_H

// PIO 找库能力太差了
// #include <lvgl.h>
// #include <SPI.h>
// #include <TFT_eSPI.h>
// #include "lv_port_disp.h"
// #include "lv_port_indev.h"

#include <Arduino.h>
#include "../generated/gui_guider.h"
#include "../our_queue/our_queque.hpp" // 消息队列头文件

#define WARNING_VOLTAGE 11 // 触发过压保护的电压

extern SemaphoreHandle_t gui_xMutex;  // gui 互斥锁句柄，LVGL 线程不安全，需要加锁

extern lv_ui guider_ui; // GUI guider 生成的 GUI 结构体


void lvgl_task(void *pvParameters);
void update_gui_task(void *pvParameters);

#endif // OUR_LVGL_INTERACTION_H