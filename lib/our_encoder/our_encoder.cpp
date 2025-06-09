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
    this->last_count = 0; // 初始化上次计数值
    this->encoder.attachSingleEdge(pin_A, pin_B);
    this->encoder.setCount(0);
}


/**
 * @brief 读取旋转编码器的计数值，并清除计数器
 * @author skyswordx
 * @return double 旋转编码器的总计数值
 * @details 默认使用 4 倍频，这里可以根据需要进行处理
 *          这里默认是进入 else 分支
 */
double encoder_handle_t::read_count_accum_clear()   
{
    // 默认 SINGLE，可以根据需要进行处理
    // 这里默认是进入 SINGLE 分支
    if (this->mode == TEN) {
        this->single_count = this->encoder.getCount() * 10.0;
        this->encoder.clearCount();
        this->total_count += this->single_count;
    } else if (this->mode == SINGLE) {
        this->single_count = this->encoder.getCount();
        this->encoder.clearCount();
        this->total_count += this->single_count;
    } else if (this->mode == HUNDRED) {
        this->single_count = this->encoder.getCount() * 100.0;
        this->encoder.clearCount();
        this->total_count += this->single_count;
    }

    return this->total_count;
}

/** 
 * @brief 设置旋转编码器的计数值
 * @author Triwalt
 * @param value 旋转编码器的计数值
 * @details 
 * 
 */

void encoder_handle_t::set_count(int64_t value) {
    this->encoder.setCount(value);
    this->total_count = value;
}

/**
 * @brief 读取旋转编码器的增量计数值
 * @author skyswordx
 * @return double 旋转编码器相对于上次调用的增量值
 * @details 该函数不会清除编码器计数器，而是计算相对于上次调用的增量
 *          根据模式进行相应的倍数处理
 */
double encoder_handle_t::read_count_increment()
{
    int64_t current_count = this->encoder.getCount();
    int64_t increment = current_count - this->last_count;
    this->last_count = current_count;
    
    double processed_increment = 0.0;
    
    // 根据模式处理增量
    if (this->mode == TEN) {
        processed_increment = increment * 10.0;
    } else if (this->mode == SINGLE) {
        processed_increment = increment;
    } else if (this->mode == HUNDRED) {
        processed_increment = increment * 100.0;
    }
    
    return processed_increment;
}

