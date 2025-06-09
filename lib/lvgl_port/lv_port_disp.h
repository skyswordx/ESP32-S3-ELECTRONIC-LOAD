/**
 * @file lv_port_disp.h
 *
 */

/*Copy this file as "lv_port_disp.h" and set this value to "1" to enable content*/
#if 1

#ifndef LV_PORT_DISP_H
#define LV_PORT_DISP_H



/*********************
 *      INCLUDES
 *********************/

#include "lvgl.h"
#include <TFT_eSPI.h>
#include "Arduino.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
/* Initialize low level display driver */
void lv_port_disp_init(void);

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void);

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void);

/*********************
 *      DEFINES
 *********************/
// 6缓冲区配置
#define BUFFER_POOL_SIZE 6
#define SINGLE_BUFFER_SIZE_CALC(hor_res, ver_res) ((hor_res) * (ver_res) / 6)

/**********************
 *      TYPEDEFS
 **********************/
// 缓冲区状态枚举
typedef enum {
    BUFFER_FREE = 0,      // 缓冲区空闲
    BUFFER_RENDERING,     // 缓冲区正在渲染
    BUFFER_READY,         // 缓冲区已就绪，等待传输
    BUFFER_TRANSFERRING   // 缓冲区正在传输到显示器
} buffer_status_t;

// 缓冲区管理结构
typedef struct {
    lv_color_t* buffer;
    buffer_status_t status;
    uint32_t size;
    uint32_t used_size;
} buffer_info_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
/* Initialize low level display driver */
void lv_port_disp_init(void);

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void);

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void);

/* 6缓冲区并行处理相关函数 */
// 获取空闲缓冲区
lv_color_t* get_free_buffer(uint8_t* buffer_index);

// 标记缓冲区为就绪状态
void mark_buffer_ready(uint8_t buffer_index, uint32_t used_size);

// 获取就绪缓冲区进行传输
lv_color_t* get_ready_buffer(uint8_t* buffer_index, uint32_t* buffer_size);

// 释放缓冲区
void release_buffer(uint8_t buffer_index);

// 获取缓冲区状态
buffer_status_t get_buffer_status(uint8_t buffer_index);

// 获取缓冲区信息
buffer_info_t* get_buffer_info(uint8_t buffer_index);



#endif /*LV_PORT_DISP_H*/

#endif /*Disable/Enable content*/
