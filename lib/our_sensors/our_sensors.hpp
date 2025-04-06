/**
 * @file our_sensors.hpp
 * @brief 包含封装好的各种传感器类和相关的任务函数
 * @author skyswordx
 */
#ifndef OUR_SENSORS_HPP
#define OUR_SENSORS_HPP

#include <Arduino.h>

// PIO 找库能力太差了
// #include <lvgl.h>
// #include <SPI.h>
// #include <TFT_eSPI.h>
// #include "lv_port_disp.h"
// #include "lv_port_indev.h"

void get_dummy_sensor_data_task(void *pvParameters); // 模拟获得传感器数据的任务函数
void get_ina226_data_task(void *pvParameters); // 获取 INA226 数据的任务函数
void voltage_protection_task(void *pvParameters); // 过压保护任务函数


#endif // OUR_SENSORS_HPP