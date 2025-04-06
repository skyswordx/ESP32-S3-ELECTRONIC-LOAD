/**
 * @file our_encoder.cpp
 * @brief 旋转编码器类，使用 ESP32Encoder 库实现，并且提供了累加计数和快慢模式接口
 * @author skyswordx
 */

#include "our_encoder.hpp"
#include "our_config.hpp"

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

/**
 * @brief 获取编码器数据的任务函数
 * @author skyswordx
 * @param pvParameters 任务参数
 * @details 仅在 setup 调用一次即可，该任务会一直运行，获取编码器的数据并发送到消息队列中
 */
void get_encoder1_data_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    // printf("\n[get_encoder1_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    
    // 获取旋转编码器数据
    msg.device_id = DEVICE_ENCODER;
    msg.value = encoder1.read_count_accum_clear();


    // printf("\n[get_encoder1_data_task] encoder1 count: %lld", count);
    // printf("\n[get_encoder1_data_task] encoder1 value: %.3f", msg.value);

    int return_value = xQueueSend(sensor_queue_handle, (void *)&msg, 0);
    if (return_value == pdTRUE) {
      // printf("\n[get_encoder1_data_task] sent message  to the queue successfully\n");
    } else if (return_value == errQUEUE_FULL) {
      // printf("\n[get_encoder1_data_task] failed to send message to queue, queue is full\n");
    } else {
      // printf("\n[get_encoder1_data_task] failed to send message to queue\n");
    }

    vTaskDelay( 1000 );
  }
}


