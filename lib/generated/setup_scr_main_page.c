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
#include "custom.h"



int main_page_running_time_min_value = 0;
int main_page_running_time_hour_value = 0;
int main_page_running_time_sec_value = 0;
void setup_scr_main_page(lv_ui *ui)
{
    //Write codes main_page
    ui->main_page = lv_obj_create(NULL);
    lv_obj_set_size(ui->main_page, 480, 320);
    lv_obj_set_scrollbar_mode(ui->main_page, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_main_tileview
    ui->main_page_main_tileview = lv_tileview_create(ui->main_page);
    ui->main_page_main_tileview_main = lv_tileview_add_tile(ui->main_page_main_tileview, 0, 0, LV_DIR_BOTTOM);
    ui->main_page_main_tileview_setting = lv_tileview_add_tile(ui->main_page_main_tileview, 0, 1, LV_DIR_TOP | LV_DIR_BOTTOM);
    ui->main_page_main_tileview_about = lv_tileview_add_tile(ui->main_page_main_tileview, 0, 2, LV_DIR_TOP);
    lv_obj_set_pos(ui->main_page_main_tileview, 337, 0);
    lv_obj_set_size(ui->main_page_main_tileview, 143, 320);
    lv_obj_set_scrollbar_mode(ui->main_page_main_tileview, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_main_tileview, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_main_tileview, 21, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_main_tileview, lv_color_hex(0xdcdcdc), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_main_tileview, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_main_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_main_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);



    //Write codes main_page_load_regulation_container
    ui->main_page_load_regulation_container = lv_obj_create(ui->main_page_main_tileview_main);
    lv_obj_set_pos(ui->main_page_load_regulation_container, 6, 101);
    lv_obj_set_size(ui->main_page_load_regulation_container, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_load_regulation_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_load_regulation_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_load_regulation_container, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_load_regulation_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_load_regulation_container, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_load_regulation_container, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_load_regulation_container, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_load_regulation_container, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_load_regulation_container, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_load_regulation_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_load_regulation_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_load_regulation_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_load_regulation_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_load_regulation_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_load_regulation_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_start_load_regulation_button
    ui->main_page_start_load_regulation_button = lv_btn_create(ui->main_page_load_regulation_container);
    ui->main_page_start_load_regulation_button_label = lv_label_create(ui->main_page_start_load_regulation_button);
    lv_label_set_text(ui->main_page_start_load_regulation_button_label, "开始测量");
    lv_label_set_long_mode(ui->main_page_start_load_regulation_button_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->main_page_start_load_regulation_button_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_start_load_regulation_button, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_start_load_regulation_button_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_start_load_regulation_button, 64, 24);
    lv_obj_set_size(ui->main_page_start_load_regulation_button, 57, 23);

    //Write style for main_page_start_load_regulation_button, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_start_load_regulation_button, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_start_load_regulation_button, lv_color_hex(0x292929), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_start_load_regulation_button, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_start_load_regulation_button, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_start_load_regulation_button, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_start_load_regulation_button, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_start_load_regulation_button, lv_color_hex(0xd8d8d8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_start_load_regulation_button, &lv_font_OPPOSans_Medium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_start_load_regulation_button, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_start_load_regulation_button, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_load_regulation_description
    ui->main_page_load_regulation_description = lv_label_create(ui->main_page_load_regulation_container);
    lv_label_set_text(ui->main_page_load_regulation_description, "负载调整率");
    lv_label_set_long_mode(ui->main_page_load_regulation_description, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_load_regulation_description, 7, 4);
    lv_obj_set_size(ui->main_page_load_regulation_description, 111, 20);

    //Write style for main_page_load_regulation_description, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_load_regulation_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_load_regulation_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_load_regulation_description, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_load_regulation_description, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_load_regulation_description, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_load_regulation_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_load_regulation_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_load_regulation_description, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_load_regulation_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_load_regulation_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_load_regulation_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_load_regulation_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_load_regulation_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_load_regulation_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_load_regulation_label
    ui->main_page_load_regulation_label = lv_label_create(ui->main_page_load_regulation_container);
    lv_label_set_text(ui->main_page_load_regulation_label, "0.000");
    lv_label_set_long_mode(ui->main_page_load_regulation_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_load_regulation_label, 4, 26);
    lv_obj_set_size(ui->main_page_load_regulation_label, 55, 18);

    //Write style for main_page_load_regulation_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_load_regulation_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_load_regulation_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_load_regulation_label, lv_color_hex(0xd0d0d0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_load_regulation_label, &lv_font_IntelOneMono_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_load_regulation_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_load_regulation_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_load_regulation_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_load_regulation_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_load_regulation_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_load_regulation_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_load_regulation_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_load_regulation_label, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_load_regulation_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_load_regulation_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_set_current_container
    ui->main_page_set_current_container = lv_obj_create(ui->main_page_main_tileview_main);
    lv_obj_set_pos(ui->main_page_set_current_container, 6, 39);
    lv_obj_set_size(ui->main_page_set_current_container, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_set_current_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_set_current_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_set_current_container, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_set_current_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_set_current_container, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_set_current_container, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_set_current_container, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_set_current_container, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_set_current_container, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_set_current_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_set_current_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_set_current_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_set_current_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_set_current_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_set_current_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_set_current_description
    ui->main_page_set_current_description = lv_label_create(ui->main_page_set_current_container);
    lv_label_set_text(ui->main_page_set_current_description, "设置电流(mA)");
    lv_label_set_long_mode(ui->main_page_set_current_description, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_set_current_description, 7, 3);
    lv_obj_set_size(ui->main_page_set_current_description, 111, 20);

    //Write style for main_page_set_current_description, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_set_current_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_set_current_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_set_current_description, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_set_current_description, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_set_current_description, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_set_current_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_set_current_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_set_current_description, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_set_current_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_set_current_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_set_current_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_set_current_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_set_current_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_set_current_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_set_current_box
    ui->main_page_set_current_box = lv_spinbox_create(ui->main_page_set_current_container);
    lv_obj_set_pos(ui->main_page_set_current_box, 31, 24);
    lv_obj_set_width(ui->main_page_set_current_box, 63);
    lv_obj_set_height(ui->main_page_set_current_box, 25);
    lv_spinbox_set_digit_format(ui->main_page_set_current_box, 4, 4);
    lv_spinbox_set_range(ui->main_page_set_current_box, -9999, 9999);
    lv_coord_t main_page_set_current_box_h = lv_obj_get_height(ui->main_page_set_current_box);
    ui->main_page_set_current_box_btn_plus = lv_btn_create(ui->main_page_set_current_container);
    lv_obj_set_size(ui->main_page_set_current_box_btn_plus, main_page_set_current_box_h, main_page_set_current_box_h);
    lv_obj_align_to(ui->main_page_set_current_box_btn_plus, ui->main_page_set_current_box, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_set_current_box_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->main_page_set_current_box_btn_plus, lv_main_page_set_current_box_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->main_page_set_current_box_btn_minus = lv_btn_create(ui->main_page_set_current_container);
    lv_obj_set_size(ui->main_page_set_current_box_btn_minus, main_page_set_current_box_h, main_page_set_current_box_h);
    lv_obj_align_to(ui->main_page_set_current_box_btn_minus, ui->main_page_set_current_box, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_set_current_box_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->main_page_set_current_box_btn_minus, lv_main_page_set_current_box_decrement_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->main_page_set_current_box, 31, 24);

    //Write style for main_page_set_current_box, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_set_current_box, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_set_current_box, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_set_current_box, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_set_current_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_set_current_box, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_set_current_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_set_current_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_set_current_box, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_set_current_box, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_set_current_box, &lv_font_IntelOneMono_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_set_current_box, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_set_current_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_set_current_box, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_set_current_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_set_current_box, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_set_current_box, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_set_current_box, &lv_font_IntelOneMono_Medium_11, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_set_current_box, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_set_current_box, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_set_current_box, lv_color_hex(0x929292), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_set_current_box, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_set_current_box_extra_btns_main_default
    static lv_style_t style_main_page_set_current_box_extra_btns_main_default;
    ui_init_style(&style_main_page_set_current_box_extra_btns_main_default);

    lv_style_set_text_color(&style_main_page_set_current_box_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_set_current_box_extra_btns_main_default, &lv_font_montserratMedium_9);
    lv_style_set_text_opa(&style_main_page_set_current_box_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_main_page_set_current_box_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_main_page_set_current_box_extra_btns_main_default, lv_color_hex(0x66686a));
    lv_style_set_bg_grad_dir(&style_main_page_set_current_box_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_set_current_box_extra_btns_main_default, 0);
    lv_style_set_radius(&style_main_page_set_current_box_extra_btns_main_default, 9);
    lv_style_set_shadow_width(&style_main_page_set_current_box_extra_btns_main_default, 0);
    lv_obj_add_style(ui->main_page_set_current_box_btn_plus, &style_main_page_set_current_box_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->main_page_set_current_box_btn_minus, &style_main_page_set_current_box_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_PRESSED for &style_main_page_set_current_box_extra_btns_main_pressed
    static lv_style_t style_main_page_set_current_box_extra_btns_main_pressed;
    ui_init_style(&style_main_page_set_current_box_extra_btns_main_pressed);

    lv_style_set_text_color(&style_main_page_set_current_box_extra_btns_main_pressed, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_set_current_box_extra_btns_main_pressed, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_main_page_set_current_box_extra_btns_main_pressed, 255);
    lv_style_set_bg_opa(&style_main_page_set_current_box_extra_btns_main_pressed, 255);
    lv_style_set_bg_color(&style_main_page_set_current_box_extra_btns_main_pressed, lv_color_hex(0x696b6e));
    lv_style_set_bg_grad_dir(&style_main_page_set_current_box_extra_btns_main_pressed, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_set_current_box_extra_btns_main_pressed, 0);
    lv_style_set_radius(&style_main_page_set_current_box_extra_btns_main_pressed, 5);
    lv_style_set_shadow_width(&style_main_page_set_current_box_extra_btns_main_pressed, 0);
    lv_obj_add_style(ui->main_page_set_current_box_btn_plus, &style_main_page_set_current_box_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_style(ui->main_page_set_current_box_btn_minus, &style_main_page_set_current_box_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes main_page_over_voltage_container
    ui->main_page_over_voltage_container = lv_obj_create(ui->main_page_main_tileview_main);
    lv_obj_set_pos(ui->main_page_over_voltage_container, 6, 163);
    lv_obj_set_size(ui->main_page_over_voltage_container, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_over_voltage_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_over_voltage_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_over_voltage_container, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_over_voltage_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_over_voltage_container, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_over_voltage_container, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_over_voltage_container, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_over_voltage_container, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_over_voltage_container, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_over_voltage_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_over_voltage_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_over_voltage_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_over_voltage_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_over_voltage_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_over_voltage_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_over_volatge_description
    ui->main_page_over_volatge_description = lv_label_create(ui->main_page_over_voltage_container);
    lv_label_set_text(ui->main_page_over_volatge_description, "过压阈值(V)");
    lv_label_set_long_mode(ui->main_page_over_volatge_description, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_over_volatge_description, 7, 4);
    lv_obj_set_size(ui->main_page_over_volatge_description, 111, 20);

    //Write style for main_page_over_volatge_description, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_over_volatge_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_over_volatge_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_over_volatge_description, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_over_volatge_description, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_over_volatge_description, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_over_volatge_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_over_volatge_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_over_volatge_description, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_over_volatge_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_over_volatge_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_over_volatge_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_over_volatge_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_over_volatge_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_over_volatge_description, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_over_voltage_box
    ui->main_page_over_voltage_box = lv_spinbox_create(ui->main_page_over_voltage_container);
    lv_obj_set_pos(ui->main_page_over_voltage_box, 31, 23);
    lv_obj_set_width(ui->main_page_over_voltage_box, 63);
    lv_obj_set_height(ui->main_page_over_voltage_box, 25);
    lv_spinbox_set_digit_format(ui->main_page_over_voltage_box, 2, 2);
    lv_spinbox_set_range(ui->main_page_over_voltage_box, -99, 99);
    lv_coord_t main_page_over_voltage_box_h = lv_obj_get_height(ui->main_page_over_voltage_box);
    ui->main_page_over_voltage_box_btn_plus = lv_btn_create(ui->main_page_over_voltage_container);
    lv_obj_set_size(ui->main_page_over_voltage_box_btn_plus, main_page_over_voltage_box_h, main_page_over_voltage_box_h);
    lv_obj_align_to(ui->main_page_over_voltage_box_btn_plus, ui->main_page_over_voltage_box, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_over_voltage_box_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->main_page_over_voltage_box_btn_plus, lv_main_page_over_voltage_box_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->main_page_over_voltage_box_btn_minus = lv_btn_create(ui->main_page_over_voltage_container);
    lv_obj_set_size(ui->main_page_over_voltage_box_btn_minus, main_page_over_voltage_box_h, main_page_over_voltage_box_h);
    lv_obj_align_to(ui->main_page_over_voltage_box_btn_minus, ui->main_page_over_voltage_box, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_over_voltage_box_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->main_page_over_voltage_box_btn_minus, lv_main_page_over_voltage_box_decrement_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->main_page_over_voltage_box, 31, 23);

    //Write style for main_page_over_voltage_box, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_over_voltage_box, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_over_voltage_box, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_over_voltage_box, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_over_voltage_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_over_voltage_box, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_over_voltage_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_over_voltage_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_over_voltage_box, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_over_voltage_box, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_over_voltage_box, &lv_font_IntelOneMono_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_over_voltage_box, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_over_voltage_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_over_voltage_box, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_over_voltage_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_over_voltage_box, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_over_voltage_box, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_over_voltage_box, &lv_font_IntelOneMono_Medium_11, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_over_voltage_box, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_over_voltage_box, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_over_voltage_box, lv_color_hex(0x929292), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_over_voltage_box, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_over_voltage_box_extra_btns_main_default
    static lv_style_t style_main_page_over_voltage_box_extra_btns_main_default;
    ui_init_style(&style_main_page_over_voltage_box_extra_btns_main_default);

    lv_style_set_text_color(&style_main_page_over_voltage_box_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_over_voltage_box_extra_btns_main_default, &lv_font_montserratMedium_9);
    lv_style_set_text_opa(&style_main_page_over_voltage_box_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_main_page_over_voltage_box_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_main_page_over_voltage_box_extra_btns_main_default, lv_color_hex(0x66686a));
    lv_style_set_bg_grad_dir(&style_main_page_over_voltage_box_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_over_voltage_box_extra_btns_main_default, 0);
    lv_style_set_radius(&style_main_page_over_voltage_box_extra_btns_main_default, 9);
    lv_style_set_shadow_width(&style_main_page_over_voltage_box_extra_btns_main_default, 0);
    lv_obj_add_style(ui->main_page_over_voltage_box_btn_plus, &style_main_page_over_voltage_box_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->main_page_over_voltage_box_btn_minus, &style_main_page_over_voltage_box_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_PRESSED for &style_main_page_over_voltage_box_extra_btns_main_pressed
    static lv_style_t style_main_page_over_voltage_box_extra_btns_main_pressed;
    ui_init_style(&style_main_page_over_voltage_box_extra_btns_main_pressed);

    lv_style_set_text_color(&style_main_page_over_voltage_box_extra_btns_main_pressed, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_over_voltage_box_extra_btns_main_pressed, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_main_page_over_voltage_box_extra_btns_main_pressed, 255);
    lv_style_set_bg_opa(&style_main_page_over_voltage_box_extra_btns_main_pressed, 255);
    lv_style_set_bg_color(&style_main_page_over_voltage_box_extra_btns_main_pressed, lv_color_hex(0x696b6e));
    lv_style_set_bg_grad_dir(&style_main_page_over_voltage_box_extra_btns_main_pressed, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_over_voltage_box_extra_btns_main_pressed, 0);
    lv_style_set_radius(&style_main_page_over_voltage_box_extra_btns_main_pressed, 5);
    lv_style_set_shadow_width(&style_main_page_over_voltage_box_extra_btns_main_pressed, 0);
    lv_obj_add_style(ui->main_page_over_voltage_box_btn_plus, &style_main_page_over_voltage_box_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_style(ui->main_page_over_voltage_box_btn_minus, &style_main_page_over_voltage_box_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes main_page_mode_container
    ui->main_page_mode_container = lv_obj_create(ui->main_page_main_tileview_main);
    lv_obj_set_pos(ui->main_page_mode_container, 6, 225);
    lv_obj_set_size(ui->main_page_mode_container, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_mode_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_mode_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_mode_container, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_mode_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_mode_container, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_mode_container, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_mode_container, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_mode_container, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_mode_container, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_mode_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_mode_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_mode_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_mode_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_mode_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_mode_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_11
    ui->main_page_label_11 = lv_label_create(ui->main_page_mode_container);
    lv_label_set_text(ui->main_page_label_11, "负载模式");
    lv_label_set_long_mode(ui->main_page_label_11, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_11, 7, 3);
    lv_obj_set_size(ui->main_page_label_11, 111, 20);

    //Write style for main_page_label_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_11, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_11, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_ddlist_1
    ui->main_page_ddlist_1 = lv_dropdown_create(ui->main_page_mode_container);
    lv_dropdown_set_options(ui->main_page_ddlist_1, "恒流 CC\n恒功率 CP\n恒阻 CR");
    lv_obj_set_pos(ui->main_page_ddlist_1, 11, 23);
    lv_obj_set_size(ui->main_page_ddlist_1, 103, 26);

    //Write style for main_page_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_ddlist_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_ddlist_1, &lv_font_OPPOSans_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_ddlist_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_ddlist_1, 181, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_main_page_ddlist_1_extra_list_selected_checked
    static lv_style_t style_main_page_ddlist_1_extra_list_selected_checked;
    ui_init_style(&style_main_page_ddlist_1_extra_list_selected_checked);

    lv_style_set_border_width(&style_main_page_ddlist_1_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_main_page_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_main_page_ddlist_1_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_main_page_ddlist_1_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_main_page_ddlist_1_extra_list_selected_checked, 4);
    lv_style_set_bg_opa(&style_main_page_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_main_page_ddlist_1_extra_list_selected_checked, lv_color_hex(0x676767));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_1), &style_main_page_ddlist_1_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_ddlist_1_extra_list_main_default
    static lv_style_t style_main_page_ddlist_1_extra_list_main_default;
    ui_init_style(&style_main_page_ddlist_1_extra_list_main_default);

    lv_style_set_max_height(&style_main_page_ddlist_1_extra_list_main_default, 90);
    lv_style_set_text_color(&style_main_page_ddlist_1_extra_list_main_default, lv_color_hex(0x252c33));
    lv_style_set_text_font(&style_main_page_ddlist_1_extra_list_main_default, &lv_font_OPPOSans_Medium_12);
    lv_style_set_text_opa(&style_main_page_ddlist_1_extra_list_main_default, 255);
    lv_style_set_border_width(&style_main_page_ddlist_1_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_main_page_ddlist_1_extra_list_main_default, 255);
    lv_style_set_border_color(&style_main_page_ddlist_1_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_main_page_ddlist_1_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_main_page_ddlist_1_extra_list_main_default, 5);
    lv_style_set_bg_opa(&style_main_page_ddlist_1_extra_list_main_default, 204);
    lv_style_set_bg_color(&style_main_page_ddlist_1_extra_list_main_default, lv_color_hex(0xdbdbdb));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_1_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_1), &style_main_page_ddlist_1_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_ddlist_1_extra_list_scrollbar_default
    static lv_style_t style_main_page_ddlist_1_extra_list_scrollbar_default;
    ui_init_style(&style_main_page_ddlist_1_extra_list_scrollbar_default);

    lv_style_set_radius(&style_main_page_ddlist_1_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_1_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_main_page_ddlist_1_extra_list_scrollbar_default, lv_color_hex(0x1d1d1d));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_1_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_1), &style_main_page_ddlist_1_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes main_page_next_page_button
    ui->main_page_next_page_button = lv_btn_create(ui->main_page_main_tileview_main);
    ui->main_page_next_page_button_label = lv_label_create(ui->main_page_next_page_button);
    lv_label_set_text(ui->main_page_next_page_button_label, " " LV_SYMBOL_DOWN " ");
    lv_label_set_long_mode(ui->main_page_next_page_button_label, LV_LABEL_LONG_CLIP);
    lv_obj_align(ui->main_page_next_page_button_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_next_page_button, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_next_page_button_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_next_page_button, 6, 288);
    lv_obj_set_size(ui->main_page_next_page_button, 129, 27);

    //Write style for main_page_next_page_button, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_next_page_button, 58, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_next_page_button, lv_color_hex(0x676868), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_next_page_button, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_next_page_button, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_next_page_button, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_next_page_button, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_next_page_button, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_next_page_button, &lv_font_FontAwesome5_28, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_next_page_button, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_next_page_button, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_project_name_en
    ui->main_page_project_name_en = lv_label_create(ui->main_page_main_tileview_main);
    lv_label_set_text(ui->main_page_project_name_en, "CONST CURRENT BOY");
    lv_label_set_long_mode(ui->main_page_project_name_en, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_project_name_en, 42, 26);
    lv_obj_set_size(ui->main_page_project_name_en, 88, 8);

    //Write style for main_page_project_name_en, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_project_name_en, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_project_name_en, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_project_name_en, lv_color_hex(0xe0e0e0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_project_name_en, &lv_font_OPPOSans_Medium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_project_name_en, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_project_name_en, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_project_name_en, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_project_name_en, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_project_name_en, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_project_name_en, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_project_name_en, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_project_name_en, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_project_name_en, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_project_name_en, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_project_name
    ui->main_page_project_name = lv_label_create(ui->main_page_main_tileview_main);
    lv_label_set_text(ui->main_page_project_name, "恒流小子 v1.0");
    lv_label_set_long_mode(ui->main_page_project_name, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_project_name, 41, 7);
    lv_obj_set_size(ui->main_page_project_name, 90, 14);

    //Write style for main_page_project_name, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_project_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_project_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_project_name, lv_color_hex(0xe0e0e0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_project_name, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_project_name, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_project_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_project_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_project_name, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_project_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_project_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_project_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_project_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_project_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_project_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_logo
    ui->main_page_logo = lv_img_create(ui->main_page_main_tileview_main);
    lv_obj_add_flag(ui->main_page_logo, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->main_page_logo, &_logo_alpha_40x40);
    lv_img_set_pivot(ui->main_page_logo, 50,50);
    lv_img_set_angle(ui->main_page_logo, 0);
    lv_obj_set_pos(ui->main_page_logo, 6, 2);
    lv_obj_set_size(ui->main_page_logo, 40, 40);

    //Write style for main_page_logo, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->main_page_logo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->main_page_logo, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_logo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->main_page_logo, true, LV_PART_MAIN|LV_STATE_DEFAULT);



    //Write codes main_page_setting_tileview
    ui->main_page_setting_tileview = lv_tileview_create(ui->main_page_main_tileview_setting);
    ui->main_page_setting_tileview_setting1 = lv_tileview_add_tile(ui->main_page_setting_tileview, 0, 0, LV_DIR_RIGHT);
    ui->main_page_setting_tileview_setting2 = lv_tileview_add_tile(ui->main_page_setting_tileview, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    ui->main_page_setting_tileview_setting3 = lv_tileview_add_tile(ui->main_page_setting_tileview, 2, 0, LV_DIR_LEFT);
    lv_obj_set_pos(ui->main_page_setting_tileview, 0, 0);
    lv_obj_set_size(ui->main_page_setting_tileview, 143, 320);
    lv_obj_set_scrollbar_mode(ui->main_page_setting_tileview, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_setting_tileview, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_setting_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_setting_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_setting_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_setting_tileview, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_setting_tileview, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_setting_tileview, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_setting_tileview, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_setting_tileview, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);



    //Write codes main_page_btn_8
    ui->main_page_btn_8 = lv_btn_create(ui->main_page_setting_tileview_setting1);
    ui->main_page_btn_8_label = lv_label_create(ui->main_page_btn_8);
    lv_label_set_text(ui->main_page_btn_8_label, "             更多           " LV_SYMBOL_RIGHT "");
    lv_label_set_long_mode(ui->main_page_btn_8_label, LV_LABEL_LONG_CLIP);
    lv_obj_align(ui->main_page_btn_8_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_8, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_8_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_8, 6, 288);
    lv_obj_set_size(ui->main_page_btn_8, 129, 27);

    //Write style for main_page_btn_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_8, 58, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_8, lv_color_hex(0x676868), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_8, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_8, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_8, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_8, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_cont_8
    ui->main_page_cont_8 = lv_obj_create(ui->main_page_setting_tileview_setting1);
    lv_obj_set_pos(ui->main_page_cont_8, 6, 39);
    lv_obj_set_size(ui->main_page_cont_8, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_cont_8, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_cont_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_cont_8, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_cont_8, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_cont_8, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_cont_8, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_cont_8, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_cont_8, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_cont_8, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_29
    ui->main_page_label_29 = lv_label_create(ui->main_page_cont_8);
    lv_label_set_text(ui->main_page_label_29, "工作限时");
    lv_label_set_long_mode(ui->main_page_label_29, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_29, 7, 3);
    lv_obj_set_size(ui->main_page_label_29, 111, 20);

    //Write style for main_page_label_29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_29, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_29, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_29, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_29, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spinbox_4
    ui->main_page_spinbox_4 = lv_spinbox_create(ui->main_page_cont_8);
    lv_obj_set_pos(ui->main_page_spinbox_4, 31, 24);
    lv_obj_set_width(ui->main_page_spinbox_4, 63);
    lv_obj_set_height(ui->main_page_spinbox_4, 25);
    lv_spinbox_set_digit_format(ui->main_page_spinbox_4, 4, 4);
    lv_spinbox_set_range(ui->main_page_spinbox_4, -9999, 9999);
    lv_coord_t main_page_spinbox_4_h = lv_obj_get_height(ui->main_page_spinbox_4);
    ui->main_page_spinbox_4_btn_plus = lv_btn_create(ui->main_page_cont_8);
    lv_obj_set_size(ui->main_page_spinbox_4_btn_plus, main_page_spinbox_4_h, main_page_spinbox_4_h);
    lv_obj_align_to(ui->main_page_spinbox_4_btn_plus, ui->main_page_spinbox_4, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_4_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_4_btn_plus, lv_main_page_spinbox_4_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->main_page_spinbox_4_btn_minus = lv_btn_create(ui->main_page_cont_8);
    lv_obj_set_size(ui->main_page_spinbox_4_btn_minus, main_page_spinbox_4_h, main_page_spinbox_4_h);
    lv_obj_align_to(ui->main_page_spinbox_4_btn_minus, ui->main_page_spinbox_4, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_4_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_4_btn_minus, lv_main_page_spinbox_4_decrement_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->main_page_spinbox_4, 31, 24);

    //Write style for main_page_spinbox_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_spinbox_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_spinbox_4, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_spinbox_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_spinbox_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_spinbox_4, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_spinbox_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_4, &lv_font_IntelOneMono_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_spinbox_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_spinbox_4, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_spinbox_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_spinbox_4, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_spinbox_4, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_4, &lv_font_IntelOneMono_Medium_11, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_4, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_4, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_4, lv_color_hex(0x929292), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_4, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spinbox_4_extra_btns_main_default
    static lv_style_t style_main_page_spinbox_4_extra_btns_main_default;
    ui_init_style(&style_main_page_spinbox_4_extra_btns_main_default);

    lv_style_set_text_color(&style_main_page_spinbox_4_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_4_extra_btns_main_default, &lv_font_montserratMedium_9);
    lv_style_set_text_opa(&style_main_page_spinbox_4_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_4_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_4_extra_btns_main_default, lv_color_hex(0x66686a));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_4_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_4_extra_btns_main_default, 0);
    lv_style_set_radius(&style_main_page_spinbox_4_extra_btns_main_default, 9);
    lv_style_set_shadow_width(&style_main_page_spinbox_4_extra_btns_main_default, 0);
    lv_obj_add_style(ui->main_page_spinbox_4_btn_plus, &style_main_page_spinbox_4_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->main_page_spinbox_4_btn_minus, &style_main_page_spinbox_4_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_PRESSED for &style_main_page_spinbox_4_extra_btns_main_pressed
    static lv_style_t style_main_page_spinbox_4_extra_btns_main_pressed;
    ui_init_style(&style_main_page_spinbox_4_extra_btns_main_pressed);

    lv_style_set_text_color(&style_main_page_spinbox_4_extra_btns_main_pressed, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_4_extra_btns_main_pressed, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_main_page_spinbox_4_extra_btns_main_pressed, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_4_extra_btns_main_pressed, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_4_extra_btns_main_pressed, lv_color_hex(0x696b6e));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_4_extra_btns_main_pressed, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_4_extra_btns_main_pressed, 0);
    lv_style_set_radius(&style_main_page_spinbox_4_extra_btns_main_pressed, 5);
    lv_style_set_shadow_width(&style_main_page_spinbox_4_extra_btns_main_pressed, 0);
    lv_obj_add_style(ui->main_page_spinbox_4_btn_plus, &style_main_page_spinbox_4_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_style(ui->main_page_spinbox_4_btn_minus, &style_main_page_spinbox_4_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes main_page_cont_7
    ui->main_page_cont_7 = lv_obj_create(ui->main_page_setting_tileview_setting1);
    lv_obj_set_pos(ui->main_page_cont_7, 6, 101);
    lv_obj_set_size(ui->main_page_cont_7, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_cont_7, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_cont_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_cont_7, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_cont_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_cont_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_cont_7, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_cont_7, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_cont_7, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_cont_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_cont_7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_cont_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_cont_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_cont_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_cont_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_cont_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_28
    ui->main_page_label_28 = lv_label_create(ui->main_page_cont_7);
    lv_label_set_text(ui->main_page_label_28, "过压策略");
    lv_label_set_long_mode(ui->main_page_label_28, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_28, 7, 4);
    lv_obj_set_size(ui->main_page_label_28, 111, 20);

    //Write style for main_page_label_28, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_28, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_28, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_28, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_28, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_ddlist_5
    ui->main_page_ddlist_5 = lv_dropdown_create(ui->main_page_cont_7);
    lv_dropdown_set_options(ui->main_page_ddlist_5, "关断\n限制功率");
    lv_obj_set_pos(ui->main_page_ddlist_5, 11, 23);
    lv_obj_set_size(ui->main_page_ddlist_5, 103, 26);

    //Write style for main_page_ddlist_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_ddlist_5, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_ddlist_5, &lv_font_OPPOSans_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_ddlist_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_ddlist_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_ddlist_5, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_ddlist_5, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_ddlist_5, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_ddlist_5, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_ddlist_5, 181, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_ddlist_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_ddlist_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_ddlist_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_main_page_ddlist_5_extra_list_selected_checked
    static lv_style_t style_main_page_ddlist_5_extra_list_selected_checked;
    ui_init_style(&style_main_page_ddlist_5_extra_list_selected_checked);

    lv_style_set_border_width(&style_main_page_ddlist_5_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_main_page_ddlist_5_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_main_page_ddlist_5_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_main_page_ddlist_5_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_main_page_ddlist_5_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_5_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_main_page_ddlist_5_extra_list_selected_checked, lv_color_hex(0x676767));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_5_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_5), &style_main_page_ddlist_5_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_ddlist_5_extra_list_main_default
    static lv_style_t style_main_page_ddlist_5_extra_list_main_default;
    ui_init_style(&style_main_page_ddlist_5_extra_list_main_default);

    lv_style_set_max_height(&style_main_page_ddlist_5_extra_list_main_default, 90);
    lv_style_set_text_color(&style_main_page_ddlist_5_extra_list_main_default, lv_color_hex(0x252c33));
    lv_style_set_text_font(&style_main_page_ddlist_5_extra_list_main_default, &lv_font_OPPOSans_Medium_12);
    lv_style_set_text_opa(&style_main_page_ddlist_5_extra_list_main_default, 255);
    lv_style_set_border_width(&style_main_page_ddlist_5_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_main_page_ddlist_5_extra_list_main_default, 255);
    lv_style_set_border_color(&style_main_page_ddlist_5_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_main_page_ddlist_5_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_main_page_ddlist_5_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_5_extra_list_main_default, 204);
    lv_style_set_bg_color(&style_main_page_ddlist_5_extra_list_main_default, lv_color_hex(0xdbdbdb));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_5_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_5), &style_main_page_ddlist_5_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_ddlist_5_extra_list_scrollbar_default
    static lv_style_t style_main_page_ddlist_5_extra_list_scrollbar_default;
    ui_init_style(&style_main_page_ddlist_5_extra_list_scrollbar_default);

    lv_style_set_radius(&style_main_page_ddlist_5_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_5_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_main_page_ddlist_5_extra_list_scrollbar_default, lv_color_hex(0x1d1d1d));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_5_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_5), &style_main_page_ddlist_5_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes main_page_cont_6
    ui->main_page_cont_6 = lv_obj_create(ui->main_page_setting_tileview_setting1);
    lv_obj_set_pos(ui->main_page_cont_6, 6, 163);
    lv_obj_set_size(ui->main_page_cont_6, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_cont_6, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_cont_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_cont_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_cont_6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_cont_6, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_cont_6, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_cont_6, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_cont_6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_cont_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_26
    ui->main_page_label_26 = lv_label_create(ui->main_page_cont_6);
    lv_label_set_text(ui->main_page_label_26, "声音");
    lv_label_set_long_mode(ui->main_page_label_26, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_26, 7, 4);
    lv_obj_set_size(ui->main_page_label_26, 111, 20);

    //Write style for main_page_label_26, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_26, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_26, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_26, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_26, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_sw_1
    ui->main_page_sw_1 = lv_switch_create(ui->main_page_cont_6);
    lv_obj_set_pos(ui->main_page_sw_1, 42, 27);
    lv_obj_set_size(ui->main_page_sw_1, 40, 20);

    //Write style for main_page_sw_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_sw_1, 64, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_sw_1, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_sw_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_sw_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_sw_1, Part: LV_PART_MAIN, State: LV_STATE_DISABLED.
    lv_obj_set_style_bg_opa(ui->main_page_sw_1, 33, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(ui->main_page_sw_1, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_bg_grad_dir(ui->main_page_sw_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_border_width(ui->main_page_sw_1, 0, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_radius(ui->main_page_sw_1, 10, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_shadow_width(ui->main_page_sw_1, 0, LV_PART_MAIN|LV_STATE_DISABLED);

    //Write style for main_page_sw_1, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->main_page_sw_1, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->main_page_sw_1, lv_color_hex(0xd6d6d6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->main_page_sw_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->main_page_sw_1, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for main_page_sw_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_sw_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_sw_1, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_sw_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_sw_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_sw_1, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes main_page_cont_5
    ui->main_page_cont_5 = lv_obj_create(ui->main_page_setting_tileview_setting1);
    lv_obj_set_pos(ui->main_page_cont_5, 6, 225);
    lv_obj_set_size(ui->main_page_cont_5, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_cont_5, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_cont_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_cont_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_cont_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_cont_5, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_cont_5, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_cont_5, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_cont_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_cont_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_25
    ui->main_page_label_25 = lv_label_create(ui->main_page_cont_5);
    lv_label_set_text(ui->main_page_label_25, "采样时间(us)");
    lv_label_set_long_mode(ui->main_page_label_25, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_25, 7, 3);
    lv_obj_set_size(ui->main_page_label_25, 111, 20);

    //Write style for main_page_label_25, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_25, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_25, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_25, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_25, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_ddlist_2
    ui->main_page_ddlist_2 = lv_dropdown_create(ui->main_page_cont_5);
    lv_dropdown_set_options(ui->main_page_ddlist_2, "140\n8300\n？");
    lv_obj_set_pos(ui->main_page_ddlist_2, 11, 23);
    lv_obj_set_size(ui->main_page_ddlist_2, 103, 26);

    //Write style for main_page_ddlist_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_ddlist_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_ddlist_2, &lv_font_OPPOSans_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_ddlist_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_ddlist_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_ddlist_2, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_ddlist_2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_ddlist_2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_ddlist_2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_ddlist_2, 181, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_ddlist_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_ddlist_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_ddlist_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_main_page_ddlist_2_extra_list_selected_checked
    static lv_style_t style_main_page_ddlist_2_extra_list_selected_checked;
    ui_init_style(&style_main_page_ddlist_2_extra_list_selected_checked);

    lv_style_set_border_width(&style_main_page_ddlist_2_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_main_page_ddlist_2_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_main_page_ddlist_2_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_main_page_ddlist_2_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_main_page_ddlist_2_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_2_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_main_page_ddlist_2_extra_list_selected_checked, lv_color_hex(0x676767));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_2_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_2), &style_main_page_ddlist_2_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_ddlist_2_extra_list_main_default
    static lv_style_t style_main_page_ddlist_2_extra_list_main_default;
    ui_init_style(&style_main_page_ddlist_2_extra_list_main_default);

    lv_style_set_max_height(&style_main_page_ddlist_2_extra_list_main_default, 90);
    lv_style_set_text_color(&style_main_page_ddlist_2_extra_list_main_default, lv_color_hex(0x252c33));
    lv_style_set_text_font(&style_main_page_ddlist_2_extra_list_main_default, &lv_font_OPPOSans_Medium_12);
    lv_style_set_text_opa(&style_main_page_ddlist_2_extra_list_main_default, 255);
    lv_style_set_border_width(&style_main_page_ddlist_2_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_main_page_ddlist_2_extra_list_main_default, 255);
    lv_style_set_border_color(&style_main_page_ddlist_2_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_main_page_ddlist_2_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_main_page_ddlist_2_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_2_extra_list_main_default, 204);
    lv_style_set_bg_color(&style_main_page_ddlist_2_extra_list_main_default, lv_color_hex(0xdbdbdb));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_2_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_2), &style_main_page_ddlist_2_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_ddlist_2_extra_list_scrollbar_default
    static lv_style_t style_main_page_ddlist_2_extra_list_scrollbar_default;
    ui_init_style(&style_main_page_ddlist_2_extra_list_scrollbar_default);

    lv_style_set_radius(&style_main_page_ddlist_2_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_2_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_main_page_ddlist_2_extra_list_scrollbar_default, lv_color_hex(0x1d1d1d));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_2_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_2), &style_main_page_ddlist_2_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes main_page_label_24
    ui->main_page_label_24 = lv_label_create(ui->main_page_setting_tileview_setting1);
    lv_label_set_text(ui->main_page_label_24, "" LV_SYMBOL_SETTINGS " 设置");
    lv_label_set_long_mode(ui->main_page_label_24, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_24, 41, 10);
    lv_obj_set_size(ui->main_page_label_24, 59, 17);

    //Write style for main_page_label_24, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_24, lv_color_hex(0xd8d8d8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_24, &lv_font_OPPOSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_24, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_24, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);



    //Write codes main_page_label_35
    ui->main_page_label_35 = lv_label_create(ui->main_page_setting_tileview_setting2);
    lv_label_set_text(ui->main_page_label_35, " " LV_SYMBOL_PLUS " 安全");
    lv_label_set_long_mode(ui->main_page_label_35, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_35, 41, 10);
    lv_obj_set_size(ui->main_page_label_35, 59, 17);

    //Write style for main_page_label_35, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_35, lv_color_hex(0xd8d8d8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_35, &lv_font_OPPOSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_35, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_35, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_cont_12
    ui->main_page_cont_12 = lv_obj_create(ui->main_page_setting_tileview_setting2);
    lv_obj_set_pos(ui->main_page_cont_12, 6, 39);
    lv_obj_set_size(ui->main_page_cont_12, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_cont_12, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_cont_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_cont_12, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_cont_12, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_cont_12, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_cont_12, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_cont_12, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_cont_12, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_cont_12, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_34
    ui->main_page_label_34 = lv_label_create(ui->main_page_cont_12);
    lv_label_set_text(ui->main_page_label_34, "温控策略");
    lv_label_set_long_mode(ui->main_page_label_34, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_34, 7, 3);
    lv_obj_set_size(ui->main_page_label_34, 111, 20);

    //Write style for main_page_label_34, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_34, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_34, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_34, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_34, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_ddlist_3
    ui->main_page_ddlist_3 = lv_dropdown_create(ui->main_page_cont_12);
    lv_dropdown_set_options(ui->main_page_ddlist_3, "严格\n静音\n始终开启");
    lv_obj_set_pos(ui->main_page_ddlist_3, 10, 22);
    lv_obj_set_size(ui->main_page_ddlist_3, 103, 26);

    //Write style for main_page_ddlist_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_ddlist_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_ddlist_3, &lv_font_OPPOSans_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_ddlist_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_ddlist_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_ddlist_3, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_ddlist_3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_ddlist_3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_ddlist_3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_ddlist_3, 181, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_ddlist_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_ddlist_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_ddlist_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_main_page_ddlist_3_extra_list_selected_checked
    static lv_style_t style_main_page_ddlist_3_extra_list_selected_checked;
    ui_init_style(&style_main_page_ddlist_3_extra_list_selected_checked);

    lv_style_set_border_width(&style_main_page_ddlist_3_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_main_page_ddlist_3_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_main_page_ddlist_3_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_main_page_ddlist_3_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_main_page_ddlist_3_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_3_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_main_page_ddlist_3_extra_list_selected_checked, lv_color_hex(0x676767));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_3_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_3), &style_main_page_ddlist_3_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_ddlist_3_extra_list_main_default
    static lv_style_t style_main_page_ddlist_3_extra_list_main_default;
    ui_init_style(&style_main_page_ddlist_3_extra_list_main_default);

    lv_style_set_max_height(&style_main_page_ddlist_3_extra_list_main_default, 90);
    lv_style_set_text_color(&style_main_page_ddlist_3_extra_list_main_default, lv_color_hex(0x252c33));
    lv_style_set_text_font(&style_main_page_ddlist_3_extra_list_main_default, &lv_font_OPPOSans_Medium_12);
    lv_style_set_text_opa(&style_main_page_ddlist_3_extra_list_main_default, 255);
    lv_style_set_border_width(&style_main_page_ddlist_3_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_main_page_ddlist_3_extra_list_main_default, 255);
    lv_style_set_border_color(&style_main_page_ddlist_3_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_main_page_ddlist_3_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_main_page_ddlist_3_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_3_extra_list_main_default, 204);
    lv_style_set_bg_color(&style_main_page_ddlist_3_extra_list_main_default, lv_color_hex(0xdbdbdb));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_3_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_3), &style_main_page_ddlist_3_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_ddlist_3_extra_list_scrollbar_default
    static lv_style_t style_main_page_ddlist_3_extra_list_scrollbar_default;
    ui_init_style(&style_main_page_ddlist_3_extra_list_scrollbar_default);

    lv_style_set_radius(&style_main_page_ddlist_3_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_3_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_main_page_ddlist_3_extra_list_scrollbar_default, lv_color_hex(0x1d1d1d));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_3_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_3), &style_main_page_ddlist_3_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes main_page_cont_11
    ui->main_page_cont_11 = lv_obj_create(ui->main_page_setting_tileview_setting2);
    lv_obj_set_pos(ui->main_page_cont_11, 6, 101);
    lv_obj_set_size(ui->main_page_cont_11, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_cont_11, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_cont_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_cont_11, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_cont_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_cont_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_cont_11, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_cont_11, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_cont_11, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_cont_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_cont_11, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_cont_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_cont_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_cont_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_cont_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_cont_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_33
    ui->main_page_label_33 = lv_label_create(ui->main_page_cont_11);
    lv_label_set_text(ui->main_page_label_33, "功率限制");
    lv_label_set_long_mode(ui->main_page_label_33, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_33, 7, 4);
    lv_obj_set_size(ui->main_page_label_33, 111, 20);

    //Write style for main_page_label_33, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_33, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_33, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_33, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_33, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spinbox_6
    ui->main_page_spinbox_6 = lv_spinbox_create(ui->main_page_cont_11);
    lv_obj_set_pos(ui->main_page_spinbox_6, 31, 23);
    lv_obj_set_width(ui->main_page_spinbox_6, 63);
    lv_obj_set_height(ui->main_page_spinbox_6, 25);
    lv_spinbox_set_digit_format(ui->main_page_spinbox_6, 2, 2);
    lv_spinbox_set_range(ui->main_page_spinbox_6, -99, 99);
    lv_coord_t main_page_spinbox_6_h = lv_obj_get_height(ui->main_page_spinbox_6);
    ui->main_page_spinbox_6_btn_plus = lv_btn_create(ui->main_page_cont_11);
    lv_obj_set_size(ui->main_page_spinbox_6_btn_plus, main_page_spinbox_6_h, main_page_spinbox_6_h);
    lv_obj_align_to(ui->main_page_spinbox_6_btn_plus, ui->main_page_spinbox_6, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_6_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_6_btn_plus, lv_main_page_spinbox_6_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->main_page_spinbox_6_btn_minus = lv_btn_create(ui->main_page_cont_11);
    lv_obj_set_size(ui->main_page_spinbox_6_btn_minus, main_page_spinbox_6_h, main_page_spinbox_6_h);
    lv_obj_align_to(ui->main_page_spinbox_6_btn_minus, ui->main_page_spinbox_6, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_6_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_6_btn_minus, lv_main_page_spinbox_6_decrement_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->main_page_spinbox_6, 31, 23);

    //Write style for main_page_spinbox_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_spinbox_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_spinbox_6, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_spinbox_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_spinbox_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_spinbox_6, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_spinbox_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_6, &lv_font_IntelOneMono_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_spinbox_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_spinbox_6, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_spinbox_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_spinbox_6, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_spinbox_6, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_6, &lv_font_IntelOneMono_Medium_11, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_6, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_6, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_6, lv_color_hex(0x929292), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_6, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spinbox_6_extra_btns_main_default
    static lv_style_t style_main_page_spinbox_6_extra_btns_main_default;
    ui_init_style(&style_main_page_spinbox_6_extra_btns_main_default);

    lv_style_set_text_color(&style_main_page_spinbox_6_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_6_extra_btns_main_default, &lv_font_montserratMedium_9);
    lv_style_set_text_opa(&style_main_page_spinbox_6_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_6_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_6_extra_btns_main_default, lv_color_hex(0x66686a));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_6_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_6_extra_btns_main_default, 0);
    lv_style_set_radius(&style_main_page_spinbox_6_extra_btns_main_default, 9);
    lv_style_set_shadow_width(&style_main_page_spinbox_6_extra_btns_main_default, 0);
    lv_obj_add_style(ui->main_page_spinbox_6_btn_plus, &style_main_page_spinbox_6_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->main_page_spinbox_6_btn_minus, &style_main_page_spinbox_6_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_PRESSED for &style_main_page_spinbox_6_extra_btns_main_pressed
    static lv_style_t style_main_page_spinbox_6_extra_btns_main_pressed;
    ui_init_style(&style_main_page_spinbox_6_extra_btns_main_pressed);

    lv_style_set_text_color(&style_main_page_spinbox_6_extra_btns_main_pressed, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_6_extra_btns_main_pressed, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_main_page_spinbox_6_extra_btns_main_pressed, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_6_extra_btns_main_pressed, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_6_extra_btns_main_pressed, lv_color_hex(0x696b6e));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_6_extra_btns_main_pressed, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_6_extra_btns_main_pressed, 0);
    lv_style_set_radius(&style_main_page_spinbox_6_extra_btns_main_pressed, 5);
    lv_style_set_shadow_width(&style_main_page_spinbox_6_extra_btns_main_pressed, 0);
    lv_obj_add_style(ui->main_page_spinbox_6_btn_plus, &style_main_page_spinbox_6_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_style(ui->main_page_spinbox_6_btn_minus, &style_main_page_spinbox_6_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes main_page_cont_10
    ui->main_page_cont_10 = lv_obj_create(ui->main_page_setting_tileview_setting2);
    lv_obj_set_pos(ui->main_page_cont_10, 6, 163);
    lv_obj_set_size(ui->main_page_cont_10, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_cont_10, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_cont_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_cont_10, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_cont_10, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_cont_10, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_cont_10, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_cont_10, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_cont_10, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_cont_10, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_32
    ui->main_page_label_32 = lv_label_create(ui->main_page_cont_10);
    lv_label_set_text(ui->main_page_label_32, "过压限制");
    lv_label_set_long_mode(ui->main_page_label_32, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_32, 7, 4);
    lv_obj_set_size(ui->main_page_label_32, 111, 20);

    //Write style for main_page_label_32, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_32, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_32, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_32, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_32, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spinbox_7
    ui->main_page_spinbox_7 = lv_spinbox_create(ui->main_page_cont_10);
    lv_obj_set_pos(ui->main_page_spinbox_7, 31, 23);
    lv_obj_set_width(ui->main_page_spinbox_7, 63);
    lv_obj_set_height(ui->main_page_spinbox_7, 25);
    lv_spinbox_set_digit_format(ui->main_page_spinbox_7, 2, 2);
    lv_spinbox_set_range(ui->main_page_spinbox_7, -99, 99);
    lv_coord_t main_page_spinbox_7_h = lv_obj_get_height(ui->main_page_spinbox_7);
    ui->main_page_spinbox_7_btn_plus = lv_btn_create(ui->main_page_cont_10);
    lv_obj_set_size(ui->main_page_spinbox_7_btn_plus, main_page_spinbox_7_h, main_page_spinbox_7_h);
    lv_obj_align_to(ui->main_page_spinbox_7_btn_plus, ui->main_page_spinbox_7, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_7_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_7_btn_plus, lv_main_page_spinbox_7_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->main_page_spinbox_7_btn_minus = lv_btn_create(ui->main_page_cont_10);
    lv_obj_set_size(ui->main_page_spinbox_7_btn_minus, main_page_spinbox_7_h, main_page_spinbox_7_h);
    lv_obj_align_to(ui->main_page_spinbox_7_btn_minus, ui->main_page_spinbox_7, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_7_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_7_btn_minus, lv_main_page_spinbox_7_decrement_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->main_page_spinbox_7, 31, 23);

    //Write style for main_page_spinbox_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_spinbox_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_spinbox_7, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_spinbox_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_spinbox_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_spinbox_7, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_spinbox_7, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_7, &lv_font_IntelOneMono_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_spinbox_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_spinbox_7, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_spinbox_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_spinbox_7, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_spinbox_7, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_7, &lv_font_IntelOneMono_Medium_11, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_7, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_7, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_7, lv_color_hex(0x929292), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_7, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spinbox_7_extra_btns_main_default
    static lv_style_t style_main_page_spinbox_7_extra_btns_main_default;
    ui_init_style(&style_main_page_spinbox_7_extra_btns_main_default);

    lv_style_set_text_color(&style_main_page_spinbox_7_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_7_extra_btns_main_default, &lv_font_montserratMedium_9);
    lv_style_set_text_opa(&style_main_page_spinbox_7_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_7_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_7_extra_btns_main_default, lv_color_hex(0x66686a));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_7_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_7_extra_btns_main_default, 0);
    lv_style_set_radius(&style_main_page_spinbox_7_extra_btns_main_default, 9);
    lv_style_set_shadow_width(&style_main_page_spinbox_7_extra_btns_main_default, 0);
    lv_obj_add_style(ui->main_page_spinbox_7_btn_plus, &style_main_page_spinbox_7_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->main_page_spinbox_7_btn_minus, &style_main_page_spinbox_7_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_PRESSED for &style_main_page_spinbox_7_extra_btns_main_pressed
    static lv_style_t style_main_page_spinbox_7_extra_btns_main_pressed;
    ui_init_style(&style_main_page_spinbox_7_extra_btns_main_pressed);

    lv_style_set_text_color(&style_main_page_spinbox_7_extra_btns_main_pressed, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_7_extra_btns_main_pressed, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_main_page_spinbox_7_extra_btns_main_pressed, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_7_extra_btns_main_pressed, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_7_extra_btns_main_pressed, lv_color_hex(0x696b6e));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_7_extra_btns_main_pressed, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_7_extra_btns_main_pressed, 0);
    lv_style_set_radius(&style_main_page_spinbox_7_extra_btns_main_pressed, 5);
    lv_style_set_shadow_width(&style_main_page_spinbox_7_extra_btns_main_pressed, 0);
    lv_obj_add_style(ui->main_page_spinbox_7_btn_plus, &style_main_page_spinbox_7_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_style(ui->main_page_spinbox_7_btn_minus, &style_main_page_spinbox_7_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes main_page_cont_9
    ui->main_page_cont_9 = lv_obj_create(ui->main_page_setting_tileview_setting2);
    lv_obj_set_pos(ui->main_page_cont_9, 6, 225);
    lv_obj_set_size(ui->main_page_cont_9, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_cont_9, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_cont_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_cont_9, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_cont_9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_cont_9, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_cont_9, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_cont_9, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_cont_9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_cont_9, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_30
    ui->main_page_label_30 = lv_label_create(ui->main_page_cont_9);
    lv_label_set_text(ui->main_page_label_30, "工作限时");
    lv_label_set_long_mode(ui->main_page_label_30, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_30, 7, 3);
    lv_obj_set_size(ui->main_page_label_30, 111, 20);

    //Write style for main_page_label_30, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_30, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_30, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_30, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_30, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spinbox_5
    ui->main_page_spinbox_5 = lv_spinbox_create(ui->main_page_cont_9);
    lv_obj_set_pos(ui->main_page_spinbox_5, 31, 24);
    lv_obj_set_width(ui->main_page_spinbox_5, 63);
    lv_obj_set_height(ui->main_page_spinbox_5, 25);
    lv_spinbox_set_digit_format(ui->main_page_spinbox_5, 4, 4);
    lv_spinbox_set_range(ui->main_page_spinbox_5, -9999, 9999);
    lv_coord_t main_page_spinbox_5_h = lv_obj_get_height(ui->main_page_spinbox_5);
    ui->main_page_spinbox_5_btn_plus = lv_btn_create(ui->main_page_cont_9);
    lv_obj_set_size(ui->main_page_spinbox_5_btn_plus, main_page_spinbox_5_h, main_page_spinbox_5_h);
    lv_obj_align_to(ui->main_page_spinbox_5_btn_plus, ui->main_page_spinbox_5, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_5_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_5_btn_plus, lv_main_page_spinbox_5_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->main_page_spinbox_5_btn_minus = lv_btn_create(ui->main_page_cont_9);
    lv_obj_set_size(ui->main_page_spinbox_5_btn_minus, main_page_spinbox_5_h, main_page_spinbox_5_h);
    lv_obj_align_to(ui->main_page_spinbox_5_btn_minus, ui->main_page_spinbox_5, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_5_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_5_btn_minus, lv_main_page_spinbox_5_decrement_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->main_page_spinbox_5, 31, 24);

    //Write style for main_page_spinbox_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_spinbox_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_spinbox_5, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_spinbox_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_spinbox_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_spinbox_5, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_spinbox_5, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_5, &lv_font_IntelOneMono_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_spinbox_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_spinbox_5, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_spinbox_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_spinbox_5, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_spinbox_5, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_5, &lv_font_IntelOneMono_Medium_11, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_5, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_5, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_5, lv_color_hex(0x929292), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_5, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spinbox_5_extra_btns_main_default
    static lv_style_t style_main_page_spinbox_5_extra_btns_main_default;
    ui_init_style(&style_main_page_spinbox_5_extra_btns_main_default);

    lv_style_set_text_color(&style_main_page_spinbox_5_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_5_extra_btns_main_default, &lv_font_montserratMedium_9);
    lv_style_set_text_opa(&style_main_page_spinbox_5_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_5_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_5_extra_btns_main_default, lv_color_hex(0x66686a));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_5_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_5_extra_btns_main_default, 0);
    lv_style_set_radius(&style_main_page_spinbox_5_extra_btns_main_default, 9);
    lv_style_set_shadow_width(&style_main_page_spinbox_5_extra_btns_main_default, 0);
    lv_obj_add_style(ui->main_page_spinbox_5_btn_plus, &style_main_page_spinbox_5_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->main_page_spinbox_5_btn_minus, &style_main_page_spinbox_5_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_PRESSED for &style_main_page_spinbox_5_extra_btns_main_pressed
    static lv_style_t style_main_page_spinbox_5_extra_btns_main_pressed;
    ui_init_style(&style_main_page_spinbox_5_extra_btns_main_pressed);

    lv_style_set_text_color(&style_main_page_spinbox_5_extra_btns_main_pressed, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_5_extra_btns_main_pressed, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_main_page_spinbox_5_extra_btns_main_pressed, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_5_extra_btns_main_pressed, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_5_extra_btns_main_pressed, lv_color_hex(0x696b6e));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_5_extra_btns_main_pressed, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_5_extra_btns_main_pressed, 0);
    lv_style_set_radius(&style_main_page_spinbox_5_extra_btns_main_pressed, 5);
    lv_style_set_shadow_width(&style_main_page_spinbox_5_extra_btns_main_pressed, 0);
    lv_obj_add_style(ui->main_page_spinbox_5_btn_plus, &style_main_page_spinbox_5_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_style(ui->main_page_spinbox_5_btn_minus, &style_main_page_spinbox_5_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes main_page_btn_10
    ui->main_page_btn_10 = lv_btn_create(ui->main_page_setting_tileview_setting2);
    ui->main_page_btn_10_label = lv_label_create(ui->main_page_btn_10);
    lv_label_set_text(ui->main_page_btn_10_label, "             性能           " LV_SYMBOL_RIGHT "");
    lv_label_set_long_mode(ui->main_page_btn_10_label, LV_LABEL_LONG_CLIP);
    lv_obj_align(ui->main_page_btn_10_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_10, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_10_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_10, 8, 288);
    lv_obj_set_size(ui->main_page_btn_10, 129, 27);

    //Write style for main_page_btn_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_10, 58, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_10, lv_color_hex(0x676868), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_10, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_10, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_10, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_10, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);



    //Write codes main_page_cont_16
    ui->main_page_cont_16 = lv_obj_create(ui->main_page_setting_tileview_setting3);
    lv_obj_set_pos(ui->main_page_cont_16, 6, 39);
    lv_obj_set_size(ui->main_page_cont_16, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_cont_16, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_cont_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_cont_16, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_cont_16, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_cont_16, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_cont_16, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_cont_16, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_cont_16, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_cont_16, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_41
    ui->main_page_label_41 = lv_label_create(ui->main_page_cont_16);
    lv_label_set_text(ui->main_page_label_41, "采样精度");
    lv_label_set_long_mode(ui->main_page_label_41, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_41, 6, 2);
    lv_obj_set_size(ui->main_page_label_41, 111, 20);

    //Write style for main_page_label_41, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_41, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_41, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_41, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_41, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_ddlist_4
    ui->main_page_ddlist_4 = lv_dropdown_create(ui->main_page_cont_16);
    lv_dropdown_set_options(ui->main_page_ddlist_4, "1\n2\n3");
    lv_obj_set_pos(ui->main_page_ddlist_4, 10, 22);
    lv_obj_set_size(ui->main_page_ddlist_4, 103, 26);

    //Write style for main_page_ddlist_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_ddlist_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_ddlist_4, &lv_font_OPPOSans_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_ddlist_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_ddlist_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_ddlist_4, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_ddlist_4, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_ddlist_4, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_ddlist_4, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_ddlist_4, 181, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_ddlist_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_ddlist_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_ddlist_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_main_page_ddlist_4_extra_list_selected_checked
    static lv_style_t style_main_page_ddlist_4_extra_list_selected_checked;
    ui_init_style(&style_main_page_ddlist_4_extra_list_selected_checked);

    lv_style_set_border_width(&style_main_page_ddlist_4_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_main_page_ddlist_4_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_main_page_ddlist_4_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_main_page_ddlist_4_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_main_page_ddlist_4_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_4_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_main_page_ddlist_4_extra_list_selected_checked, lv_color_hex(0x676767));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_4_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_4), &style_main_page_ddlist_4_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_ddlist_4_extra_list_main_default
    static lv_style_t style_main_page_ddlist_4_extra_list_main_default;
    ui_init_style(&style_main_page_ddlist_4_extra_list_main_default);

    lv_style_set_max_height(&style_main_page_ddlist_4_extra_list_main_default, 90);
    lv_style_set_text_color(&style_main_page_ddlist_4_extra_list_main_default, lv_color_hex(0x252c33));
    lv_style_set_text_font(&style_main_page_ddlist_4_extra_list_main_default, &lv_font_OPPOSans_Medium_12);
    lv_style_set_text_opa(&style_main_page_ddlist_4_extra_list_main_default, 255);
    lv_style_set_border_width(&style_main_page_ddlist_4_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_main_page_ddlist_4_extra_list_main_default, 255);
    lv_style_set_border_color(&style_main_page_ddlist_4_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_main_page_ddlist_4_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_main_page_ddlist_4_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_4_extra_list_main_default, 204);
    lv_style_set_bg_color(&style_main_page_ddlist_4_extra_list_main_default, lv_color_hex(0xdbdbdb));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_4_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_4), &style_main_page_ddlist_4_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_ddlist_4_extra_list_scrollbar_default
    static lv_style_t style_main_page_ddlist_4_extra_list_scrollbar_default;
    ui_init_style(&style_main_page_ddlist_4_extra_list_scrollbar_default);

    lv_style_set_radius(&style_main_page_ddlist_4_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_main_page_ddlist_4_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_main_page_ddlist_4_extra_list_scrollbar_default, lv_color_hex(0x1d1d1d));
    lv_style_set_bg_grad_dir(&style_main_page_ddlist_4_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->main_page_ddlist_4), &style_main_page_ddlist_4_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes main_page_cont_15
    ui->main_page_cont_15 = lv_obj_create(ui->main_page_setting_tileview_setting3);
    lv_obj_set_pos(ui->main_page_cont_15, 6, 101);
    lv_obj_set_size(ui->main_page_cont_15, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_cont_15, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_cont_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_cont_15, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_cont_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_cont_15, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_cont_15, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_cont_15, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_cont_15, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_cont_15, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_cont_15, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_cont_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_cont_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_cont_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_cont_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_cont_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_40
    ui->main_page_label_40 = lv_label_create(ui->main_page_cont_15);
    lv_label_set_text(ui->main_page_label_40, "2");
    lv_label_set_long_mode(ui->main_page_label_40, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_40, 7, 4);
    lv_obj_set_size(ui->main_page_label_40, 111, 20);

    //Write style for main_page_label_40, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_40, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_40, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_40, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_40, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spinbox_10
    ui->main_page_spinbox_10 = lv_spinbox_create(ui->main_page_cont_15);
    lv_obj_set_pos(ui->main_page_spinbox_10, 31, 23);
    lv_obj_set_width(ui->main_page_spinbox_10, 63);
    lv_obj_set_height(ui->main_page_spinbox_10, 25);
    lv_spinbox_set_digit_format(ui->main_page_spinbox_10, 2, 2);
    lv_spinbox_set_range(ui->main_page_spinbox_10, -99, 99);
    lv_coord_t main_page_spinbox_10_h = lv_obj_get_height(ui->main_page_spinbox_10);
    ui->main_page_spinbox_10_btn_plus = lv_btn_create(ui->main_page_cont_15);
    lv_obj_set_size(ui->main_page_spinbox_10_btn_plus, main_page_spinbox_10_h, main_page_spinbox_10_h);
    lv_obj_align_to(ui->main_page_spinbox_10_btn_plus, ui->main_page_spinbox_10, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_10_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_10_btn_plus, lv_main_page_spinbox_10_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->main_page_spinbox_10_btn_minus = lv_btn_create(ui->main_page_cont_15);
    lv_obj_set_size(ui->main_page_spinbox_10_btn_minus, main_page_spinbox_10_h, main_page_spinbox_10_h);
    lv_obj_align_to(ui->main_page_spinbox_10_btn_minus, ui->main_page_spinbox_10, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_10_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_10_btn_minus, lv_main_page_spinbox_10_decrement_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->main_page_spinbox_10, 31, 23);

    //Write style for main_page_spinbox_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_10, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_10, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_spinbox_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_spinbox_10, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_spinbox_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_spinbox_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_spinbox_10, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_spinbox_10, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_10, &lv_font_IntelOneMono_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_spinbox_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_spinbox_10, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_spinbox_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_spinbox_10, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_spinbox_10, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_10, &lv_font_IntelOneMono_Medium_11, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_10, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_10, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_10, lv_color_hex(0x929292), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_10, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spinbox_10_extra_btns_main_default
    static lv_style_t style_main_page_spinbox_10_extra_btns_main_default;
    ui_init_style(&style_main_page_spinbox_10_extra_btns_main_default);

    lv_style_set_text_color(&style_main_page_spinbox_10_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_10_extra_btns_main_default, &lv_font_montserratMedium_9);
    lv_style_set_text_opa(&style_main_page_spinbox_10_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_10_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_10_extra_btns_main_default, lv_color_hex(0x66686a));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_10_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_10_extra_btns_main_default, 0);
    lv_style_set_radius(&style_main_page_spinbox_10_extra_btns_main_default, 9);
    lv_style_set_shadow_width(&style_main_page_spinbox_10_extra_btns_main_default, 0);
    lv_obj_add_style(ui->main_page_spinbox_10_btn_plus, &style_main_page_spinbox_10_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->main_page_spinbox_10_btn_minus, &style_main_page_spinbox_10_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_PRESSED for &style_main_page_spinbox_10_extra_btns_main_pressed
    static lv_style_t style_main_page_spinbox_10_extra_btns_main_pressed;
    ui_init_style(&style_main_page_spinbox_10_extra_btns_main_pressed);

    lv_style_set_text_color(&style_main_page_spinbox_10_extra_btns_main_pressed, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_10_extra_btns_main_pressed, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_main_page_spinbox_10_extra_btns_main_pressed, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_10_extra_btns_main_pressed, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_10_extra_btns_main_pressed, lv_color_hex(0x696b6e));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_10_extra_btns_main_pressed, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_10_extra_btns_main_pressed, 0);
    lv_style_set_radius(&style_main_page_spinbox_10_extra_btns_main_pressed, 5);
    lv_style_set_shadow_width(&style_main_page_spinbox_10_extra_btns_main_pressed, 0);
    lv_obj_add_style(ui->main_page_spinbox_10_btn_plus, &style_main_page_spinbox_10_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_style(ui->main_page_spinbox_10_btn_minus, &style_main_page_spinbox_10_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes main_page_cont_14
    ui->main_page_cont_14 = lv_obj_create(ui->main_page_setting_tileview_setting3);
    lv_obj_set_pos(ui->main_page_cont_14, 6, 163);
    lv_obj_set_size(ui->main_page_cont_14, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_cont_14, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_cont_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_cont_14, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_cont_14, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_cont_14, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_cont_14, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_cont_14, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_cont_14, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_cont_14, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_39
    ui->main_page_label_39 = lv_label_create(ui->main_page_cont_14);
    lv_label_set_text(ui->main_page_label_39, "3");
    lv_label_set_long_mode(ui->main_page_label_39, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_39, 7, 4);
    lv_obj_set_size(ui->main_page_label_39, 111, 20);

    //Write style for main_page_label_39, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_39, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_39, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_39, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_39, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spinbox_9
    ui->main_page_spinbox_9 = lv_spinbox_create(ui->main_page_cont_14);
    lv_obj_set_pos(ui->main_page_spinbox_9, 31, 23);
    lv_obj_set_width(ui->main_page_spinbox_9, 63);
    lv_obj_set_height(ui->main_page_spinbox_9, 25);
    lv_spinbox_set_digit_format(ui->main_page_spinbox_9, 2, 2);
    lv_spinbox_set_range(ui->main_page_spinbox_9, -99, 99);
    lv_coord_t main_page_spinbox_9_h = lv_obj_get_height(ui->main_page_spinbox_9);
    ui->main_page_spinbox_9_btn_plus = lv_btn_create(ui->main_page_cont_14);
    lv_obj_set_size(ui->main_page_spinbox_9_btn_plus, main_page_spinbox_9_h, main_page_spinbox_9_h);
    lv_obj_align_to(ui->main_page_spinbox_9_btn_plus, ui->main_page_spinbox_9, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_9_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_9_btn_plus, lv_main_page_spinbox_9_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->main_page_spinbox_9_btn_minus = lv_btn_create(ui->main_page_cont_14);
    lv_obj_set_size(ui->main_page_spinbox_9_btn_minus, main_page_spinbox_9_h, main_page_spinbox_9_h);
    lv_obj_align_to(ui->main_page_spinbox_9_btn_minus, ui->main_page_spinbox_9, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_9_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_9_btn_minus, lv_main_page_spinbox_9_decrement_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->main_page_spinbox_9, 31, 23);

    //Write style for main_page_spinbox_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_9, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_spinbox_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_spinbox_9, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_spinbox_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_spinbox_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_spinbox_9, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_spinbox_9, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_9, &lv_font_IntelOneMono_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_spinbox_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_spinbox_9, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_spinbox_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_spinbox_9, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_spinbox_9, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_9, &lv_font_IntelOneMono_Medium_11, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_9, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_9, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_9, lv_color_hex(0x929292), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_9, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spinbox_9_extra_btns_main_default
    static lv_style_t style_main_page_spinbox_9_extra_btns_main_default;
    ui_init_style(&style_main_page_spinbox_9_extra_btns_main_default);

    lv_style_set_text_color(&style_main_page_spinbox_9_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_9_extra_btns_main_default, &lv_font_montserratMedium_9);
    lv_style_set_text_opa(&style_main_page_spinbox_9_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_9_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_9_extra_btns_main_default, lv_color_hex(0x66686a));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_9_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_9_extra_btns_main_default, 0);
    lv_style_set_radius(&style_main_page_spinbox_9_extra_btns_main_default, 9);
    lv_style_set_shadow_width(&style_main_page_spinbox_9_extra_btns_main_default, 0);
    lv_obj_add_style(ui->main_page_spinbox_9_btn_plus, &style_main_page_spinbox_9_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->main_page_spinbox_9_btn_minus, &style_main_page_spinbox_9_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_PRESSED for &style_main_page_spinbox_9_extra_btns_main_pressed
    static lv_style_t style_main_page_spinbox_9_extra_btns_main_pressed;
    ui_init_style(&style_main_page_spinbox_9_extra_btns_main_pressed);

    lv_style_set_text_color(&style_main_page_spinbox_9_extra_btns_main_pressed, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_9_extra_btns_main_pressed, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_main_page_spinbox_9_extra_btns_main_pressed, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_9_extra_btns_main_pressed, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_9_extra_btns_main_pressed, lv_color_hex(0x696b6e));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_9_extra_btns_main_pressed, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_9_extra_btns_main_pressed, 0);
    lv_style_set_radius(&style_main_page_spinbox_9_extra_btns_main_pressed, 5);
    lv_style_set_shadow_width(&style_main_page_spinbox_9_extra_btns_main_pressed, 0);
    lv_obj_add_style(ui->main_page_spinbox_9_btn_plus, &style_main_page_spinbox_9_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_style(ui->main_page_spinbox_9_btn_minus, &style_main_page_spinbox_9_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes main_page_cont_13
    ui->main_page_cont_13 = lv_obj_create(ui->main_page_setting_tileview_setting3);
    lv_obj_set_pos(ui->main_page_cont_13, 6, 225);
    lv_obj_set_size(ui->main_page_cont_13, 129, 57);
    lv_obj_set_scrollbar_mode(ui->main_page_cont_13, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_page_cont_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_cont_13, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_cont_13, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_cont_13, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_cont_13, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_cont_13, 62, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_cont_13, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_cont_13, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_38
    ui->main_page_label_38 = lv_label_create(ui->main_page_cont_13);
    lv_label_set_text(ui->main_page_label_38, "4");
    lv_label_set_long_mode(ui->main_page_label_38, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_38, 7, 3);
    lv_obj_set_size(ui->main_page_label_38, 111, 20);

    //Write style for main_page_label_38, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_38, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_38, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_38, 187, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_38, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_spinbox_8
    ui->main_page_spinbox_8 = lv_spinbox_create(ui->main_page_cont_13);
    lv_obj_set_pos(ui->main_page_spinbox_8, 31, 24);
    lv_obj_set_width(ui->main_page_spinbox_8, 63);
    lv_obj_set_height(ui->main_page_spinbox_8, 25);
    lv_spinbox_set_digit_format(ui->main_page_spinbox_8, 4, 4);
    lv_spinbox_set_range(ui->main_page_spinbox_8, -9999, 9999);
    lv_coord_t main_page_spinbox_8_h = lv_obj_get_height(ui->main_page_spinbox_8);
    ui->main_page_spinbox_8_btn_plus = lv_btn_create(ui->main_page_cont_13);
    lv_obj_set_size(ui->main_page_spinbox_8_btn_plus, main_page_spinbox_8_h, main_page_spinbox_8_h);
    lv_obj_align_to(ui->main_page_spinbox_8_btn_plus, ui->main_page_spinbox_8, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_8_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_8_btn_plus, lv_main_page_spinbox_8_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->main_page_spinbox_8_btn_minus = lv_btn_create(ui->main_page_cont_13);
    lv_obj_set_size(ui->main_page_spinbox_8_btn_minus, main_page_spinbox_8_h, main_page_spinbox_8_h);
    lv_obj_align_to(ui->main_page_spinbox_8_btn_minus, ui->main_page_spinbox_8, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_img_src(ui->main_page_spinbox_8_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->main_page_spinbox_8_btn_minus, lv_main_page_spinbox_8_decrement_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->main_page_spinbox_8, 31, 24);

    //Write style for main_page_spinbox_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_8, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_8, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_spinbox_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_spinbox_8, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_spinbox_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_spinbox_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_spinbox_8, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_spinbox_8, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_8, &lv_font_IntelOneMono_Medium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_spinbox_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_spinbox_8, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_spinbox_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_spinbox_8, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_spinbox_8, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_spinbox_8, &lv_font_IntelOneMono_Medium_11, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_spinbox_8, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_spinbox_8, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_spinbox_8, lv_color_hex(0x929292), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_spinbox_8, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_spinbox_8_extra_btns_main_default
    static lv_style_t style_main_page_spinbox_8_extra_btns_main_default;
    ui_init_style(&style_main_page_spinbox_8_extra_btns_main_default);

    lv_style_set_text_color(&style_main_page_spinbox_8_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_8_extra_btns_main_default, &lv_font_montserratMedium_9);
    lv_style_set_text_opa(&style_main_page_spinbox_8_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_8_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_8_extra_btns_main_default, lv_color_hex(0x66686a));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_8_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_8_extra_btns_main_default, 0);
    lv_style_set_radius(&style_main_page_spinbox_8_extra_btns_main_default, 9);
    lv_style_set_shadow_width(&style_main_page_spinbox_8_extra_btns_main_default, 0);
    lv_obj_add_style(ui->main_page_spinbox_8_btn_plus, &style_main_page_spinbox_8_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->main_page_spinbox_8_btn_minus, &style_main_page_spinbox_8_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_PRESSED for &style_main_page_spinbox_8_extra_btns_main_pressed
    static lv_style_t style_main_page_spinbox_8_extra_btns_main_pressed;
    ui_init_style(&style_main_page_spinbox_8_extra_btns_main_pressed);

    lv_style_set_text_color(&style_main_page_spinbox_8_extra_btns_main_pressed, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_spinbox_8_extra_btns_main_pressed, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_main_page_spinbox_8_extra_btns_main_pressed, 255);
    lv_style_set_bg_opa(&style_main_page_spinbox_8_extra_btns_main_pressed, 255);
    lv_style_set_bg_color(&style_main_page_spinbox_8_extra_btns_main_pressed, lv_color_hex(0x696b6e));
    lv_style_set_bg_grad_dir(&style_main_page_spinbox_8_extra_btns_main_pressed, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_spinbox_8_extra_btns_main_pressed, 0);
    lv_style_set_radius(&style_main_page_spinbox_8_extra_btns_main_pressed, 5);
    lv_style_set_shadow_width(&style_main_page_spinbox_8_extra_btns_main_pressed, 0);
    lv_obj_add_style(ui->main_page_spinbox_8_btn_plus, &style_main_page_spinbox_8_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_style(ui->main_page_spinbox_8_btn_minus, &style_main_page_spinbox_8_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes main_page_btn_11
    ui->main_page_btn_11 = lv_btn_create(ui->main_page_setting_tileview_setting3);
    ui->main_page_btn_11_label = lv_label_create(ui->main_page_btn_11);
    lv_label_set_text(ui->main_page_btn_11_label, "             杂项           " LV_SYMBOL_RIGHT "");
    lv_label_set_long_mode(ui->main_page_btn_11_label, LV_LABEL_LONG_CLIP);
    lv_obj_align(ui->main_page_btn_11_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_btn_11, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_btn_11_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_btn_11, 6, 288);
    lv_obj_set_size(ui->main_page_btn_11, 129, 27);

    //Write style for main_page_btn_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_btn_11, 58, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_btn_11, lv_color_hex(0x676868), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_btn_11, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_btn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_btn_11, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_btn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_btn_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_btn_11, &lv_font_OPPOSans_Medium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_btn_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_btn_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_42
    ui->main_page_label_42 = lv_label_create(ui->main_page_setting_tileview_setting3);
    lv_label_set_text(ui->main_page_label_42, "" LV_SYMBOL_CHARGE " 性能");
    lv_label_set_long_mode(ui->main_page_label_42, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_42, 41, 10);
    lv_obj_set_size(ui->main_page_label_42, 59, 17);

    //Write style for main_page_label_42, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_42, lv_color_hex(0xd8d8d8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_42, &lv_font_OPPOSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_42, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_42, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);



    //Write codes main_page_img_3
    ui->main_page_img_3 = lv_img_create(ui->main_page_main_tileview_about);
    lv_obj_add_flag(ui->main_page_img_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->main_page_img_3, &_logo_alpha_57x59);
    lv_img_set_pivot(ui->main_page_img_3, 50,50);
    lv_img_set_angle(ui->main_page_img_3, 0);
    lv_obj_set_pos(ui->main_page_img_3, 43, 46);
    lv_obj_set_size(ui->main_page_img_3, 57, 59);

    //Write style for main_page_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->main_page_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->main_page_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->main_page_img_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_23
    ui->main_page_label_23 = lv_label_create(ui->main_page_main_tileview_about);
    lv_label_set_text(ui->main_page_label_23, "CONST\nCURRENT\nBOY");
    lv_label_set_long_mode(ui->main_page_label_23, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_23, 21, 100);
    lv_obj_set_size(ui->main_page_label_23, 100, 49);

    //Write style for main_page_label_23, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_23, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_23, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_23, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_23, &lv_font_OPPOSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_23, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_23, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_23, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_23, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_23, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_23, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_23, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_23, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_23, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_23, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_22
    ui->main_page_label_22 = lv_label_create(ui->main_page_main_tileview_about);
    lv_label_set_text(ui->main_page_label_22, "Power\nDevourer");
    lv_label_set_long_mode(ui->main_page_label_22, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_22, 33, 170);
    lv_obj_set_size(ui->main_page_label_22, 78, 31);

    //Write style for main_page_label_22, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_22, lv_color_hex(0x714040), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_22, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_22, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_22, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_line_3
    ui->main_page_line_3 = lv_line_create(ui->main_page_main_tileview_about);
    static lv_point_t main_page_line_3[] = {{0, 0},{75, 0},};
    lv_line_set_points(ui->main_page_line_3, main_page_line_3, 2);
    lv_obj_set_pos(ui->main_page_line_3, 34, 208);
    lv_obj_set_size(ui->main_page_line_3, 75, 1);

    //Write style for main_page_line_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->main_page_line_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->main_page_line_3, lv_color_hex(0x714040), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->main_page_line_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->main_page_line_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_21
    ui->main_page_label_21 = lv_label_create(ui->main_page_main_tileview_about);
    lv_label_set_text(ui->main_page_label_21, "@SYSU/SEIT");
    lv_label_set_long_mode(ui->main_page_label_21, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_21, 38, 305);
    lv_obj_set_size(ui->main_page_label_21, 69, 12);

    //Write style for main_page_label_21, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_21, lv_color_hex(0x777777), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_21, &lv_font_montserratMedium_9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_21, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_21, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_img_2
    ui->main_page_img_2 = lv_img_create(ui->main_page_main_tileview_about);
    lv_obj_add_flag(ui->main_page_img_2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->main_page_img_2, &_circLEMoon_alpha_30x30);
    lv_img_set_pivot(ui->main_page_img_2, 50,50);
    lv_img_set_angle(ui->main_page_img_2, 0);
    lv_obj_set_pos(ui->main_page_img_2, 90, 253);
    lv_obj_set_size(ui->main_page_img_2, 30, 30);

    //Write style for main_page_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->main_page_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->main_page_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_img_2, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->main_page_img_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_img_1
    ui->main_page_img_1 = lv_img_create(ui->main_page_main_tileview_about);
    lv_obj_add_flag(ui->main_page_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->main_page_img_1, &_Triwalt_alpha_30x30);
    lv_img_set_pivot(ui->main_page_img_1, 50,50);
    lv_img_set_angle(ui->main_page_img_1, 0);
    lv_obj_set_pos(ui->main_page_img_1, 26, 253);
    lv_obj_set_size(ui->main_page_img_1, 30, 30);

    //Write style for main_page_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->main_page_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->main_page_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_img_1, 30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->main_page_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_20
    ui->main_page_label_20 = lv_label_create(ui->main_page_main_tileview_about);
    lv_label_set_text(ui->main_page_label_20, "Triwalt          circLΣMoon");
    lv_label_set_long_mode(ui->main_page_label_20, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_20, 24, 287);
    lv_obj_set_size(ui->main_page_label_20, 109, 12);

    //Write style for main_page_label_20, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_20, lv_color_hex(0xd0d0d0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_20, &lv_font_OPPOSans_Medium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_20, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_19
    ui->main_page_label_19 = lv_label_create(ui->main_page_main_tileview_about);
    lv_label_set_text(ui->main_page_label_19, "" LV_SYMBOL_CLOSE " ");
    lv_label_set_long_mode(ui->main_page_label_19, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_19, 60, 265);
    lv_obj_set_size(ui->main_page_label_19, 27, 14);

    //Write style for main_page_label_19, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_19, lv_color_hex(0xc5c5c5), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_19, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_19, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_36
    ui->main_page_label_36 = lv_label_create(ui->main_page_main_tileview_about);
    lv_label_set_text(ui->main_page_label_36, "" LV_SYMBOL_EDIT " 关于");
    lv_label_set_long_mode(ui->main_page_label_36, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_36, 41, 10);
    lv_obj_set_size(ui->main_page_label_36, 59, 17);

    //Write style for main_page_label_36, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_36, lv_color_hex(0xd8d8d8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_36, &lv_font_OPPOSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_36, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_36, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_label_37
    ui->main_page_label_37 = lv_label_create(ui->main_page_main_tileview_about);
    lv_label_set_text(ui->main_page_label_37, "Dev 0.2.4.30");
    lv_label_set_long_mode(ui->main_page_label_37, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_label_37, 24, 215);
    lv_obj_set_size(ui->main_page_label_37, 94, 15);

    //Write style for main_page_label_37, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_label_37, lv_color_hex(0x949494), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_label_37, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_label_37, 192, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_label_37, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_line_1
    ui->main_page_line_1 = lv_line_create(ui->main_page);
    static lv_point_t main_page_line_1[] = {{0, 0},{0, 200},};
    lv_line_set_points(ui->main_page_line_1, main_page_line_1, 2);
    lv_obj_set_pos(ui->main_page_line_1, 168, 32);
    lv_obj_set_size(ui->main_page_line_1, 1, 200);

    //Write style for main_page_line_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->main_page_line_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->main_page_line_1, lv_color_hex(0x757575), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->main_page_line_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->main_page_line_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_line_2
    ui->main_page_line_2 = lv_line_create(ui->main_page);
    static lv_point_t main_page_line_2[] = {{0, 0},{300, 0},};
    lv_line_set_points(ui->main_page_line_2, main_page_line_2, 2);
    lv_obj_set_pos(ui->main_page_line_2, 30, 130);
    lv_obj_set_size(ui->main_page_line_2, 280, 1);

    //Write style for main_page_line_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->main_page_line_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->main_page_line_2, lv_color_hex(0x757575), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->main_page_line_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->main_page_line_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_ONOFF
    ui->main_page_ONOFF = lv_btn_create(ui->main_page);
    lv_obj_add_flag(ui->main_page_ONOFF, LV_OBJ_FLAG_CHECKABLE);
    ui->main_page_ONOFF_label = lv_label_create(ui->main_page_ONOFF);
    lv_label_set_text(ui->main_page_ONOFF_label, "" LV_SYMBOL_POWER "");
    lv_label_set_long_mode(ui->main_page_ONOFF_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->main_page_ONOFF_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->main_page_ONOFF, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->main_page_ONOFF_label, LV_PCT(100));
    lv_obj_set_pos(ui->main_page_ONOFF, 30, 254);
    lv_obj_set_size(ui->main_page_ONOFF, 45, 45);

    //Write style for main_page_ONOFF, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_ONOFF, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_ONOFF, lv_color_hex(0x757575), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_ONOFF, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_ONOFF, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_ONOFF, 34, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_ONOFF, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_ONOFF, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_ONOFF, &lv_font_IntelOneMono_Medium_19, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_ONOFF, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_ONOFF, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_ONOFF, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_bg_opa(ui->main_page_ONOFF, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(ui->main_page_ONOFF, lv_color_hex(0x871021), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(ui->main_page_ONOFF, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_border_width(ui->main_page_ONOFF, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_radius(ui->main_page_ONOFF, 60, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->main_page_ONOFF, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->main_page_ONOFF, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->main_page_ONOFF, &lv_font_montserratMedium_17, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->main_page_ONOFF, 255, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for main_page_ONOFF, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->main_page_ONOFF, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->main_page_ONOFF, lv_color_hex(0xc5001e), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->main_page_ONOFF, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->main_page_ONOFF, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_radius(ui->main_page_ONOFF, 60, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->main_page_ONOFF, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->main_page_ONOFF, lv_color_hex(0xe7e7e7), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->main_page_ONOFF, &lv_font_montserratMedium_19, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->main_page_ONOFF, 255, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for main_page_ONOFF, Part: LV_PART_MAIN, State: LV_STATE_DISABLED.
    lv_obj_set_style_bg_opa(ui->main_page_ONOFF, 255, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(ui->main_page_ONOFF, lv_color_hex(0x7d7d7d), LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_bg_grad_dir(ui->main_page_ONOFF, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_border_width(ui->main_page_ONOFF, 0, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_radius(ui->main_page_ONOFF, 60, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_shadow_width(ui->main_page_ONOFF, 0, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_text_color(ui->main_page_ONOFF, lv_color_hex(0xa6a6a6), LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_text_font(ui->main_page_ONOFF, &lv_font_montserratMedium_19, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_text_opa(ui->main_page_ONOFF, 255, LV_PART_MAIN|LV_STATE_DISABLED);

    //Write codes main_page_running_time
    static bool main_page_running_time_timer_enabled = false;
    ui->main_page_running_time = lv_dclock_create(ui->main_page, "0:00:00");
    if (!main_page_running_time_timer_enabled) {
        lv_timer_create(main_page_running_time_timer, 1000, NULL);
        main_page_running_time_timer_enabled = true;
    }
    lv_obj_set_pos(ui->main_page_running_time, 230, 281);
    lv_obj_set_size(ui->main_page_running_time, 90, 22);

    //Write style for main_page_running_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->main_page_running_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_running_time, lv_color_hex(0x00d9ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_running_time, &lv_font_IntelOneMono_Medium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_running_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_running_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_running_time, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_running_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_running_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_running_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_running_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_running_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_running_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_time_title
    ui->main_page_time_title = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_time_title, "运行时间:");
    lv_label_set_long_mode(ui->main_page_time_title, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_time_title, 162, 284);
    lv_obj_set_size(ui->main_page_time_title, 68, 17);

    //Write style for main_page_time_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_time_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_time_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_time_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_time_title, &lv_font_OPPOSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_time_title, 171, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_time_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_time_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_time_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_time_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_time_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_time_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_time_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_time_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_time_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_degree_Celsius
    ui->main_page_degree_Celsius = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_degree_Celsius, "℃");
    lv_label_set_long_mode(ui->main_page_degree_Celsius, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_degree_Celsius, 300, 256);
    lv_obj_set_size(ui->main_page_degree_Celsius, 18, 15);

    //Write style for main_page_degree_Celsius, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_degree_Celsius, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_degree_Celsius, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_degree_Celsius, lv_color_hex(0xe4ff37), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_degree_Celsius, &lv_font_OPPOSans_Medium_17, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_degree_Celsius, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_degree_Celsius, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_degree_Celsius, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_degree_Celsius, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_degree_Celsius, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_degree_Celsius, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_degree_Celsius, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_degree_Celsius, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_degree_Celsius, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_degree_Celsius, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_temperature_label
    ui->main_page_temperature_label = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_temperature_label, "25 ");
    lv_label_set_long_mode(ui->main_page_temperature_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_temperature_label, 230, 254);
    lv_obj_set_size(ui->main_page_temperature_label, 79, 22);

    //Write style for main_page_temperature_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_temperature_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_temperature_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_temperature_label, lv_color_hex(0xe4ff37), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_temperature_label, &lv_font_IntelOneMono_Medium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_temperature_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_temperature_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_temperature_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_temperature_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_temperature_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_temperature_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_temperature_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_temperature_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_temperature_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_temperature_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_temperature_title
    ui->main_page_temperature_title = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_temperature_title, "负载温度:");
    lv_label_set_long_mode(ui->main_page_temperature_title, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_temperature_title, 162, 259);
    lv_obj_set_size(ui->main_page_temperature_title, 68, 17);

    //Write style for main_page_temperature_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_temperature_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_temperature_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_temperature_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_temperature_title, &lv_font_OPPOSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_temperature_title, 171, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_temperature_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_temperature_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_temperature_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_temperature_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_temperature_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_temperature_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_temperature_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_temperature_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_temperature_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_measure_resistance_label
    ui->main_page_measure_resistance_label = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_measure_resistance_label, "0.000");
    lv_label_set_long_mode(ui->main_page_measure_resistance_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_measure_resistance_label, 21, 180);
    lv_obj_set_size(ui->main_page_measure_resistance_label, 130, 31);
    lv_obj_add_flag(ui->main_page_measure_resistance_label, LV_OBJ_FLAG_CLICKABLE);

    //Write style for main_page_measure_resistance_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_measure_resistance_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_resistance_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_measure_resistance_label, lv_color_hex(0x85bbff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_resistance_label, &lv_font_IntelOneMono_Medium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_measure_resistance_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_measure_resistance_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_measure_resistance_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_measure_resistance_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_measure_resistance_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_measure_resistance_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_measure_resistance_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_measure_resistance_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_measure_resistance_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_measure_resistance_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_measure_resistance_chart
    ui->main_page_measure_resistance_chart = lv_chart_create(ui->main_page);
    lv_chart_set_type(ui->main_page_measure_resistance_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_div_line_count(ui->main_page_measure_resistance_chart, 3, 5);
    lv_chart_set_point_count(ui->main_page_measure_resistance_chart, 5);
    lv_chart_set_range(ui->main_page_measure_resistance_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_range(ui->main_page_measure_resistance_chart, LV_CHART_AXIS_SECONDARY_Y, 0, 100);
    lv_chart_set_zoom_x(ui->main_page_measure_resistance_chart, 256);
    lv_chart_set_zoom_y(ui->main_page_measure_resistance_chart, 256);
    lv_obj_set_style_size(ui->main_page_measure_resistance_chart, 0, LV_PART_INDICATOR);
    ui->main_page_measure_resistance_chart_0 = lv_chart_add_series(ui->main_page_measure_resistance_chart, lv_color_hex(0x85bbff), LV_CHART_AXIS_PRIMARY_Y);
#if LV_USE_FREEMASTER == 0
    lv_chart_set_next_value(ui->main_page_measure_resistance_chart, ui->main_page_measure_resistance_chart_0, 1);
    lv_chart_set_next_value(ui->main_page_measure_resistance_chart, ui->main_page_measure_resistance_chart_0, 20);
    lv_chart_set_next_value(ui->main_page_measure_resistance_chart, ui->main_page_measure_resistance_chart_0, 30);
    lv_chart_set_next_value(ui->main_page_measure_resistance_chart, ui->main_page_measure_resistance_chart_0, 40);
    lv_chart_set_next_value(ui->main_page_measure_resistance_chart, ui->main_page_measure_resistance_chart_0, 5);
#endif
    lv_obj_set_pos(ui->main_page_measure_resistance_chart, 30, 173);
    lv_obj_set_size(ui->main_page_measure_resistance_chart, 110, 44);
    lv_obj_set_scrollbar_mode(ui->main_page_measure_resistance_chart, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->main_page_measure_resistance_chart, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->main_page_measure_resistance_chart, LV_OBJ_FLAG_CLICKABLE);

    //Write style for main_page_measure_resistance_chart, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_measure_resistance_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_measure_resistance_chart, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_measure_resistance_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_measure_resistance_chart, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_measure_resistance_chart, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_resistance_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->main_page_measure_resistance_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_measure_resistance_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_measure_resistance_chart, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_measure_resistance_chart, lv_color_hex(0x151212), LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_resistance_chart, &lv_font_montserratMedium_12, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_measure_resistance_chart, 255, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->main_page_measure_resistance_chart, 0, LV_PART_TICKS|LV_STATE_DEFAULT);

    //Write codes main_page_measure_resistance_title
    ui->main_page_measure_resistance_title = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_measure_resistance_title, "负载电阻(Ω)");
    lv_label_set_long_mode(ui->main_page_measure_resistance_title, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_measure_resistance_title, 21, 143);
    lv_obj_set_size(ui->main_page_measure_resistance_title, 130, 25);

    //Write style for main_page_measure_resistance_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_measure_resistance_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_resistance_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_measure_resistance_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_resistance_title, &lv_font_OPPOSans_Medium_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_measure_resistance_title, 171, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_measure_resistance_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_measure_resistance_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_measure_resistance_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_measure_resistance_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_measure_resistance_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_measure_resistance_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_measure_resistance_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_measure_resistance_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_measure_resistance_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_measure_power_label
    ui->main_page_measure_power_label = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_measure_power_label, "0.000");
    lv_label_set_long_mode(ui->main_page_measure_power_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_measure_power_label, 185, 180);
    lv_obj_set_size(ui->main_page_measure_power_label, 130, 30);
    lv_obj_add_flag(ui->main_page_measure_power_label, LV_OBJ_FLAG_CLICKABLE);

    //Write style for main_page_measure_power_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_measure_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_measure_power_label, lv_color_hex(0xf98aff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_power_label, &lv_font_IntelOneMono_Medium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_measure_power_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_measure_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_measure_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_measure_power_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_measure_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_measure_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_measure_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_measure_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_measure_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_measure_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_measure_power_chart
    ui->main_page_measure_power_chart = lv_chart_create(ui->main_page);
    lv_chart_set_type(ui->main_page_measure_power_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_div_line_count(ui->main_page_measure_power_chart, 3, 5);
    lv_chart_set_point_count(ui->main_page_measure_power_chart, 5);
    lv_chart_set_range(ui->main_page_measure_power_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_range(ui->main_page_measure_power_chart, LV_CHART_AXIS_SECONDARY_Y, 0, 100);
    lv_chart_set_zoom_x(ui->main_page_measure_power_chart, 256);
    lv_chart_set_zoom_y(ui->main_page_measure_power_chart, 256);
    lv_obj_set_style_size(ui->main_page_measure_power_chart, 0, LV_PART_INDICATOR);
    ui->main_page_measure_power_chart_0 = lv_chart_add_series(ui->main_page_measure_power_chart, lv_color_hex(0xf98aff), LV_CHART_AXIS_PRIMARY_Y);
#if LV_USE_FREEMASTER == 0
    lv_chart_set_next_value(ui->main_page_measure_power_chart, ui->main_page_measure_power_chart_0, 1);
    lv_chart_set_next_value(ui->main_page_measure_power_chart, ui->main_page_measure_power_chart_0, 20);
    lv_chart_set_next_value(ui->main_page_measure_power_chart, ui->main_page_measure_power_chart_0, 30);
    lv_chart_set_next_value(ui->main_page_measure_power_chart, ui->main_page_measure_power_chart_0, 40);
    lv_chart_set_next_value(ui->main_page_measure_power_chart, ui->main_page_measure_power_chart_0, 5);
#endif
    lv_obj_set_pos(ui->main_page_measure_power_chart, 195, 173);
    lv_obj_set_size(ui->main_page_measure_power_chart, 110, 44);
    lv_obj_set_scrollbar_mode(ui->main_page_measure_power_chart, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->main_page_measure_power_chart, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->main_page_measure_power_chart, LV_OBJ_FLAG_CLICKABLE);

    //Write style for main_page_measure_power_chart, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_measure_power_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_measure_power_chart, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_measure_power_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_measure_power_chart, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_measure_power_chart, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_power_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->main_page_measure_power_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_measure_power_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_measure_power_chart, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_measure_power_chart, lv_color_hex(0x151212), LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_power_chart, &lv_font_montserratMedium_12, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_measure_power_chart, 255, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->main_page_measure_power_chart, 0, LV_PART_TICKS|LV_STATE_DEFAULT);

    //Write codes main_page_measure_power_title
    ui->main_page_measure_power_title = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_measure_power_title, "负载功率(W)");
    lv_label_set_long_mode(ui->main_page_measure_power_title, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_measure_power_title, 185, 143);
    lv_obj_set_size(ui->main_page_measure_power_title, 130, 25);

    //Write style for main_page_measure_power_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_measure_power_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_power_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_measure_power_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_power_title, &lv_font_OPPOSans_Medium_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_measure_power_title, 171, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_measure_power_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_measure_power_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_measure_power_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_measure_power_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_measure_power_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_measure_power_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_measure_power_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_measure_power_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_measure_power_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_measure_voltage_label
    ui->main_page_measure_voltage_label = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_measure_voltage_label, "0.000");
    lv_label_set_long_mode(ui->main_page_measure_voltage_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_measure_voltage_label, 185, 83);
    lv_obj_set_size(ui->main_page_measure_voltage_label, 130, 30);
    lv_obj_add_flag(ui->main_page_measure_voltage_label, LV_OBJ_FLAG_CLICKABLE);

    //Write style for main_page_measure_voltage_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_measure_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_measure_voltage_label, lv_color_hex(0xa6ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_voltage_label, &lv_font_IntelOneMono_Medium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
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

    //Write codes main_page_measure_voltage_chart
    ui->main_page_measure_voltage_chart = lv_chart_create(ui->main_page);
    lv_chart_set_type(ui->main_page_measure_voltage_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_div_line_count(ui->main_page_measure_voltage_chart, 3, 5);
    lv_chart_set_point_count(ui->main_page_measure_voltage_chart, 5);
    lv_chart_set_range(ui->main_page_measure_voltage_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_range(ui->main_page_measure_voltage_chart, LV_CHART_AXIS_SECONDARY_Y, 0, 100);
    lv_chart_set_zoom_x(ui->main_page_measure_voltage_chart, 256);
    lv_chart_set_zoom_y(ui->main_page_measure_voltage_chart, 256);
    lv_obj_set_style_size(ui->main_page_measure_voltage_chart, 0, LV_PART_INDICATOR);
    ui->main_page_measure_voltage_chart_0 = lv_chart_add_series(ui->main_page_measure_voltage_chart, lv_color_hex(0xa6ff00), LV_CHART_AXIS_PRIMARY_Y);
#if LV_USE_FREEMASTER == 0
    lv_chart_set_next_value(ui->main_page_measure_voltage_chart, ui->main_page_measure_voltage_chart_0, 1);
    lv_chart_set_next_value(ui->main_page_measure_voltage_chart, ui->main_page_measure_voltage_chart_0, 20);
    lv_chart_set_next_value(ui->main_page_measure_voltage_chart, ui->main_page_measure_voltage_chart_0, 30);
    lv_chart_set_next_value(ui->main_page_measure_voltage_chart, ui->main_page_measure_voltage_chart_0, 40);
    lv_chart_set_next_value(ui->main_page_measure_voltage_chart, ui->main_page_measure_voltage_chart_0, 5);
#endif
    lv_obj_set_pos(ui->main_page_measure_voltage_chart, 195, 76);
    lv_obj_set_size(ui->main_page_measure_voltage_chart, 110, 44);
    lv_obj_set_scrollbar_mode(ui->main_page_measure_voltage_chart, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->main_page_measure_voltage_chart, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->main_page_measure_voltage_chart, LV_OBJ_FLAG_CLICKABLE);

    //Write style for main_page_measure_voltage_chart, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_measure_voltage_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_measure_voltage_chart, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_measure_voltage_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_measure_voltage_chart, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_measure_voltage_chart, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_voltage_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->main_page_measure_voltage_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_measure_voltage_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_measure_voltage_chart, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_measure_voltage_chart, lv_color_hex(0x151212), LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_voltage_chart, &lv_font_montserratMedium_12, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_measure_voltage_chart, 255, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->main_page_measure_voltage_chart, 0, LV_PART_TICKS|LV_STATE_DEFAULT);

    //Write codes main_page_measure_voltage_title
    ui->main_page_measure_voltage_title = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_measure_voltage_title, "电压(V)");
    lv_label_set_long_mode(ui->main_page_measure_voltage_title, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_measure_voltage_title, 185, 45);
    lv_obj_set_size(ui->main_page_measure_voltage_title, 130, 25);

    //Write style for main_page_measure_voltage_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_measure_voltage_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_voltage_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_measure_voltage_title, lv_color_hex(0xfafafa), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_voltage_title, &lv_font_OPPOSans_Medium_25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_measure_voltage_title, 171, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_measure_voltage_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_measure_voltage_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_measure_voltage_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_measure_voltage_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_measure_voltage_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_measure_voltage_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_measure_voltage_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_measure_voltage_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_measure_voltage_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_measure_current_label
    ui->main_page_measure_current_label = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_measure_current_label, "0.000");
    lv_label_set_long_mode(ui->main_page_measure_current_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_measure_current_label, 11, 83);
    lv_obj_set_size(ui->main_page_measure_current_label, 150, 31);
    lv_obj_add_flag(ui->main_page_measure_current_label, LV_OBJ_FLAG_CLICKABLE);

    //Write style for main_page_measure_current_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_measure_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_measure_current_label, lv_color_hex(0xc4c4c4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_current_label, &lv_font_IntelOneMono_Medium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
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

    //Write codes main_page_measure_current_chart
    ui->main_page_measure_current_chart = lv_chart_create(ui->main_page);
    lv_chart_set_type(ui->main_page_measure_current_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_div_line_count(ui->main_page_measure_current_chart, 3, 5);
    lv_chart_set_point_count(ui->main_page_measure_current_chart, 5);
    lv_chart_set_range(ui->main_page_measure_current_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_range(ui->main_page_measure_current_chart, LV_CHART_AXIS_SECONDARY_Y, 0, 100);
    lv_chart_set_zoom_x(ui->main_page_measure_current_chart, 256);
    lv_chart_set_zoom_y(ui->main_page_measure_current_chart, 256);
    lv_obj_set_style_size(ui->main_page_measure_current_chart, 0, LV_PART_INDICATOR);
    ui->main_page_measure_current_chart_0 = lv_chart_add_series(ui->main_page_measure_current_chart, lv_color_hex(0xff9600), LV_CHART_AXIS_PRIMARY_Y);
#if LV_USE_FREEMASTER == 0
    lv_chart_set_next_value(ui->main_page_measure_current_chart, ui->main_page_measure_current_chart_0, 1);
    lv_chart_set_next_value(ui->main_page_measure_current_chart, ui->main_page_measure_current_chart_0, 20);
    lv_chart_set_next_value(ui->main_page_measure_current_chart, ui->main_page_measure_current_chart_0, 30);
    lv_chart_set_next_value(ui->main_page_measure_current_chart, ui->main_page_measure_current_chart_0, 40);
    lv_chart_set_next_value(ui->main_page_measure_current_chart, ui->main_page_measure_current_chart_0, 5);
#endif
    lv_obj_set_pos(ui->main_page_measure_current_chart, 30, 76);
    lv_obj_set_size(ui->main_page_measure_current_chart, 110, 44);
    lv_obj_set_scrollbar_mode(ui->main_page_measure_current_chart, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->main_page_measure_current_chart, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->main_page_measure_current_chart, LV_OBJ_FLAG_CLICKABLE);

    //Write style for main_page_measure_current_chart, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_measure_current_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_measure_current_chart, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->main_page_measure_current_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->main_page_measure_current_chart, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->main_page_measure_current_chart, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_current_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->main_page_measure_current_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_measure_current_chart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_page_measure_current_chart, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_page_measure_current_chart, lv_color_hex(0x151212), LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_current_chart, &lv_font_montserratMedium_12, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_measure_current_chart, 255, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->main_page_measure_current_chart, 0, LV_PART_TICKS|LV_STATE_DEFAULT);

    //Write codes main_page_measure_current_title
    ui->main_page_measure_current_title = lv_label_create(ui->main_page);
    lv_label_set_text(ui->main_page_measure_current_title, "电流(mA)");
    lv_label_set_long_mode(ui->main_page_measure_current_title, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_page_measure_current_title, 21, 45);
    lv_obj_set_size(ui->main_page_measure_current_title, 130, 25);

    //Write style for main_page_measure_current_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_page_measure_current_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_measure_current_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_page_measure_current_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_page_measure_current_title, &lv_font_OPPOSans_Medium_25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_page_measure_current_title, 164, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_page_measure_current_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_page_measure_current_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_page_measure_current_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_page_measure_current_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_page_measure_current_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_page_measure_current_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_page_measure_current_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_page_measure_current_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_measure_current_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_page_over_voltage_warning_msgbox
    static const char * main_page_over_voltage_warning_msgbox_btns[] = {""};
    ui->main_page_over_voltage_warning_msgbox = lv_msgbox_create(ui->main_page, "OVER  VOLATGE  WARNING", "\n极限过压了\n\n\n\n", main_page_over_voltage_warning_msgbox_btns, false);
    lv_obj_set_size(lv_msgbox_get_btns(ui->main_page_over_voltage_warning_msgbox), 0, 30);
    lv_obj_set_pos(ui->main_page_over_voltage_warning_msgbox, 101, 83);
    lv_obj_set_size(ui->main_page_over_voltage_warning_msgbox, 280, 150);
    lv_obj_add_flag(ui->main_page_over_voltage_warning_msgbox, LV_OBJ_FLAG_HIDDEN);

    //Write style for main_page_over_voltage_warning_msgbox, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_page_over_voltage_warning_msgbox, 156, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_page_over_voltage_warning_msgbox, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_page_over_voltage_warning_msgbox, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_page_over_voltage_warning_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_page_over_voltage_warning_msgbox, 18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_page_over_voltage_warning_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_over_voltage_warning_msgbox_extra_title_main_default
    static lv_style_t style_main_page_over_voltage_warning_msgbox_extra_title_main_default;
    ui_init_style(&style_main_page_over_voltage_warning_msgbox_extra_title_main_default);

    lv_style_set_text_color(&style_main_page_over_voltage_warning_msgbox_extra_title_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_over_voltage_warning_msgbox_extra_title_main_default, &lv_font_OPPOSans_Medium_19);
    lv_style_set_text_opa(&style_main_page_over_voltage_warning_msgbox_extra_title_main_default, 255);
    lv_style_set_text_letter_space(&style_main_page_over_voltage_warning_msgbox_extra_title_main_default, 0);
    lv_style_set_text_line_space(&style_main_page_over_voltage_warning_msgbox_extra_title_main_default, 30);
    lv_obj_add_style(lv_msgbox_get_title(ui->main_page_over_voltage_warning_msgbox), &style_main_page_over_voltage_warning_msgbox_extra_title_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_over_voltage_warning_msgbox_extra_content_main_default
    static lv_style_t style_main_page_over_voltage_warning_msgbox_extra_content_main_default;
    ui_init_style(&style_main_page_over_voltage_warning_msgbox_extra_content_main_default);

    lv_style_set_text_color(&style_main_page_over_voltage_warning_msgbox_extra_content_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_main_page_over_voltage_warning_msgbox_extra_content_main_default, &lv_font_OPPOSans_Medium_14);
    lv_style_set_text_opa(&style_main_page_over_voltage_warning_msgbox_extra_content_main_default, 255);
    lv_style_set_text_letter_space(&style_main_page_over_voltage_warning_msgbox_extra_content_main_default, 0);
    lv_style_set_text_line_space(&style_main_page_over_voltage_warning_msgbox_extra_content_main_default, 10);
    lv_obj_add_style(lv_msgbox_get_text(ui->main_page_over_voltage_warning_msgbox), &style_main_page_over_voltage_warning_msgbox_extra_content_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_main_page_over_voltage_warning_msgbox_extra_btns_items_default
    static lv_style_t style_main_page_over_voltage_warning_msgbox_extra_btns_items_default;
    ui_init_style(&style_main_page_over_voltage_warning_msgbox_extra_btns_items_default);

    lv_style_set_bg_opa(&style_main_page_over_voltage_warning_msgbox_extra_btns_items_default, 255);
    lv_style_set_bg_color(&style_main_page_over_voltage_warning_msgbox_extra_btns_items_default, lv_color_hex(0x7ce8fb));
    lv_style_set_bg_grad_dir(&style_main_page_over_voltage_warning_msgbox_extra_btns_items_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_main_page_over_voltage_warning_msgbox_extra_btns_items_default, 0);
    lv_style_set_radius(&style_main_page_over_voltage_warning_msgbox_extra_btns_items_default, 4);
    lv_style_set_text_color(&style_main_page_over_voltage_warning_msgbox_extra_btns_items_default, lv_color_hex(0x4e4e4e));
    lv_style_set_text_font(&style_main_page_over_voltage_warning_msgbox_extra_btns_items_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_main_page_over_voltage_warning_msgbox_extra_btns_items_default, 255);
    lv_obj_add_style(lv_msgbox_get_btns(ui->main_page_over_voltage_warning_msgbox), &style_main_page_over_voltage_warning_msgbox_extra_btns_items_default, LV_PART_ITEMS|LV_STATE_DEFAULT);

    //The custom code of main_page.


    //Update current screen layout.
    lv_obj_update_layout(ui->main_page);

    //Init events for screen.
    events_init_main_page(ui);
}
