#include "my_gui_app.h"


lv_ui guider_ui;

void my_gui_app(void)
{
    // // Set up the LVGL && GUI
    // lv_init();
    // lv_port_disp_init();
    // lv_port_indev_init();

    // setup_ui(&guider_ui); // 初始化 gui_guider

    // Manage the GUI pages

    bool is_page1 = true;

    {
        lv_obj_t * current_page = lv_scr_act();
        if (current_page != NULL) {
            lv_obj_clean(current_page); // 释放当前页面的内存
        }

        if (is_page1 == true) {
            setup_scr_screen(&guider_ui); // gui_guider 为每一个页面生成的，这里是名字为 screen 的页面
            lv_scr_load(guider_ui.screen); //每一个页面的名字都是 gui_guider 结构体的元素
        }

    }

}