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

// 包含自定义的编码器类（依赖 ESP32Encoder 库）
#include "our_encoder.h"


/******** Encoder Setup *****/
#define ENCODER_1_PIN_A 17
#define ENCODER_1_PIN_B 18
#define ENCODER_1_PIN_S 16
encoder_handle_t encoder1(ENCODER_1_PIN_A, ENCODER_1_PIN_B, ENCODER_1_PIN_S);


#include <Wire.h>
#include "INA226.h"
INA226 ina226_device(0x45); // INA226 电流传感器

/******** LVGL-SetUP *******/
// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

// GUI guider
lv_ui guider_ui;

/******** RTOS-SetUP *******/
TaskHandle_t lvgl_task_handle; // LVGL 任务结构句柄
SemaphoreHandle_t gui_xMutex;  // gui 互斥锁句柄，LVGL 线程不安全，需要加锁
SemaphoreHandle_t button_xBinarySemaphore; // 按键二值信号量

// GUI 更新使用的消息队列
QueueHandle_t sensor_queue_handle; // 消息队列句柄
const int queue_element_size = 10; // 消息队列元素大小
QueueHandle_t button_queue_handle; // 按键消息队列句柄


// 定义一个枚举类型，表示发送消息的源设备 id
enum device_id_t {
  DEVICE_DUMMY_SENSOR = 0,
  DEVICE_ENCODER = 1,
  DEVICE_INA226 = 2,
  DEVICE_UNKNOWN = 99
};

typedef struct {
  float value1; // 传感器数据
  float value2; // 传感器数据
  float value3; // 传感器数据
} device_data_t;

typedef struct {
  device_id_t device_id; // 设备ID
  float value; // 存放的数据（简单设置为一个值，后期有需求再改为专门的结构体
  device_data_t device_data; // INA226 传感器数据
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


static void switch_event_handler(lv_event_t* event){
  lv_event_code_t event_code = lv_event_get_code(event);

  if (event_code == LV_EVENT_VALUE_CHANGED){
    // 任何事件都会触发回调函数，当值切换时执行
    /* 回调函数执行 */
    printf("xxxxxxx");
  }
}

// lv_obj_add_event_cb(sw_obj, switch_event_handler, LV_EVENT_ALL, NULL);

void encoder1_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    printf("\n[encoder1_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    
    // 获取旋转编码器数据
    msg.device_id = DEVICE_ENCODER;
    int64_t count = encoder1.read_count_accum_clear();

    // 对旋转编码器的值进行处理
    if (count < 0) {
      msg.value = 0;
    } else {
      if (encoder1.mode == QUAD) {
        msg.value = count;
      } else {
        msg.value = count/4.0;
      }
    }
    
    // printf("\n[encoder1_task] encoder1 count: %lld", msg.value);

    int return_value = xQueueSend(sensor_queue_handle, (void *)&msg, 0);
    if (return_value == pdTRUE) {
      // printf("\n[encoder1_task] sent message  to the queue successfully\n");
    } else if (return_value == errQUEUE_FULL) {
      // printf("\n[encoder1_task] failed to send message to queue, queue is full\n");
    } else {
      // printf("\n[encoder1_task] failed to send message to queue\n");
    }

    vTaskDelay( 1000 );
  }
}


void get_dummy_sensor_data_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    printf("\n[get_sensor_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));

    // 模拟传感器数据
    msg.device_id = DEVICE_DUMMY_SENSOR;
    msg.value = 4.0 + (rand() % 100) / 100.0;
   

    int return_value = xQueueSend(sensor_queue_handle, (void *)&msg, 0);
    if (return_value == pdTRUE) {
      // printf("\n[get_sensor_data_task] sent message  to the queue successfully\n");
    } else if (return_value == errQUEUE_FULL) {
      // printf("\n[get_sensor_data_task] failed to send message to queue, queue is full\n");
    } else {
      // printf("\n[get_sensor_data_task] failed to send message to queue\n");
    }

    vTaskDelay( 1000 );
  }
}


void get_ina226_data_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    printf("\n[get_ina226_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));


    msg.device_id = DEVICE_INA226;
  
    // msg.ina226_data.measured_current = ina226_device.getCurrent_mA();
    // msg.ina226_data.measured_voltage = ina226_device.getBusVoltage();
    // msg.ina226_data.measured_power = ina226_device.getPower_mW();

   
    int return_value = xQueueSend(sensor_queue_handle, (void *)&msg, 0);
    if (return_value == pdTRUE) {
      // printf("\n[get_ina226_data_task] sent message  to the queue successfully\n");
    } else if (return_value == errQUEUE_FULL) {
      // printf("\n[get_ina226_data_task] failed to send message to queue, queue is full\n");
    } else {
      // printf("\n[get_ina226_data_task] failed to send message to queue\n");
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

    if( sensor_queue_handle != NULL){ // 检查消息队列是否创建成功
      if (xQueueReceive(sensor_queue_handle, &msg, portMAX_DELAY) == pdTRUE) {
        // printf("\n[update_gui_task] received message from queue");

        if (xSemaphoreTake(gui_xMutex, portMAX_DELAY) == pdTRUE) { // 获取互斥锁
          // 检测元素是否准备齐全并更新 GUI

          // if (!isnan(msg.measured_Current)){
          //   if ( guider_ui.xxx_label !=NULL){ lv_label_set_text_fmt(guider_ui.xxx_label, "%.3f", msg.measured_Current); }
          // }
          /*
            printf("\n[update_gui_task] using msg.measured_Current: %.3f", msg.value);
            printf("\n[update_gui_task] updated lvgl label");
          */

          switch (msg.device_id)
          {
          case DEVICE_INA226:
            
            break;
          case DEVICE_ENCODER:
            if (guider_ui.main_page_set_current_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_set_current_label, "%.3f", msg.value); }
            break;
          case DEVICE_DUMMY_SENSOR:
            if (guider_ui.main_page_measure_current_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_measure_current_label, "%.3f", msg.value); }
            if (guider_ui.main_page_measure_voltage_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_measure_voltage_label, "%.3f", msg.value); }
            if (guider_ui.main_page_measure_power_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_measure_power_label, "%.3f", msg.value); }
            if (guider_ui.main_page_measure_register_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_measure_register_label, "%.3f", msg.value); }
            break;
          default:
            break;
          }

          xSemaphoreGive(gui_xMutex); // 释放互斥锁
        }
      } else {
        printf("\n[update_gui_task] failed to receive message from queue\n");
      }
    }
  }
}


void button_handler_task(void *pvParameters){

  BaseType_t button_down = pdFALSE;
  BaseType_t button_up = pdFALSE;
  int time = 0;

  gpio_num_t GPIO_PIN;
  while(1){
  
    xSemaphoreTake(button_xBinarySemaphore, portMAX_DELAY); // 总是保持阻塞等待二值信号量


    printf("\n[button_handler_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    if ( button_queue_handle != NULL){ // 检查消息队列是否创建成功
      if (xQueueReceive(button_queue_handle, &GPIO_PIN, portMAX_DELAY) == pdTRUE) {
        printf("\n[button_handler_task] received message from queue");

        /* 收到中断数据，开始检测 */

        // 记录下用户按下按键的时间点
        if (gpio_get_level(GPIO_PIN) == LOW) {
          button_down = pdTRUE;
          time = millis();

        // 记录下用户松开按键的时间点  
        } else if (button_down == pdTRUE) {
          
          button_up = pdTRUE;
          // 如果当前GPIO口的电平已经记录为按下，则开始减去上次按下按键的时间点
          time = millis() - time;
        } 
        if (button_down == pdTRUE && button_up == pdTRUE) {
          // printf("\n[button_handler_task] button pressed for %d ms", time);
          button_down = pdFALSE;
          button_up = pdFALSE;

          if (time < 1000000) { // 1s
            printf("\n[button_handler_task] short press");
          } else {
            printf("\n[button_handler_task] long press");
          }
        }
      } else {
        printf("\n[button_handler_task] failed to receive message from queue\n");
      }
    }
  }
}



void IRAM_ATTR button_press_ISR(void *arg ){ // 产生 GPIO 中断时调用
  BaseType_t xHigherPriorityTaskWoken = pdFALSE; 
  // 初始化为 pdFALSE，以便最后把耗时的数据处理移出中断给辅助任务处理
  
  // 释放二值信号量，对应阻塞于此信号量的任务（辅助任务）将被唤醒
  xSemaphoreGiveFromISR(button_xBinarySemaphore, &xHigherPriorityTaskWoken);

  gpio_num_t GPIO_PIN = encoder1.pin_S;
  xQueueSendFromISR(button_queue_handle, &GPIO_PIN, &xHigherPriorityTaskWoken); // 发送消息到按键消息队列

 
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 
  // 如果有更高优先级的任务需要运行，立即切换到那个任务（辅助任务，因为此任务设定为最高优先级）
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

  // 初始化 INA226 电流传感器
  // Wire.begin();
  // if (!ina226_device.begin()) {
  //   printf("could not connect. Fix and Reboot");
  // }
  // ina226_device.setMaxCurrentShunt(1, 0.002);

  setup_ui(&guider_ui); // 初始化 gui_guider
  
  /* 挂起 GUI guider 生成的页面 */
  setup_scr_main_page(&guider_ui); // gui_guider 为每一个页面生成的，这里是名字为 xxx 的页面
  lv_scr_load(guider_ui.main_page); //每一个页面的名字都是 gui_guider 结构体的元素
  // lv_demo_benchmark();

  /* 创建消息队列 */
  // Create the queue which will have <queue_element_size> number of elements, each of size `message_t` and pass the address to <sensor_queue_handle>.
  sensor_queue_handle = xQueueCreate(queue_element_size, sizeof(message_t));
  button_queue_handle = xQueueCreate(2, sizeof(gpio_num_t));

  // Check if the queue was successfully created
  if (sensor_queue_handle == NULL) {
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

  xTaskCreatePinnedToCore(encoder1_task,
              "encoder1_task",
              1024*4,
              NULL,
              2,
              NULL,
              1
            );

  // xTaskCreatePinnedToCore(get_ina226_data_task,
  //             "get_ina226_data_task",
  //             1024*4,
  //             NULL,
  //             2,
  //             NULL,
  //             1
  //           );

  /* 暂时不使用这个旋转编码器的 GPIO 硬件中断*/

  // // 创建二值信号量
  // button_xBinarySemaphore = xSemaphoreCreateBinary();
  // if (button_xBinarySemaphore != NULL) {
  //   // Core 1 运行按键处理任务
  //   xTaskCreatePinnedToCore(button_handler_task,
  //             "button_task",
  //             1024*4,
  //             NULL,
  //             4, // 必须是最高优先级
  //             NULL,
  //             1
  //           );
  // // 绑定 GPIO (ENCODER_1_PIN_S)中断 和中断处理函数
  // // 注册中断服务
  // gpio_set_intr_type((gpio_num_t)encoder1.pin_S, GPIO_INTR_ANYEDGE);
	// gpio_install_isr_service(0);
	// gpio_isr_handler_add((gpio_num_t)encoder1.pin_S, button_press_ISR, (void *) &encoder1.pin_S);
  // // 设置 GPIO 中断类型
  // }

}

void loop() {


}
