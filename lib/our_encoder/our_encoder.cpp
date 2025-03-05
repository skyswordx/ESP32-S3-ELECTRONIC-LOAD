#include "our_encoder.h"

encoder_handle_t::encoder_handle_t(int pin_A, int pin_B, int pin_S)
{
    this->pin_A = pin_A;
    this->pin_B = pin_B;
    this->pin_S = pin_S;
    this->total_count = 0;
    this->single_count = 0;
    this->encoder.attachFullQuad(pin_A, pin_B);
    this->encoder.setCount(0);

    pinMode(pin_S, INPUT);
    //配置GPIO，下降沿和上升沿触发中断
	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_INTR_ANYEDGE;
	io_conf.pin_bit_mask = 1 << pin_S;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_up_en = 1;
	gpio_config(&io_conf);











    

	gpio_set_intr_type(key_gpio_pin, GPIO_INTR_ANYEDGE);
	gpio_evt_queue = xQueueCreate(2, sizeof(uint32_t));
    //注册中断服务
	gpio_install_isr_service(0);
	gpio_isr_handler_add(key_gpio_pin, gpio_isr_handler, (void *) key_gpio_pin);
}


int64_t encoder_handle_t::read_count_accum_clear()
{
    this->single_count = this->encoder.getCount();
    this->encoder.clearCount();
    this->total_count += this->single_count;

    return this->total_count;
}

bool encoder_handle_t::is_pressed()
{
    // 按下时返回 pinS == LOW
    return digitalRead(this->pin_S == LOW);
}

void encoder_handle_t::set_mode(encoder_mode_t mode)
{
    this->mode = mode;
}

void encoder_handle_t::check_press_to_change_mode()
{
    if (this->is_pressed())
    {
        if (this->mode == QUAD)
        {
            this->mode = SINGLE;
        }
        else
        {
            this->mode = QUAD;
        }
    }

    

}