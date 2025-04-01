#include "our_encoder.h"

encoder_handle_t::encoder_handle_t(int pin_A, int pin_B)
{
    this->pin_A = (gpio_num_t)pin_A;
    this->pin_B = (gpio_num_t)pin_B;

    this->total_count = 0;
    this->single_count = 0;
    this->encoder.attachFullQuad(pin_A, pin_B);
    this->encoder.setCount(0);
}



int64_t encoder_handle_t::read_count_accum_clear()
{
    this->single_count = this->encoder.getCount();
    this->encoder.clearCount();
    this->total_count += this->single_count;

    return this->total_count;
}


