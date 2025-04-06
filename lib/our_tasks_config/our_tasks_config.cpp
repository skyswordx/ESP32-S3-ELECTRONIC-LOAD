#include "our_tasks_config.hpp"
#include "our_lvgl_interaction.h"
#include "our_encoder.hpp"
#include "our_adc.hpp"
#include "our_button.hpp"


/*************************************** Encoder Setup *****************************/

encoder_handle_t encoder1(ENCODER_1_PIN_A, ENCODER_1_PIN_B);

/*********************************** INA226 & MCP4725 Setup *************************/
INA226 INA226_device(0x40); // INA226 电流传感器
MCP4725 MCP4725_device(0x60); // MCP4725 DAC 芯片


/***************************************** ADC1 Setup *******************************/
ADC_channel_handler_t MY_ADC_GPIO6(ADC1_CHANNEL_5, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 64, ADC_UNIT_1);

/************************************* GPIO-button Setup ****************************/

GPIO_button_handler_t button1(GPIO_NUM_2); // GPIO 2 作为按键输入引脚
GPIO_button_handler_t button2(GPIO_NUM_1);
GPIO_button_handler_t button3(GPIO_NUM_7);
GPIO_button_handler_t button4(GPIO_NUM_15); 

GPIO_button_handler_t encoder1_button(GPIO_NUM_16); // 旋转编码器的按键引脚

/************************************* LVGL-SetUP ***********************************/
// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

// GUI guider
lv_ui guider_ui;

/************************************* RTOS-SetUP ***********************************/
TaskHandle_t lvgl_task_handle; // LVGL 任务结构句柄
SemaphoreHandle_t gui_xMutex;  // gui 互斥锁句柄，LVGL 线程不安全，需要加锁
SemaphoreHandle_t button_xBinarySemaphore; // 按键二值信号量
SemaphoreHandle_t voltage_protection_xBinarySemaphore; // 过压保护二值信号量

// GUI 更新使用的消息队列
QueueHandle_t sensor_queue_handle; // 消息队列句柄
const int queue_element_size = 10; // 消息队列元素大小
QueueHandle_t button_queue_handle; // 按键消息队列句柄

/************************************** tasks ***************************************/
/**
 * @brief 模拟获得传感器数据的任务函数
 * @author skyswordx
 */
void get_dummy_sensor_data_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    // printf("\n[get_sensor_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));

    // 模拟传感器数据
    msg.device_id = DEVICE_DUMMY_SENSOR;
    msg.value = 4.0 + (rand() % 100) / 100.0;
   

    int return_value = xQueueSend(sensor_queue_handle, (void *)&msg, 0);
    if (return_value == pdTRUE) {
      // printf("\n[get_sensor_data_task] sent message  to the queue successfully\n");
    } else if (return_value == errQUEUE_FULL) {
      // printf("\n[get_sensor_data_task] failed to send message to queue, queue is full\n");
    } else {
      // printf("\n[get_sensor_data_task] failed to send message to queue\n");
    }

    vTaskDelay( 1000 );
  }
}

/**
 * @brief 获取 INA226 数据的任务函数
 * @author skyswordx
 * @details 该任务会一直运行，获取 INA226 的数据并发送到消息队列中
 */
void get_ina226_data_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    // printf("\n[get_ina226_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));

    // printf("\n[get_ina226_data_task] now_time: %d", millis());
    msg.device_id = DEVICE_INA226;
  
    float measure_current_mA = INA226_device.getCurrent_mA();
    float measure_voltage_V = INA226_device.getBusVoltage();
    float measure_power_W = INA226_device.getPower();
    float measure_resistance_Kohm = abs((measure_voltage_V )/ (measure_current_mA ));

    // 负载调整率？
            /*
              measure_current = 0A ->  记录 bus_V0
              measure_current = 1A ->  记录 bus_V1
              rate = (bus_V1 - bus_V0) / bus_V0
             */
    static float bus_V0 = 0.0;
    static float bus_V1 = 0.0;
    if ( abs((abs(measure_current_mA) - 0.0) < 0.1) ){
      // 记录 bus_V0
      bus_V0 = measure_voltage_V;
    } else if ( abs((abs(measure_current_mA)*1e3 - 1.0) < 0.1) ){
      // 记录 bus_V1
      bus_V1 = measure_voltage_V;
    }

    float rate = (bus_V1 - bus_V0) / bus_V0;

    msg.device_data.value1 = measure_current_mA;
    msg.device_data.value2 = measure_voltage_V;
    msg.device_data.value3 = measure_power_W;
    msg.device_data.value4 = measure_resistance_Kohm;
    msg.device_data.value5 = rate;

    // printf("\n[get_ina226_data_task] INA226: %.3f mA, %.3f V, %.3f W, %.3f KOhm, %.3f", measure_current_mA, measure_voltage_V, measure_power_W, measure_resistance_Kohm, rate);
    printf("[get_ina226_data_task] INA226 V/mA: %.3f,%.3f\n", measure_voltage_V, measure_current_mA); 

    // 检查 INA226 电压是否超过警告值，如果超过则进行过压保护
    if(measure_voltage_V >= WARNING_VOLTAGE){
      // 电压过高，警告
      msg.value = WARNING_VOLTAGE; // LVGL 读取到这个数据就会弹出警告

      // 这里触发过压之后的保护程序
      // xSemaphoreGive(voltage_protection_xBinarySemaphore);
    }

    int return_value = xQueueSend(sensor_queue_handle, (void *)&msg, 0);
    if (return_value == pdTRUE) {
      // printf("\n[get_ina226_data_task] sent message  to the queue successfully\n");
    } else if (return_value == errQUEUE_FULL) {
      // printf("\n[get_ina226_data_task] failed to send message to queue, queue is full\n");
    } else {
      // printf("\n[get_ina226_data_task] failed to send message to queue\n");
    }

    vTaskDelay( 1000 );
  }
}

/**
 * @brief 过压保护任务函数
 * @author skyswordx
 * @details 该任务会一直运行，等待过压保护信号量
 *          过压保护信号量由 get_ina226_data_task 任务触发
 */
void voltage_protection_task(void *pvParameters){
    while(1){
    
      printf("\n[voltage_protection_task] waiting");
      xSemaphoreTake(voltage_protection_xBinarySemaphore, portMAX_DELAY); // 总是保持阻塞等待二值信号量
      
      printf("\n[voltage_protection_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
  
      // 执行过压保护任务，关闭 DAC 输出，让 MOSFET 关断
      MCP4725_device.setVoltage(0.0); // 设置输出电压为 0V
      printf("\n[voltage_protection_task] DAC output voltage set to 0V");
    }
  } 

  
/**
 * @brief ADC1 读取任务
 * @author skyswordx
 * @details 该任务用于读取 ADC1 的电压值，并将其发送到消息队列中
 * @param pvParameters 任务参数
 * @return void* 任务返回值
 */
void ADC1_read_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    // printf("\n[ADC1_read_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    
    msg.device_id = DEVICE_ADC1;

    float adc_value_3 = MY_ADC_GPIO6.get_ADC1_voltage_average_mV();

    float adc_value_average = adc_value_3 / 1.0;

    msg.value = adc_value_average;

    printf("\n[ADC1_read_task] ADC1_CHANNEL_5: %.3f", adc_value_3);
    
    int return_value = xQueueSend(sensor_queue_handle, (void *)&msg, 0);
    if (return_value == pdTRUE) {
      // printf("\n[ADC1_read_task] sent message  to the queue successfully\n");
    } else if (return_value == errQUEUE_FULL) {
      // printf("\n[ADC1_read_task] failed to send message to queue, queue is full\n");
    } else {
      // printf("\n[ADC1_read_task] failed to send message to queue\n");
    }

    vTaskDelay( 1000 );
  }
}

/**
 * @brief 按键中断辅助处理任务
 * @author skyswordx
 * @details 该任务日常被阻塞在二值信号量上，必须等待中断函数释放信号量才会被唤醒
 *          该任务的优先级必须高于其他函数的优先级，否则会导致中断处理逻辑无法正常工作
 */
void button_handler_task(void *pvParameters){

    BaseType_t button_down = pdFALSE;
    BaseType_t button_up = pdFALSE;
    BaseType_t short_press = pdFALSE;
    BaseType_t long_press = pdFALSE;
    int time_ms = 0;
  
    gpio_num_t GPIO_PIN;
    while(1){
      // printf("\n[button_handler_task] waiting");
      xSemaphoreTake(button_xBinarySemaphore, portMAX_DELAY); // 总是保持阻塞等待二值信号量
  
  
      // printf("\n[button_handler_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
      if ( button_queue_handle != NULL){ // 检查消息队列是否创建成功
        if (xQueueReceive(button_queue_handle, &GPIO_PIN, portMAX_DELAY) == pdTRUE) {
          // printf("\n[button_handler_task] received message from queue");
  
          /* 收到中断数据，开始检测 */
  
          // 利用 RTOS 系统时间片轮询和标志位来进行长短按检测
          button_down = pdTRUE;
          time_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
  
          // 等待按键释放
          while (gpio_get_level(GPIO_PIN) == LOW) {
            vTaskDelay(10 / portTICK_PERIOD_MS); // 延时 10ms，避免频繁轮询
          }
  
          button_up = pdTRUE;
          time_ms = (xTaskGetTickCount() * portTICK_PERIOD_MS) - time_ms;
      
  
          // 使用 while 循环来阻塞检测
          // time_ms = millis();
          // while (gpio_get_level(GPIO_PIN) == LOW) {
          //   // printf("\n[button_handler_task] button pressed");
          //   button_down = pdTRUE;
  
          //   if (millis() - time_ms > 10000) { // 10s
          //     break; // 超过 10s 就退出，避免一直阻塞
          //   }
          // }
          // button_up = pdTRUE;
      
          // time_ms = millis() - time_ms;
  
  
          if (button_down == pdTRUE && button_up == pdTRUE && time_ms > 0) {
            // printf("\n[button_handler_task] button pressed for %d ms", time);
            button_down = pdFALSE;
            button_up = pdFALSE;
  
            if (time_ms < 1000) { // 1s
              short_press = pdTRUE;
              printf("\n[button_handler_task] GPIO %d short press", GPIO_PIN);
  
            } else {
              long_press = pdTRUE;
              printf("\n[button_handler_task] GPIO %d long press", GPIO_PIN);
            }
  
            // 这里模拟过压保护的触发
            // xSemaphoreGive(voltage_protection_xBinarySemaphore);
            printf("\n[button_handler_task] duration: %d ms", time_ms);
            if (GPIO_PIN == encoder1_button.pin){
              // 按下编码器俺家之后切换模式
              if (encoder1.mode == QUAD) {
                encoder1.mode = SINGLE;
                printf("\n[button_handler_task] encoder1 mode changed to SINGLE");
              } else {
                encoder1.mode = QUAD;
                printf("\n[button_handler_task] encoder1 mode changed to QUAD");
              }
            }else if(GPIO_PIN == button1.pin){
              
            }else if(GPIO_PIN == button2.pin){
              
            }else if(GPIO_PIN == button3.pin){
              
            }else if(GPIO_PIN == button4.pin){
              /* 页面切换 */
              // 从 chart -> main sw
              // ui_load_scr_animation(&guider_ui, &guider_ui.main_page, guider_ui.main_page_del, &guider_ui.chart_page_del, setup_scr_main_page, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
            }
          }
        } else {
          // printf("\n[button_handler_task] failed to receive messag e from queue\n");
        }
      }
    }
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

