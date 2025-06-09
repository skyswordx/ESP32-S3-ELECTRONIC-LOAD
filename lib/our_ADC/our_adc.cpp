/**
 * @file our_adc.h
 * @brief 自定义 ADC 类头文件
 * @author skyswordx 
 * @details 该文件基于 ESP-IDF 接口，包含了自定义 ADC 类的定义和函数声明
 */
#include "our_adc.hpp"
#include <math.h>  // 添加数学函数支持


/**
 * @brief 自定义 ADC 类的构造函数
 * @author skyswordx
 * @details 该函数用于初始化 ADC 的配置结构体和校准结构体
 */
ADC_channel_handler_t::ADC_channel_handler_t(adc1_channel_t adc1_channel, adc_atten_t atten, adc_bits_width_t width, uint32_t num_samples, adc_unit_t unit) {
            
    /* 初始化采样设置 */
    this->adc1_channel = adc1_channel;
    this->atten = atten;
    this->width = width;
    this->num_samples = num_samples;
    this->unit = unit;

    this->adc_digi_pattern.channel = adc1_channel;
    this->adc_digi_pattern.atten = atten;
    this->adc_digi_pattern.unit = unit;
    this->adc_digi_pattern.bit_width = width;

    this->adc_digi_config.adc_pattern = &adc_digi_pattern;
    adc_digi_controller_configure(&adc_digi_config);

    /* 创建校准方案 */
    this->adc_chars= (esp_adc_cal_characteristics_t *) malloc(sizeof(esp_adc_cal_characteristics_t));
    this->val_type = esp_adc_cal_characterize(this->unit, this->atten, this->width, DEFAULT_VREF, this->adc_chars);
}

/**
 * @brief 自定义 ADC 类的构造函数
 * @author skyswordx
 * @details 该函数用于初始化 ADC 的配置结构体和校准结构体
 */
ADC_channel_handler_t::ADC_channel_handler_t(adc2_channel_t adc2_channel, adc_atten_t atten, adc_bits_width_t width, uint32_t num_samples) {
            
    this->adc2_channel = adc2_channel;
    this->atten = atten;
    this->width = width;
    this->num_samples = num_samples;

    this->adc_digi_pattern.channel = adc2_channel;
    this->adc_digi_pattern.atten = atten;
    this->adc_digi_pattern.unit = ADC_UNIT_2;
    this->adc_digi_pattern.bit_width = width;

    this->adc_digi_config.adc_pattern = &adc_digi_pattern;
    adc_digi_controller_configure(&adc_digi_config);
}

/**
 * @brief 获取 ADC1 的原始值平均值
 * @author skyswordx
 * @details 该函数用于获取 ADC1 的原始值平均值
 * @param none
 * @return uint32_t ADC1 的原始值平均值
 */
uint32_t ADC_channel_handler_t::get_ADC1_raw_average(){
    uint32_t temp_value = 0; 

    for (uint8_t i = 0; i < this->num_samples; i++) {
        temp_value += adc1_get_raw(adc1_channel);
        vTaskDelay(5); // 延时 5ms
    }

    return temp_value / this->num_samples;
}

/**
 * @brief 获取 ADC1 的电压值平均值
 * @author skyswordx
 * @details 该函数用于获取 ADC1 的电压值平均值
 * @param none
 * @return float ADC1 的电压值平均值
 */
float ADC_channel_handler_t::get_ADC1_voltage_average_mV(){
    uint32_t temp_value = 0; 

    for (uint8_t i = 0; i < this->num_samples; i++) {
        temp_value += adc1_get_raw(adc1_channel);
        vTaskDelay(5); // 延时 5ms
    }

    this->adc_raw_value = temp_value / this->num_samples;
    this->adc_voltage = esp_adc_cal_raw_to_voltage(this->adc_raw_value, this->adc_chars);

    return this->adc_voltage;
}

/**
 * @brief 获取 ADC2 的原始值平均值
 * @author skyswordx
 * @details 该函数用于获取 ADC2 的原始值平均值
 * @param none
 * @return uint32_t ADC2 的原始值平均值
 */
uint32_t ADC_channel_handler_t::get_ADC2_raw_average(){
    uint32_t temp_value = 0; 

    int raw_out;
    for (uint8_t i = 0; i < this->num_samples; i++) {
        adc2_get_raw(adc2_channel, this->width, &raw_out);
        temp_value += raw_out;
        vTaskDelay(5); // 延时 5ms
    }

    return temp_value / this->num_samples;
}

/**
 * @brief 获取 ADC2 的电压值平均值
 * @author skyswordx
 * @details 该函数用于获取 ADC2 的电压值平均值
 * @param none
 * @return float ADC2 的电压值平均值
 */
float ADC_channel_handler_t::get_ADC2_voltage_average_mV(){
    uint32_t temp_value = 0; 

    int raw_out;
    for (uint8_t i = 0; i < this->num_samples; i++) {
        adc2_get_raw(adc2_channel, this->width, &raw_out);
        temp_value += raw_out;
        vTaskDelay(5); // 延时 5ms
    }

    this->adc_raw_value = temp_value / this->num_samples;
    this->adc_voltage = esp_adc_cal_raw_to_voltage(this->adc_raw_value, this->adc_chars);

    return this->adc_voltage;
}

/**
 * @brief 根据电压值计算NTC温度
 * @author skyswordx
 * @details 根据分压电路和NTC特性计算温度值
 * @param voltage_mV ADC采样得到的电压值(mV)
 * @return float 计算得到的温度值(°C)
 */
float ADC_channel_handler_t::calculate_temperature_from_voltage(float voltage_mV) {
    // 将电压从mV转换为V
    double voltage_V = voltage_mV / 1000.0;
    
    // 计算NTC电阻值
    // 分压电路: VCC - R_SERIES - NTC - GND
    // ADC测量的是NTC两端的电压
    double R_ntc = (voltage_V * R_SERIES) / (VCC - voltage_V);
    
    // 使用Steinhart-Hart方程计算温度
    // 1/T = 1/T0 + (1/B) * ln(R/R0)
    // 其中 T0 = 298.15K (25°C), R0 = 100kΩ
    double temp_K = 1.0 / (1.0/T_25 + (1.0/B_VALUE) * log(R_ntc / R_25));
    
    // 转换为摄氏度
    float temp_C = temp_K - 273.15;
    
    this->temperature_celsius = temp_C;
    
    return temp_C;
}

/**
 * @brief 获取温度值
 * @author skyswordx
 * @details 先读取ADC电压值，然后计算温度
 * @return float 温度值(°C)
 */
float ADC_channel_handler_t::get_temperature_celsius() {
    // 先获取电压值
    float voltage = get_ADC1_voltage_average_mV();
    
    // 根据电压计算温度
    return calculate_temperature_from_voltage(voltage);
}
