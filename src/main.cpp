#include <Arduino.h>
#include <lvgl.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "demos/lv_demos.h"

// 包含显示接口和触摸接口绑定部分
#include "lv_port_disp.h"
#include "lv_port_indev.h"

// 包含使用 GUI guider 生成的 GUI 部分
#include "gui_guider.h"

/******** LVGL-SetUP *******/
// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

// GUI guider
lv_ui guider_ui;

/******** RTOS-SetUP *******/
TaskHandle_t lvgl_task_handle; // LVGL 任务结构句柄
SemaphoreHandle_t gui_xMutex;  // gui 互斥锁句柄，LVGL 线程不安全，需要加锁


// GUI 更新使用的消息队列
QueueHandle_t queue_handle; // 消息队列句柄
const int queue_element_size = 10; // 消息队列元素大小
typedef struct {
  float measured_Current;
  float measured_Voltage;
  float measured_Power;
  // ...
} message_t;

void lvgl_task(void *pvParameters)
{
  while(1)
  {
    if (xSemaphoreTake(gui_xMutex, portMAX_DELAY)) { // 获取互斥锁
      lv_task_handler();  //开启LVGL任务调度
      xSemaphoreGive(gui_xMutex); // 释放互斥锁
    }
    
    vTaskDelay( 20 );
  }
  vTaskDelete(NULL);
}

void get_dummy_sensor_data_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    printf("\n[get_sensor_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));

    // 模拟传感器数据
    msg.measured_Current = 4.0 + (rand() % 100) / 100.0;
    msg.measured_Voltage = 5.0 + (rand() % 100) / 100.0;
    msg.measured_Power = 20.0 + (rand() % 100) / 100.0;

    int return_value = xQueueSend(queue_handle, (void *)&msg, 0);
    if (return_value == pdTRUE) {
      printf("\n[get_sensor_data_task] sent message  to the queue successfully\n");
    } else if (return_value == errQUEUE_FULL) {
      printf("\n[get_sensor_data_task] failed to send message to queue, queue is full\n");
    } else {
      printf("\n[get_sensor_data_task] failed to send message to queue\n");
    }

    vTaskDelay( 1000 );
  }
}


void update_gui_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    printf("\n[update_gui_task] running on core: %d, Free stack space: %d\n", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));

    if( queue_handle != NULL){ // 检查消息队列是否创建成功
      if (xQueueReceive(queue_handle, &msg, portMAX_DELAY) == pdTRUE) {
        printf("\n[update_gui_task] received message from queue");

        if (xSemaphoreTake(gui_xMutex, portMAX_DELAY) == pdTRUE) { // 获取互斥锁
          // 检测元素是否准备齐全并更新 GUI

          // if (!isnan(msg.measured_Current)){
          //   if ( guider_ui.xxx_label !=NULL){ lv_label_set_text_fmt(guider_ui.xxx_label, "%.3f", msg.measured_Current); }
          // }
          if (!isnan(msg.measured_Current)){
            if ( guider_ui.main_page_measure_current_label !=NULL){ 
              lv_label_set_text_fmt(guider_ui.main_page_measure_current_label, "%.3f", msg.measured_Current);
              printf("\n[update_gui_task] using msg.measured_Current: %.3f", msg.measured_Current);
            }
            printf("\n[update_gui_task] updated lvgl label");
          }
          xSemaphoreGive(gui_xMutex); // 释放互斥锁
        }
      } else {
        printf("\n[update_gui_task] failed to receive message from queue\n");
      }
    }
  }
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

  /* 系统和一系列初始化 */
  system_init(); // 打印系统信息
  lv_init(); // 初始化 LVGL 必须运行的
  lv_port_disp_init(); // 初始化绑定显示接口
  lv_port_indev_init(); // 初始化和绑定触摸接口

  setup_ui(&guider_ui); // 初始化 gui_guider
  
  /* 挂起 GUI guider 生成的页面 */
  setup_scr_main_page(&guider_ui); // gui_guider 为每一个页面生成的，这里是名字为 xxx 的页面
  lv_scr_load(guider_ui.main_page); //每一个页面的名字都是 gui_guider 结构体的元素
  // lv_demo_benchmark();

  /* 创建消息队列 */
  // Create the queue which will have <queue_element_size> number of elements, each of size `message_t` and pass the address to <queue_handle>.
  queue_handle = xQueueCreate(queue_element_size, sizeof(message_t));

  // Check if the queue was successfully created
  if (queue_handle == NULL) {
    printf("Queue could not be created. Halt.");
    while (1) delay(1000);  // Halt at this point as is not possible to continue
  }

  gui_xMutex = xSemaphoreCreateMutex(); // 创建一个互斥信号量
  if (gui_xMutex == NULL) {
    // Handle semaphore creation failure
    printf("semaphore creation failure");
    return;
  }

  // Core 0 运行 LVGL main task handler
  xTaskCreatePinnedToCore(lvgl_task,
              "demo_test",
              1024*10,
              NULL,
              3,
              &lvgl_task_handle,
              0
            );

  // Core 1 运行（获取传感器数据任务）+ （更新 GUI 任务）
  xTaskCreatePinnedToCore(get_dummy_sensor_data_task,
              "get_sensor_data_task",
              1024*4,
              NULL,
              2,
              NULL,
              1
            );
  
  xTaskCreatePinnedToCore(update_gui_task,
              "update_gui_task",
              1024*4,
              NULL,
              1,
              NULL,
              1
            );

}

void loop() {


}
