/**
 * @file main.cpp
 * @brief 主函数，包含了 ESP32S3 的初始化和任务调度
 * @author skyswordx
 * @details 该文件包含了 ESP32S3 的初始化和任务调度，使用 FreeRTOS 实现多任务调度
 *          包含了 LVGL 的初始化和任务调度
 *          编码器类和自定义的 PID 控制器类
 *          传感器(INA226、MCP4725)类、自定义 ADC 类和 GPIO 按键类
 */

#include "our_config.hpp"
#include "our_lvgl_interaction.h"
#include "our_encoder.hpp"
#include "our_sensors.hpp"
#include "our_adc.hpp"
#include "our_button.hpp"


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
  MCP4725_device.setMaxVoltage(5.0); // 设置最大输出电压
  MCP4725_device.setVoltage(1); // 设置输出电压为 3.3V


  /* 设置自己的显示任务 */
  setup_ui(&guider_ui); // 初始化 gui_guider
  
  // /* 挂起 GUI guider 生成的页面 */
  setup_scr_main_page(&guider_ui); // gui_guider 为每一个页面生成的，这里是名字为 xxx 的页面
  setup_scr_chart_page(&guider_ui); 
  // lv_scr_load(guider_ui.main_page); //每一个页面的名字都是 gui_guider 结构体的元素
  lv_scr_load(guider_ui.chart_page); 
  
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

  
  xTaskCreatePinnedToCore(update_gui_task,
              "update_gui_task",
              1024*4,
              NULL,
              1,
              NULL,
              0
            );

  xTaskCreatePinnedToCore(get_encoder1_data_task,
              "get_encoder1_data_task",
              1024*4,
              NULL,
              2,
              NULL,
              1
            );

  voltage_protection_xBinarySemaphore = xSemaphoreCreateBinary();
  if (voltage_protection_xBinarySemaphore != NULL) {
    // Core 1 运行过压保护任务
    xTaskCreatePinnedToCore(voltage_protection_task,
              "voltage_protection_task",
              1024*4,
              NULL,
              4, // 必须是最高优先级
              NULL,
              1
            );
  }

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
  button_xBinarySemaphore = xSemaphoreCreateBinary();
  if (button_xBinarySemaphore != NULL) {
    // Core 1 运行按键处理任务
    xTaskCreatePinnedToCore(button_handler_task,
              "button_task",
              1024*4,
              NULL,
              5, // 必须是最高优先级
              NULL,
              1
            );

    gpio_install_isr_service(0); // 不能放到 button 类里面，因为本次安装会安装到所有的 GPIO 中断服务函数，只需要安装一次即可。否则看门狗跑丢
    gpio_isr_handler_add(button1.pin, button1_press_ISR, (void *) &button1.pin); // 绑定 GPIO 中断处理函数
    gpio_isr_handler_add(button2.pin, button2_press_ISR, (void *) &button2.pin); // 绑定 GPIO 中断处理函数
    gpio_isr_handler_add(button3.pin, button3_press_ISR, (void *) &button3.pin); // 绑定 GPIO 中断处理函数
    gpio_isr_handler_add(button4.pin, button4_press_ISR, (void *) &button4.pin); // 绑定 GPIO 中断处理函数
    gpio_isr_handler_add(encoder1_button.pin, encoder1_button_press_ISR, (void *) &encoder1_button.pin); // 绑定 GPIO 中断处理函数
    
    // 设置 GPIO 中断类型
  }
  /************** 从上面开始不使用 **************/ 

}


void loop() {
  

}
