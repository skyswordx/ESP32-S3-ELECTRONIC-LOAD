#ifndef OUR_BUTTON_H
#define OUR_BUTTON_H

#include <Arduino.h>
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


#endif // OUR_BUTTON_H