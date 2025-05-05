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



void setup_scr_initial_page(lv_ui *ui)
{
    //Write codes initial_page
    ui->initial_page = lv_obj_create(NULL);
    lv_obj_set_size(ui->initial_page, 480, 320);
    lv_obj_set_scrollbar_mode(ui->initial_page, LV_SCROLLBAR_MODE_OFF);

    //Write style for initial_page, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->initial_page, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->initial_page, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->initial_page, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes initial_page_line_1
    ui->initial_page_line_1 = lv_line_create(ui->initial_page);
    static lv_point_t initial_page_line_1[] = {{0, 0},{0, 60},};
    lv_line_set_points(ui->initial_page_line_1, initial_page_line_1, 2);
    lv_obj_set_pos(ui->initial_page_line_1, 475, 316);
    lv_obj_set_size(ui->initial_page_line_1, 2, 2);

    //Write style for initial_page_line_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->initial_page_line_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->initial_page_line_1, lv_color_hex(0x000001), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->initial_page_line_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of initial_page.


    //Update current screen layout.
    lv_obj_update_layout(ui->initial_page);

}
