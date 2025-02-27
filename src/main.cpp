#include <Arduino.h>
#include <lvgl.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "demos/lv_demos.h"

// 包含显示接口和触摸接口绑定
#include "lv_port_disp.h"
#include "lv_port_indev.h" 

/******** LVGL-SetUP *******/
// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

/******** RTOS-SetUP *******/
TaskHandle_t lvgl_task_handle; // LVGL 任务结构对象


void lvgl_task(void *pvParameters)
{
  while(1)
  {
    lv_task_handler();  //开启LVGL任务调度
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
  lv_init(); // 必须运行的
  lv_port_disp_init();
  lv_port_indev_init();
  
  lv_demo_benchmark();

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
