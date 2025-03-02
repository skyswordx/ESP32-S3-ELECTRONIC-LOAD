#include <Arduino.h>
#include <lvgl.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "demos/lv_demos.h"

// 包含显示接口和触摸接口绑定
#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "gui_guider.h"

/******** LVGL-SetUP *******/
// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

// GUI guider
lv_ui guider_ui;

/******** RTOS-SetUP *******/
TaskHandle_t lvgl_task_handle; // LVGL 任务结构对象
SemaphoreHandle_t xMutex;  // 互斥锁句柄，LVGL 线程不安全，需要加锁

void lvgl_task(void *pvParameters)
{
  while(1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY)) { // 获取互斥锁
      lv_task_handler();  //开启LVGL任务调度
      xSemaphoreGive(xMutex); // 释放互斥锁
    }
    
    vTaskDelay( 20 );
  }
  vTaskDelete(NULL);
}


static void system_init(void) {
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());
}


void setup() {

  system_init(); // 打印系统信息
  lv_init(); // 初始化 LVGL 必须运行的
  lv_port_disp_init(); // 初始化绑定显示接口
  lv_port_indev_init(); // 初始化和绑定触摸接口

  setup_ui(&guider_ui); // 初始化 gui_guider
  
  /* test */
  setup_scr_screen_1(&guider_ui); // gui_guider 为每一个页面生成的，这里是名字为 screen 的页面
  lv_scr_load(guider_ui.screen_1); //每一个页面的名字都是 gui_guider 结构体的元素

  // lv_demo_benchmark();

  xMutex = xSemaphoreCreateMutex(); // 创建一个互斥信号量
  xTaskCreate(lvgl_task,
              "demo_test",
              1024*4,
              NULL,
              1,
              &lvgl_task_handle
            );
}

void loop() {


}
