#ifndef OUR_ENCODER
#define OUR_ENCODER 

#include <ESP32Encoder.h>
#include <Arduino.h>

enum encoder_mode_t {
    QUAD = 0,
    SINGLE = 1
};

class encoder_handle_t
{
    public:
        ESP32Encoder encoder;
        gpio_num_t pin_A;
        gpio_num_t pin_B;


        int64_t total_count;
        int64_t single_count;
        int64_t read_count_accum_clear();

        encoder_mode_t mode = SINGLE;


        encoder_handle_t(int pin_A, int pin_B);
    private:
        /* 没想好 */
};

#endif // OUR_ENCODER