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
        int pin_A;
        int pin_B;
        int pin_S;

        int64_t total_count;
        int64_t single_count;
        int64_t read_count_accum_clear();

        encoder_mode_t mode = SINGLE;
        bool is_pressed();
        void set_mode(encoder_mode_t mode);
        void check_press_to_change_mode();

        encoder_handle_t(int pin_A, int pin_B, int pin_S);
    private:
        /* 没想好 */
};

#endif // OUR_ENCODER