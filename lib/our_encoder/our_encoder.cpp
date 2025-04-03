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



float encoder_handle_t::read_count_accum_clear()   
{
    // 默认使用 4 倍频，这里可以根据需要进行处理
    // 这里默认是进入 else 分支
    if (this->mode == QUAD) {
        this->single_count = this->encoder.getCount();
        this->encoder.clearCount();
        this->total_count += this->single_count;
    } else if (this->mode == SINGLE) {
        this->single_count = this->encoder.getCount() / 4.0;
        this->encoder.clearCount();
        this->total_count += this->single_count;
    }

    return this->total_count;
}


