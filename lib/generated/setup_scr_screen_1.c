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



void setup_scr_screen_1(lv_ui *ui)
{
    //Write codes screen_1
    ui->screen_1 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1, lv_color_hex(0x59c7ec), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_img_1
    ui->screen_1_img_1 = lv_img_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_1_img_1, &_white_alpha_239x206);
    lv_img_set_pivot(ui->screen_1_img_1, 50,50);
    lv_img_set_angle(ui->screen_1_img_1, 0);
    lv_obj_set_pos(ui->screen_1_img_1, 0, 34);
    lv_obj_set_size(ui->screen_1_img_1, 239, 206);

    //Write style for screen_1_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_img_1, 79, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_1_img_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_1_img_1, 112, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_img_3
    ui->screen_1_img_3 = lv_img_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_img_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_1_img_3, &_electric_sensor_3239_alpha_28x24);
    lv_img_set_pivot(ui->screen_1_img_3, 50,50);
    lv_img_set_angle(ui->screen_1_img_3, 0);
    lv_obj_set_pos(ui->screen_1_img_3, 5, 5);
    lv_obj_set_size(ui->screen_1_img_3, 28, 24);

    //Write style for screen_1_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_1_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_img_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_btn_4
    ui->screen_1_btn_4 = lv_btn_create(ui->screen_1);
    ui->screen_1_btn_4_label = lv_label_create(ui->screen_1_btn_4);
    lv_label_set_text(ui->screen_1_btn_4_label, "Mode: CC");
    lv_label_set_long_mode(ui->screen_1_btn_4_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_4_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_4, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_4_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_4, 40, 5);
    lv_obj_set_size(ui->screen_1_btn_4, 88, 23);

    //Write style for screen_1_btn_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_4, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_4, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_4, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_img_4
    ui->screen_1_img_4 = lv_img_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_img_4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_1_img_4, &_blue_alpha_59x240);
    lv_img_set_pivot(ui->screen_1_img_4, 50,50);
    lv_img_set_angle(ui->screen_1_img_4, 0);
    lv_obj_set_pos(ui->screen_1_img_4, 237, 0);
    lv_obj_set_size(ui->screen_1_img_4, 59, 240);

    //Write style for screen_1_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_1_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_img_4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_btn_3
    ui->screen_1_btn_3 = lv_btn_create(ui->screen_1);
    ui->screen_1_btn_3_label = lv_label_create(ui->screen_1_btn_3);
    lv_label_set_text(ui->screen_1_btn_3_label, "+0.01");
    lv_label_set_long_mode(ui->screen_1_btn_3_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_3_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_3, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_3_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_3, 275, 34);
    lv_obj_set_size(ui->screen_1_btn_3, 41, 26);

    //Write style for screen_1_btn_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_3, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_3, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_3, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_btn_1
    ui->screen_1_btn_1 = lv_btn_create(ui->screen_1);
    ui->screen_1_btn_1_label = lv_label_create(ui->screen_1_btn_1);
    lv_label_set_text(ui->screen_1_btn_1_label, "+1");
    lv_label_set_long_mode(ui->screen_1_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_1, 243, 34);
    lv_obj_set_size(ui->screen_1_btn_1, 29, 26);

    //Write style for screen_1_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_spangroup_1
    ui->screen_1_spangroup_1 = lv_spangroup_create(ui->screen_1);
    lv_spangroup_set_align(ui->screen_1_spangroup_1, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_1_spangroup_1, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_1_spangroup_1, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_1_spangroup_1_span = lv_spangroup_new_span(ui->screen_1_spangroup_1);
    lv_span_set_text(ui->screen_1_spangroup_1_span, "Details\n");
    lv_style_set_text_color(&ui->screen_1_spangroup_1_span->style, lv_color_hex(0xffffff));
    lv_style_set_text_decor(&ui->screen_1_spangroup_1_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->screen_1_spangroup_1_span->style, &lv_font_montserratMedium_16);
    lv_obj_set_pos(ui->screen_1_spangroup_1, 133, 9);
    lv_obj_set_size(ui->screen_1_spangroup_1, 81, 11);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_spangroup_1_main_main_default
    static lv_style_t style_screen_1_spangroup_1_main_main_default;
    ui_init_style(&style_screen_1_spangroup_1_main_main_default);

    lv_style_set_border_width(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_radius(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_spangroup_1, &style_screen_1_spangroup_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_1_spangroup_1);

    //Write codes screen_1_sw_1
    ui->screen_1_sw_1 = lv_switch_create(ui->screen_1);
    lv_obj_set_pos(ui->screen_1_sw_1, 194, 5);
    lv_obj_set_size(ui->screen_1_sw_1, 35, 20);

    //Write style for screen_1_sw_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_sw_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_sw_1, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_sw_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_sw_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_1_sw_1, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->screen_1_sw_1, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->screen_1_sw_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_sw_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->screen_1_sw_1, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for screen_1_sw_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_sw_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_sw_1, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_sw_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_sw_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_sw_1, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_1_spangroup_2
    ui->screen_1_spangroup_2 = lv_spangroup_create(ui->screen_1);
    lv_spangroup_set_align(ui->screen_1_spangroup_2, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_1_spangroup_2, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_1_spangroup_2, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_1_spangroup_2_span = lv_spangroup_new_span(ui->screen_1_spangroup_2);
    lv_span_set_text(ui->screen_1_spangroup_2_span, "SET");
    lv_style_set_text_color(&ui->screen_1_spangroup_2_span->style, lv_color_hex(0xffffff));
    lv_style_set_text_decor(&ui->screen_1_spangroup_2_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->screen_1_spangroup_2_span->style, &lv_font_montserratMedium_23);
    lv_obj_set_pos(ui->screen_1_spangroup_2, 248, 5);
    lv_obj_set_size(ui->screen_1_spangroup_2, 93, 21);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_spangroup_2_main_main_default
    static lv_style_t style_screen_1_spangroup_2_main_main_default;
    ui_init_style(&style_screen_1_spangroup_2_main_main_default);

    lv_style_set_border_width(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_radius(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_spangroup_2, &style_screen_1_spangroup_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_1_spangroup_2);

    //Write codes screen_1_btn_6
    ui->screen_1_btn_6 = lv_btn_create(ui->screen_1);
    ui->screen_1_btn_6_label = lv_label_create(ui->screen_1_btn_6);
    lv_label_set_text(ui->screen_1_btn_6_label, "-0.01");
    lv_label_set_long_mode(ui->screen_1_btn_6_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_6_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_6, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_6_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_6, 275, 208);
    lv_obj_set_size(ui->screen_1_btn_6, 41, 26);

    //Write style for screen_1_btn_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_6, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_6, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_6, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_btn_5
    ui->screen_1_btn_5 = lv_btn_create(ui->screen_1);
    ui->screen_1_btn_5_label = lv_label_create(ui->screen_1_btn_5);
    lv_label_set_text(ui->screen_1_btn_5_label, "-1");
    lv_label_set_long_mode(ui->screen_1_btn_5_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_5_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_5, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_5_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_5, 243, 208);
    lv_obj_set_size(ui->screen_1_btn_5, 29, 26);

    //Write style for screen_1_btn_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_5, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_5, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_5, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_btn_7
    ui->screen_1_btn_7 = lv_btn_create(ui->screen_1);
    ui->screen_1_btn_7_label = lv_label_create(ui->screen_1_btn_7);
    lv_label_set_text(ui->screen_1_btn_7_label, "");
    lv_label_set_long_mode(ui->screen_1_btn_7_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_7_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_7, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_7_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_7, 7, 41);
    lv_obj_set_size(ui->screen_1_btn_7, 108, 70);

    //Write style for screen_1_btn_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_7, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_7, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_7, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_spangroup_3
    ui->screen_1_spangroup_3 = lv_spangroup_create(ui->screen_1);
    lv_spangroup_set_align(ui->screen_1_spangroup_3, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_1_spangroup_3, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_1_spangroup_3, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_1_spangroup_3_span = lv_spangroup_new_span(ui->screen_1_spangroup_3);
    lv_span_set_text(ui->screen_1_spangroup_3_span, "Current");
    lv_style_set_text_color(&ui->screen_1_spangroup_3_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->screen_1_spangroup_3_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->screen_1_spangroup_3_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->screen_1_spangroup_3, 33, 43);
    lv_obj_set_size(ui->screen_1_spangroup_3, 80, 100);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_spangroup_3_main_main_default
    static lv_style_t style_screen_1_spangroup_3_main_main_default;
    ui_init_style(&style_screen_1_spangroup_3_main_main_default);

    lv_style_set_border_width(&style_screen_1_spangroup_3_main_main_default, 0);
    lv_style_set_radius(&style_screen_1_spangroup_3_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_spangroup_3_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_1_spangroup_3_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_spangroup_3_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_spangroup_3_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_spangroup_3_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_1_spangroup_3_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_spangroup_3, &style_screen_1_spangroup_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_1_spangroup_3);

    //Write codes screen_1_img_5
    ui->screen_1_img_5 = lv_img_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_img_5, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_1_img_5, &_cc_alpha_20x19);
    lv_img_set_pivot(ui->screen_1_img_5, 50,50);
    lv_img_set_angle(ui->screen_1_img_5, 0);
    lv_obj_set_pos(ui->screen_1_img_5, 12, 43);
    lv_obj_set_size(ui->screen_1_img_5, 20, 19);

    //Write style for screen_1_img_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_img_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_1_img_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_img_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_img_5, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_label_1
    ui->screen_1_label_1 = lv_label_create(ui->screen_1);
    lv_label_set_text(ui->screen_1_label_1, "1000.000");
    lv_label_set_long_mode(ui->screen_1_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_1, 15, 69);
    lv_obj_set_size(ui->screen_1_label_1, 93, 32);

    //Write style for screen_1_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_1, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_spangroup_4
    ui->screen_1_spangroup_4 = lv_spangroup_create(ui->screen_1);
    lv_spangroup_set_align(ui->screen_1_spangroup_4, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_1_spangroup_4, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_1_spangroup_4, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_1_spangroup_4_span = lv_spangroup_new_span(ui->screen_1_spangroup_4);
    lv_span_set_text(ui->screen_1_spangroup_4_span, "mA\n");
    lv_style_set_text_color(&ui->screen_1_spangroup_4_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->screen_1_spangroup_4_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->screen_1_spangroup_4_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->screen_1_spangroup_4, 71, 87);
    lv_obj_set_size(ui->screen_1_spangroup_4, 80, 16);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_spangroup_4_main_main_default
    static lv_style_t style_screen_1_spangroup_4_main_main_default;
    ui_init_style(&style_screen_1_spangroup_4_main_main_default);

    lv_style_set_border_width(&style_screen_1_spangroup_4_main_main_default, 0);
    lv_style_set_radius(&style_screen_1_spangroup_4_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_spangroup_4_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_1_spangroup_4_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_spangroup_4_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_spangroup_4_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_spangroup_4_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_1_spangroup_4_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_spangroup_4, &style_screen_1_spangroup_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_1_spangroup_4);

    //Write codes screen_1_btn_8
    ui->screen_1_btn_8 = lv_btn_create(ui->screen_1);
    ui->screen_1_btn_8_label = lv_label_create(ui->screen_1_btn_8);
    lv_label_set_text(ui->screen_1_btn_8_label, "");
    lv_label_set_long_mode(ui->screen_1_btn_8_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_8_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_8, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_8_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_8, 122, 41);
    lv_obj_set_size(ui->screen_1_btn_8, 108, 70);

    //Write style for screen_1_btn_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_8, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_8, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_8, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_8, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_8, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_btn_9
    ui->screen_1_btn_9 = lv_btn_create(ui->screen_1);
    ui->screen_1_btn_9_label = lv_label_create(ui->screen_1_btn_9);
    lv_label_set_text(ui->screen_1_btn_9_label, "");
    lv_label_set_long_mode(ui->screen_1_btn_9_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_9_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_9, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_9_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_9, 7, 120);
    lv_obj_set_size(ui->screen_1_btn_9, 108, 70);

    //Write style for screen_1_btn_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_9, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_9, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_9, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_9, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_btn_10
    ui->screen_1_btn_10 = lv_btn_create(ui->screen_1);
    ui->screen_1_btn_10_label = lv_label_create(ui->screen_1_btn_10);
    lv_label_set_text(ui->screen_1_btn_10_label, "");
    lv_label_set_long_mode(ui->screen_1_btn_10_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_10_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_10, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_10_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_10, 122, 120);
    lv_obj_set_size(ui->screen_1_btn_10, 108, 70);

    //Write style for screen_1_btn_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_10, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_10, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_10, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_10, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_10, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_spangroup_5
    ui->screen_1_spangroup_5 = lv_spangroup_create(ui->screen_1);
    lv_spangroup_set_align(ui->screen_1_spangroup_5, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_1_spangroup_5, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_1_spangroup_5, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_1_spangroup_5_span = lv_spangroup_new_span(ui->screen_1_spangroup_5);
    lv_span_set_text(ui->screen_1_spangroup_5_span, "Voltage\n\n\n\n\n");
    lv_style_set_text_color(&ui->screen_1_spangroup_5_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->screen_1_spangroup_5_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->screen_1_spangroup_5_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->screen_1_spangroup_5, 148, 43);
    lv_obj_set_size(ui->screen_1_spangroup_5, 87, 22);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_spangroup_5_main_main_default
    static lv_style_t style_screen_1_spangroup_5_main_main_default;
    ui_init_style(&style_screen_1_spangroup_5_main_main_default);

    lv_style_set_border_width(&style_screen_1_spangroup_5_main_main_default, 0);
    lv_style_set_radius(&style_screen_1_spangroup_5_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_spangroup_5_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_1_spangroup_5_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_spangroup_5_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_spangroup_5_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_spangroup_5_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_1_spangroup_5_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_spangroup_5, &style_screen_1_spangroup_5_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_1_spangroup_5);

    //Write codes screen_1_spangroup_6
    ui->screen_1_spangroup_6 = lv_spangroup_create(ui->screen_1);
    lv_spangroup_set_align(ui->screen_1_spangroup_6, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_1_spangroup_6, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_1_spangroup_6, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_1_spangroup_6_span = lv_spangroup_new_span(ui->screen_1_spangroup_6);
    lv_span_set_text(ui->screen_1_spangroup_6_span, "Register\n\n\n\n\n");
    lv_style_set_text_color(&ui->screen_1_spangroup_6_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->screen_1_spangroup_6_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->screen_1_spangroup_6_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->screen_1_spangroup_6, 27, 123);
    lv_obj_set_size(ui->screen_1_spangroup_6, 109, 19);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_spangroup_6_main_main_default
    static lv_style_t style_screen_1_spangroup_6_main_main_default;
    ui_init_style(&style_screen_1_spangroup_6_main_main_default);

    lv_style_set_border_width(&style_screen_1_spangroup_6_main_main_default, 0);
    lv_style_set_radius(&style_screen_1_spangroup_6_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_spangroup_6_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_1_spangroup_6_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_spangroup_6_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_spangroup_6_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_spangroup_6_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_1_spangroup_6_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_spangroup_6, &style_screen_1_spangroup_6_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_1_spangroup_6);

    //Write codes screen_1_label_2
    ui->screen_1_label_2 = lv_label_create(ui->screen_1);
    lv_label_set_text(ui->screen_1_label_2, "1000.000");
    lv_label_set_long_mode(ui->screen_1_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_2, 128, 69);
    lv_obj_set_size(ui->screen_1_label_2, 93, 32);

    //Write style for screen_1_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_2, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_label_3
    ui->screen_1_label_3 = lv_label_create(ui->screen_1);
    lv_label_set_text(ui->screen_1_label_3, "1000.000");
    lv_label_set_long_mode(ui->screen_1_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_3, 15, 149);
    lv_obj_set_size(ui->screen_1_label_3, 93, 32);

    //Write style for screen_1_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_3, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_spangroup_7
    ui->screen_1_spangroup_7 = lv_spangroup_create(ui->screen_1);
    lv_spangroup_set_align(ui->screen_1_spangroup_7, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_1_spangroup_7, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_1_spangroup_7, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_1_spangroup_7_span = lv_spangroup_new_span(ui->screen_1_spangroup_7);
    lv_span_set_text(ui->screen_1_spangroup_7_span, "V\n\n");
    lv_style_set_text_color(&ui->screen_1_spangroup_7_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->screen_1_spangroup_7_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->screen_1_spangroup_7_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->screen_1_spangroup_7, 205, 87);
    lv_obj_set_size(ui->screen_1_spangroup_7, 80, 16);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_spangroup_7_main_main_default
    static lv_style_t style_screen_1_spangroup_7_main_main_default;
    ui_init_style(&style_screen_1_spangroup_7_main_main_default);

    lv_style_set_border_width(&style_screen_1_spangroup_7_main_main_default, 0);
    lv_style_set_radius(&style_screen_1_spangroup_7_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_spangroup_7_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_1_spangroup_7_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_spangroup_7_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_spangroup_7_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_spangroup_7_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_1_spangroup_7_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_spangroup_7, &style_screen_1_spangroup_7_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_1_spangroup_7);

    //Write codes screen_1_spangroup_8
    ui->screen_1_spangroup_8 = lv_spangroup_create(ui->screen_1);
    lv_spangroup_set_align(ui->screen_1_spangroup_8, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_1_spangroup_8, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_1_spangroup_8, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_1_spangroup_8_span = lv_spangroup_new_span(ui->screen_1_spangroup_8);
    lv_span_set_text(ui->screen_1_spangroup_8_span, "kΩ\n");
    lv_style_set_text_color(&ui->screen_1_spangroup_8_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->screen_1_spangroup_8_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->screen_1_spangroup_8_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->screen_1_spangroup_8, 74, 167);
    lv_obj_set_size(ui->screen_1_spangroup_8, 80, 16);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_spangroup_8_main_main_default
    static lv_style_t style_screen_1_spangroup_8_main_main_default;
    ui_init_style(&style_screen_1_spangroup_8_main_main_default);

    lv_style_set_border_width(&style_screen_1_spangroup_8_main_main_default, 0);
    lv_style_set_radius(&style_screen_1_spangroup_8_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_spangroup_8_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_1_spangroup_8_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_spangroup_8_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_spangroup_8_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_spangroup_8_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_1_spangroup_8_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_spangroup_8, &style_screen_1_spangroup_8_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_1_spangroup_8);

    //Write codes screen_1_spangroup_9
    ui->screen_1_spangroup_9 = lv_spangroup_create(ui->screen_1);
    lv_spangroup_set_align(ui->screen_1_spangroup_9, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_1_spangroup_9, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_1_spangroup_9, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_1_spangroup_9_span = lv_spangroup_new_span(ui->screen_1_spangroup_9);
    lv_span_set_text(ui->screen_1_spangroup_9_span, "Power\n\n\n\n\n\n");
    lv_style_set_text_color(&ui->screen_1_spangroup_9_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->screen_1_spangroup_9_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->screen_1_spangroup_9_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->screen_1_spangroup_9, 148, 123);
    lv_obj_set_size(ui->screen_1_spangroup_9, 109, 19);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_spangroup_9_main_main_default
    static lv_style_t style_screen_1_spangroup_9_main_main_default;
    ui_init_style(&style_screen_1_spangroup_9_main_main_default);

    lv_style_set_border_width(&style_screen_1_spangroup_9_main_main_default, 0);
    lv_style_set_radius(&style_screen_1_spangroup_9_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_spangroup_9_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_1_spangroup_9_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_spangroup_9_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_spangroup_9_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_spangroup_9_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_1_spangroup_9_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_spangroup_9, &style_screen_1_spangroup_9_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_1_spangroup_9);

    //Write codes screen_1_label_4
    ui->screen_1_label_4 = lv_label_create(ui->screen_1);
    lv_label_set_text(ui->screen_1_label_4, "1000.000");
    lv_label_set_long_mode(ui->screen_1_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_4, 133, 149);
    lv_obj_set_size(ui->screen_1_label_4, 93, 32);

    //Write style for screen_1_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_4, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_spangroup_10
    ui->screen_1_spangroup_10 = lv_spangroup_create(ui->screen_1);
    lv_spangroup_set_align(ui->screen_1_spangroup_10, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_1_spangroup_10, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_1_spangroup_10, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_1_spangroup_10_span = lv_spangroup_new_span(ui->screen_1_spangroup_10);
    lv_span_set_text(ui->screen_1_spangroup_10_span, "W\n");
    lv_style_set_text_color(&ui->screen_1_spangroup_10_span->style, lv_color_hex(0x2F92DA));
    lv_style_set_text_decor(&ui->screen_1_spangroup_10_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->screen_1_spangroup_10_span->style, &lv_font_montserratMedium_20);
    lv_obj_set_pos(ui->screen_1_spangroup_10, 205, 167);
    lv_obj_set_size(ui->screen_1_spangroup_10, 80, 16);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_spangroup_10_main_main_default
    static lv_style_t style_screen_1_spangroup_10_main_main_default;
    ui_init_style(&style_screen_1_spangroup_10_main_main_default);

    lv_style_set_border_width(&style_screen_1_spangroup_10_main_main_default, 0);
    lv_style_set_radius(&style_screen_1_spangroup_10_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_spangroup_10_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_1_spangroup_10_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_spangroup_10_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_spangroup_10_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_spangroup_10_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_1_spangroup_10_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_spangroup_10, &style_screen_1_spangroup_10_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_1_spangroup_10);

    //Write codes screen_1_btn_11
    ui->screen_1_btn_11 = lv_btn_create(ui->screen_1);
    ui->screen_1_btn_11_label = lv_label_create(ui->screen_1_btn_11);
    lv_label_set_text(ui->screen_1_btn_11_label, "SET Point\nCurrent");
    lv_label_set_long_mode(ui->screen_1_btn_11_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_btn_11_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_btn_11, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_btn_11_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_btn_11, 243, 78);
    lv_obj_set_size(ui->screen_1_btn_11, 73, 110);

    //Write style for screen_1_btn_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_btn_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_btn_11, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_11, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_btn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_btn_11, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_btn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_btn_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_btn_11, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_btn_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_btn_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_1.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_1);

}
