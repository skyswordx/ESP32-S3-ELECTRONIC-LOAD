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



void setup_scr_startup_page(lv_ui *ui)
{
    //Write codes startup_page
    ui->startup_page = lv_obj_create(NULL);
    lv_obj_set_size(ui->startup_page, 480, 320);
    lv_obj_set_scrollbar_mode(ui->startup_page, LV_SCROLLBAR_MODE_OFF);

    //Write style for startup_page, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->startup_page, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->startup_page, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->startup_page, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes startup_page_img_1
    ui->startup_page_img_1 = lv_img_create(ui->startup_page);
    lv_obj_add_flag(ui->startup_page_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->startup_page_img_1, &_logo_alpha_152x152);
    lv_img_set_pivot(ui->startup_page_img_1, 50,50);
    lv_img_set_angle(ui->startup_page_img_1, 0);
    lv_obj_set_pos(ui->startup_page_img_1, 92, 25);
    lv_obj_set_size(ui->startup_page_img_1, 152, 152);

    //Write style for startup_page_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->startup_page_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->startup_page_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->startup_page_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->startup_page_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes startup_page_label_1
    ui->startup_page_label_1 = lv_label_create(ui->startup_page);
    lv_label_set_text(ui->startup_page_label_1, "CONST CURRENT BOY                 恒流小子 v1.0");
    lv_label_set_long_mode(ui->startup_page_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->startup_page_label_1, 227, 38);
    lv_obj_set_size(ui->startup_page_label_1, 160, 110);

    //Write style for startup_page_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->startup_page_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->startup_page_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->startup_page_label_1, lv_color_hex(0xe0e0e0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->startup_page_label_1, &lv_font_OPPOSans_Medium_25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->startup_page_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->startup_page_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->startup_page_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->startup_page_label_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->startup_page_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->startup_page_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->startup_page_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->startup_page_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->startup_page_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->startup_page_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes startup_page_label_5
    ui->startup_page_label_5 = lv_label_create(ui->startup_page);
    lv_label_set_text(ui->startup_page_label_5, "All Rights Reserved");
    lv_label_set_long_mode(ui->startup_page_label_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->startup_page_label_5, 173, 291);
    lv_obj_set_size(ui->startup_page_label_5, 125, 12);

    //Write style for startup_page_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->startup_page_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->startup_page_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->startup_page_label_5, lv_color_hex(0x777777), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->startup_page_label_5, &lv_font_montserratMedium_9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->startup_page_label_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->startup_page_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->startup_page_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->startup_page_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->startup_page_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->startup_page_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->startup_page_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->startup_page_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->startup_page_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->startup_page_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes startup_page_label_4
    ui->startup_page_label_4 = lv_label_create(ui->startup_page);
    lv_label_set_text(ui->startup_page_label_4, "Triwalt        circLΣMoon");
    lv_label_set_long_mode(ui->startup_page_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->startup_page_label_4, 173, 273);
    lv_obj_set_size(ui->startup_page_label_4, 131, 12);

    //Write style for startup_page_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->startup_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->startup_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->startup_page_label_4, lv_color_hex(0xd0d0d0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->startup_page_label_4, &lv_font_OPPOSans_Medium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->startup_page_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->startup_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->startup_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->startup_page_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->startup_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->startup_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->startup_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->startup_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->startup_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->startup_page_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes startup_page_label_3
    ui->startup_page_label_3 = lv_label_create(ui->startup_page);
    lv_label_set_text(ui->startup_page_label_3, "" LV_SYMBOL_CLOSE " ");
    lv_label_set_long_mode(ui->startup_page_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->startup_page_label_3, 220, 245);
    lv_obj_set_size(ui->startup_page_label_3, 27, 14);

    //Write style for startup_page_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->startup_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->startup_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->startup_page_label_3, lv_color_hex(0xc5c5c5), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->startup_page_label_3, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->startup_page_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->startup_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->startup_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->startup_page_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->startup_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->startup_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->startup_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->startup_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->startup_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->startup_page_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes startup_page_img_4
    ui->startup_page_img_4 = lv_img_create(ui->startup_page);
    lv_obj_add_flag(ui->startup_page_img_4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->startup_page_img_4, &_circLEMoon_alpha_40x40);
    lv_img_set_pivot(ui->startup_page_img_4, 50,50);
    lv_img_set_angle(ui->startup_page_img_4, 0);
    lv_obj_set_pos(ui->startup_page_img_4, 248, 227);
    lv_obj_set_size(ui->startup_page_img_4, 40, 40);

    //Write style for startup_page_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->startup_page_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->startup_page_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->startup_page_img_4, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->startup_page_img_4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes startup_page_img_3
    ui->startup_page_img_3 = lv_img_create(ui->startup_page);
    lv_obj_add_flag(ui->startup_page_img_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->startup_page_img_3, &_Triwalt_alpha_40x40);
    lv_img_set_pivot(ui->startup_page_img_3, 50,50);
    lv_img_set_angle(ui->startup_page_img_3, 0);
    lv_obj_set_pos(ui->startup_page_img_3, 176, 227);
    lv_obj_set_size(ui->startup_page_img_3, 40, 40);

    //Write style for startup_page_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->startup_page_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->startup_page_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->startup_page_img_3, 30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->startup_page_img_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes startup_page_spinner_1
    ui->startup_page_spinner_1 = lv_spinner_create(ui->startup_page, 1000, 60);
    lv_obj_set_pos(ui->startup_page_spinner_1, 220, 172);
    lv_obj_set_size(ui->startup_page_spinner_1, 27, 26);

    //Write style for startup_page_spinner_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_top(ui->startup_page_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->startup_page_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->startup_page_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->startup_page_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->startup_page_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->startup_page_spinner_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->startup_page_spinner_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->startup_page_spinner_1, lv_color_hex(0x727272), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->startup_page_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for startup_page_spinner_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->startup_page_spinner_1, 6, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->startup_page_spinner_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->startup_page_spinner_1, lv_color_hex(0xffffff), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //The custom code of startup_page.


    //Update current screen layout.
    lv_obj_update_layout(ui->startup_page);

}
