/**
 * @file our_adc.h
 * @brief 自定义 ADC 类头文件
 * @author skyswordx 
 * @details 该文件基于 ESP-IDF 接口，包含了自定义 ADC 类的定义和函数声明
 */
#ifndef OUR_ADC_H
#define OUR_ADC_H


#include <Arduino.h>

// PIO 找库能力太差了
// #include <lvgl.h>
// #include <SPI.h>
// #include <TFT_eSPI.h>
// #include "lv_port_disp.h"
// #include "lv_port_indev.h"

// 包含 ESP-IDF ADC 相关的头文件 
#include "esp_adc_cal.h"
#include "driver/adc.h"

    // ADC1_CHANNEL_0 = 0, /*!< ADC1 channel 0 is GPIO1  */
    // ADC1_CHANNEL_1,     /*!< ADC1 channel 1 is GPIO2  */
    // ADC1_CHANNEL_2,     /*!< ADC1 channel 2 is GPIO3  */
    // ADC1_CHANNEL_3,     /*!< ADC1 channel 3 is GPIO4  */
    // ADC1_CHANNEL_4,     /*!< ADC1 channel 4 is GPIO5  */
    // ADC1_CHANNEL_5,     /*!< ADC1 channel 5 is GPIO6  */
    // ADC1_CHANNEL_6,     /*!< ADC1 channel 6 is GPIO7  */
    // ADC1_CHANNEL_7,     /*!< ADC1 channel 7 is GPIO8  */
    // ADC1_CHANNEL_8,     /*!< ADC1 channel 8 is GPIO9  */
    // ADC1_CHANNEL_9,     /*!< ADC1 channel 9 is GPIO10 */

    // S3 的 ADC2 端口是错误的，IDF-1776，暂时不要使用
    // ADC2_CHANNEL_0 = 0, /*!< ADC2 channel 0 is GPIO4  (ESP32), GPIO11 (ESP32-S2) */
    // ADC2_CHANNEL_1,     /*!< ADC2 channel 1 is GPIO0  (ESP32), GPIO12 (ESP32-S2) */
    // ADC2_CHANNEL_2,     /*!< ADC2 channel 2 is GPIO2  (ESP32), GPIO13 (ESP32-S2) */
    // ADC2_CHANNEL_3,     /*!< ADC2 channel 3 is GPIO15 (ESP32), GPIO14 (ESP32-S2) */
    // ADC2_CHANNEL_4,     /*!< ADC2 channel 4 is GPIO13 (ESP32), GPIO15 (ESP32-S2) */
    // ADC2_CHANNEL_5,     /*!< ADC2 channel 5 is GPIO12 (ESP32), GPIO16 (ESP32-S2) */
    // ADC2_CHANNEL_6,     /*!< ADC2 channel 6 is GPIO14 (ESP32), GPIO17 (ESP32-S2) */
    // ADC2_CHANNEL_7,     /*!< ADC2 channel 7 is GPIO27 (ESP32), GPIO18 (ESP32-S2) */
    // ADC2_CHANNEL_8,     /*!< ADC2 channel 8 is GPIO25 (ESP32), GPIO19 (ESP32-S2) */
    // ADC2_CHANNEL_9,     /*!< ADC2 channel 9 is GPIO26 (ESP32), GPIO20 (ESP32-S2) */

# define DEFAULT_VREF    1100        // can use adc2_vref_to_gpio() to obtain a better estimate
class ADC_channel_handler_t {

    private:
        /********** ADC 初始化成员变量 **********/
        // 选定初始化不同的 ADC (ADC1 或 ADC2) 和对应的第几个通道
        adc1_channel_t adc1_channel;
        adc2_channel_t adc2_channel; // 如果使用了 ADC2 就无法使用 DMA 模式
        
        // 配置要初始化的 ADC 单元所对应的工作模式，用于填充 adc_digi_pattern_config_t 结构体
        adc_digi_pattern_config_t adc_digi_pattern; 
        adc_atten_t atten; /* 设置 ADC 衰减 */
        adc_unit_t unit; /* 设置 ADC 单元 */
        adc_bits_width_t width; /* 设置 ADC 位宽 */

        // 定义 ADC 的初始化配置结构体
        adc_digi_configuration_t adc_digi_config;

        // 定义 ADC 校准结构体
        esp_adc_cal_characteristics_t *adc_chars;
        esp_adc_cal_value_t val_type = ESP_ADC_CAL_VAL_EFUSE_TP;

    public:
        
        uint32_t num_samples; // 采样次数，后期可以更改
        uint32_t adc_raw_value; // 保存 ADC 的值
        float adc_voltage; // 保存 ADC 转换后的电压值

        /***************** ADC 成员函数 *****************/
        /* 构造函数及其重载，适配 ADC1 和 ADC2 的初始化 */
        ADC_channel_handler_t(adc1_channel_t adc1_channel, adc_atten_t atten, adc_bits_width_t width, uint32_t num_samples, adc_unit_t unit);
        ADC_channel_handler_t(adc2_channel_t adc2_channel, adc_atten_t atten, adc_bits_width_t width, uint32_t num_samples); 
        
        
        uint32_t get_ADC1_raw_average();
        float get_ADC1_voltage_average_mV(); // 获取 ADC1 的电压值
        uint32_t get_ADC2_raw_average();
        float get_ADC2_voltage_average_mV(); // 获取 ADC2 的电压值
        
        
};

#endif