/**
 * @file our_button.h
 * @brief GPIO 按键类的实现文件
 * @author skyswordx
 * @details 该文件包含了 GPIO 按键类的实现文件，包含了 GPIO 按键的初始化和中断处理函数
 *          这里使用 RTOS 规范的中断处理流程，仅仅在中断函数中释放信号量，标记触发中断的 GPIO 引脚号
 *          具体的按键处理逻辑在辅助任务中进行
 */

#ifndef OUR_BUTTON_H
#define OUR_BUTTON_H

#include <Arduino.h>
// PIO 找库能力太差了
// #include <lvgl.h>
// #include <SPI.h>
// #include <TFT_eSPI.h>
// #include "lv_port_disp.h"
// #include "lv_port_indev.h"

/*
    中断函数的绑定和实现还是在 main 函数中进行
    这里仅仅完成 GPIO 按键的初始化
*/
class GPIO_button_handler_t{
    public:
        gpio_num_t pin; // GPIO 引脚号
        bool button_state; // 按键状态
        GPIO_button_handler_t(gpio_num_t pin); 
};


void IRAM_ATTR button1_press_ISR(void *arg ); 
void IRAM_ATTR button2_press_ISR(void *arg );
void IRAM_ATTR button3_press_ISR(void *arg );
void IRAM_ATTR button4_press_ISR(void *arg );
void IRAM_ATTR encoder1_button_press_ISR(void *arg );

void button_handler_task(void *pvParameters); // 按键处理任务

#endif // OUR_BUTTON_H