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
#include "gui_guider.h"
#define WARNING_VOLTAGE 18 // 触发过压保护的电压

extern SemaphoreHandle_t gui_xMutex;  // gui 互斥锁句柄，LVGL 线程不安全，需要加锁
extern QueueHandle_t sensor_queue_handle; // 消息队列句柄
extern QueueHandle_t button_queue_handle; // 按键消息队列句柄
extern lv_ui guider_ui; // GUI guider 生成的 GUI 结构体


// 定义一个枚举类型，表示发送消息的源设备 id
enum device_id_t {
    DEVICE_DUMMY_SENSOR = 0,
    DEVICE_ENCODER = 1,
    DEVICE_INA226 = 2,
    DEVICE_ADC1 = 3,
    DEVICE_UNKNOWN = 99
  };
  
  typedef struct {
    float value1; // 传感器数据
    float value2; // 传感器数据
    float value3; // 传感器数据
    float value4; // 传感器数据
    float value5; // 传感器数据
  } device_data_t;
  
  typedef struct {
    device_id_t device_id; // 设备ID
    float value; // 存放的数据（简单设置为一个值，后期有需求再改为专门的结构体
    device_data_t device_data; // INA226 传感器数据
  } message_t;

void lvgl_task(void *pvParameters);
void update_gui_task(void *pvParameters);

#endif // OUR_LVGL_INTERACTION_H