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



void lvgl_task(void *pvParameters);
void update_gui_task(void *pvParameters);

#endif // OUR_LVGL_INTERACTION_H