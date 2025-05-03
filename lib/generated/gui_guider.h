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
	lv_obj_t *main_page_main_tileview;
	lv_obj_t *main_page_main_tileview_main;
	lv_obj_t *main_page_main_tileview_setting;
	lv_obj_t *main_page_main_tileview_about;
	lv_obj_t *main_page_load_regulation_container;
	lv_obj_t *main_page_start_load_regulation_button;
	lv_obj_t *main_page_start_load_regulation_button_label;
	lv_obj_t *main_page_load_regulation_description;
	lv_obj_t *main_page_load_regulation_label;
	lv_obj_t *main_page_set_current_container;
	lv_obj_t *main_page_set_current_description;
	lv_obj_t *main_page_set_current_box;
	lv_obj_t *main_page_set_current_box_btn_plus;
	lv_obj_t *main_page_set_current_box_btn_minus;
	lv_obj_t *main_page_over_voltage_container;
	lv_obj_t *main_page_over_volatge_description;
	lv_obj_t *main_page_over_voltage_box;
	lv_obj_t *main_page_over_voltage_box_btn_plus;
	lv_obj_t *main_page_over_voltage_box_btn_minus;
	lv_obj_t *main_page_mode_container;
	lv_obj_t *main_page_label_11;
	lv_obj_t *main_page_ddlist_1;
	lv_obj_t *main_page_next_page_button;
	lv_obj_t *main_page_next_page_button_label;
	lv_obj_t *main_page_project_name_en;
	lv_obj_t *main_page_project_name;
	lv_obj_t *main_page_logo;
	lv_obj_t *main_page_setting_tileview;
	lv_obj_t *main_page_setting_tileview_setting1;
	lv_obj_t *main_page_setting_tileview_setting2;
	lv_obj_t *main_page_setting_tileview_setting3;
	lv_obj_t *main_page_btn_8;
	lv_obj_t *main_page_btn_8_label;
	lv_obj_t *main_page_cont_8;
	lv_obj_t *main_page_label_29;
	lv_obj_t *main_page_spinbox_4;
	lv_obj_t *main_page_spinbox_4_btn_plus;
	lv_obj_t *main_page_spinbox_4_btn_minus;
	lv_obj_t *main_page_cont_7;
	lv_obj_t *main_page_label_28;
	lv_obj_t *main_page_ddlist_5;
	lv_obj_t *main_page_cont_6;
	lv_obj_t *main_page_label_26;
	lv_obj_t *main_page_sw_1;
	lv_obj_t *main_page_cont_5;
	lv_obj_t *main_page_label_25;
	lv_obj_t *main_page_ddlist_2;
	lv_obj_t *main_page_label_24;
	lv_obj_t *main_page_label_35;
	lv_obj_t *main_page_cont_12;
	lv_obj_t *main_page_label_34;
	lv_obj_t *main_page_ddlist_3;
	lv_obj_t *main_page_cont_11;
	lv_obj_t *main_page_label_33;
	lv_obj_t *main_page_spinbox_6;
	lv_obj_t *main_page_spinbox_6_btn_plus;
	lv_obj_t *main_page_spinbox_6_btn_minus;
	lv_obj_t *main_page_cont_10;
	lv_obj_t *main_page_label_32;
	lv_obj_t *main_page_spinbox_7;
	lv_obj_t *main_page_spinbox_7_btn_plus;
	lv_obj_t *main_page_spinbox_7_btn_minus;
	lv_obj_t *main_page_cont_9;
	lv_obj_t *main_page_label_30;
	lv_obj_t *main_page_spinbox_5;
	lv_obj_t *main_page_spinbox_5_btn_plus;
	lv_obj_t *main_page_spinbox_5_btn_minus;
	lv_obj_t *main_page_btn_10;
	lv_obj_t *main_page_btn_10_label;
	lv_obj_t *main_page_cont_16;
	lv_obj_t *main_page_label_41;
	lv_obj_t *main_page_ddlist_4;
	lv_obj_t *main_page_cont_15;
	lv_obj_t *main_page_label_40;
	lv_obj_t *main_page_spinbox_10;
	lv_obj_t *main_page_spinbox_10_btn_plus;
	lv_obj_t *main_page_spinbox_10_btn_minus;
	lv_obj_t *main_page_cont_14;
	lv_obj_t *main_page_label_39;
	lv_obj_t *main_page_spinbox_9;
	lv_obj_t *main_page_spinbox_9_btn_plus;
	lv_obj_t *main_page_spinbox_9_btn_minus;
	lv_obj_t *main_page_cont_13;
	lv_obj_t *main_page_label_38;
	lv_obj_t *main_page_spinbox_8;
	lv_obj_t *main_page_spinbox_8_btn_plus;
	lv_obj_t *main_page_spinbox_8_btn_minus;
	lv_obj_t *main_page_btn_11;
	lv_obj_t *main_page_btn_11_label;
	lv_obj_t *main_page_label_42;
	lv_obj_t *main_page_img_3;
	lv_obj_t *main_page_label_23;
	lv_obj_t *main_page_label_22;
	lv_obj_t *main_page_line_3;
	lv_obj_t *main_page_label_21;
	lv_obj_t *main_page_img_2;
	lv_obj_t *main_page_img_1;
	lv_obj_t *main_page_label_20;
	lv_obj_t *main_page_label_19;
	lv_obj_t *main_page_label_36;
	lv_obj_t *main_page_label_37;
	lv_obj_t *main_page_line_1;
	lv_obj_t *main_page_line_2;
	lv_obj_t *main_page_ONOFF;
	lv_obj_t *main_page_ONOFF_label;
	lv_obj_t *main_page_running_time;
	lv_obj_t *main_page_time_title;
	lv_obj_t *main_page_degree_Celsius;
	lv_obj_t *main_page_temperature_label;
	lv_obj_t *main_page_temperature_title;
	lv_obj_t *main_page_measure_resistance_label;
	lv_obj_t *main_page_measure_resistance_chart;
	lv_chart_series_t *main_page_measure_resistance_chart_0;
	lv_obj_t *main_page_measure_resistance_title;
	lv_obj_t *main_page_measure_power_label;
	lv_obj_t *main_page_measure_power_chart;
	lv_chart_series_t *main_page_measure_power_chart_0;
	lv_obj_t *main_page_measure_power_title;
	lv_obj_t *main_page_measure_voltage_label;
	lv_obj_t *main_page_measure_voltage_chart;
	lv_chart_series_t *main_page_measure_voltage_chart_0;
	lv_obj_t *main_page_measure_voltage_title;
	lv_obj_t *main_page_measure_current_label;
	lv_obj_t *main_page_measure_current_chart;
	lv_chart_series_t *main_page_measure_current_chart_0;
	lv_obj_t *main_page_measure_current_title;
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
LV_IMG_DECLARE(_logo_alpha_40x40);
LV_IMG_DECLARE(_logo_alpha_57x59);
LV_IMG_DECLARE(_circLEMoon_alpha_30x30);
LV_IMG_DECLARE(_Triwalt_alpha_30x30);

LV_FONT_DECLARE(lv_font_OPPOSans_Medium_13)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_OPPOSans_Medium_15)
LV_FONT_DECLARE(lv_font_IntelOneMono_Medium_16)
LV_FONT_DECLARE(lv_font_IntelOneMono_Medium_11)
LV_FONT_DECLARE(lv_font_montserratMedium_9)
LV_FONT_DECLARE(lv_font_OPPOSans_Medium_11)
LV_FONT_DECLARE(lv_font_OPPOSans_Medium_12)
LV_FONT_DECLARE(lv_font_FontAwesome5_28)
LV_FONT_DECLARE(lv_font_OPPOSans_Medium_8)
LV_FONT_DECLARE(lv_font_OPPOSans_Medium_16)
LV_FONT_DECLARE(lv_font_OPPOSans_Medium_10)
LV_FONT_DECLARE(lv_font_montserratMedium_11)
LV_FONT_DECLARE(lv_font_IntelOneMono_Medium_19)
LV_FONT_DECLARE(lv_font_montserratMedium_19)
LV_FONT_DECLARE(lv_font_montserratMedium_17)
LV_FONT_DECLARE(lv_font_IntelOneMono_Medium_20)
LV_FONT_DECLARE(lv_font_OPPOSans_Medium_17)
LV_FONT_DECLARE(lv_font_IntelOneMono_Medium_30)
LV_FONT_DECLARE(lv_font_OPPOSans_Medium_22)
LV_FONT_DECLARE(lv_font_OPPOSans_Medium_25)


#ifdef __cplusplus
}
#endif
#endif
