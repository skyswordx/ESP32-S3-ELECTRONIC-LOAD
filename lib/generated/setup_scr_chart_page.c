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
    lv_obj_set_style_bg_opa(ui->chart_page, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->chart_page, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->chart_page, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes chart_page_canvas_1
    ui->chart_page_canvas_1 = lv_canvas_create(ui->chart_page);
    static lv_color_t buf_chart_page_canvas_1[LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(480, 320)];
    lv_canvas_set_buffer(ui->chart_page_canvas_1, buf_chart_page_canvas_1, 480, 320, LV_IMG_CF_TRUE_COLOR_ALPHA);
    lv_canvas_fill_bg(ui->chart_page_canvas_1, lv_color_hex(0xffffff), 255);
    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_0;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_0);
    chart_page_canvas_1_label_dsc_0.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_0.font = &lv_font_Acme_Regular_20;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 158, 7, 200, &chart_page_canvas_1_label_dsc_0, "Stacked Area Chart");

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_1[] = {{30, 36},{450, 36},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_1;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_1);
    chart_page_canvas_1_line_dsc_1.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_1.opa = 255;
    chart_page_canvas_1_line_dsc_1.width = 1;
    chart_page_canvas_1_line_dsc_1.round_start = false;
    chart_page_canvas_1_line_dsc_1.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_1, 2, &chart_page_canvas_1_line_dsc_1);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_2[] = {{30, 76},{450, 76},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_2;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_2);
    chart_page_canvas_1_line_dsc_2.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_2.opa = 255;
    chart_page_canvas_1_line_dsc_2.width = 1;
    chart_page_canvas_1_line_dsc_2.round_start = false;
    chart_page_canvas_1_line_dsc_2.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_2, 2, &chart_page_canvas_1_line_dsc_2);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_3[] = {{30, 116},{450, 116},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_3;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_3);
    chart_page_canvas_1_line_dsc_3.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_3.opa = 255;
    chart_page_canvas_1_line_dsc_3.width = 1;
    chart_page_canvas_1_line_dsc_3.round_start = false;
    chart_page_canvas_1_line_dsc_3.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_3, 2, &chart_page_canvas_1_line_dsc_3);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_4[] = {{30, 156},{450, 156},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_4;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_4);
    chart_page_canvas_1_line_dsc_4.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_4.opa = 255;
    chart_page_canvas_1_line_dsc_4.width = 1;
    chart_page_canvas_1_line_dsc_4.round_start = false;
    chart_page_canvas_1_line_dsc_4.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_4, 2, &chart_page_canvas_1_line_dsc_4);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_5[] = {{30, 196},{450, 196},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_5;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_5);
    chart_page_canvas_1_line_dsc_5.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_5.opa = 255;
    chart_page_canvas_1_line_dsc_5.width = 1;
    chart_page_canvas_1_line_dsc_5.round_start = false;
    chart_page_canvas_1_line_dsc_5.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_5, 2, &chart_page_canvas_1_line_dsc_5);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_6[] = {{30, 236},{450, 236},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_6;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_6);
    chart_page_canvas_1_line_dsc_6.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_6.opa = 255;
    chart_page_canvas_1_line_dsc_6.width = 1;
    chart_page_canvas_1_line_dsc_6.round_start = false;
    chart_page_canvas_1_line_dsc_6.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_6, 2, &chart_page_canvas_1_line_dsc_6);

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_7;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_7);
    chart_page_canvas_1_label_dsc_7.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_7.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 1, 32, 100, &chart_page_canvas_1_label_dsc_7, "2500");

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_8;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_8);
    chart_page_canvas_1_label_dsc_8.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_8.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 1, 72, 100, &chart_page_canvas_1_label_dsc_8, "2000");

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_9;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_9);
    chart_page_canvas_1_label_dsc_9.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_9.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 1, 112, 100, &chart_page_canvas_1_label_dsc_9, "1500");

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_10;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_10);
    chart_page_canvas_1_label_dsc_10.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_10.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 1, 152, 100, &chart_page_canvas_1_label_dsc_10, "1000");

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_11;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_11);
    chart_page_canvas_1_label_dsc_11.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_11.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 3, 192, 100, &chart_page_canvas_1_label_dsc_11, "500");

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_12;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_12);
    chart_page_canvas_1_label_dsc_12.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_12.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 15, 232, 100, &chart_page_canvas_1_label_dsc_12, "0");

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_13[] = {{30, 236},{30, 240},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_13;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_13);
    chart_page_canvas_1_line_dsc_13.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_13.opa = 255;
    chart_page_canvas_1_line_dsc_13.width = 0;
    chart_page_canvas_1_line_dsc_13.round_start = false;
    chart_page_canvas_1_line_dsc_13.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_13, 2, &chart_page_canvas_1_line_dsc_13);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_14[] = {{100, 236},{100, 240},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_14;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_14);
    chart_page_canvas_1_line_dsc_14.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_14.opa = 255;
    chart_page_canvas_1_line_dsc_14.width = 0;
    chart_page_canvas_1_line_dsc_14.round_start = false;
    chart_page_canvas_1_line_dsc_14.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_14, 2, &chart_page_canvas_1_line_dsc_14);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_15[] = {{170, 236},{170, 240},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_15;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_15);
    chart_page_canvas_1_line_dsc_15.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_15.opa = 255;
    chart_page_canvas_1_line_dsc_15.width = 0;
    chart_page_canvas_1_line_dsc_15.round_start = false;
    chart_page_canvas_1_line_dsc_15.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_15, 2, &chart_page_canvas_1_line_dsc_15);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_16[] = {{240, 236},{240, 240},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_16;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_16);
    chart_page_canvas_1_line_dsc_16.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_16.opa = 255;
    chart_page_canvas_1_line_dsc_16.width = 0;
    chart_page_canvas_1_line_dsc_16.round_start = false;
    chart_page_canvas_1_line_dsc_16.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_16, 2, &chart_page_canvas_1_line_dsc_16);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_17[] = {{310, 236},{310, 240},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_17;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_17);
    chart_page_canvas_1_line_dsc_17.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_17.opa = 255;
    chart_page_canvas_1_line_dsc_17.width = 0;
    chart_page_canvas_1_line_dsc_17.round_start = false;
    chart_page_canvas_1_line_dsc_17.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_17, 2, &chart_page_canvas_1_line_dsc_17);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_18[] = {{380, 236},{380, 240},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_18;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_18);
    chart_page_canvas_1_line_dsc_18.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_18.opa = 255;
    chart_page_canvas_1_line_dsc_18.width = 0;
    chart_page_canvas_1_line_dsc_18.round_start = false;
    chart_page_canvas_1_line_dsc_18.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_18, 2, &chart_page_canvas_1_line_dsc_18);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_19[] = {{450, 236},{450, 240},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_19;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_19);
    chart_page_canvas_1_line_dsc_19.color = lv_color_hex(0xb8b8b8);
    chart_page_canvas_1_line_dsc_19.opa = 255;
    chart_page_canvas_1_line_dsc_19.width = 0;
    chart_page_canvas_1_line_dsc_19.round_start = false;
    chart_page_canvas_1_line_dsc_19.round_end = false;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_19, 2, &chart_page_canvas_1_line_dsc_19);

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_20;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_20);
    chart_page_canvas_1_label_dsc_20.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_20.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 18, 245, 100, &chart_page_canvas_1_label_dsc_20, "Mon");

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_21;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_21);
    chart_page_canvas_1_label_dsc_21.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_21.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 90, 245, 100, &chart_page_canvas_1_label_dsc_21, "Tue");

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_22;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_22);
    chart_page_canvas_1_label_dsc_22.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_22.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 156, 245, 100, &chart_page_canvas_1_label_dsc_22, "Wed");

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_23;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_23);
    chart_page_canvas_1_label_dsc_23.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_23.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 231, 245, 100, &chart_page_canvas_1_label_dsc_23, "Thu");

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_24;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_24);
    chart_page_canvas_1_label_dsc_24.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_24.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 302, 245, 100, &chart_page_canvas_1_label_dsc_24, "Fri");

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_25;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_25);
    chart_page_canvas_1_label_dsc_25.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_25.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 371, 245, 100, &chart_page_canvas_1_label_dsc_25, "Sat");

    //Canvas draw text
    lv_draw_label_dsc_t chart_page_canvas_1_label_dsc_26;
    lv_draw_label_dsc_init(&chart_page_canvas_1_label_dsc_26);
    chart_page_canvas_1_label_dsc_26.color = lv_color_hex(0x000000);
    chart_page_canvas_1_label_dsc_26.font = &lv_font_montserratMedium_10;
    lv_canvas_draw_text(ui->chart_page_canvas_1, 440, 245, 100, &chart_page_canvas_1_label_dsc_26, "Sun");

    //Canvas draw polygon
    static lv_point_t chart_page_canvas_1_points_array_27[] = {{30, 150},{31, 236},{450, 236},{450, 65},};
    lv_draw_rect_dsc_t chart_page_canvas_1_rect_dsc_27;
    lv_draw_rect_dsc_init(&chart_page_canvas_1_rect_dsc_27);
    chart_page_canvas_1_rect_dsc_27.radius = 0;
    chart_page_canvas_1_rect_dsc_27.bg_opa = 150;
    chart_page_canvas_1_rect_dsc_27.bg_color = lv_color_hex(0x8bd7ff);
    chart_page_canvas_1_rect_dsc_27.bg_grad.dir = LV_GRAD_DIR_NONE;
    chart_page_canvas_1_rect_dsc_27.border_width = 0;
    chart_page_canvas_1_rect_dsc_27.border_opa = 255;
    chart_page_canvas_1_rect_dsc_27.border_color = lv_color_hex(0xfab0b0);
    lv_canvas_draw_polygon(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_27, 4, &chart_page_canvas_1_rect_dsc_27);

    //Canvas draw polygon
    static lv_point_t chart_page_canvas_1_points_array_28[] = {{30, 170},{30, 236},{450, 236},{450, 124},};
    lv_draw_rect_dsc_t chart_page_canvas_1_rect_dsc_28;
    lv_draw_rect_dsc_init(&chart_page_canvas_1_rect_dsc_28);
    chart_page_canvas_1_rect_dsc_28.radius = 0;
    chart_page_canvas_1_rect_dsc_28.bg_opa = 150;
    chart_page_canvas_1_rect_dsc_28.bg_color = lv_color_hex(0xd84545);
    chart_page_canvas_1_rect_dsc_28.bg_grad.dir = LV_GRAD_DIR_NONE;
    chart_page_canvas_1_rect_dsc_28.border_width = 0;
    chart_page_canvas_1_rect_dsc_28.border_opa = 0;
    chart_page_canvas_1_rect_dsc_28.border_color = lv_color_hex(0x000000);
    lv_canvas_draw_polygon(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_28, 4, &chart_page_canvas_1_rect_dsc_28);

    //Canvas draw polygon
    static lv_point_t chart_page_canvas_1_points_array_29[] = {{30, 187},{30, 236},{450, 236},{450, 160},};
    lv_draw_rect_dsc_t chart_page_canvas_1_rect_dsc_29;
    lv_draw_rect_dsc_init(&chart_page_canvas_1_rect_dsc_29);
    chart_page_canvas_1_rect_dsc_29.radius = 0;
    chart_page_canvas_1_rect_dsc_29.bg_opa = 150;
    chart_page_canvas_1_rect_dsc_29.bg_color = lv_color_hex(0xf6d94b);
    chart_page_canvas_1_rect_dsc_29.bg_grad.dir = LV_GRAD_DIR_NONE;
    chart_page_canvas_1_rect_dsc_29.border_width = 0;
    chart_page_canvas_1_rect_dsc_29.border_opa = 0;
    chart_page_canvas_1_rect_dsc_29.border_color = lv_color_hex(0x000000);
    lv_canvas_draw_polygon(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_29, 4, &chart_page_canvas_1_rect_dsc_29);

    //Canvas draw polygon
    static lv_point_t chart_page_canvas_1_points_array_30[] = {{30, 208},{30, 236},{450, 236},{450, 195},};
    lv_draw_rect_dsc_t chart_page_canvas_1_rect_dsc_30;
    lv_draw_rect_dsc_init(&chart_page_canvas_1_rect_dsc_30);
    chart_page_canvas_1_rect_dsc_30.radius = 0;
    chart_page_canvas_1_rect_dsc_30.bg_opa = 150;
    chart_page_canvas_1_rect_dsc_30.bg_color = lv_color_hex(0x58bb6b);
    chart_page_canvas_1_rect_dsc_30.bg_grad.dir = LV_GRAD_DIR_NONE;
    chart_page_canvas_1_rect_dsc_30.border_width = 0;
    chart_page_canvas_1_rect_dsc_30.border_opa = 0;
    chart_page_canvas_1_rect_dsc_30.border_color = lv_color_hex(0x000000);
    lv_canvas_draw_polygon(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_30, 4, &chart_page_canvas_1_rect_dsc_30);

    //Canvas draw polygon
    static lv_point_t chart_page_canvas_1_points_array_31[] = {{30, 230},{30, 236},{450, 236},{450, 215},};
    lv_draw_rect_dsc_t chart_page_canvas_1_rect_dsc_31;
    lv_draw_rect_dsc_init(&chart_page_canvas_1_rect_dsc_31);
    chart_page_canvas_1_rect_dsc_31.radius = 0;
    chart_page_canvas_1_rect_dsc_31.bg_opa = 150;
    chart_page_canvas_1_rect_dsc_31.bg_color = lv_color_hex(0x1165ce);
    chart_page_canvas_1_rect_dsc_31.bg_grad.dir = LV_GRAD_DIR_NONE;
    chart_page_canvas_1_rect_dsc_31.border_width = 0;
    chart_page_canvas_1_rect_dsc_31.border_opa = 0;
    chart_page_canvas_1_rect_dsc_31.border_color = lv_color_hex(0x000000);
    lv_canvas_draw_polygon(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_31, 4, &chart_page_canvas_1_rect_dsc_31);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_32[] = {{214, 262},{238, 262},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_32;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_32);
    chart_page_canvas_1_line_dsc_32.color = lv_color_hex(0x474575);
    chart_page_canvas_1_line_dsc_32.opa = 255;
    chart_page_canvas_1_line_dsc_32.width = 6;
    chart_page_canvas_1_line_dsc_32.round_start = true;
    chart_page_canvas_1_line_dsc_32.round_end = true;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_32, 2, &chart_page_canvas_1_line_dsc_32);

    //Canvas draw line
    static lv_point_t chart_page_canvas_1_points_array_33[] = {{245, 262},{252, 262},};
    lv_draw_line_dsc_t chart_page_canvas_1_line_dsc_33;
    lv_draw_line_dsc_init(&chart_page_canvas_1_line_dsc_33);
    chart_page_canvas_1_line_dsc_33.color = lv_color_hex(0x7d7c9d);
    chart_page_canvas_1_line_dsc_33.opa = 255;
    chart_page_canvas_1_line_dsc_33.width = 4;
    chart_page_canvas_1_line_dsc_33.round_start = true;
    chart_page_canvas_1_line_dsc_33.round_end = true;
    lv_canvas_draw_line(ui->chart_page_canvas_1, chart_page_canvas_1_points_array_33, 2, &chart_page_canvas_1_line_dsc_33);

    lv_obj_set_pos(ui->chart_page_canvas_1, 5, 3);
    lv_obj_set_size(ui->chart_page_canvas_1, 480, 320);
    lv_obj_set_scrollbar_mode(ui->chart_page_canvas_1, LV_SCROLLBAR_MODE_OFF);

    //The custom code of chart_page.


    //Update current screen layout.
    lv_obj_update_layout(ui->chart_page);

    //Init events for screen.
    events_init_chart_page(ui);
}
