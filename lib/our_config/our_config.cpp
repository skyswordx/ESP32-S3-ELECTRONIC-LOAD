#include "our_config.hpp"
#include "our_lvgl_interaction.h"
#include "our_encoder.hpp"
#include "our_sensors.hpp"
#include "our_adc.hpp"
#include "our_button.hpp"


/******** Encoder Setup *****/

encoder_handle_t encoder1(ENCODER_1_PIN_A, ENCODER_1_PIN_B);

/******** INA226 & MCP4725 Setup **********/
INA226 INA226_device(0x40); // INA226 电流传感器
MCP4725 MCP4725_device(0x60); // MCP4725 DAC 芯片


/******** ADC1 Setup *********/
ADC_channel_handler_t MY_ADC_GPIO6(ADC1_CHANNEL_5, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 64, ADC_UNIT_1);

/******** GPIO-button Setup *********/

GPIO_button_handler_t button1(GPIO_NUM_2); // GPIO 2 作为按键输入引脚
GPIO_button_handler_t button2(GPIO_NUM_1);
GPIO_button_handler_t button3(GPIO_NUM_7);
GPIO_button_handler_t button4(GPIO_NUM_15); 

GPIO_button_handler_t encoder1_button(GPIO_NUM_16); // 旋转编码器的按键引脚

/******** LVGL-SetUP *******/
// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

// GUI guider
lv_ui guider_ui;

/******** RTOS-SetUP *******/
TaskHandle_t lvgl_task_handle; // LVGL 任务结构句柄
SemaphoreHandle_t gui_xMutex;  // gui 互斥锁句柄，LVGL 线程不安全，需要加锁
SemaphoreHandle_t button_xBinarySemaphore; // 按键二值信号量
SemaphoreHandle_t voltage_protection_xBinarySemaphore; // 过压保护二值信号量

// GUI 更新使用的消息队列
QueueHandle_t sensor_queue_handle; // 消息队列句柄
const int queue_element_size = 10; // 消息队列元素大小
QueueHandle_t button_queue_handle; // 按键消息队列句柄

