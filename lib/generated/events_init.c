/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif


static void main_page_next_page_button_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_state(guider_ui.main_page_main_tileview, LV_STATE_SCROLLED);
        break;
    }
    default:
        break;
    }
}

static void main_page_ONOFF_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        break;
    }
    default:
        break;
    }
}

static void main_page_measure_resistance_label_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_clear_flag(guider_ui.main_page_measure_resistance_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_resistance_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.main_page_measure_resistance_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.main_page_measure_resistance_label, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void main_page_measure_resistance_chart_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.main_page_measure_resistance_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.main_page_measure_resistance_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_resistance_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_resistance_label, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void main_page_measure_power_label_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_clear_flag(guider_ui.main_page_measure_power_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_power_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.main_page_measure_power_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.main_page_measure_power_label, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void main_page_measure_power_chart_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.main_page_measure_power_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.main_page_measure_power_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_power_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_power_label, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void main_page_measure_voltage_label_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_clear_flag(guider_ui.main_page_measure_voltage_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_voltage_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.main_page_measure_voltage_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.main_page_measure_voltage_label, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void main_page_measure_voltage_chart_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.main_page_measure_voltage_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.main_page_measure_voltage_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_voltage_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_voltage_label, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void main_page_measure_current_label_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.main_page_measure_current_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.main_page_measure_current_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_current_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_current_chart, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void main_page_measure_current_chart_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SHORT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.main_page_measure_current_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.main_page_measure_current_chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_current_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.main_page_measure_current_label, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_main_page (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->main_page_next_page_button, main_page_next_page_button_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_page_ONOFF, main_page_ONOFF_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_page_measure_resistance_label, main_page_measure_resistance_label_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_page_measure_resistance_chart, main_page_measure_resistance_chart_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_page_measure_power_label, main_page_measure_power_label_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_page_measure_power_chart, main_page_measure_power_chart_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_page_measure_voltage_label, main_page_measure_voltage_label_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_page_measure_voltage_chart, main_page_measure_voltage_chart_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_page_measure_current_label, main_page_measure_current_label_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_page_measure_current_chart, main_page_measure_current_chart_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
