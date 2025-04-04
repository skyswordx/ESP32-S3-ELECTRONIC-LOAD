#ifndef _OUR_SERIAL_DEBUGGER_H
#define _OUR_SERIAL_DEBUGGER_H

#include <Arduino.h> // 为了可以使用 Serial 这个接口
#include "driver/uart.h" // 底层 esp-idf 的 uart 驱动
#include "soc/uart_struct.h" // 底层 esp-idf 的 uart 一些结构体的定义
#include "our_motor_controller.h" // 为了可以使用 Motor 这个类进行参数传递



#define RECEIVED_DATA_BUFF 32


void serial_uart0_init_with_mannual_ISR(int baud_rate);

void IRAM_ATTR uart0_isr_callback(void* arg);

double divided_by_10_power(uint8_t input, uint8_t power);

void serial_debug_service();

#endif