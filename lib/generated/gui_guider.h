/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *main_page;
	bool main_page_del;
	lv_obj_t *main_page_img_1;
	lv_obj_t *main_page_img_3;
	lv_obj_t *main_page_btn_4;
	lv_obj_t *main_page_btn_4_label;
	lv_obj_t *main_page_img_4;
	lv_obj_t *main_page_spangroup_1;
	lv_span_t *main_page_spangroup_1_span;
	lv_obj_t *main_page_sw_1;
	lv_obj_t *main_page_spangroup_2;
	lv_span_t *main_page_spangroup_2_span;
	lv_obj_t *main_page_btn_7;
	lv_obj_t *main_page_btn_7_label;
	lv_obj_t *main_page_spangroup_3;
	lv_span_t *main_page_spangroup_3_span;
	lv_obj_t *main_page_img_5;
	lv_obj_t *main_page_measure_current_label;
	lv_obj_t *main_page_spangroup_4;
	lv_span_t *main_page_spangroup_4_span;
	lv_obj_t *main_page_btn_8;
	lv_obj_t *main_page_btn_8_label;
	lv_obj_t *main_page_btn_9;
	lv_obj_t *main_page_btn_9_label;
	lv_obj_t *main_page_btn_10;
	lv_obj_t *main_page_btn_10_label;
	lv_obj_t *main_page_spangroup_5;
	lv_span_t *main_page_spangroup_5_span;
	lv_obj_t *main_page_spangroup_6;
	lv_span_t *main_page_spangroup_6_span;
	lv_obj_t *main_page_measure_voltage_label;
	lv_obj_t *main_page_measure_register_label;
	lv_obj_t *main_page_spangroup_7;
	lv_span_t *main_page_spangroup_7_span;
	lv_obj_t *main_page_spangroup_8;
	lv_span_t *main_page_spangroup_8_span;
	lv_obj_t *main_page_spangroup_15;
	lv_span_t *main_page_spangroup_15_span;
	lv_obj_t *main_page_measure_power_label;
	lv_obj_t *main_page_spangroup_10;
	lv_span_t *main_page_spangroup_10_span;
	lv_obj_t *main_page_btn_11;
	lv_obj_t *main_page_btn_11_label;
	lv_obj_t *main_page_set_current_label;
	lv_obj_t *main_page_spangroup_11;
	lv_span_t *main_page_spangroup_11_span;
	lv_obj_t *main_page_spangroup_12;
	lv_span_t *main_page_spangroup_12_span;
	lv_obj_t *debug_test;
	bool debug_test_del;
	lv_obj_t *debug_test_img_1;
	lv_obj_t *debug_test_img_2;
	lv_obj_t *debug_test_btn_1;
	lv_obj_t *debug_test_btn_1_label;
	lv_obj_t *debug_test_sw_1;
	lv_obj_t *debug_test_slider_1;
	lv_obj_t *debug_test_ddlist_1;
	lv_obj_t *debug_test_qrcode_1;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_main_page(lv_ui *ui);
void setup_scr_debug_test(lv_ui *ui);
LV_IMG_DECLARE(_white_alpha_239x206);
LV_IMG_DECLARE(_electric_sensor_3239_alpha_28x24);
LV_IMG_DECLARE(_blue_alpha_83x240);
LV_IMG_DECLARE(_cc_alpha_20x19);
LV_IMG_DECLARE(_Asuna_alpha_239x245);
LV_IMG_DECLARE(_blue_alpha_98x271);

LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_montserratMedium_23)
LV_FONT_DECLARE(lv_font_montserratMedium_20)
LV_FONT_DECLARE(lv_font_montserratMedium_18)
LV_FONT_DECLARE(lv_font_montserratMedium_15)


#ifdef __cplusplus
}
#endif
#endif
