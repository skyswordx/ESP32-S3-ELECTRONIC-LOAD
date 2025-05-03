/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef WIDGET_INIT_H
#define WIDGET_INIT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "gui_guider.h"

__attribute__((unused)) void kb_event_cb(lv_event_t *e);
__attribute__((unused)) void ta_event_cb(lv_event_t *e);
#if LV_USE_ANALOGCLOCK != 0
void clock_count(int *hour, int *min, int *sec);
#endif

extern lv_obj_t * main_page_set_current_box;
void lv_main_page_set_current_box_increment_event_cb(lv_event_t * event);
void lv_main_page_set_current_box_decrement_event_cb(lv_event_t * event);
extern lv_obj_t * main_page_over_voltage_box;
void lv_main_page_over_voltage_box_increment_event_cb(lv_event_t * event);
void lv_main_page_over_voltage_box_decrement_event_cb(lv_event_t * event);
extern lv_obj_t * main_page_spinbox_4;
void lv_main_page_spinbox_4_increment_event_cb(lv_event_t * event);
void lv_main_page_spinbox_4_decrement_event_cb(lv_event_t * event);
extern lv_obj_t * main_page_spinbox_6;
void lv_main_page_spinbox_6_increment_event_cb(lv_event_t * event);
void lv_main_page_spinbox_6_decrement_event_cb(lv_event_t * event);
extern lv_obj_t * main_page_spinbox_7;
void lv_main_page_spinbox_7_increment_event_cb(lv_event_t * event);
void lv_main_page_spinbox_7_decrement_event_cb(lv_event_t * event);
extern lv_obj_t * main_page_spinbox_5;
void lv_main_page_spinbox_5_increment_event_cb(lv_event_t * event);
void lv_main_page_spinbox_5_decrement_event_cb(lv_event_t * event);
extern lv_obj_t * main_page_spinbox_10;
void lv_main_page_spinbox_10_increment_event_cb(lv_event_t * event);
void lv_main_page_spinbox_10_decrement_event_cb(lv_event_t * event);
extern lv_obj_t * main_page_spinbox_9;
void lv_main_page_spinbox_9_increment_event_cb(lv_event_t * event);
void lv_main_page_spinbox_9_decrement_event_cb(lv_event_t * event);
extern lv_obj_t * main_page_spinbox_8;
void lv_main_page_spinbox_8_increment_event_cb(lv_event_t * event);
void lv_main_page_spinbox_8_decrement_event_cb(lv_event_t * event);

void main_page_running_time_timer(lv_timer_t *timer);

#ifdef __cplusplus
}
#endif
#endif
