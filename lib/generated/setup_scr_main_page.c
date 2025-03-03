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



void setup_scr_main_page(lv_ui *ui)
{
    //Write codes main_page
    ui->main_page = lv_obj_create(NULL);
    lv_obj_set_size(ui->main_page, 320, 240);
    lv_obj_set_scrollbar_mode(ui->main_page, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page, lv_color_hex(0x59c7ec), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_img_1
    ui->main_page_img_1 = lv_img_create(ui->main_page);
    lv_obj_add_flag(ui->main_page_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->main_page_img_1, &_white_alpha_239x206);
    lv_img_set_pivot(ui->main_page_img_1, 50,50);
    lv_img_set_angle(ui->main_page_img_1, 0);
    lv_obj_set_pos(ui->main_page_img_1, 0, 34);
    lv_obj_set_size(ui->main_page_img_1, 239, 206);

    //Write style for main_page_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->main_page_img_1, 79, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->main_page_img_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->main_page_img_1, 112, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->main_page_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_img_3
    ui->main_page_img_3 = lv_img_create(ui->main_page);
    lv_obj_add_flag(ui->main_page_img_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->main_page_img_3, &_electric_sensor_3239_alpha_28x24);
    lv_img_set_pivot(ui->main_page_img_3, 50,50);
    lv_img_set_angle(ui->main_page_img_3, 0);
    lv_obj_set_pos(ui->main_page_img_3, 5, 5);
    lv_obj_set_size(ui->main_page_img_3, 28, 24);

    //Write style for main_page_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->main_page_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->main_page_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->main_page_img_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_btn_4
    ui->main_page_btn_4 = lv_btn_create(ui->main_page);
    ui->main_page_btn_4_label = lv_label_create(ui->main_page_btn_4);
    lv_label_set_text(ui->main_page_btn_4_label, "Mode: CC");
    lv_label_set_long_mode(ui->main_page_btn_4_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->main_page_btn_4_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_4, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_4_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_4, 40, 5);
    lv_obj_set_size(ui->main_page_btn_4, 88, 23);

    //Write style for main_page_btn_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_4, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_4, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_4, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_img_4
    ui->main_page_img_4 = lv_img_create(ui->main_page);
    lv_obj_add_flag(ui->main_page_img_4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->main_page_img_4, &_blue_alpha_83x240);
    lv_img_set_pivot(ui->main_page_img_4, 50,50);
    lv_img_set_angle(ui->main_page_img_4, 0);
    lv_obj_set_pos(ui->main_page_img_4, 237, 0);
    lv_obj_set_size(ui->main_page_img_4, 83, 240);

    //Write style for main_page_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->main_page_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->main_page_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->main_page_img_4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_btn_3
    ui->main_page_btn_3 = lv_btn_create(ui->main_page);
    ui->main_page_btn_3_label = lv_label_create(ui->main_page_btn_3);
    lv_label_set_text(ui->main_page_btn_3_label, "+0.01");
    lv_label_set_long_mode(ui->main_page_btn_3_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->main_page_btn_3_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_3, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_3_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_3, 275, 34);
    lv_obj_set_size(ui->main_page_btn_3, 41, 26);

    //Write style for main_page_btn_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_3, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_3, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_3, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_btn_1
    ui->main_page_btn_1 = lv_btn_create(ui->main_page);
    ui->main_page_btn_1_label = lv_label_create(ui->main_page_btn_1);
    lv_label_set_text(ui->main_page_btn_1_label, "+1");
    lv_label_set_long_mode(ui->main_page_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->main_page_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_1, 243, 34);
    lv_obj_set_size(ui->main_page_btn_1, 29, 26);

    //Write style for main_page_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spangroup_1
    ui->main_page_spangroup_1 = lv_spangroup_create(ui->main_page);
    lv_spangroup_set_align(ui->main_page_spangroup_1, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->main_page_spangroup_1, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->main_page_spangroup_1, LV_SPAN_MODE_BREAK);
    //create span
    ui->main_page_spangroup_1_span = lv_spangroup_new_span(ui->main_page_spangroup_1);
    lv_span_set_text(ui->main_page_spangroup_1_span, "Details\n");
    lv_style_set_text_color(&ui->main_page_spangroup_1_span->style, lv_color_hex(0xffffff));
    lv_style_set_text_decor(&ui->main_page_spangroup_1_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->main_page_spangroup_1_span->style, &lv_font_montserratMedium_16);
    lv_obj_set_pos(ui->main_page_spangroup_1, 133, 9);
    lv_obj_set_size(ui->main_page_spangroup_1, 81, 11);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spangroup_1_main_main_default
    static lv_style_t style_main_page_spangroup_1_main_main_default;
    ui_init_style(&style_main_page_spangroup_1_main_main_default);

    lv_style_set_border_width(&style_main_page_spangroup_1_main_main_default, 0);
    lv_style_set_radius(&style_main_page_spangroup_1_main_main_default, 0);
    lv_style_set_bg_opa(&style_main_page_spangroup_1_main_main_default, 0);
    lv_style_set_pad_top(&style_main_page_spangroup_1_main_main_default, 0);
    lv_style_set_pad_right(&style_main_page_spangroup_1_main_main_default, 0);
    lv_style_set_pad_bottom(&style_main_page_spangroup_1_main_main_default, 0);
    lv_style_set_pad_left(&style_main_page_spangroup_1_main_main_default, 0);
    lv_style_set_shadow_width(&style_main_page_spangroup_1_main_main_default, 0);
    lv_obj_add_style(ui->main_page_spangroup_1, &style_main_page_spangroup_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->main_page_spangroup_1);

    //Write codes main_page_sw_1
    ui->main_page_sw_1 = lv_switch_create(ui->main_page);
    lv_obj_set_pos(ui->main_page_sw_1, 194, 5);
    lv_obj_set_size(ui->main_page_sw_1, 35, 20);

    //Write style for main_page_sw_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_sw_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_sw_1, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_sw_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_sw_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_sw_1, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->main_page_sw_1, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->main_page_sw_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->main_page_sw_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->main_page_sw_1, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for main_page_sw_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_sw_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_sw_1, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_sw_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_sw_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_sw_1, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes main_page_spangroup_2
    ui->main_page_spangroup_2 = lv_spangroup_create(ui->main_page);
    lv_spangroup_set_align(ui->main_page_spangroup_2, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->main_page_spangroup_2, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->main_page_spangroup_2, LV_SPAN_MODE_BREAK);
    //create span
    ui->main_page_spangroup_2_span = lv_spangroup_new_span(ui->main_page_spangroup_2);
    lv_span_set_text(ui->main_page_spangroup_2_span, "SET");
    lv_style_set_text_color(&ui->main_page_spangroup_2_span->style, lv_color_hex(0xffffff));
    lv_style_set_text_decor(&ui->main_page_spangroup_2_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->main_page_spangroup_2_span->style, &lv_font_montserratMedium_23);
    lv_obj_set_pos(ui->main_page_spangroup_2, 248, 5);
    lv_obj_set_size(ui->main_page_spangroup_2, 93, 21);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spangroup_2_main_main_default
    static lv_style_t style_main_page_spangroup_2_main_main_default;
    ui_init_style(&style_main_page_spangroup_2_main_main_default);

    lv_style_set_border_width(&style_main_page_spangroup_2_main_main_default, 0);
    lv_style_set_radius(&style_main_page_spangroup_2_main_main_default, 0);
    lv_style_set_bg_opa(&style_main_page_spangroup_2_main_main_default, 0);
    lv_style_set_pad_top(&style_main_page_spangroup_2_main_main_default, 0);
    lv_style_set_pad_right(&style_main_page_spangroup_2_main_main_default, 0);
    lv_style_set_pad_bottom(&style_main_page_spangroup_2_main_main_default, 0);
    lv_style_set_pad_left(&style_main_page_spangroup_2_main_main_default, 0);
    lv_style_set_shadow_width(&style_main_page_spangroup_2_main_main_default, 0);
    lv_obj_add_style(ui->main_page_spangroup_2, &style_main_page_spangroup_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->main_page_spangroup_2);

    //Write codes main_page_btn_6
    ui->main_page_btn_6 = lv_btn_create(ui->main_page);
    ui->main_page_btn_6_label = lv_label_create(ui->main_page_btn_6);
    lv_label_set_text(ui->main_page_btn_6_label, "-0.01");
    lv_label_set_long_mode(ui->main_page_btn_6_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->main_page_btn_6_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_6, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_6_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_6, 275, 208);
    lv_obj_set_size(ui->main_page_btn_6, 41, 26);

    //Write style for main_page_btn_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_6, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_6, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_6, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_btn_5
    ui->main_page_btn_5 = lv_btn_create(ui->main_page);
    ui->main_page_btn_5_label = lv_label_create(ui->main_page_btn_5);
    lv_label_set_text(ui->main_page_btn_5_label, "-1");
    lv_label_set_long_mode(ui->main_page_btn_5_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->main_page_btn_5_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_5, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_5_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_5, 243, 208);
    lv_obj_set_size(ui->main_page_btn_5, 29, 26);

    //Write style for main_page_btn_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_5, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_5, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_5, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_btn_7
    ui->main_page_btn_7 = lv_btn_create(ui->main_page);
    ui->main_page_btn_7_label = lv_label_create(ui->main_page_btn_7);
    lv_label_set_text(ui->main_page_btn_7_label, "");
    lv_label_set_long_mode(ui->main_page_btn_7_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->main_page_btn_7_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_7, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_7_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_7, 7, 41);
    lv_obj_set_size(ui->main_page_btn_7, 108, 70);

    //Write style for main_page_btn_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_7, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_7, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_7, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spangroup_3
    ui->main_page_spangroup_3 = lv_spangroup_create(ui->main_page);
    lv_spangroup_set_align(ui->main_page_spangroup_3, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->main_page_spangroup_3, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->main_page_spangroup_3, LV_SPAN_MODE_BREAK);
    //create span
    ui->main_page_spangroup_3_span = lv_spangroup_new_span(ui->main_page_spangroup_3);
    lv_span_set_text(ui->main_page_spangroup_3_span, "Current");
    lv_style_set_text_color(&ui->main_page_spangroup_3_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->main_page_spangroup_3_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->main_page_spangroup_3_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->main_page_spangroup_3, 33, 43);
    lv_obj_set_size(ui->main_page_spangroup_3, 80, 100);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spangroup_3_main_main_default
    static lv_style_t style_main_page_spangroup_3_main_main_default;
    ui_init_style(&style_main_page_spangroup_3_main_main_default);

    lv_style_set_border_width(&style_main_page_spangroup_3_main_main_default, 0);
    lv_style_set_radius(&style_main_page_spangroup_3_main_main_default, 0);
    lv_style_set_bg_opa(&style_main_page_spangroup_3_main_main_default, 0);
    lv_style_set_pad_top(&style_main_page_spangroup_3_main_main_default, 0);
    lv_style_set_pad_right(&style_main_page_spangroup_3_main_main_default, 0);
    lv_style_set_pad_bottom(&style_main_page_spangroup_3_main_main_default, 0);
    lv_style_set_pad_left(&style_main_page_spangroup_3_main_main_default, 0);
    lv_style_set_shadow_width(&style_main_page_spangroup_3_main_main_default, 0);
    lv_obj_add_style(ui->main_page_spangroup_3, &style_main_page_spangroup_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->main_page_spangroup_3);

    //Write codes main_page_img_5
    ui->main_page_img_5 = lv_img_create(ui->main_page);
    lv_obj_add_flag(ui->main_page_img_5, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->main_page_img_5, &_cc_alpha_20x19);
    lv_img_set_pivot(ui->main_page_img_5, 50,50);
    lv_img_set_angle(ui->main_page_img_5, 0);
    lv_obj_set_pos(ui->main_page_img_5, 12, 43);
    lv_obj_set_size(ui->main_page_img_5, 20, 19);

    //Write style for main_page_img_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->main_page_img_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->main_page_img_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_img_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->main_page_img_5, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_measure_current_label
    ui->main_page_measure_current_label = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_measure_current_label, "1000.000");
    lv_label_set_long_mode(ui->main_page_measure_current_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_measure_current_label, 15, 69);
    lv_obj_set_size(ui->main_page_measure_current_label, 93, 32);

    //Write style for main_page_measure_current_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_measure_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_measure_current_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_current_label, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_measure_current_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_measure_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_measure_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_measure_current_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_measure_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_measure_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_measure_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_measure_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_measure_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_measure_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spangroup_4
    ui->main_page_spangroup_4 = lv_spangroup_create(ui->main_page);
    lv_spangroup_set_align(ui->main_page_spangroup_4, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->main_page_spangroup_4, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->main_page_spangroup_4, LV_SPAN_MODE_BREAK);
    //create span
    ui->main_page_spangroup_4_span = lv_spangroup_new_span(ui->main_page_spangroup_4);
    lv_span_set_text(ui->main_page_spangroup_4_span, "mA\n");
    lv_style_set_text_color(&ui->main_page_spangroup_4_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->main_page_spangroup_4_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->main_page_spangroup_4_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->main_page_spangroup_4, 71, 87);
    lv_obj_set_size(ui->main_page_spangroup_4, 80, 16);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spangroup_4_main_main_default
    static lv_style_t style_main_page_spangroup_4_main_main_default;
    ui_init_style(&style_main_page_spangroup_4_main_main_default);

    lv_style_set_border_width(&style_main_page_spangroup_4_main_main_default, 0);
    lv_style_set_radius(&style_main_page_spangroup_4_main_main_default, 0);
    lv_style_set_bg_opa(&style_main_page_spangroup_4_main_main_default, 0);
    lv_style_set_pad_top(&style_main_page_spangroup_4_main_main_default, 0);
    lv_style_set_pad_right(&style_main_page_spangroup_4_main_main_default, 0);
    lv_style_set_pad_bottom(&style_main_page_spangroup_4_main_main_default, 0);
    lv_style_set_pad_left(&style_main_page_spangroup_4_main_main_default, 0);
    lv_style_set_shadow_width(&style_main_page_spangroup_4_main_main_default, 0);
    lv_obj_add_style(ui->main_page_spangroup_4, &style_main_page_spangroup_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->main_page_spangroup_4);

    //Write codes main_page_btn_8
    ui->main_page_btn_8 = lv_btn_create(ui->main_page);
    ui->main_page_btn_8_label = lv_label_create(ui->main_page_btn_8);
    lv_label_set_text(ui->main_page_btn_8_label, "");
    lv_label_set_long_mode(ui->main_page_btn_8_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->main_page_btn_8_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_8, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_8_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_8, 122, 41);
    lv_obj_set_size(ui->main_page_btn_8, 108, 70);

    //Write style for main_page_btn_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_8, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_8, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_8, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_8, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_8, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_btn_9
    ui->main_page_btn_9 = lv_btn_create(ui->main_page);
    ui->main_page_btn_9_label = lv_label_create(ui->main_page_btn_9);
    lv_label_set_text(ui->main_page_btn_9_label, "");
    lv_label_set_long_mode(ui->main_page_btn_9_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->main_page_btn_9_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_9, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_9_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_9, 7, 120);
    lv_obj_set_size(ui->main_page_btn_9, 108, 70);

    //Write style for main_page_btn_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_9, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_9, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_9, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_9, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_btn_10
    ui->main_page_btn_10 = lv_btn_create(ui->main_page);
    ui->main_page_btn_10_label = lv_label_create(ui->main_page_btn_10);
    lv_label_set_text(ui->main_page_btn_10_label, "");
    lv_label_set_long_mode(ui->main_page_btn_10_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->main_page_btn_10_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_10, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_10_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_10, 122, 120);
    lv_obj_set_size(ui->main_page_btn_10, 108, 70);

    //Write style for main_page_btn_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_10, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_10, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_10, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_10, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_10, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spangroup_5
    ui->main_page_spangroup_5 = lv_spangroup_create(ui->main_page);
    lv_spangroup_set_align(ui->main_page_spangroup_5, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->main_page_spangroup_5, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->main_page_spangroup_5, LV_SPAN_MODE_BREAK);
    //create span
    ui->main_page_spangroup_5_span = lv_spangroup_new_span(ui->main_page_spangroup_5);
    lv_span_set_text(ui->main_page_spangroup_5_span, "Voltage\n\n\n\n\n");
    lv_style_set_text_color(&ui->main_page_spangroup_5_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->main_page_spangroup_5_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->main_page_spangroup_5_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->main_page_spangroup_5, 148, 43);
    lv_obj_set_size(ui->main_page_spangroup_5, 87, 22);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spangroup_5_main_main_default
    static lv_style_t style_main_page_spangroup_5_main_main_default;
    ui_init_style(&style_main_page_spangroup_5_main_main_default);

    lv_style_set_border_width(&style_main_page_spangroup_5_main_main_default, 0);
    lv_style_set_radius(&style_main_page_spangroup_5_main_main_default, 0);
    lv_style_set_bg_opa(&style_main_page_spangroup_5_main_main_default, 0);
    lv_style_set_pad_top(&style_main_page_spangroup_5_main_main_default, 0);
    lv_style_set_pad_right(&style_main_page_spangroup_5_main_main_default, 0);
    lv_style_set_pad_bottom(&style_main_page_spangroup_5_main_main_default, 0);
    lv_style_set_pad_left(&style_main_page_spangroup_5_main_main_default, 0);
    lv_style_set_shadow_width(&style_main_page_spangroup_5_main_main_default, 0);
    lv_obj_add_style(ui->main_page_spangroup_5, &style_main_page_spangroup_5_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->main_page_spangroup_5);

    //Write codes main_page_measure_register_label
    ui->main_page_measure_register_label = lv_spangroup_create(ui->main_page);
    lv_spangroup_set_align(ui->main_page_measure_register_label, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->main_page_measure_register_label, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->main_page_measure_register_label, LV_SPAN_MODE_BREAK);
    //create span
    ui->main_page_measure_register_label_span = lv_spangroup_new_span(ui->main_page_measure_register_label);
    lv_span_set_text(ui->main_page_measure_register_label_span, "Register\n\n\n\n\n");
    lv_style_set_text_color(&ui->main_page_measure_register_label_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->main_page_measure_register_label_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->main_page_measure_register_label_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->main_page_measure_register_label, 27, 123);
    lv_obj_set_size(ui->main_page_measure_register_label, 109, 19);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_measure_register_label_main_main_default
    static lv_style_t style_main_page_measure_register_label_main_main_default;
    ui_init_style(&style_main_page_measure_register_label_main_main_default);

    lv_style_set_border_width(&style_main_page_measure_register_label_main_main_default, 0);
    lv_style_set_radius(&style_main_page_measure_register_label_main_main_default, 0);
    lv_style_set_bg_opa(&style_main_page_measure_register_label_main_main_default, 0);
    lv_style_set_pad_top(&style_main_page_measure_register_label_main_main_default, 0);
    lv_style_set_pad_right(&style_main_page_measure_register_label_main_main_default, 0);
    lv_style_set_pad_bottom(&style_main_page_measure_register_label_main_main_default, 0);
    lv_style_set_pad_left(&style_main_page_measure_register_label_main_main_default, 0);
    lv_style_set_shadow_width(&style_main_page_measure_register_label_main_main_default, 0);
    lv_obj_add_style(ui->main_page_measure_register_label, &style_main_page_measure_register_label_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->main_page_measure_register_label);

    //Write codes main_page_measure_voltage_label
    ui->main_page_measure_voltage_label = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_measure_voltage_label, "1000.000");
    lv_label_set_long_mode(ui->main_page_measure_voltage_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_measure_voltage_label, 128, 69);
    lv_obj_set_size(ui->main_page_measure_voltage_label, 93, 32);

    //Write style for main_page_measure_voltage_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_measure_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_measure_voltage_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_voltage_label, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_measure_voltage_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_measure_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_measure_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_measure_voltage_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_measure_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_measure_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_measure_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_measure_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_measure_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_measure_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_3
    ui->main_page_label_3 = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_label_3, "1000.000");
    lv_label_set_long_mode(ui->main_page_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_3, 15, 149);
    lv_obj_set_size(ui->main_page_label_3, 93, 32);

    //Write style for main_page_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_3, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spangroup_7
    ui->main_page_spangroup_7 = lv_spangroup_create(ui->main_page);
    lv_spangroup_set_align(ui->main_page_spangroup_7, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->main_page_spangroup_7, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->main_page_spangroup_7, LV_SPAN_MODE_BREAK);
    //create span
    ui->main_page_spangroup_7_span = lv_spangroup_new_span(ui->main_page_spangroup_7);
    lv_span_set_text(ui->main_page_spangroup_7_span, "V\n\n");
    lv_style_set_text_color(&ui->main_page_spangroup_7_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->main_page_spangroup_7_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->main_page_spangroup_7_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->main_page_spangroup_7, 205, 87);
    lv_obj_set_size(ui->main_page_spangroup_7, 80, 16);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spangroup_7_main_main_default
    static lv_style_t style_main_page_spangroup_7_main_main_default;
    ui_init_style(&style_main_page_spangroup_7_main_main_default);

    lv_style_set_border_width(&style_main_page_spangroup_7_main_main_default, 0);
    lv_style_set_radius(&style_main_page_spangroup_7_main_main_default, 0);
    lv_style_set_bg_opa(&style_main_page_spangroup_7_main_main_default, 0);
    lv_style_set_pad_top(&style_main_page_spangroup_7_main_main_default, 0);
    lv_style_set_pad_right(&style_main_page_spangroup_7_main_main_default, 0);
    lv_style_set_pad_bottom(&style_main_page_spangroup_7_main_main_default, 0);
    lv_style_set_pad_left(&style_main_page_spangroup_7_main_main_default, 0);
    lv_style_set_shadow_width(&style_main_page_spangroup_7_main_main_default, 0);
    lv_obj_add_style(ui->main_page_spangroup_7, &style_main_page_spangroup_7_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->main_page_spangroup_7);

    //Write codes main_page_spangroup_8
    ui->main_page_spangroup_8 = lv_spangroup_create(ui->main_page);
    lv_spangroup_set_align(ui->main_page_spangroup_8, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->main_page_spangroup_8, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->main_page_spangroup_8, LV_SPAN_MODE_BREAK);
    //create span
    ui->main_page_spangroup_8_span = lv_spangroup_new_span(ui->main_page_spangroup_8);
    lv_span_set_text(ui->main_page_spangroup_8_span, "kΩ\n");
    lv_style_set_text_color(&ui->main_page_spangroup_8_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->main_page_spangroup_8_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->main_page_spangroup_8_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->main_page_spangroup_8, 74, 167);
    lv_obj_set_size(ui->main_page_spangroup_8, 80, 16);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spangroup_8_main_main_default
    static lv_style_t style_main_page_spangroup_8_main_main_default;
    ui_init_style(&style_main_page_spangroup_8_main_main_default);

    lv_style_set_border_width(&style_main_page_spangroup_8_main_main_default, 0);
    lv_style_set_radius(&style_main_page_spangroup_8_main_main_default, 0);
    lv_style_set_bg_opa(&style_main_page_spangroup_8_main_main_default, 0);
    lv_style_set_pad_top(&style_main_page_spangroup_8_main_main_default, 0);
    lv_style_set_pad_right(&style_main_page_spangroup_8_main_main_default, 0);
    lv_style_set_pad_bottom(&style_main_page_spangroup_8_main_main_default, 0);
    lv_style_set_pad_left(&style_main_page_spangroup_8_main_main_default, 0);
    lv_style_set_shadow_width(&style_main_page_spangroup_8_main_main_default, 0);
    lv_obj_add_style(ui->main_page_spangroup_8, &style_main_page_spangroup_8_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->main_page_spangroup_8);

    //Write codes main_page_measure_power_label
    ui->main_page_measure_power_label = lv_spangroup_create(ui->main_page);
    lv_spangroup_set_align(ui->main_page_measure_power_label, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->main_page_measure_power_label, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->main_page_measure_power_label, LV_SPAN_MODE_BREAK);
    //create span
    ui->main_page_measure_power_label_span = lv_spangroup_new_span(ui->main_page_measure_power_label);
    lv_span_set_text(ui->main_page_measure_power_label_span, "Power\n\n\n\n\n\n");
    lv_style_set_text_color(&ui->main_page_measure_power_label_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->main_page_measure_power_label_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->main_page_measure_power_label_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->main_page_measure_power_label, 148, 123);
    lv_obj_set_size(ui->main_page_measure_power_label, 109, 19);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_measure_power_label_main_main_default
    static lv_style_t style_main_page_measure_power_label_main_main_default;
    ui_init_style(&style_main_page_measure_power_label_main_main_default);

    lv_style_set_border_width(&style_main_page_measure_power_label_main_main_default, 0);
    lv_style_set_radius(&style_main_page_measure_power_label_main_main_default, 0);
    lv_style_set_bg_opa(&style_main_page_measure_power_label_main_main_default, 0);
    lv_style_set_pad_top(&style_main_page_measure_power_label_main_main_default, 0);
    lv_style_set_pad_right(&style_main_page_measure_power_label_main_main_default, 0);
    lv_style_set_pad_bottom(&style_main_page_measure_power_label_main_main_default, 0);
    lv_style_set_pad_left(&style_main_page_measure_power_label_main_main_default, 0);
    lv_style_set_shadow_width(&style_main_page_measure_power_label_main_main_default, 0);
    lv_obj_add_style(ui->main_page_measure_power_label, &style_main_page_measure_power_label_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->main_page_measure_power_label);

    //Write codes main_page_label_4
    ui->main_page_label_4 = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_label_4, "1000.000");
    lv_label_set_long_mode(ui->main_page_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_4, 133, 149);
    lv_obj_set_size(ui->main_page_label_4, 93, 32);

    //Write style for main_page_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_4, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spangroup_10
    ui->main_page_spangroup_10 = lv_spangroup_create(ui->main_page);
    lv_spangroup_set_align(ui->main_page_spangroup_10, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->main_page_spangroup_10, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->main_page_spangroup_10, LV_SPAN_MODE_BREAK);
    //create span
    ui->main_page_spangroup_10_span = lv_spangroup_new_span(ui->main_page_spangroup_10);
    lv_span_set_text(ui->main_page_spangroup_10_span, "W\n");
    lv_style_set_text_color(&ui->main_page_spangroup_10_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->main_page_spangroup_10_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->main_page_spangroup_10_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->main_page_spangroup_10, 205, 167);
    lv_obj_set_size(ui->main_page_spangroup_10, 80, 16);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spangroup_10_main_main_default
    static lv_style_t style_main_page_spangroup_10_main_main_default;
    ui_init_style(&style_main_page_spangroup_10_main_main_default);

    lv_style_set_border_width(&style_main_page_spangroup_10_main_main_default, 0);
    lv_style_set_radius(&style_main_page_spangroup_10_main_main_default, 0);
    lv_style_set_bg_opa(&style_main_page_spangroup_10_main_main_default, 0);
    lv_style_set_pad_top(&style_main_page_spangroup_10_main_main_default, 0);
    lv_style_set_pad_right(&style_main_page_spangroup_10_main_main_default, 0);
    lv_style_set_pad_bottom(&style_main_page_spangroup_10_main_main_default, 0);
    lv_style_set_pad_left(&style_main_page_spangroup_10_main_main_default, 0);
    lv_style_set_shadow_width(&style_main_page_spangroup_10_main_main_default, 0);
    lv_obj_add_style(ui->main_page_spangroup_10, &style_main_page_spangroup_10_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->main_page_spangroup_10);

    //Write codes main_page_btn_11
    ui->main_page_btn_11 = lv_btn_create(ui->main_page);
    ui->main_page_btn_11_label = lv_label_create(ui->main_page_btn_11);
    lv_label_set_text(ui->main_page_btn_11_label, "\n\n\n");
    lv_label_set_long_mode(ui->main_page_btn_11_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->main_page_btn_11_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_11, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_11_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_11, 243, 69);
    lv_obj_set_size(ui->main_page_btn_11, 73, 117);

    //Write style for main_page_btn_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_11, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_11, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_11, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_11, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_set_current_label
    ui->main_page_set_current_label = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_set_current_label, "1000.000");
    lv_label_set_long_mode(ui->main_page_set_current_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_set_current_label, 229, 123);
    lv_obj_set_size(ui->main_page_set_current_label, 100, 32);

    //Write style for main_page_set_current_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_set_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_set_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_set_current_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_set_current_label, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_set_current_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_set_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_set_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_set_current_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_set_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_set_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_set_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_set_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_set_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_set_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spangroup_11
    ui->main_page_spangroup_11 = lv_spangroup_create(ui->main_page);
    lv_spangroup_set_align(ui->main_page_spangroup_11, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->main_page_spangroup_11, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->main_page_spangroup_11, LV_SPAN_MODE_BREAK);
    //create span
    ui->main_page_spangroup_11_span = lv_spangroup_new_span(ui->main_page_spangroup_11);
    lv_span_set_text(ui->main_page_spangroup_11_span, "SetPoint\nCurrent");
    lv_style_set_text_color(&ui->main_page_spangroup_11_span->style, lv_color_hex(0xffffff));
    lv_style_set_text_decor(&ui->main_page_spangroup_11_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->main_page_spangroup_11_span->style, &lv_font_montserratMedium_15);
    lv_obj_set_pos(ui->main_page_spangroup_11, 248, 80);
    lv_obj_set_size(ui->main_page_spangroup_11, 80, 33);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spangroup_11_main_main_default
    static lv_style_t style_main_page_spangroup_11_main_main_default;
    ui_init_style(&style_main_page_spangroup_11_main_main_default);

    lv_style_set_border_width(&style_main_page_spangroup_11_main_main_default, 0);
    lv_style_set_radius(&style_main_page_spangroup_11_main_main_default, 0);
    lv_style_set_bg_opa(&style_main_page_spangroup_11_main_main_default, 0);
    lv_style_set_pad_top(&style_main_page_spangroup_11_main_main_default, 0);
    lv_style_set_pad_right(&style_main_page_spangroup_11_main_main_default, 0);
    lv_style_set_pad_bottom(&style_main_page_spangroup_11_main_main_default, 0);
    lv_style_set_pad_left(&style_main_page_spangroup_11_main_main_default, 0);
    lv_style_set_shadow_width(&style_main_page_spangroup_11_main_main_default, 0);
    lv_obj_add_style(ui->main_page_spangroup_11, &style_main_page_spangroup_11_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->main_page_spangroup_11);

    //Write codes main_page_spangroup_12
    ui->main_page_spangroup_12 = lv_spangroup_create(ui->main_page);
    lv_spangroup_set_align(ui->main_page_spangroup_12, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->main_page_spangroup_12, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->main_page_spangroup_12, LV_SPAN_MODE_BREAK);
    //create span
    ui->main_page_spangroup_12_span = lv_spangroup_new_span(ui->main_page_spangroup_12);
    lv_span_set_text(ui->main_page_spangroup_12_span, "mA");
    lv_style_set_text_color(&ui->main_page_spangroup_12_span->style, lv_color_hex(0xffffff));
    lv_style_set_text_decor(&ui->main_page_spangroup_12_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->main_page_spangroup_12_span->style, &lv_font_montserratMedium_15);
    lv_obj_set_pos(ui->main_page_spangroup_12, 282, 138);
    lv_obj_set_size(ui->main_page_spangroup_12, 80, 33);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spangroup_12_main_main_default
    static lv_style_t style_main_page_spangroup_12_main_main_default;
    ui_init_style(&style_main_page_spangroup_12_main_main_default);

    lv_style_set_border_width(&style_main_page_spangroup_12_main_main_default, 0);
    lv_style_set_radius(&style_main_page_spangroup_12_main_main_default, 0);
    lv_style_set_bg_opa(&style_main_page_spangroup_12_main_main_default, 0);
    lv_style_set_pad_top(&style_main_page_spangroup_12_main_main_default, 0);
    lv_style_set_pad_right(&style_main_page_spangroup_12_main_main_default, 0);
    lv_style_set_pad_bottom(&style_main_page_spangroup_12_main_main_default, 0);
    lv_style_set_pad_left(&style_main_page_spangroup_12_main_main_default, 0);
    lv_style_set_shadow_width(&style_main_page_spangroup_12_main_main_default, 0);
    lv_obj_add_style(ui->main_page_spangroup_12, &style_main_page_spangroup_12_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->main_page_spangroup_12);

    //The custom code of main_page.


    //Update current screen layout.
    lv_obj_update_layout(ui->main_page);

}
