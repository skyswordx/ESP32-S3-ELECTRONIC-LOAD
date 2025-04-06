/**
 * @file our_vofa_debuger.hpp
 * @brief 包含封装好的接受 VOFA 发送的调试信息的类和相关的任务函数
 * @author skyswordx
 * @note 在开发中，未使用
 */
#ifndef OUR_VOFA_DEBUGER_HPP
#define OUR_VOFA_DEBUGER_HPP


class VOFA_serial_salve_t {
};


#include <Arduino.h> 
#include "driver/uart.h" // 底层 esp-idf 的 uart 驱动
#include "soc/uart_struct.h" // 底层 esp-idf 的 uart 一些结构体的定义

#define RX_BUF_SIZE  (1024)
#define TX_BUF_SIZE  (1024)

/*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/
#define PATTERN_CHR_NUM    (3)       
#define RD_BUF_SIZE        (RX_BUF_SIZE)

#define CONFIG_VOFA_DEBUGER_PRINTF 1
// #define CONFIG_VOFA_DEBUGER_UART1 1 // 选择使用的串口
// #define CONFOG_VOFA_DEBUGER_UART0 1 // 选择使用的串口

    /**
     * 枚举uart_event_type_t 环形缓冲区中使用的UART事件类型
        UART_DATA                UART 数据事件
        UART_BREAK               UART 中断事件
        UART_BUFFER_FULL         UART RX 缓冲区已满事件
        UART_FIFO_OVF            UART FIFO 溢出事件
        UART_FRAME_ERR           UART RX 帧错误事件
        UART_PARITY_ERR          UART RX 奇偶校验事件
        UART_DATA_BREAK          UART TX 数据和中断事件
        UART_PATTERN_DET         检测到 UART 模式
        UART_EVENT_MAX           UART 事件最大索引
    */

#ifdef CONFIG_VOFA_DEBUGER_UART0
/*
 * 使用 UART0 作为调试串口
 * - Rx buffer: on
 * - Tx buffer: off
 * - Flow control: off
 * - Event_queue: on
 * - Pin assignment: TxD(Default), RxD(Default)
 */

    static const char *UART0_TAG = "uart0_events";
    #define UART_NUM_0 (0)
    #define TXD_PIN (GPIO_NUM_1)
    #define RXD_PIN (GPIO_NUM_3)

    /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/
    #define EX_UART_NUM  UART_NUM_0

    static QueueHandle_t uart0_queue;
    #define UART0_QUEUE_SIZE   (30)


    static inline void uart0_init(void)
    {
        /* Configure parameters of an UART driver,
         * communication pins and install the driver */
        const uart_config_t uart_config = {
            .baud_rate = 115200,           // 波特率 115200
            .data_bits = UART_DATA_8_BITS, // 数据位 8 位
            .parity = UART_PARITY_DISABLE, // 无校验位
            .stop_bits = UART_STOP_BITS_1, // 停止位 1 位
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // 无流控
            .source_clk = UART_SCLK_APB, // 时钟源 APB 80MHz，如果用 RTC 才 8k
            };
    
        // 安装驱动，这里启用事件检测
        uart_driver_install(EX_UART_NUM, RX_BUF_SIZE * 2, TX_BUF_SIZE * 2, UART0_QUEUE_SIZE, &uart0_queue, 0);
        uart_param_config(EX_UART_NUM, &uart_config);
        
        //Set UART pins (using UART0 default pins ie no changes.)
        uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

        //Set uart pattern detect function.
        uart_enable_pattern_det_baud_intr(EX_UART_NUM, '+', PATTERN_CHR_NUM, 9, 0, 0);
        //Reset the pattern queue length to record at most 20 pattern positions.
        uart_pattern_queue_reset(EX_UART_NUM, UART0_QUEUE_SIZE);
    
    }

    
    static void uart0_event_task(void *pvParameters)
    {
        uart_event_t event;
        size_t buffered_size;
        
        // 开辟一段内存存放接收到的数据 无符号字节型（动态分配的内存dtmp的地址不可以发生改变
        uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE);  
    
        for(;;) {
            // 从uart事件队列中接收一个项目并拷贝到事件结构体中 成功接收到项目将从队列中删除
            if(xQueueReceive(uart0_queue, (void * )&event, (TickType_t)portMAX_DELAY)) {
    
                bzero(dtmp, RD_BUF_SIZE);
                // ESP_LOGI(UART1_TAG, "uart[%d] event:", EX_UART_NUM);
    
                switch(event.type) {
                    //Event of UART receving data
                    /*We'd better handler data event fast, there would be much more data events than
                    other types of events. If we take too much time on data event, the queue might
                    be full.*/
                    case UART_DATA:

                        // Handle UART data received event
                        ESP_LOGI(UART0_TAG, "[UART DATA]: %d bytes received", event.size);
                        uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);  // Read received data
                        uart_write_bytes(EX_UART_NUM, (const char*) dtmp, event.size);  // Echo the received data
                        ESP_LOGI(UART0_TAG, "[DATA EVT]: Data echoed back: %s", dtmp);  // Log the echoed data
                        break;

                    //Event of HW FIFO overflow detected
                    case UART_FIFO_OVF:
                        ESP_LOGI(UART0_TAG, "hw fifo overflow");
                        // If fifo overflow happened, you should consider adding flow control for your application.
                        // The ISR has already reset the rx FIFO,
                        // As an example, we directly flush the rx buffer here in order to read more data.
                        uart_flush_input(EX_UART_NUM);
                        xQueueReset(uart0_queue);
                        break;
                    //Event of UART ring buffer full
                    case UART_BUFFER_FULL:
                        ESP_LOGI(UART0_TAG, "ring buffer full");
                        // If buffer full happened, you should consider increasing your buffer size
                        // As an example, we directly flush the rx buffer here in order to read more data.
                        uart_flush_input(EX_UART_NUM);
                        xQueueReset(uart0_queue);
                        break;
                    //Event of UART RX break detected
                    case UART_BREAK:
                        ESP_LOGI(UART0_TAG, "uart rx break");
                        break;
                    //Event of UART parity check error
                    case UART_PARITY_ERR:
                        ESP_LOGI(UART0_TAG, "uart parity error");
                        break;
                    //Event of UART frame error
                    case UART_FRAME_ERR:
                        ESP_LOGI(UART0_TAG, "uart frame error");
                        break;
                    //UART_PATTERN_DET
                    case UART_PATTERN_DET:
                        ESP_LOGI(UART0_TAG, "uart pattern detected");
                        // uart_get_buffered_data_len(EX_UART_NUM, &buffered_size);
                        // int pos = uart_pattern_pop_pos(EX_UART_NUM);
                        // ESP_LOGI(UART0_TAG, "[UART PATTERN DETECTED] pos: %d, buffered size: %d", pos, buffered_size);
                        // if (pos == -1) {
                        //     // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
                        //     // record the position. We should set a larger queue size.
                        //     // As an example, we directly flush the rx buffer here.
                        //     uart_flush_input(EX_UART_NUM);
                        // } else {
                        //     uart_read_bytes(EX_UART_NUM, dtmp, pos, 100 / portTICK_PERIOD_MS);
                        //     uint8_t pat[PATTERN_CHR_NUM + 1];
                        //     memset(pat, 0, sizeof(pat));
                        //     uart_read_bytes(EX_UART_NUM, pat, PATTERN_CHR_NUM, 100 / portTICK_PERIOD_MS);
                        //     ESP_LOGI(UART0_TAG, "read data: %s", dtmp);
                        //     ESP_LOGI(UART0_TAG, "read pat : %s", pat);
                        // }
                        break;
                    // Others
                    default:
                        ESP_LOGI(UART1_TAG, "uart event type: %d", event.type);
                        break;
                }
            }
        }
    
        free(dtmp);
        dtmp = NULL;
    }


#endif



#ifdef CONFIG_VOFA_DEBUGER_UART1
    static const char *UART1_TAG = "uart1_events";
    #define UART_NUM_1 (1)
    #define TXD_PIN (GPIO_NUM_17)
    #define RXD_PIN (GPIO_NUM_18)

    #define EX_UART_NUM  UART_NUM_1

    static QueueHandle_t uart1_queue;
    #define UART1_QUEUE_SIZE   (30)


    static inline void uart1_init(void)
    {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_RTC,
        };
    
    uart_driver_install(EX_UART_NUM, RX_BUF_SIZE * 2, 0,UART1_QUEUE_SIZE,&uart1_queue, 0);
    uart_param_config(EX_UART_NUM, &uart_config);
    uart_set_pin(EX_UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    
    //Set uart pattern detect function.
    uart_enable_pattern_det_baud_intr(EX_UART_NUM, '+', PATTERN_CHR_NUM, 9, 0, 0);
    //Reset the pattern queue length to record at most 20 pattern positions.
    uart_pattern_queue_reset(EX_UART_NUM, UART1_QUEUE_SIZE);
    
    }

    static inline void uart1_send(unsigned char*data,int len)
    {
        int offset  = 0;
        int txbytes = 0;
    
        while(len > 0)
        {
            txbytes = uart_write_bytes(EX_UART_NUM, data + offset, len);
    
            offset += txbytes;
            len    -= txbytes;
        }
    }

        
    static void uart1_event_task(void *pvParameters)
    {
        uart_event_t event;
        size_t buffered_size;
        uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE);
    
        for(;;) {
            //Waiting for UART event.
            if(xQueueReceive(uart1_queue, (void * )&event, (TickType_t)portMAX_DELAY)) {
    
                //bzero(dtmp, RD_BUF_SIZE);
                //ESP_LOGI(UART1_TAG, "uart[%d] event:", EX_UART_NUM);
    
                switch(event.type) {
                    //Event of UART receving data
                    /*We'd better handler data event fast, there would be much more data events than
                    other types of events. If we take too much time on data event, the queue might
                    be full.*/
                    case UART_DATA:
    
                        //ESP_LOGI(UART1_TAG, "[UART DATA]: %d", event.size);
                        uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
                        //ESP_LOGI(UART1_TAG, "[DATA EVT]:");
                        //uart_write_bytes(EX_UART_NUM, (const char*) dtmp, event.size);
                        // print0x(dtmp,event.size);
                        printf("%s",dtmp);
    
                        break;
                    //Event of HW FIFO overflow detected
                    case UART_FIFO_OVF:
                        ESP_LOGI(UART1_TAG, "hw fifo overflow");
                        // If fifo overflow happened, you should consider adding flow control for your application.
                        // The ISR has already reset the rx FIFO,
                        // As an example, we directly flush the rx buffer here in order to read more data.
                        uart_flush_input(EX_UART_NUM);
                        xQueueReset(uart1_queue);
                        break;
                    //Event of UART ring buffer full
                    case UART_BUFFER_FULL:
                        ESP_LOGI(UART1_TAG, "ring buffer full");
                        // If buffer full happened, you should consider increasing your buffer size
                        // As an example, we directly flush the rx buffer here in order to read more data.
                        uart_flush_input(EX_UART_NUM);
                        xQueueReset(uart1_queue);
                        break;
                    //Event of UART RX break detected
                    case UART_BREAK:
                        ESP_LOGI(UART1_TAG, "uart rx break");
                        break;
                    //Event of UART parity check error
                    case UART_PARITY_ERR:
                        ESP_LOGI(UART1_TAG, "uart parity error");
                        break;
                    //Event of UART frame error
                    case UART_FRAME_ERR:
                        ESP_LOGI(UART1_TAG, "uart frame error");
                        break;
                    //UART_PATTERN_DET
                    case UART_PATTERN_DET:
                        uart_get_buffered_data_len(EX_UART_NUM, &buffered_size);
                        int pos = uart_pattern_pop_pos(EX_UART_NUM);
                        ESP_LOGI(UART1_TAG, "[UART PATTERN DETECTED] pos: %d, buffered size: %d", pos, buffered_size);
                        if (pos == -1) {
                            // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
                            // record the position. We should set a larger queue size.
                            // As an example, we directly flush the rx buffer here.
                            uart_flush_input(EX_UART_NUM);
                        } else {
                            uart_read_bytes(EX_UART_NUM, dtmp, pos, 100 / portTICK_PERIOD_MS);
                            uint8_t pat[PATTERN_CHR_NUM + 1];
                            memset(pat, 0, sizeof(pat));
                            uart_read_bytes(EX_UART_NUM, pat, PATTERN_CHR_NUM, 100 / portTICK_PERIOD_MS);
                            ESP_LOGI(UART1_TAG, "read data: %s", dtmp);
                            ESP_LOGI(UART1_TAG, "read pat : %s", pat);
                        }
                        break;
                    //Others
                    // default:
                    //     ESP_LOGI(UART1_TAG, "uart event type: %d", event.type);
                    //     break;
                }
            }
        }
    
        free(dtmp);
        dtmp = NULL;
        vTaskDelete(NULL);
    }
    
 

#endif





 
 

 
 
 
 



 
 
 

#endif // OUR_VOFA_DEBUGER_HPP