/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
// #include "custom.h"



void setup_scr_chart_page(lv_ui *ui)
{
    //Write codes chart_page
    ui->chart_page = lv_obj_create(NULL);
    lv_obj_set_size(ui->chart_page, 480, 320);
    lv_obj_set_scrollbar_mode(ui->chart_page, LV_SCROLLBAR_MODE_OFF);

    //Write style for chart_page, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->chart_page, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes chart_page_sw_1
    ui->chart_page_sw_1 = lv_switch_create(ui->chart_page);
    lv_obj_set_pos(ui->chart_page_sw_1, 29, 224);
    lv_obj_set_size(ui->chart_page_sw_1, 66, 22);

    //Write style for chart_page_sw_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->chart_page_sw_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->chart_page_sw_1, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->chart_page_sw_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->chart_page_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->chart_page_sw_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->chart_page_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for chart_page_sw_1, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->chart_page_sw_1, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->chart_page_sw_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->chart_page_sw_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->chart_page_sw_1, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for chart_page_sw_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->chart_page_sw_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->chart_page_sw_1, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->chart_page_sw_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->chart_page_sw_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->chart_page_sw_1, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes chart_page_btn_1
    ui->chart_page_btn_1 = lv_btn_create(ui->chart_page);
    ui->chart_page_btn_1_label = lv_label_create(ui->chart_page_btn_1);
    lv_label_set_text(ui->chart_page_btn_1_label, "Button");
    lv_label_set_long_mode(ui->chart_page_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->chart_page_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->chart_page_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->chart_page_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->chart_page_btn_1, 29, 255);
    lv_obj_set_size(ui->chart_page_btn_1, 100, 50);

    //Write style for chart_page_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->chart_page_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->chart_page_btn_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->chart_page_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->chart_page_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->chart_page_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->chart_page_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->chart_page_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->chart_page_btn_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->chart_page_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->chart_page_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes chart_page_chart_1
    ui->chart_page_chart_1 = lv_chart_create(ui->chart_page);
    lv_chart_set_type(ui->chart_page_chart_1, LV_CHART_TYPE_LINE);
    lv_chart_set_div_line_count(ui->chart_page_chart_1, 3, 5);
    lv_chart_set_point_count(ui->chart_page_chart_1, 5);
    lv_chart_set_range(ui->chart_page_chart_1, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_axis_tick(ui->chart_page_chart_1, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 10, true, 40);
    lv_chart_set_range(ui->chart_page_chart_1, LV_CHART_AXIS_SECONDARY_Y, 0, 100);
    lv_chart_set_zoom_x(ui->chart_page_chart_1, 256);
    lv_chart_set_zoom_y(ui->chart_page_chart_1, 256);
    ui->chart_page_chart_1_0 = lv_chart_add_series(ui->chart_page_chart_1, lv_color_hex(0x2F35DA), LV_CHART_AXIS_PRIMARY_Y);
#if LV_USE_FREEMASTER == 0
    lv_chart_set_next_value(ui->chart_page_chart_1, ui->chart_page_chart_1_0, 0);
    lv_chart_set_next_value(ui->chart_page_chart_1, ui->chart_page_chart_1_0, 0);
    lv_chart_set_next_value(ui->chart_page_chart_1, ui->chart_page_chart_1_0, 0);
    lv_chart_set_next_value(ui->chart_page_chart_1, ui->chart_page_chart_1_0, 0);
    lv_chart_set_next_value(ui->chart_page_chart_1, ui->chart_page_chart_1_0, 0);
#endif
    lv_obj_set_pos(ui->chart_page_chart_1, 117, 10);
    lv_obj_set_size(ui->chart_page_chart_1, 356, 238);
    lv_obj_set_scrollbar_mode(ui->chart_page_chart_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for chart_page_chart_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->chart_page_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->chart_page_chart_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->chart_page_chart_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->chart_page_chart_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->chart_page_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->chart_page_chart_1, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->chart_page_chart_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->chart_page_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(ui->chart_page_chart_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->chart_page_chart_1, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->chart_page_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->chart_page_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for chart_page_chart_1, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->chart_page_chart_1, lv_color_hex(0x151212), LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->chart_page_chart_1, &lv_font_montserratMedium_12, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->chart_page_chart_1, 255, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(ui->chart_page_chart_1, 2, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->chart_page_chart_1, lv_color_hex(0xe8e8e8), LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->chart_page_chart_1, 255, LV_PART_TICKS|LV_STATE_DEFAULT);

    //The custom code of chart_page.


    //Update current screen layout.
    lv_obj_update_layout(ui->chart_page);

}
