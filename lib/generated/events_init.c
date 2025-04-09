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


static void main_page_switch2chart_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t * status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? true : false;
        ui_load_scr_animation(&guider_ui, &guider_ui.chart_page, guider_ui.chart_page_del, &guider_ui.main_page_del, setup_scr_chart_page, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void main_page_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.chart_page, guider_ui.chart_page_del, &guider_ui.main_page_del, setup_scr_chart_page, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_main_page (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->main_page_switch2chart, main_page_switch2chart_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_page_btn_1, main_page_btn_1_event_handler, LV_EVENT_ALL, ui);
}

static void chart_page_sw_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t * status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? true : false;
        ui_load_scr_animation(&guider_ui, &guider_ui.main_page, guider_ui.main_page_del, &guider_ui.chart_page_del, setup_scr_main_page, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void chart_page_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.main_page, guider_ui.main_page_del, &guider_ui.chart_page_del, setup_scr_main_page, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_chart_page (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->chart_page_sw_1, chart_page_sw_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->chart_page_btn_1, chart_page_btn_1_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
