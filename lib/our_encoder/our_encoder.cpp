/**
 * @file our_encoder.cpp
 * @brief 旋转编码器类，使用 ESP32Encoder 库实现，并且提供了累加计数和快慢模式接口
 * @author skyswordx
 */

#include "our_encoder.hpp"


/**
 * @brief 旋转编码器类构造函数
 * @author skyswordx
 * @param pin_A 旋转编码器 A 相引脚
 * @param pin_B 旋转编码器 B 相引脚
 */
encoder_handle_t::encoder_handle_t(int pin_A, int pin_B)
{
    this->pin_A = (gpio_num_t)pin_A;
    this->pin_B = (gpio_num_t)pin_B;

    this->total_count = 0;
    this->single_count = 0;
    this->encoder.attachFullQuad(pin_A, pin_B);
    this->encoder.setCount(0);
}


/**
 * @brief 读取旋转编码器的计数值，并清除计数器
 * @author skyswordx
 * @return float 旋转编码器的总计数值
 * @details 默认使用 4 倍频，这里可以根据需要进行处理
 *          这里默认是进入 else 分支
 */
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


