#include <Arduino.h>
#include <lvgl.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "demos/lv_demos.h"

/**
 * 若使用 2.4 寸屏幕，请在
 *  - TFT_eSPI/User_Setup_Select.h 中包含 ILI9341.h
 *  - Lib/lvg_port/lv_port_disp.cpp 中修改尺寸为 240x320
 *  - Lib/lvg_port/lv_port_indev.cpp 中修改校准数据
 * 
 * 若使用 3.5 寸屏幕，请在
 * - TFT_eSPI/User_Setup_Select.h 中包含 ILI9488.h
 * - Lib/lvg_port/lv_port_disp.cpp 中修改尺寸为 320x480
 * - Lib/lvg_port/lv_port_indev.cpp 中修改校准数据
 */

// 包含显示接口和触摸接口绑定部分
#include "lv_port_disp.h"
#include "lv_port_indev.h"

// 包含使用 GUI guider 生成的 GUI 部分
#include "gui_guider.h"

// 包含自定义的编码器类（依赖 ESP32Encoder 库）
#include "our_encoder.h"

// 包含自定义的 PID 控制器类和 VOFA 下位机
#include "our_pid_controller.hpp"
#include "our_vofa_debuger.hpp"


/******** Encoder Setup *****/
#define ENCODER_1_PIN_A 18
#define ENCODER_1_PIN_B 17
// #define ENCODER_1_PIN_S 16
encoder_handle_t encoder1(ENCODER_1_PIN_A, ENCODER_1_PIN_B);


/******** INA226 & MCP4725 Setup **********/
#include <Wire.h>
// 第一条 IIC 总线 Wire 的引脚
#define IIC_SDA 4
#define IIC_SCL 5

#include "INA226.h"
INA226 INA226_device(0x40); // INA226 电流传感器

#include "MCP4725.h"
MCP4725 MCP4725_device(0x60); // MCP4725 DAC 芯片
#define WARNING_VOLTAGE 18 // 触发过压保护的电压

/******** ADC1 Setup *********/
#include "our_adc.h"
ADC_channel_handler_t MY_ADC_GPIO6(ADC1_CHANNEL_5, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 64, ADC_UNIT_1);

/******** GPIO-button Setup *********/
#include "our_button.hpp"
GPIO_button_handler_t button1(GPIO_NUM_2); // GPIO 2 作为按键输入引脚
GPIO_button_handler_t button2(GPIO_NUM_1);
GPIO_button_handler_t button3(GPIO_NUM_7);
GPIO_button_handler_t button4(GPIO_NUM_15); 

GPIO_button_handler_t encoder1_button(GPIO_NUM_16); // 旋转编码器的按键引脚


/******** LVGL-SetUP *******/
// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

// GUI guider
lv_ui guider_ui;

/******** RTOS-SetUP *******/
TaskHandle_t lvgl_task_handle; // LVGL 任务结构句柄
SemaphoreHandle_t gui_xMutex;  // gui 互斥锁句柄，LVGL 线程不安全，需要加锁
SemaphoreHandle_t button_xBinarySemaphore; // 按键二值信号量
SemaphoreHandle_t voltage_protection_xBinarySemaphore; // 过压保护二值信号量

// GUI 更新使用的消息队列
QueueHandle_t sensor_queue_handle; // 消息队列句柄
const int queue_element_size = 10; // 消息队列元素大小
QueueHandle_t button_queue_handle; // 按键消息队列句柄


// 定义一个枚举类型，表示发送消息的源设备 id
enum device_id_t {
  DEVICE_DUMMY_SENSOR = 0,
  DEVICE_ENCODER = 1,
  DEVICE_INA226 = 2,
  DEVICE_ADC1 = 3,
  DEVICE_UNKNOWN = 99
};

typedef struct {
  float value1; // 传感器数据
  float value2; // 传感器数据
  float value3; // 传感器数据
  float value4; // 传感器数据
  float value5; // 传感器数据
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



void get_encoder1_data_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    // printf("\n[get_encoder1_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    
    // 获取旋转编码器数据
    msg.device_id = DEVICE_ENCODER;
    msg.value = encoder1.read_count_accum_clear();


    // printf("\n[get_encoder1_data_task] encoder1 count: %lld", count);
    // printf("\n[get_encoder1_data_task] encoder1 value: %.3f", msg.value);

    int return_value = xQueueSend(sensor_queue_handle, (void *)&msg, 0);
    if (return_value == pdTRUE) {
      // printf("\n[get_encoder1_data_task] sent message  to the queue successfully\n");
    } else if (return_value == errQUEUE_FULL) {
      // printf("\n[get_encoder1_data_task] failed to send message to queue, queue is full\n");
    } else {
      // printf("\n[get_encoder1_data_task] failed to send message to queue\n");
    }

    vTaskDelay( 1000 );
  }
}


void get_dummy_sensor_data_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    // printf("\n[get_sensor_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));

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
    // printf("\n[get_ina226_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));

    // printf("\n[get_ina226_data_task] now_time: %d", millis());
    msg.device_id = DEVICE_INA226;
  
    float measure_current_mA = INA226_device.getCurrent_mA_plus();
    float measure_voltage_V = INA226_device.getBusVoltage_plus();
    float measure_power_W = INA226_device.getPower();
    float measure_resistance_Kohm = abs((measure_voltage_V )/ (measure_current_mA ));

    // 负载调整率？
            /*
              measure_current = 0A ->  记录 bus_V0
              measure_current = 1A ->  记录 bus_V1
              rate = (bus_V1 - bus_V0) / bus_V0
             */
    static float bus_V0 = 0.0;
    static float bus_V1 = 0.0;
    if ( abs((abs(measure_current_mA) - 0.0) < 0.1) ){
      // 记录 bus_V0
      bus_V0 = measure_voltage_V;
    } else if ( abs((abs(measure_current_mA)*1e3 - 1.0) < 0.1) ){
      // 记录 bus_V1
      bus_V1 = measure_voltage_V;
    }

    float rate = (bus_V1 - bus_V0) / bus_V0;

    msg.device_data.value1 = measure_current_mA;
    msg.device_data.value2 = measure_voltage_V;
    msg.device_data.value3 = measure_power_W;
    msg.device_data.value4 = measure_resistance_Kohm;
    msg.device_data.value5 = rate;

    // printf("\n[get_ina226_data_task] INA226: %.3f mA, %.3f V, %.3f W, %.3f KOhm, %.3f", measure_current_mA, measure_voltage_V, measure_power_W, measure_resistance_Kohm, rate);
    printf("[get_ina226_data_task] INA226 V/mA: %.3f,%.3f\n", measure_voltage_V, measure_current_mA); 

    // 检查 INA226 电压是否超过警告值，如果超过则进行过压保护
    if(measure_voltage_V >= WARNING_VOLTAGE){
      // 电压过高，警告
      msg.value = WARNING_VOLTAGE; // LVGL 读取到这个数据就会弹出警告

      // 这里触发过压之后的保护程序
      // xSemaphoreGive(voltage_protection_xBinarySemaphore);
    }

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
    // printf("\n[update_gui_task] running on core: %d, Free stack space: %d\n", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));

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

            // 显示 INA226 测量的电流
            if (guider_ui.main_page_measure_current_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_measure_current_label, "%.3f", msg.device_data.value1); }
            
            // 显示 INA226 测量的电压
            if (guider_ui.main_page_measure_voltage_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_measure_voltage_label, "%.3f", msg.device_data.value2); }
            
            // 显示 INA226 测量的功率
            if (guider_ui.main_page_measure_power_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_measure_power_label, "%.3f", msg.device_data.value3); }
            
            // 显示 INA226 测量的等效电阻
            if (guider_ui.main_page_measure_resistance_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_measure_resistance_label, "%.3f", msg.device_data.value4); }
            
            // 显示 INA226 测量的电源调整率

            // 如果 INA226 测量的电压超过警告值，就进行过压保护
            if (msg.value == WARNING_VOLTAGE){
              // 这里专门设置一个控件进行展示

              // 同时关闭 DAC 输出，让 MOSFET 关断，避免继续吸收功率（在中断函数中处理）
              
            }

            break;
          case DEVICE_ENCODER:
            if (guider_ui.main_page_set_current_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_set_current_label, "%.3f", msg.value); }

            // 测试波形图。编码器作为数据源
            // guider_ui.chart_page_chart_1_0  是 series
            if (guider_ui.chart_page_chart_1 != NULL && guider_ui.chart_page_chart_1_0 != NULL){
              lv_chart_set_next_value(guider_ui.chart_page_chart_1, guider_ui.chart_page_chart_1_0, msg.value);
            }
            
            break;
          case DEVICE_DUMMY_SENSOR:
            if (guider_ui.main_page_measure_current_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_measure_current_label, "%.3f", msg.value); }
            if (guider_ui.main_page_measure_voltage_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_measure_voltage_label, "%.3f", msg.value); }
            if (guider_ui.main_page_measure_power_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_measure_power_label, "%.3f", msg.value); }
            if (guider_ui.main_page_measure_resistance_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_measure_resistance_label, "%.3f", msg.value); }
            break;
          default:
            break;
          }

          xSemaphoreGive(gui_xMutex); // 释放互斥锁
        }
      } else {
        // printf("\n[update_gui_task] failed to receive message from queue\n");
      }
    }
  }
}

void voltage_protection_task(void *pvParameters){
  while(1){
  
    printf("\n[voltage_protection_task] waiting");
    xSemaphoreTake(voltage_protection_xBinarySemaphore, portMAX_DELAY); // 总是保持阻塞等待二值信号量
    
    printf("\n[voltage_protection_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));

    // 执行过压保护任务，关闭 DAC 输出，让 MOSFET 关断
    MCP4725_device.setVoltage(0.0); // 设置输出电压为 0V
    printf("\n[voltage_protection_task] DAC output voltage set to 0V");
  }
}

void button_handler_task(void *pvParameters){

  BaseType_t button_down = pdFALSE;
  BaseType_t button_up = pdFALSE;
  BaseType_t short_press = pdFALSE;
  BaseType_t long_press = pdFALSE;
  int time_ms = 0;

  gpio_num_t GPIO_PIN;
  while(1){
    // printf("\n[button_handler_task] waiting");
    xSemaphoreTake(button_xBinarySemaphore, portMAX_DELAY); // 总是保持阻塞等待二值信号量


    // printf("\n[button_handler_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    if ( button_queue_handle != NULL){ // 检查消息队列是否创建成功
      if (xQueueReceive(button_queue_handle, &GPIO_PIN, portMAX_DELAY) == pdTRUE) {
        // printf("\n[button_handler_task] received message from queue");

        /* 收到中断数据，开始检测 */

        // 利用 RTOS 系统时间片轮询和标志位来进行长短按检测
        button_down = pdTRUE;
        time_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;

        // 等待按键释放
        while (gpio_get_level(GPIO_PIN) == LOW) {
          vTaskDelay(10 / portTICK_PERIOD_MS); // 延时 10ms，避免频繁轮询
        }

        button_up = pdTRUE;
        time_ms = (xTaskGetTickCount() * portTICK_PERIOD_MS) - time_ms;
    

        // 使用 while 循环来阻塞检测
        // time_ms = millis();
        // while (gpio_get_level(GPIO_PIN) == LOW) {
        //   // printf("\n[button_handler_task] button pressed");
        //   button_down = pdTRUE;

        //   if (millis() - time_ms > 10000) { // 10s
        //     break; // 超过 10s 就退出，避免一直阻塞
        //   }
        // }
        // button_up = pdTRUE;
    
        // time_ms = millis() - time_ms;


        if (button_down == pdTRUE && button_up == pdTRUE && time_ms > 0) {
          // printf("\n[button_handler_task] button pressed for %d ms", time);
          button_down = pdFALSE;
          button_up = pdFALSE;

          if (time_ms < 1000) { // 1s
            short_press = pdTRUE;
            printf("\n[button_handler_task] GPIO %d short press", GPIO_PIN);

          } else {
            long_press = pdTRUE;
            printf("\n[button_handler_task] GPIO %d long press", GPIO_PIN);
          }

          // 这里模拟过压保护的触发
          // xSemaphoreGive(voltage_protection_xBinarySemaphore);
          printf("\n[button_handler_task] duration: %d ms", time_ms);
          if (GPIO_PIN == encoder1_button.pin){
            // 按下编码器俺家之后切换模式
            if (encoder1.mode == QUAD) {
              encoder1.mode = SINGLE;
              printf("\n[button_handler_task] encoder1 mode changed to SINGLE");
            } else {
              encoder1.mode = QUAD;
              printf("\n[button_handler_task] encoder1 mode changed to QUAD");
            }
          }else if(GPIO_PIN == button1.pin){
            
          }else if(GPIO_PIN == button2.pin){
            
          }else if(GPIO_PIN == button3.pin){
            
          }else if(GPIO_PIN == button4.pin){
            // 从 chart -> main sw
            ui_load_scr_animation(&guider_ui, &guider_ui.main_page, guider_ui.main_page_del, &guider_ui.chart_page_del, setup_scr_main_page, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
          }
        }
      } else {
        // printf("\n[button_handler_task] failed to receive messag e from queue\n");
      }
    }
  }
}


void ADC1_read_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    // printf("\n[ADC1_read_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    
    msg.device_id = DEVICE_ADC1;

    float adc_value_3 = MY_ADC_GPIO6.get_ADC1_voltage_average_mV();

    float adc_value_average = (adc_value_3) / 1.0;

    msg.value = adc_value_average;
   
    // printf("[ADC1_read_task] ADC1_CHANNEL_5:%.3f\n", adc_value_3);

    int return_value = xQueueSend(sensor_queue_handle, (void *)&msg, 0);
    if (return_value == pdTRUE) {
      // printf("\n[ADC1_read_task] sent message  to the queue successfully\n");
    } else if (return_value == errQUEUE_FULL) {
      // printf("\n[ADC1_read_task] failed to send message to queue, queue is full\n");
    } else {
      // printf("\n[ADC1_read_task] failed to send message to queue\n");
    }

    vTaskDelay( 1000 );
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

#define USE_INA226_MODULE 1

void setup() {

  /* 系统和一系列初始化 */
  system_init(); // 打印系统信息


  // lv_init(); // 初始化 LVGL 必须运行的
  // lv_port_disp_init(); // 初始化绑定显示接口
  // lv_port_indev_init(); // 初始化和绑定触摸接口

  // 初始化 Wire IIC 总线
  Wire.begin(IIC_SDA, IIC_SCL);
  /* 初始化 INA226 */
#ifdef USE_INA226_MODULE
  // 下面的是校准模块上的 INA226 得到的参数
  float shunt = 0.010;     // R010  
  float current_LSB_mA = 0.05;
  const float CURRENT_OFFET_mA = -0.525; // 校准程序串口输出的电流偏置
  const float BUS_V_DMM = 6.013; // DMM 数字万用表测量的电压
  const float BUS_V_SERIAL= 6.109; // 校准程序测量的电压
  uint16_t bus_V_scaling_e4 = 10000 / BUS_V_SERIAL * BUS_V_DMM; 
#else
  // 下面的是校准功率板上的 INA226 得到的参数
  float shunt = 0.020;     // R020  
  float current_LSB_mA = 0.05;
  const float CURRENT_OFFET_mA = -0.525; // 校准程序串口输出的电流偏置
  const float BUS_V_DMM = 6.013; // DMM 数字万用表测量的电压
  const float BUS_V_SERIAL= 6.109; // 校准程序测量的电压
  uint16_t bus_V_scaling_e4 = 10000 / BUS_V_SERIAL * BUS_V_DMM; 
#endif 
         

  if (!INA226_device.begin()) {
    printf("could not connect INA226. Fix and Reboot");
  }
  INA226_device.configure(shunt, current_LSB_mA, CURRENT_OFFET_mA, bus_V_scaling_e4);

  /* 初始化 MCP4725 */
  if (!MCP4725_device.begin()) {
    printf("could not connect MCP4725. Fix and Reboot");
  }
  MCP4725_device.setMaxVoltage(3.3); // 设置最大输出电压
  MCP4725_device.setVoltage(3.0); // 设置输出电压为 3.3V


  /* 设置自己的显示任务 */
  // setup_ui(&guider_ui); // 初始化 gui_guider
  
  // // /* 挂起 GUI guider 生成的页面 */
  // setup_scr_main_page(&guider_ui); // gui_guider 为每一个页面生成的，这里是名字为 xxx 的页面
  // setup_scr_chart_page(&guider_ui); 
  // // lv_scr_load(guider_ui.main_page); //每一个页面的名字都是 gui_guider 结构体的元素
  // lv_scr_load(guider_ui.chart_page); 
  
  /* 或者运行 LVGL demo */
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

  // gui_xMutex = xSemaphoreCreateMutex(); // 创建一个互斥信号量
  // if (gui_xMutex == NULL) {
  //   // Handle semaphore creation failure
  //   printf("semaphore creation failure");
  //   return;
  // }

  // Core 0 运行 LVGL main task handler
  // xTaskCreatePinnedToCore(lvgl_task,
  //             "demo_test",
  //             1024*10,
  //             NULL,
  //             3,
  //             &lvgl_task_handle,
  //             0
  //           );

  // Core 1 运行（获取传感器数据任务）+ （更新 GUI 任务）
  // xTaskCreatePinnedToCore(get_dummy_sensor_data_task,
  //             "get_sensor_data_task",
  //             1024*4,
  //             NULL,
  //             2,
  //             NULL,
  //             1
  //           );
  // xTaskCreatePinnedToCore(uart0_event_task, 
  //             "uart1_event_task", 
  //             1024*4, 
  //             NULL, 
  //             2, 
  //             NULL,
  //             1
  //         );

  
  // xTaskCreatePinnedToCore(update_gui_task,
  //             "update_gui_task",
  //             1024*4,
  //             NULL,
  //             1,
  //             NULL,
  //             0
  //           );

  // xTaskCreatePinnedToCore(get_encoder1_data_task,
  //             "get_encoder1_data_task",
  //             1024*4,
  //             NULL,
  //             2,
  //             NULL,
  //             1
  //           );

  // voltage_protection_xBinarySemaphore = xSemaphoreCreateBinary();
  // if (voltage_protection_xBinarySemaphore != NULL) {
  //   // Core 1 运行过压保护任务
  //   xTaskCreatePinnedToCore(voltage_protection_task,
  //             "voltage_protection_task",
  //             1024*4,
  //             NULL,
  //             4, // 必须是最高优先级
  //             NULL,
  //             1
  //           );
  // }

  xTaskCreatePinnedToCore(get_ina226_data_task,
              "get_ina226_data_task",
              1024*4,
              NULL,
              2,
              NULL,
              1
            );

  xTaskCreatePinnedToCore(ADC1_read_task,
              "ADC1_read_task",
              1024*4,
              NULL,
              2,
              NULL,
              1
            );

  /* 使用这下面的按键做 GPIO 硬件中断*/
  // 创建二值信号量
  // button_xBinarySemaphore = xSemaphoreCreateBinary();
  // if (button_xBinarySemaphore != NULL) {
  //   // Core 1 运行按键处理任务
  //   xTaskCreatePinnedToCore(button_handler_task,
  //             "button_task",
  //             1024*4,
  //             NULL,
  //             5, // 必须是最高优先级
  //             NULL,
  //             1
  //           );

  //   gpio_install_isr_service(0); // 不能放到 button 类里面，因为本次安装会安装到所有的 GPIO 中断服务函数，只需要安装一次即可。否则看门狗跑丢
  //   gpio_isr_handler_add(button1.pin, button1_press_ISR, (void *) &button1.pin); // 绑定 GPIO 中断处理函数
  //   gpio_isr_handler_add(button2.pin, button2_press_ISR, (void *) &button2.pin); // 绑定 GPIO 中断处理函数
  //   gpio_isr_handler_add(button3.pin, button3_press_ISR, (void *) &button3.pin); // 绑定 GPIO 中断处理函数
  //   gpio_isr_handler_add(button4.pin, button4_press_ISR, (void *) &button4.pin); // 绑定 GPIO 中断处理函数
  //   gpio_isr_handler_add(encoder1_button.pin, encoder1_button_press_ISR, (void *) &encoder1_button.pin); // 绑定 GPIO 中断处理函数
    
  //   // 设置 GPIO 中断类型
  // }
  /************** 从上面开始不使用 **************/ 

}


void loop() {
  

}
