#include "our_button.hpp"


GPIO_button_handler_t::GPIO_button_handler_t(gpio_num_t pin) {
    this->pin = pin;
    this->button_state = false;
    
    // 绑定 GPIO 引脚至中断和中断处理函数
    // 配置GPIO，下降沿和上升沿触发中断
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = 1 << pin; // 为 GPIO pin 设置中断
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    // 注册中断服务
    gpio_set_intr_type((gpio_num_t)pin, GPIO_INTR_ANYEDGE);
    gpio_install_isr_service(0);
    // gpio_isr_handler_add((gpio_num_t)pin, button_press_ISR, (void *) &pin);
    // 设置 GPIO 中断类型
}


