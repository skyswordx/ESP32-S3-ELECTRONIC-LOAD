/**
 * @file our_config.cpp
 * @brief ESP32S3 配置文件，包含了工程用到的所有配置和全局变量的声明
 * @author skyswordx
 * @details 全局变量在源文件中定义，头文件中声明 extern 这样可以避免重复定义
 */

#include "our_tasks_config.hpp"
#include "our_lvgl_interaction.h"
#include "our_encoder.hpp"
#include "our_adc.hpp"
#include "our_button.hpp"

BaseType_t debug_flag1 = pdFALSE;
BaseType_t debug_flag2 = pdFALSE;

/****************************** OUTPUT CAlibration *******************************/

#ifdef USE_OUTPUT_CALIBRATION // 启用输出校准
SemaphoreHandle_t output_calibration_xBinarySemaphore; // 输出校准二值信号量
extern double targ_data[OUTPUT_CALIBRATION]; // 输出测试数据-目标值
extern double real_data[OUTPUT_CALIBRATION]; // 输出测试数据-实际值

/**
 * @brief 开环输出的校准测试函数
 * @author Triwalt
 * @details 该函数应该在任务中循环调用，进行输出校准测试
 */
void output_data_collection_task(void *pvParameters) {

}
#endif

/*********************************** PID Setup ***********************************/
// 包含自定义的 PID 控制器类和 VOFA 下位机
#ifdef USE_PID_CONTROLLER
    #include "our_pid_controller.hpp"
    PID_controller_t<double> current_ctrl(DAC_OUTPUT_V_MAX, DAC_OUTPUT_V_MIN, CURRENT_TASK_KP, CURRENT_TASK_KI, CURRENT_TASK_KD);  

    void set_current_task(void *pvParameters) {
      // 该任务用于设置电流值
      while (1) {
        // 等待二值信号量触发任务开始
        // printf("[set_current_task] 闭环控制设置电流值中\n");

        current_ctrl.pid_control_service();

        // vTaskDelay(100 / portTICK_PERIOD_MS); // 延时 100 ms
        // printf("Actual DAC output: %.3f(V)\n", MCP4725_device.getVoltage()); 
      }
    }

    #ifdef USE_CURRENT_OPEN_LOOP_TEST
      SemaphoreHandle_t open_loop_test_xBinarySemaphore; // 开环测试二值信号量
      double pv_data[OPEN_LOOP_TEST_LENGTH] = {}; // 开环测试数据
      double op_data[OPEN_LOOP_TEST_LENGTH] = {}; // 开环测试数据

      /**
       * @brief PID 控制器的开环阶跃测试函数
       * @author skyswordx
       * @details 该函数应该在任务中循环调用，进行 PID 控制器的开环阶跃测试
       *          主要用于获取阶跃响应中的过程变量和控制器输出的数据
       *          以便使用 Lambda 整定法进行参数整定
       */
      void open_loop_data_collection_task(void *pvParameters) {

        // 采样间隔计算(以毫秒为单位)
        const int sample_interval_ms = (OPEN_LOOP_T1_ms + OPEN_LOOP_T2_ms) / OPEN_LOOP_TEST_LENGTH;
        
        while (1) {
          // 等待二值信号量触发任务开始
          xSemaphoreTake(open_loop_test_xBinarySemaphore, portMAX_DELAY);
          INA226_device.setShuntVoltageConversionTime(0);
          printf ("\n[open_loop_data_collection_task] 当前ina226电流转换时间: %dus\n", INA226_device.getShuntVoltageConversionTime());
          printf("[open_loop_data_collection_task] 开始采集开环测试数据\n");
          
          // 将数组清零
          memset(pv_data, 0, sizeof(pv_data));
          memset(op_data, 0, sizeof(op_data));
          
          // 第一阶段：MCP输出0V
          MCP4725_device.setVoltage(0.0);
          printf("[open_loop_data_collection_task] MCP输出电压设置为0V\n");
          
          // 采集第一阶段数据
          int sample_count = 0;
          int max_samples_phase1 = OPEN_LOOP_T1_ms / sample_interval_ms;
          
          for (int i = 0; i < max_samples_phase1 && sample_count < OPEN_LOOP_TEST_LENGTH; i++, sample_count++) {
            // 记录当前输出电压到op_data
            op_data[sample_count] = 0.0;
            // 读取并记录INA226电流值到pv_data
            pv_data[sample_count] = INA226_device.getCurrent_mA();
            
            // 等待采样间隔
            vTaskDelay(sample_interval_ms / portTICK_PERIOD_MS / 4);
          }
          
          // 第二阶段：MCP输出5.0V
          MCP4725_device.setVoltage(5.0);
          printf("[open_loop_data_collection_task] MCP输出电压设置为5.0V\n");
          
          // 采集第二阶段数据
          int max_samples_phase2 = OPEN_LOOP_T2_ms / sample_interval_ms;
          
          for (int i = 0; i < max_samples_phase2 && sample_count < OPEN_LOOP_TEST_LENGTH; i++, sample_count++) {
            // 记录当前输出电压到op_data
            op_data[sample_count] = 5.0;
            // 读取并记录INA226电流值到pv_data
            pv_data[sample_count] = INA226_device.getCurrent_mA();
            
            // 等待采样间隔
            vTaskDelay(sample_interval_ms / portTICK_PERIOD_MS / 4);
          }
          
          // 测试结束后将MCP输出电压设置回0V
          MCP4725_device.setVoltage(0.0);
          printf("[open_loop_data_collection_task] 测试完成，MCP输出电压设置为0V\n");
          
          // 输出采集到的数据
          printf("[open_loop_data_collection_task] 共采集数据 %d 个\n", sample_count);
          printf("序号,输出电压(V),测量电流(mA)\n");
          
          printf("op,pv\n");
          for (int i = 0; i < sample_count; i++) {
            // printf("[%d]:%.3f,%.3f\n", i, op_data[i], pv_data[i]);
            printf("%.3f,%.3f\n", op_data[i], pv_data[i]);
          }
        }
        vTaskDelete(NULL); // 删除任务
      }
    #endif
#endif
/*************************************** Encoder Setup *****************************/
#ifdef USE_ENCODER1 
  encoder_handle_t encoder1(ENCODER_1_PIN_A, ENCODER_1_PIN_B);
#endif



/*********************************** INA226 & MCP4725 Setup *************************/
#ifdef USE_IIC_DEVICE
  INA226 INA226_device(0x40); // INA226 电流传感器
  MCP4725 MCP4725_device(0x60); // MCP4725 DAC 芯片

  /*********************************** Current Measurement Setup *************************/
  SemaphoreHandle_t  load_testing_xBinarySemaphore;
  BaseType_t testing_load_flag = pdFALSE; // 测试负载标志位
#endif

/***************************************** ADC1 Setup *******************************/
#ifdef USE_ADC1
  ADC_channel_handler_t MY_ADC_GPIO6(ADC1_CHANNEL_5, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 64, ADC_UNIT_1);
#endif
/************************************* GPIO-button Setup ****************************/
#ifdef USE_BUTTON1
  GPIO_button_handler_t button1(GPIO_NUM_2); // GPIO 2 作为按键输入引脚
#endif
#ifdef USE_BUTTON2
  GPIO_button_handler_t button2(GPIO_NUM_1);
#endif
#ifdef USE_BUTTON3
  GPIO_button_handler_t button3(GPIO_NUM_7);
#endif
#ifdef USE_BUTTON4
  GPIO_button_handler_t button4(GPIO_NUM_15); 
#endif
#ifdef USE_ENCODER1
  GPIO_button_handler_t encoder1_button(GPIO_NUM_16); // 旋转编码器的按键引脚
#endif


/************************************* LVGL-SetUP ***********************************/
#ifdef USE_LCD_DISPLAY
  // Use hardware SPI
  TFT_eSPI tft = TFT_eSPI();
  // GUI guider
  lv_ui guider_ui;
#endif

/************************************* RTOS-SetUP ***********************************/
#ifdef USE_LCD_DISPLAY
  TaskHandle_t lvgl_task_handle; // LVGL 任务结构句柄
  SemaphoreHandle_t gui_xMutex;  // gui 互斥锁句柄，LVGL 线程不安全，需要加锁
#endif

#ifdef USE_BUTTON
  SemaphoreHandle_t button_xBinarySemaphore; // 按键二值信号量
  QueueHandle_t button_queue_handle; // 按键消息队列句柄
#endif

#ifdef USE_VOLTAGE_PROTECTION
  SemaphoreHandle_t voltage_protection_xBinarySemaphore; // 过压保护二值信号量
#endif


// GUI 更新使用的消息队列
QueueHandle_t sensor_queue_handle; // 消息队列句柄
const int queue_element_size = 10; // 消息队列元素大小


/************************************** tasks ***************************************/

#ifdef USE_DUMMY_SENSOR
/**
 * @brief 模拟获得传感器数据的任务函数
 * @author skyswordx
 */
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

    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
}
#endif // USE_DUMMY_SENSOR

#ifdef USE_IIC_DEVICE


/**
 * @brief 获取 INA226 数据的任务函数
 * @author skyswordx
 * @details 该任务会一直运行，获取 INA226 的数据并发送到消息队列中
 */
void get_ina226_data_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    // printf("\n[get_ina226_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));

    // printf("\n[get_ina226_data_task] now_time: %d", millis());
    msg.device_id = DEVICE_INA226;
    
    // 读取 INA226 的数据(修正后)
    // 使用 plus 版本
    double measure_current_mA = INA226_device.getCurrent_mA_plus(); // 读取电流值（plus版）
    double measure_voltage_V = INA226_device.getBusVoltage_plus(); // 读取电压值（plus版）
    double measure_power_W = abs((measure_current_mA * measure_voltage_V) / 1000); // 功率值
    double measure_resistance_Kohm = abs((measure_voltage_V )/ (measure_current_mA ));

    msg.device_data.value1 = measure_current_mA;
    msg.device_data.value2 = measure_voltage_V;
    msg.device_data.value3 = measure_power_W;
    msg.device_data.value4 = measure_resistance_Kohm;

    double DAC_output_V = MCP4725_device.getVoltage();
    // printf("\n[get_sensors_task] INA226: %.3f mA, %.3f V, DAC set: %.3f V, I_target: %.3f A", measure_current_mA, measure_voltage_V, DAC_output_V, (DAC_output_V/(125 * RSHUNT)));
    printf("\n[get_ina226_data_task] INA226 V/mA: %.3f,%.3f", measure_voltage_V, measure_current_mA); 

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


void get_load_changing_rate_task(void *pvParameters){
  /* 负载调整率计算
   *  到达 check_current1 = ->  记录 bus_voltage1
   *  到达 check_current2 = ->  记录 bus_voltage2
   * rate = (bus_V1 - bus_V0) / bus_V0
   */
  static double check_voltage_L_V = 0.0;
  static double check_voltage_H_V = 0.0;
  static double check_current_L_A = 0.0;
  static double check_current_H_A = 0.0; 
  BaseType_t check_L = pdFALSE;
  BaseType_t check_H = pdFALSE;
  static double rate = 0.0;

  message_t msg;

  while(1){
    #ifdef USE_IIC_DEVICE
      xSemaphoreTake(load_testing_xBinarySemaphore, portMAX_DELAY); // 总是保持阻塞等待二值信号量
      testing_load_flag = pdTRUE; // 设置测试负载标志位
      
      #ifdef USE_PID_CONTROLLER
          // 这里是 PID 控制器的电流值
          current_ctrl.process_variable.target = TESING_MIN_CURRENT_A * 1000; // 设置 PID 控制器的目标值
      #else
          // 这里是 DAC 的电流值
          MCP4725_device.setVoltage(from_set_current2voltage_V(TESING_MIN_CURRENT_A)); // 设置输出电压为第一个电流值
      #endif // 设置输出电压为第一个电流值
      vTaskDelay(100 / portTICK_PERIOD_MS); // 延时 100 ms

      printf("\nFrom TEST_MIN_CURRENT to set DAC: %.3f(V)", MCP4725_device.getVoltage());

      check_current_L_A = INA226_device.getCurrent(); // 读取电流值
      if (abs(check_current_L_A - TESING_MIN_CURRENT_A) < THRESHOLD_A){
        check_voltage_L_V = INA226_device.getBusVoltage_plus(); // 读取电压值
        printf("\n[get_load_changing_rate_task] check_voltage_L: %.3f (V), check_current_L: %.3f (A)", check_voltage_L_V, check_current_L_A);
        check_L = pdTRUE;
      }

      
      #ifdef USE_PID_CONTROLLER
          // 这里是 PID 控制器的电流值
          current_ctrl.process_variable.target = TESING_MAX_CURRENT_A * 1000; // 设置 PID 控制器的目标值
      #else
          // 这里是 DAC 的电流值
          MCP4725_device.setVoltage(from_set_current2voltage_V(TESING_MAX_CURRENT_A)); // 设置输出电压为第一个电流值
      #endif  // 设置输出电压为第二个电流值
      vTaskDelay(200 / portTICK_PERIOD_MS); // 延时 200 ms
      printf("\n From TEST_MAX_CURRENT to set DAC: %.3f (V)", MCP4725_device.getVoltage());

      check_current_H_A = INA226_device.getCurrent(); // 读取电流值
      if ( abs(check_current_H_A - TESING_MAX_CURRENT_A) < THRESHOLD_A){
        check_voltage_H_V = INA226_device.getBusVoltage_plus(); // 读取电压值
        printf("\n[get_load_changing_rate_task] check_voltage_H: %.3f (V), check_current_H: %.3f (A)", check_voltage_H_V, check_current_H_A);
        check_H = pdTRUE;

      }
      
      MCP4725_device.setVoltage(0.0); // 设置输出电压为 0V
      printf("\n[get_load_changing_rate_task] DAC output voltage set to 0V");
      
      if (check_L == pdTRUE && check_H == pdTRUE){
        // 计算负载变化率
        rate =(check_voltage_L_V - check_voltage_H_V ) / check_voltage_L_V;
        printf("\n[get_load_changing_rate_task] load changing rate: %.3f\n", rate);
        check_L = pdFALSE;
        check_H = pdFALSE;

        msg.device_id = EVENT_TESING_LOAD_RATE;

        msg.value = rate; // 发送负载变化率到消息队列
  
        int return_value = xQueueSend(sensor_queue_handle, (void *)&msg, 0);
        if (return_value == pdTRUE) {
          printf("\n[get_load_changing_rate_task] sent message to the queue successfully\n");
        } else if (return_value == errQUEUE_FULL) {
          printf("\n[get_load_changing_rate_task] failed to send message to queue, queue is full\n");
        } else {
          printf("\n[get_load_changing_rate_task] failed to send message to queue\n");
        }

        testing_load_flag = pdFALSE; // 清除测试负载标志位
      }

    #endif // USE_IIC_DEVICE
    
  }
}


#endif // USE_IIC_DEVICE

#ifdef USE_VOLTAGE_PROTECTION
/**
 * @brief 过压保护任务函数
 * @author skyswordx
 * @details 该任务会一直运行，等待过压保护信号量
 *          过压保护信号量由 get_ina226_data_task 任务触发
 */
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
#endif // USE_VOLTAGE_PROTECTION
  
/**
 * @brief ADC1 读取任务
 * @author skyswordx
 * @details 该任务用于读取 ADC1 的电压值，并将其发送到消息队列中
 * @param pvParameters 任务参数
 * @return void* 任务返回值
 */

#ifdef USE_ADC1
void ADC1_read_task(void *pvParameters)
{
  message_t msg;
  while(1)
  {
    // printf("\n[ADC1_read_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    
    msg.device_id = DEVICE_ADC1;

    double adc_value_3 = MY_ADC_GPIO6.get_ADC1_voltage_average_mV();

    double adc_value_average = adc_value_3 / 1.0;

    msg.value = adc_value_average;

    printf("\n[ADC1_read_task] ADC1_CHANNEL_5: %.3f", adc_value_3);
    
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
#endif // USE_ADC1



#ifdef USE_BUTTON
/**
 * @brief 按键中断辅助处理任务
 * @author skyswordx
 * @details 该任务日常被阻塞在二值信号量上，必须等待中断函数释放信号量才会被唤醒
 *          该任务的优先级必须高于其他函数的优先级，否则会导致中断处理逻辑无法正常工作
 */
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
          #ifdef USE_ENCODER1
            if (GPIO_PIN == encoder1_button.pin){
              // 按下编码器俺家之后切换模式
              if (encoder1.mode == QUAD) {
                encoder1.mode = SINGLE;
                printf("\n[button_handler_task] encoder1 mode changed to SINGLE");
              } else {
                encoder1.mode = QUAD;
                printf("\n[button_handler_task] encoder1 mode changed to QUAD");
              }
            }
          #endif // USE_ENCODER1
          #ifdef USE_BUTTON1
            if(GPIO_PIN == button1.pin){
              #ifdef USE_CURRENT_OPEN_LOOP_TEST
                xSemaphoreGive(open_loop_test_xBinarySemaphore); // 释放二值信号量，触发开环测试
              #endif
              #ifdef USE_OUTPUT_CALIBRATION
                xSemaphoreGive(output_calibration_xBinarySemaphore); // 释放二值信号量，触发输出标定
              #endif
            }
          #endif // USE_BUTTON1
          #ifdef USE_BUTTON2
            if(GPIO_PIN == button2.pin){
              #ifdef USE_IIC_DEVICE
                xSemaphoreGive(load_testing_xBinarySemaphore); // 释放二值信号量，触发负载测试
              #endif // IIC_DEVICE
              
            }
          #endif // USE_BUTTON2
          #ifdef USE_BUTTON3
            if(GPIO_PIN == button3.pin){
              #ifdef USE_IIC_DEVICE
                MCP4725_device.setVoltage(0.0); // 设置输出电压为 0V
              #endif // IIC_DEVICE
              
            }
          #endif // USE_BUTTON3
          #ifdef USE_BUTTON4
            if(GPIO_PIN == button4.pin){
              /* 页面切换 */
              // 从 chart -> main sw
              // ui_load_scr_animation(&guider_ui, &guider_ui.main_page, guider_ui.main_page_del, &guider_ui.chart_page_del, setup_scr_main_page, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);

              #ifdef USE_IIC_DEVICE
                MCP4725_device.setVoltage(2.5); // 设置输出电压为 0V
              #endif // IIC_DEVICE
            
            }
          #endif // USE_BUTTON4
          }
        } else {
          // printf("\n[button_handler_task] failed to receive messag e from queue\n");
        }
      }
    }
  }
#endif // USE_BUTTON

#ifdef USE_ENCODER1
/**
 * @brief 获取编码器数据的任务函数，用来设置电流
 * @author skyswordx
 * @param pvParameters 任务参数
 * @details 仅在 setup 调用一次即可，该任务会一直运行，获取编码器的数据（期望电流）转换所需相应的 DAC 电压输出
 *          并把期望电流数据发送到消息队列中给 LVGL 更新
 */
void get_encoder1_data_task(void *pvParameters)
{
  message_t msg;

  
  while(1)
  {
    // printf("\n[get_encoder1_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    
    // 获取旋转编码器数据
    msg.device_id = DEVICE_ENCODER;

    msg.value = encoder1.read_count_accum_clear(); // 获取编码器的总计数值
    // printf("\n[get_encoder1_data_task] encoder1 value: %.3f", msg.value);

    // 进行电流调节映射，把编码器的值进行限幅
    if (msg.value > 1800.0) {
      msg.value = 1800.0;
      // encoder1.set_count(1800.0); // 设置编码器的值为 1800.0，避免溢出
      // printf("\n[get_encoder1_data_task] upper limit is %.3f A", msg.value);
    } else if (msg.value < 50) {
      msg.value = 50;
      // encoder1.set_count(50); // 设置编码器的值为 50.0，避免溢出
      // printf("\n[get_encoder1_data_task] lower limit is %.3f A", msg.value);
    }
  
    // printf("\n[get_encoder1_data_task] encoder1 value: %.3f", msg.value);
    /* 实现电流调节 */
    #ifdef USE_IIC_DEVICE
      // 计算设置电压
    
      if (testing_load_flag == pdFALSE){
        #ifdef USE_PID_CONTROLLER
          // 这里是 PID 控制器的电流值
          current_ctrl.process_variable.target = msg.value; // 设置 PID 控制器的目标值
          printf("\n[get_encoder1_data_task] setpoint current(A): %.3f", msg.value);
        #else
          // 未启用 PID 控制器
          printf("\n[get_encoder1_data_task] setpoint current(mA): %.3f", msg.value);
          MCP4725_device.setVoltage(from_set_current2voltage_V(msg.value/1000.0)); // 设置 DAC 的目标值
          // printf("\n[get_encoder1_data_task] setpoint current(A): %.3f", msg.value);
        #endif
      }
  
      // printf("\n[get_encoder1_data_task] setpoint current(A): %.3f", msg.value);
      printf("\n[get_encoder1_data_task] setpoint voltage(V): %.3f", MCP4725_device.getVoltage());
    
    #endif
    
    // printf("\n[get_encoder1_data_task] encoder1 count: %lld", count);
    
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

/*********************** 辅助函数 ***********************/
#ifdef USE_IIC_DEVICE
  double from_set_current2voltage_V(double set_current_A){
    // const double DAC_OUTPUT_Calibration_A = 1.1530154317483; // DAC 输出校准系数A
    // const double DAC_OUTPUT_Calibration_B = -43.853404582016; // DAC 输出校准系数B
    // return (set_current_A * 1000 * DAC_OUTPUT_Calibration_A + DAC_OUTPUT_Calibration_B) * 0.125 * RSHUNT; // 计算电压值
    return (set_current_A * 125 * RSHUNT); // 计算电压值
  }

#endif

#endif // USE_ENCODER1