/**
 * @file lv_port_disp.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"

/*********************
 *      DEFINES
 *********************/

// #define OUR_USE_DMA 1 // 我们使用 DMA 传输数据
// #define ESP32_DMA 1 // ESP32 DMA 传输数据

#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES   480
    // #define MY_DISP_HOR_RES   320
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    320
    // #define MY_DISP_VER_RES    240
#endif


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/
// 6缓冲区管理相关全局变量
static buffer_info_t buffer_pool_info[BUFFER_POOL_SIZE];
static SemaphoreHandle_t buffer_mutex = NULL;  // 缓冲区访问互斥锁

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

    /* Example for 1) */
    // static lv_disp_draw_buf_t draw_buf_dsc_1;
    // static lv_color_t buf_1[MY_DISP_HOR_RES * 10];                          /*A buffer for 10 rows*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/    // 选择这个选项，实现真正的6缓冲区并行处理
    /* 6缓冲区配置 - 使用多个小缓冲区进行并行处理 */
    static lv_disp_draw_buf_t draw_buf_dsc_6;
    
    // 每个缓冲区大小为屏幕的1/6，使用多个缓冲区实现并行处理
    #define SINGLE_BUFFER_SIZE (MY_DISP_HOR_RES * MY_DISP_VER_RES / 6)
    
    // 定义6个缓冲区指针
#if OUR_USE_DMA
    static lv_color_t* buffer_pool[6] = {
        (lv_color_t*) heap_caps_malloc(SINGLE_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM),
        (lv_color_t*) heap_caps_malloc(SINGLE_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM),
        (lv_color_t*) heap_caps_malloc(SINGLE_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM),
        (lv_color_t*) heap_caps_malloc(SINGLE_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM),
        (lv_color_t*) heap_caps_malloc(SINGLE_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM),
        (lv_color_t*) heap_caps_malloc(SINGLE_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM)
    };
#else
    static lv_color_t* buffer_pool[6] = {
        (lv_color_t*) heap_caps_malloc(SINGLE_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM),
        (lv_color_t*) heap_caps_malloc(SINGLE_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM),
        (lv_color_t*) heap_caps_malloc(SINGLE_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM),
        (lv_color_t*) heap_caps_malloc(SINGLE_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM),
        (lv_color_t*) heap_caps_malloc(SINGLE_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM),
        (lv_color_t*) heap_caps_malloc(SINGLE_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM)
    };
#endif

    // 检查所有缓冲区是否分配成功
    bool all_buffers_ok = true;
    for(int i = 0; i < 6; i++) {
        if(buffer_pool[i] == NULL) {
            ESP_LOGI("SIX_BUFFER_POOL", "Buffer %d allocation failed--------", i);
            all_buffers_ok = false;
        }
    }
    
    if(!all_buffers_ok) {
        ESP_LOGI("SIX_BUFFER_POOL", "One or more buffers failed to allocate--------");
    } else {
        ESP_LOGI("SIX_BUFFER_POOL", "All 6 buffers allocated successfully--------");
    }
    
    // 使用前两个缓冲区初始化LVGL绘制缓冲区（双缓冲机制）
    // 其他4个缓冲区可以用于自定义的并行处理逻辑
    lv_disp_draw_buf_init(&draw_buf_dsc_6, buffer_pool[0], buffer_pool[1], SINGLE_BUFFER_SIZE);    //使用 PSRAM 时启用，已改为6缓冲区并行处理配置
    /* Example for 3) - 已注释，现在使用6缓冲区配置 */
    // static lv_disp_draw_buf_t draw_buf_dsc_3;
    // 不使用下面的开辟缓存区的构造函数，因为这样会使用 lv_conf 中指定的 stdlib 中的 malloc
    // static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
    // static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/
// #if OUR_USE_DMA
//     static lv_color_t* buf_3_1 = (lv_color_t*) heap_caps_malloc(MY_DISP_HOR_RES * MY_DISP_VER_RES * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM);
//     static lv_color_t* buf_3_2 = (lv_color_t*) heap_caps_malloc(MY_DISP_HOR_RES * MY_DISP_VER_RES*  sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM);
//     
// #else
//     static lv_color_t* buf_3_1 = (lv_color_t*) heap_caps_malloc(MY_DISP_HOR_RES * MY_DISP_VER_RES * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
//     static lv_color_t* buf_3_2 = (lv_color_t*) heap_caps_malloc(MY_DISP_HOR_RES * MY_DISP_VER_RES*  sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
// #endif   
//     if(buf_3_1 == NULL || buf_3_2 == NULL)
//     {
//         ESP_LOGI("FLASH_BUFFER", "allocate memery fail--------");
//     }
//     lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
//                           MY_DISP_VER_RES * MY_DISP_HOR_RES);   /*Initialize the display buffer*/

    // 初始化缓冲区管理
    buffer_mutex = xSemaphoreCreateMutex();
    if (buffer_mutex == NULL) {
        ESP_LOGI("BUFFER_MUTEX", "Failed to create buffer mutex--------");
    }
    
    // 初始化缓冲区信息
    for(int i = 0; i < BUFFER_POOL_SIZE; i++) {
        buffer_pool_info[i].buffer = buffer_pool[i];
        buffer_pool_info[i].status = BUFFER_FREE;
        buffer_pool_info[i].size = SINGLE_BUFFER_SIZE;
        buffer_pool_info[i].used_size = 0;
    }
    
    // 前两个缓冲区由LVGL管理，标记为特殊状态
    buffer_pool_info[0].status = BUFFER_RENDERING;  // LVGL使用
    buffer_pool_info[1].status = BUFFER_RENDERING;  // LVGL使用
    
    ESP_LOGI("BUFFER_MANAGER", "6-buffer parallel processing initialized--------");

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;    /*Set a display buffer*/
    // disp_drv.draw_buf = &draw_buf_dsc_1;
    disp_drv.draw_buf = &draw_buf_dsc_6;  // 使用6缓冲区配置
    // disp_drv.draw_buf = &draw_buf_dsc_3;    /*Required for 6缓冲区配置 - 不需要full_refresh*/
    // disp_drv.full_refresh = 1;

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

extern TFT_eSPI tft; // 使用外部定义的 tft

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
    // 这个函数是设置显示屏 IC 的初始化配置
    // 例如：tft.begin();  //初始化配置

    tft.begin();  //初始化配置

#ifdef OUR_USE_DMA
    tft.initDMA();
#endif  

    tft.setRotation(1);//设置显示方向

}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/



static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    if(disp_flush_enabled) {
        /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

        // 采用画点的方式刷屏，不够快
        // int32_t x;
        // int32_t y;
        // for(y = area->y1; y <= area->y2; y++) {
        //     for(x = area->x1; x <= area->x2; x++) {
        //         /*Put a pixel to the display. For example:*/
        //         /*put_px(x, y, *color_p)*/
        //         color_p++;
        //     }
        // }
    
        // 采用画矩形的方式刷屏，比画点快
        uint32_t w = ( area->x2 - area->x1 + 1 );
        uint32_t h = ( area->y2 - area->y1 + 1 );

        tft.startWrite();

#if OUR_USE_DMA
        /* 使用 DMA 方式刷屏 */
        tft.pushImageDMA( area->x1, area->y1, w, h, ( uint16_t * )&color_p->full );
        
#else 
        /* 使用普通的画矩形的方式刷屏 */
        tft.setAddrWindow( area->x1, area->y1, w, h );
        tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
#endif

        tft.endWrite();
    }

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
#if OUR_USE_DMA
    tft.dmaWait();
#endif
    lv_disp_flush_ready(disp_drv);
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}

/**********************
 * 6缓冲区并行处理实现
 **********************/

/**
 * @brief 获取空闲缓冲区
 * @param buffer_index 返回的缓冲区索引
 * @return 缓冲区指针，NULL表示没有空闲缓冲区
 */
lv_color_t* get_free_buffer(uint8_t* buffer_index) {
    if (buffer_mutex == NULL || buffer_index == NULL) {
        return NULL;
    }
    
    lv_color_t* result = NULL;
    
    if (xSemaphoreTake(buffer_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        // 从索引2开始查找，因为0和1被LVGL使用
        for (uint8_t i = 2; i < BUFFER_POOL_SIZE; i++) {
            if (buffer_pool_info[i].status == BUFFER_FREE) {
                buffer_pool_info[i].status = BUFFER_RENDERING;
                buffer_pool_info[i].used_size = 0;
                *buffer_index = i;
                result = buffer_pool_info[i].buffer;
                break;
            }
        }
        xSemaphoreGive(buffer_mutex);
    }
    
    return result;
}

/**
 * @brief 标记缓冲区为就绪状态
 * @param buffer_index 缓冲区索引
 * @param used_size 实际使用的缓冲区大小
 */
void mark_buffer_ready(uint8_t buffer_index, uint32_t used_size) {
    if (buffer_mutex == NULL || buffer_index >= BUFFER_POOL_SIZE) {
        return;
    }
    
    if (xSemaphoreTake(buffer_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        if (buffer_pool_info[buffer_index].status == BUFFER_RENDERING) {
            buffer_pool_info[buffer_index].status = BUFFER_READY;
            buffer_pool_info[buffer_index].used_size = used_size;
        }
        xSemaphoreGive(buffer_mutex);
    }
}

/**
 * @brief 获取就绪缓冲区进行传输
 * @param buffer_index 返回的缓冲区索引
 * @param buffer_size 返回的缓冲区大小
 * @return 缓冲区指针，NULL表示没有就绪缓冲区
 */
lv_color_t* get_ready_buffer(uint8_t* buffer_index, uint32_t* buffer_size) {
    if (buffer_mutex == NULL || buffer_index == NULL || buffer_size == NULL) {
        return NULL;
    }
    
    lv_color_t* result = NULL;
    
    if (xSemaphoreTake(buffer_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        for (uint8_t i = 2; i < BUFFER_POOL_SIZE; i++) {
            if (buffer_pool_info[i].status == BUFFER_READY) {
                buffer_pool_info[i].status = BUFFER_TRANSFERRING;
                *buffer_index = i;
                *buffer_size = buffer_pool_info[i].used_size;
                result = buffer_pool_info[i].buffer;
                break;
            }
        }
        xSemaphoreGive(buffer_mutex);
    }
    
    return result;
}

/**
 * @brief 释放缓冲区
 * @param buffer_index 缓冲区索引
 */
void release_buffer(uint8_t buffer_index) {
    if (buffer_mutex == NULL || buffer_index >= BUFFER_POOL_SIZE) {
        return;
    }
    
    if (xSemaphoreTake(buffer_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        buffer_pool_info[buffer_index].status = BUFFER_FREE;
        buffer_pool_info[buffer_index].used_size = 0;
        xSemaphoreGive(buffer_mutex);
    }
}

/**
 * @brief 获取缓冲区状态
 * @param buffer_index 缓冲区索引
 * @return 缓冲区状态
 */
buffer_status_t get_buffer_status(uint8_t buffer_index) {
    if (buffer_index >= BUFFER_POOL_SIZE) {
        return BUFFER_FREE;
    }
    
    buffer_status_t status = BUFFER_FREE;
    
    if (buffer_mutex != NULL && xSemaphoreTake(buffer_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        status = buffer_pool_info[buffer_index].status;
        xSemaphoreGive(buffer_mutex);
    }
    
    return status;
}

/**
 * @brief 获取缓冲区信息
 * @param buffer_index 缓冲区索引
 * @return 缓冲区信息指针
 */
buffer_info_t* get_buffer_info(uint8_t buffer_index) {
    if (buffer_index >= BUFFER_POOL_SIZE) {
        return NULL;
    }
    
    return &buffer_pool_info[buffer_index];
}
#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
