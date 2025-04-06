/**
 * @file our_config.hpp
 * @brief ESP32S3 配置文件，包含了工程用到的所有配置和全局变量的声明
 * @author skyswordx
 * @details 全局变量在源文件中定义，头文件中声明 extern 这样可以避免重复定义
 */

#ifndef OUR_CONFIG_HPP
#define OUR_CONFIG_HPP

/**
 * 若使用 2.4 寸屏幕，请在
 *  - TFT_eSPI/User_Setup_Select.h 中包含 ILI9341.h
 *  - Lib/lvg_port/lv_port_disp.cpp 中修改尺寸为 240x320
 *  - Lib/lvg_port/lv_port_indev.cpp 中修改校准数据
 * 
 * 若使用 3.5 寸屏幕，请在
 * - TFT_eSPI/User_Setup_Select.h 中包含 ILI9488.h
 * - Lib/lvg_port/lv_port_disp.cpp 中修改尺寸为 320x480
 * - Lib/lvg_port/lv_port_indev.cpp 中修改校准数据
 */

/********** 条件编译选项 **********/
#define USE_INA226_MODULE 1

/********** ESP32S3 Setup **********/
#include <Arduino.h>
#include <lvgl.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "demos/lv_demos.h"

// 包含显示接口和触摸接口绑定部分
#include "lv_port_disp.h"
#include "lv_port_indev.h"

// 包含使用 GUI guider 生成的 GUI 部分
#include "gui_guider.h"

// 包含自定义的编码器类（依赖 ESP32Encoder 库）
#include "our_encoder.hpp"

// 包含自定义的 PID 控制器类和 VOFA 下位机
#include "our_pid_controller.hpp"
#include "our_vofa_debuger.hpp"

/********** Encoder Setup *****/
#define ENCODER_1_PIN_A 18
#define ENCODER_1_PIN_B 17
extern encoder_handle_t encoder1; // 旋转编码器对象


/********** INA226 & MCP4725 Setup **********/
#include <Wire.h>
// 第一条 IIC 总线 Wire 的引脚
#define IIC_SDA 4
#define IIC_SCL 5

#include "INA226.h"
extern INA226 INA226_device; // INA226 电流传感器对象
#include "MCP4725.h"
extern MCP4725 MCP4725_device; // MCP4725 DAC 芯片对象
#define WARNING_VOLTAGE 18 // 触发过压保护的电压


/********** ADC1 Setup **********/
#include "our_adc.hpp"
extern ADC_channel_handler_t MY_ADC_GPIO6; // ADC1 通道 5 对应 GPIO 6

/********** GPIO-button Setup **********/
#include "our_button.hpp"
extern GPIO_button_handler_t button1; 
extern GPIO_button_handler_t button2; 
extern GPIO_button_handler_t button3;
extern GPIO_button_handler_t button4;

extern GPIO_button_handler_t encoder1_button; // 旋转编码器的按键引脚

/********** LVGL-SetUP **********/
extern TFT_eSPI tft; // TFT_eSPI 对象
extern lv_ui guider_ui; // GUI guider 对象

/********** RTOS-SetUP **********/
extern TaskHandle_t lvgl_task_handle; // LVGL 任务结构句柄
extern SemaphoreHandle_t gui_xMutex;  // gui 互斥锁句柄，LVGL 线程不安全，需要加锁
extern SemaphoreHandle_t button_xBinarySemaphore; // 按键二值信号量
extern SemaphoreHandle_t voltage_protection_xBinarySemaphore; // 过压保护二值信号量

// GUI 更新使用的消息队列
extern QueueHandle_t sensor_queue_handle; // 消息队列句柄
extern const int queue_element_size; // 消息队列元素大小
extern QueueHandle_t button_queue_handle; // 按键消息队列句柄

// 定义一个枚举类型，表示发送消息的源设备 id
enum device_id_t {
    DEVICE_DUMMY_SENSOR = 0,
    DEVICE_ENCODER = 1,
    DEVICE_INA226 = 2,
    DEVICE_ADC1 = 3,
    DEVICE_UNKNOWN = 99
  };
  
  typedef struct {
    float value1; // 传感器数据
    float value2; // 传感器数据
    float value3; // 传感器数据
    float value4; // 传感器数据
    float value5; // 传感器数据
  } device_data_t;
  
  typedef struct {
    device_id_t device_id; // 设备ID
    float value; // 存放的数据（简单设置为一个值，后期有需求再改为专门的结构体
    device_data_t device_data; // INA226 传感器数据
  } message_t;



#endif // OUR_CONFIG_HPP