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

/************************************ 条件编译选项 ***********************************/
#define USE_PID_CONTROLLER 1
//     #define USE_CURRENT_OPEN_LOOP_TEST 1
#define USE_IIC_DEVICE 1 // 是否使用 IIC 设备
    #define USE_INA226 1
    #define USE_MCP4725 1
    //#define USE_INA226_MODULE 1 // 是否使用 INA226 模块
    #define USE_LOAD_RATE_TESTING


#define USE_LCD_DISPLAY 1  // 是否使用 LCD 显示屏
#define USE_ENCODER1 1 // 是否使用编码器 1
#define USE_ADC1 1 // 是否使用 ADC1 通道 5 读取电压值

#define USE_BUTTON 1 // 是否使用按键
    #define USE_BUTTON1 1 // 是否使用按键 1
    #define USE_BUTTON2 1 // 是否使用按键 2
    #define USE_BUTTON3 1 // 是否使用按键 3
    #define USE_BUTTON4 1 // 是否使用按键 4


#define USE_VOLTAGE_PROTECTION 1 // 是否使用过压保护功能
// #define USE_DUMMY_SENSOR 1 // 是否使用虚拟传感器数据


/*********************************** ESP32S3 Setup **********************************/
#include <Arduino.h>
#include "our_queque.hpp"

extern BaseType_t debug_flag1;
extern BaseType_t debug_flag2;
extern SemaphoreHandle_t startup_xBinarySemaphore; // 启动二值信号量

/*********************************** Current PID Setup ***********************************/
// 包含自定义的 PID 控制器类和 VOFA 下位机
#ifdef USE_PID_CONTROLLER
    #include "our_pid_controller.hpp"
    // #include "our_vofa_debuger.hpp"

    extern PID_controller_t<double> current_ctrl;

    #define DAC_OUTPUT_V_MAX 5.0 
    #define DAC_OUTPUT_V_MIN 0.0
    // #define CURRENT_TASK_KP 0.0022226816 // 电流控制器比例系数
    // #define CURRENT_TASK_KI 0.00 // 电流控制器积分系数
            
    #define CURRENT_TASK_KP 0.0005 // 电流控制器比例系数
    #define CURRENT_TASK_KI 0.0000 // 电流控制器积分系数
    #define CURRENT_TASK_KD 0.000 // 电流控制器微分系数
            
    void set_current_task(void *pvParameters); // 设置电流任务函数
            
    #ifdef USE_CURRENT_OPEN_LOOP_TEST
        void open_loop_data_collection_task(void *pvParameters); // 开环数据采集任务函数
                    
            
        #define OPEN_LOOP_TEST_LENGTH  50 // 开环测试数据长度
            
        extern SemaphoreHandle_t open_loop_test_xBinarySemaphore; // 开环测试二值信号量
        extern double pv_data[OPEN_LOOP_TEST_LENGTH]; // 开环测试数据
        extern double op_data[OPEN_LOOP_TEST_LENGTH]; // 开环测试数据
                    
        #define OPEN_LOOP_T1_ms  10
        #define OPEN_LOOP_T2_ms  40
            
        #define USE_BUTTON 1
        #define USE_BUTTON4 1 // DAC OUTPUT MAX
        #define USE_BUTTON3 1 // DAC OUTPUT MIN
    #endif // USE_CURRENT_OPEN_LOOP_TEST 
            

#endif

/*********************************** Current Measurement Setup *********************/
#ifdef USE_IIC_DEVICE // 启用电子负载测试
    #ifdef USE_INA226_MODULE
        #define RSHUNT 0.010 // 10mΩ 电阻
        #define CURRENT_LSB_mA 0.05 // 电流 LSB 值
        #define CURRENT_OFFSET_mA -0.525 // 校准程序串口输出的电流偏置
        #define BUS_V_DMM 6.013 // DMM 数字万用表测量的电压
        #define BUS_V_SERIAL 6.109 // 校准程序测量的电压
        #define BUS_V_SCALING_E4 (10000 / BUS_V_SERIAL * BUS_V_DMM) // 电压缩放系数
    #else // 启用功率板
        #define RSHUNT 0.020 // 20mΩ 电阻
        #define CURRENT_LSB_mA 0.05 // 电流 LSB 值
        #define CURRENT_OFFSET_mA -0.525 // 校准程序串口输出的电流偏置
        #define BUS_V_DMM 6.013 // DMM 数字万用表测量的电压
        #define BUS_V_SERIAL 6.109 // 校准程序测量的电压
        #define BUS_V_SCALING_E4 (10000 / BUS_V_SERIAL * BUS_V_DMM) // 电压缩放系数
    #endif // USE_INA226_MODULE

    #ifdef USE_LOAD_RATE_TESTING
        // #define DAC_BIAS_CURRENT_mA 40  
        #define TESING_MAX_CURRENT_A 1.00
        #define TESING_MIN_CURRENT_A 0.1
        #define THRESHOLD_A 0.5
        extern BaseType_t testing_load_flag;
        double from_set_current2voltage_V( double set_current_A); // 在旋转编码器任务 (get_encoder1_data_task) 中使用

        extern SemaphoreHandle_t  load_testing_xBinarySemaphore;
        void get_load_changing_rate_task(void *pvParameters); // 测量负载调整率任务函数

    #endif 
#endif // USE_IIC_DEVICE

/****************************** OUTPUT CAlibration *******************************/

#ifdef USE_OUTPUT_CALIBRATION // 启用输出校准
     void output_data_collection_task(void *pvParameters); // 输出校准采集任务函数
        
    #define OUTPUT_CALIBRATION  50 // 输出校准测试数据长度

    extern SemaphoreHandle_t output_calibration_xBinarySemaphore; // 输出测试二值信号量
    extern double targ_data[OUTPUT_CALIBRATION]; // 输出测试数据-目标值
    extern double real_data[OUTPUT_CALIBRATION]; // 输出测试数据-实际值
        
    #define OUTPUT_CALIB_TIME_ms  10000

    #define USE_BUTTON 1
    #define USE_BUTTON4 1 // DAC OUTPUT MAX
    #define USE_BUTTON3 1 // DAC OUTPUT MIN
#endif // USE_OUTPUT_CALIBRATION 


/*************************************** Safty Setup ********************************/
extern uint8_t Warning_Voltage; // 过压保护阈值
extern bool circuit_enabled; // 电路开关状态（true=开启，false=关闭）

/*************************************** Encoder Setup *****************************/
#ifdef USE_ENCODER1
    #include "our_encoder.hpp"
    #define ENCODER_1_PIN_A 18
    #define ENCODER_1_PIN_B 17
    extern TaskHandle_t encoder1_task_handle;
    extern encoder_handle_t encoder1; // 旋转编码器对象

    void get_encoder1_data_task(void *pvParameters); // 获取编码器数据的任务函数
    void set_encoder_current_setpoint(double setpoint_mA); // 设置编码器任务的当前电流设定值
    double get_encoder_current_setpoint(); // 获取编码器任务的当前电流设定值
#endif // USE_ENCODER1

/*********************************** INA226 & MCP4725 Setup *************************/
#ifdef USE_IIC_DEVICE
    #include <Wire.h>
    // 第一条 IIC 总线 Wire 的引脚
    #define IIC_SDA 4
    #define IIC_SCL 5

    
    #ifdef  USE_INA226_MODULE
        #include "INA226.h"
        extern INA226 INA226_device; // INA226 电流传感器对象

        extern const double current_calibration_A; // 电流校准系数A
        extern const double current_calibration_B; // 电流校准系数B
        extern const double voltage_calibration_A; // 电压校准系数A
        extern const double voltage_calibration_B; // 电压校准系数B
        void get_ina226_data_task(void *pvParameters); // 获取 INA226 数据的任务函数
    #else
        #include "INA226.h"
        extern INA226 INA226_device; // INA226 电流传感器对象

        extern const double current_calibration_A; // 电流校准系数A
        extern const double current_calibration_B; // 电流校准系数B
        extern const double voltage_calibration_A; // 电压校准系数A
        extern const double voltage_calibration_B; // 电压校准系数B
        void get_ina226_data_task(void *pvParameters); // 获取 INA226 数据的任务函数
    #endif

    #ifdef  USE_MCP4725
        #include "MCP4725.h"
        extern MCP4725 MCP4725_device; // MCP4725 DAC 芯片对象

        #ifdef USE_VOLTAGE_PROTECTION
            extern BaseType_t over_voltage_protection_flag; // 过压保护标志位
            extern SemaphoreHandle_t over_voltage_protection_xBinarySemaphore; // 过压保护二值信号量
            void over_voltage_protection_task(void *pvParameters); // 过压保护任务函数
        #endif // USE_VOLTAGE_PROTECTION
    #endif // USE_MCP4725

    // 添加I2C互斥锁保护，防止并发访问冲突
    extern SemaphoreHandle_t i2c_device_mutex;

    // 线程安全的INA226读取函数
    double safe_read_ina226_current_mA();
    double safe_read_ina226_voltage_V();
    double safe_read_ina226_power_W();
    void init_pid_controller();


#endif // USE_IIC_DEVICE


/***************************************** ADC1 Setup *******************************/
#ifdef USE_ADC1
    #include "our_adc.hpp"
    extern ADC_channel_handler_t MY_ADC_GPIO6; // ADC1 通道 5 对应 GPIO 6
    void ADC1_read_task(void *pvParameters); // ADC1 读取任务
#endif // USE_ADC1
/************************************* GPIO-button Setup ****************************/
#ifdef USE_BUTTON
#include "our_button.hpp"
    #ifdef USE_BUTTON1
        extern GPIO_button_handler_t button1; 
    #endif // USE_BUTTON1

    #ifdef USE_BUTTON2
        extern GPIO_button_handler_t button2;
    #endif // USE_BUTTON2
    
    #ifdef USE_BUTTON3
        extern GPIO_button_handler_t button3;
    #endif // USE_BUTTON3
    
    #ifdef USE_BUTTON4
        extern GPIO_button_handler_t button4;
    #endif // USE_BUTTON4
    
    #ifdef USE_ENCODER1
        extern GPIO_button_handler_t encoder1_button; // 旋转编码器的按键引脚
    #endif // USE_ENCODER1

    extern SemaphoreHandle_t button_xBinarySemaphore; // 按键二值信号量
    void button_handler_task(void *pvParameters); // 按键处理任务
#endif // USE_BUTTON

/************************************* LVGL-SetUP ***********************************/
#include "our_lvgl_interaction.h"
#ifdef USE_LCD_DISPLAY
    #include <lvgl.h>
    #include <SPI.h>
    #include <TFT_eSPI.h>
    #include "demos/lv_demos.h"

    // 包含显示接口和触摸接口绑定部分
    #include "lv_port_disp.h"
    #include "lv_port_indev.h"

    // 包含使用 GUI guider 生成的 GUI 部分
    #include "gui_guider.h"
    extern TFT_eSPI tft; // TFT_eSPI 对象
    extern lv_ui guider_ui; // GUI guider 对象

    extern TaskHandle_t lvgl_task_handle; // LVGL 任务结构句柄
    extern SemaphoreHandle_t gui_xMutex;  // gui 互斥锁句柄，LVGL 线程不安全，需要加锁
#endif // USE_LCD_DISPLAY
/************************************* RTOS-SetUP ***********************************/




/************************************** tasks ***************************************/
#ifdef USE_DUMMY_SENSOR
    void get_dummy_sensor_data_task(void *pvParameters); // 模拟获得传感器数据的任务函数
#endif // USE_DUMMY_SENSOR

/*********************************** Load Mode Setup *********************************/
/**
 * @brief 负载模式枚举
 * @author Triwalt
 * @details 支持三种负载模式：恒流、恒功率、恒阻
 */
enum LoadMode {
    CONSTANT_CURRENT = 0,   // 恒流模式
    CONSTANT_POWER = 1,     // 恒功率模式  
    CONSTANT_RESISTANCE = 2  // 恒阻模式
};

// 全局变量声明
extern LoadMode current_load_mode;       // 当前负载模式
extern double load_setpoint_current_mA;  // 恒流模式设定值(mA)
extern double load_setpoint_power_W;     // 恒功率模式设定值(W)
extern double load_setpoint_resistance_ohm; // 恒阻模式设定值(Ω)

// 负载模式相关函数声明
void switch_load_mode();                 // 切换负载模式
void update_load_mode_display();         // 更新负载模式显示
double calculate_target_current_for_mode(); // 根据当前模式计算目标电流

#endif // OUR_CONFIG_HPP