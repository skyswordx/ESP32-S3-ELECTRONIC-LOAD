#include <Arduino.h>
#include <our_adc.h>


// ADC1_CHANNEL_7 --> GPIO8

ADC_channel_handler_t MY_ADC_GPIO8(ADC1_CHANNEL_7, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 64, ADC_UNIT_1);
ADC_channel_handler_t MY_ADC_GPIO7(ADC1_CHANNEL_6, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 64, ADC_UNIT_1);
ADC_channel_handler_t MY_ADC_GPIO6(ADC1_CHANNEL_5, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 64, ADC_UNIT_1);

void setup() {
 
}

void loop() {
    // put your main code here, to run repeatedly:
    float adc_value_1 = MY_ADC_GPIO8.get_ADC1_voltage_average_mV();
    float adc_value_2 = MY_ADC_GPIO7.get_ADC1_voltage_average_mV();
    float adc_value_3 = MY_ADC_GPIO6.get_ADC1_voltage_average_mV();

    float adc_value_average = (adc_value_1 + adc_value_2 + adc_value_3) / 3.0;

    printf("\nADC1_CHANNEL_7: %f, ADC1_CHANNEL_6: %f, ADC1_CHANNEL_5: %f, Average: %f", adc_value_1, adc_value_2, adc_value_3, adc_value_average);
}