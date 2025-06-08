/**
 * @file our_encoder.h
 * @brief 旋转编码器类，使用 ESP32Encoder 库实现，并且提供了累加计数和快慢模式接口
 * @author skyswordx
 */

#ifndef OUR_ENCODER
#define OUR_ENCODER 

#include <ESP32Encoder.h>
#include <Arduino.h>

// PIO 找库能力太差了
// #include <lvgl.h>
// #include <SPI.h>
// #include <TFT_eSPI.h>
// #include "lv_port_disp.h"
// #include "lv_port_indev.h"


/**
 * @brief 旋转编码器步进长度
 * @author skyswordx
 * @details SINGLE 模式：单倍模式
 *          TEN 模式：十倍模式
 *          HUNDRED 模式：百倍模式
 */
enum encoder_mode_t {
    TEN = 0,
    SINGLE = 1,
    HUNDRED = 2
};


/**
 * @brief 旋转编码器类，使用 ESP32Encoder 库实现
 * @author skyswordx
 * @details 提供了累加计数和快慢模式接口
 */
class encoder_handle_t
{
    public:
        ESP32Encoder encoder;
        gpio_num_t pin_A;
        gpio_num_t pin_B;


        int64_t total_count;
        int64_t single_count;
        double read_count_accum_clear();
        void set_count(int64_t value);

        encoder_mode_t mode = SINGLE;


        encoder_handle_t(int pin_A, int pin_B);
    private:
        /* 没想好 */
};



#endif // OUR_ENCODER