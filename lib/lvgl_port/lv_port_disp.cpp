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
    // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/    // 选择这个选项，可以实现半屏双缓冲，节省内存并提供良好性能
    /* Example for 2) */
    static lv_disp_draw_buf_t draw_buf_dsc_2;
    // 半屏缓冲区大小，每个缓冲区为屏幕高度的一半
    #define HALF_SCREEN_BUFFER_SIZE (MY_DISP_HOR_RES * MY_DISP_VER_RES / 2)
#if OUR_USE_DMA
    static lv_color_t* buf_2_1 = (lv_color_t*) heap_caps_malloc(HALF_SCREEN_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM);
    static lv_color_t* buf_2_2 = (lv_color_t*) heap_caps_malloc(HALF_SCREEN_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM);
#else
    static lv_color_t* buf_2_1 = (lv_color_t*) heap_caps_malloc(HALF_SCREEN_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    static lv_color_t* buf_2_2 = (lv_color_t*) heap_caps_malloc(HALF_SCREEN_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
#endif
    if(buf_2_1 == NULL || buf_2_2 == NULL)
    {
        ESP_LOGI("HALF_SCREEN_BUFFER", "allocate memory fail--------");
    }
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, HALF_SCREEN_BUFFER_SIZE);   /*Initialize the display buffer*/

    //使用 PSRAM 时启用，最大化利用 PSRAM 刷屏（已改为半屏双缓冲）
    /* Example for 3) also set disp_drv.full_refresh = 1 below*/
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
    disp_drv.draw_buf = &draw_buf_dsc_2;
    // disp_drv.draw_buf = &draw_buf_dsc_3;

    /*Required for Example 3) - 半屏双缓冲不需要full_refresh*/
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


#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
