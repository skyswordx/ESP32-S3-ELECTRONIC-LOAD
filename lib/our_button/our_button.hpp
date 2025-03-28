#ifndef OUR_BUTTON_H
#define OUR_BUTTON_H

#include <Arduino.h>
/*
    中断函数的绑定和实现还是在 main 函数中进行
    这里仅仅完成 GPIO 按键的初始化
*/
class GPIO_button_handler_t{
    private:
        gpio_num_t pin; // GPIO 引脚号
        bool button_state; // 按键状态
       

    public:
        GPIO_button_handler_t(gpio_num_t pin); //
}



#endif // OUR_BUTTON_H