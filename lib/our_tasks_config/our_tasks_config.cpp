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
/******************************  ESP32S3 Setup ***********************************/
SemaphoreHandle_t startup_xBinarySemaphore;
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
      // 添加优先级状态管理变量
    static priority_state_t current_priority_state = PRIORITY_STATE_NORMAL;
    static double last_normal_current = 0.0; // 保存正常状态下的最后电流值，用于退出保护状态时恢复
    static TickType_t over_voltage_start_time = 0; // 过压保护开始时间
    
    /**
     * @brief 检查消息是否应该被接受（优先级仲裁）
     * @param message 接收到的消息
     * @return true 接受消息，false 拒绝消息
     */
    bool should_accept_message(const QueueElement_t<double>& message) {
        switch (current_priority_state) {
            case PRIORITY_STATE_NORMAL:
                // 正常状态：接受所有消息
                return true;
                
            case PRIORITY_STATE_OVER_VOLTAGE:
                // 过压保护状态：最高优先级，只接受过压相关消息
                // 忽略编码器和负载测试消息
                return (message.task_id == EVENT_OVER_VOLTAGE || 
                        message.task_id == EVENT_OVER_VOLTAGE_CLEAR);
                
            case PRIORITY_STATE_TESTING:
                // 负载测试状态：接受负载测试消息和过压消息（过压可以托管测试状态）
                // 忽略编码器消息，防止编码器退出测试状态
                return (message.task_id == EVENT_OVER_VOLTAGE || 
                        message.task_id == EVENT_TESING_LOAD_RATE ||
                        message.task_id == EVENT_TESTING_COMPLETE);
                
            case PRIORITY_STATE_EMERGENCY:
                // 紧急状态：只接受紧急消息
                return (message.task_id == EVENT_OVER_VOLTAGE);
                
            default:
                return false;
        }
    }/**
     * @brief 处理状态转换
     * @param message 触发状态转换的消息
     */
    void handle_state_transition(const QueueElement_t<double>& message) {
        switch (message.task_id) {
            case EVENT_OVER_VOLTAGE:
                // 过压消息具有最高优先级，可以从任何状态转入过压状态
                if (current_priority_state != PRIORITY_STATE_OVER_VOLTAGE) {
                    // 保存当前正常电流值（如果不是从过压状态转入）
                    if (current_priority_state == PRIORITY_STATE_NORMAL) {
                        last_normal_current = current_ctrl.process_variable.target;
                    }
                    current_priority_state = PRIORITY_STATE_OVER_VOLTAGE;
                    over_voltage_start_time = xTaskGetTickCount();
                    printf("\n[set_current_task] State transition to OVER_VOLTAGE protection");
                }
                break;
                  case EVENT_OVER_VOLTAGE_CLEAR:
                // 过压解除，返回正常状态
                if (current_priority_state == PRIORITY_STATE_OVER_VOLTAGE) {
                    current_priority_state = PRIORITY_STATE_NORMAL;
                    printf("\n[set_current_task] Over-voltage cleared, returned to NORMAL state");
                    // 注意：电流值恢复将在状态处理逻辑中统一管理
                }
                break;
                
            case EVENT_TESING_LOAD_RATE:
                // 负载测试消息只能在正常状态下启动测试
                if (current_priority_state == PRIORITY_STATE_NORMAL) {
                    last_normal_current = current_ctrl.process_variable.target;
                    current_priority_state = PRIORITY_STATE_TESTING;
                    printf("\n[set_current_task] State transition to TESTING mode");
                }
                // 如果在其他状态（如过压），忽略负载测试请求
                break;
                  case EVENT_TESTING_COMPLETE:
                // 负载测试完成，返回正常状态
                if (current_priority_state == PRIORITY_STATE_TESTING) {
                    current_priority_state = PRIORITY_STATE_NORMAL;
                    printf("\n[set_current_task] Load testing complete, returned to NORMAL state");
                    // 注意：电流值恢复将在状态处理逻辑中统一管理
                }
                break;
                
            case TASK_ENCODER:
                // 编码器消息不会主动改变状态，只在正常状态下工作
                // 不允许编码器消息退出测试或过压状态
                if (current_priority_state == PRIORITY_STATE_NORMAL) {
                    // 在正常状态下更新last_normal_current
                    last_normal_current = message.data;
                }
                break;
                
            default:
                break;
        }
    }
    
    /**
     * @brief 检查是否可以退出过压保护状态
     * @return true 可以退出，false 不能退出
     */
    bool can_exit_over_voltage_protection() {
        // 可以添加额外的条件检查，比如电压是否已经降到安全范围
        // 这里简单地检查是否在过压状态超过一定时间
        const TickType_t MIN_PROTECTION_TIME = pdMS_TO_TICKS(1000); // 最少保护1秒
        
        return (current_priority_state == PRIORITY_STATE_OVER_VOLTAGE && 
                (xTaskGetTickCount() - over_voltage_start_time) > MIN_PROTECTION_TIME);
    }
    
    /**
     * @brief 获取当前优先级状态（用于调试和监控）
     * @return 当前的优先级状态
     */
    priority_state_t get_current_priority_state() {
        return current_priority_state;
    }
    
    /**
     * @brief 强制退出当前优先级状态（紧急情况下使用）
     * @param force_state 要强制设置的状态
     */
    void force_priority_state(priority_state_t force_state) {
        printf("\n[set_current_task] Force state transition from %d to %d", current_priority_state, force_state);
        current_priority_state = force_state;
    }
    
    void set_current_task(void *pvParameters) {
      QueueElement_t<double> received_message;
      
      while (1) {
        // 阻塞等待队列消息
        if (xQueueReceive(current_control_queue, &received_message, portMAX_DELAY) == pdTRUE) {
          printf("\n[set_current_task] Received message from task_id: %d, data: %.3f mA, current_state: %d", 
                 received_message.task_id, received_message.data, current_priority_state);
          
          // 优先级仲裁：检查是否应该接受此消息
          if (!should_accept_message(received_message)) {
            printf("\n[set_current_task] Message rejected due to priority arbitration");
            continue; // 拒绝消息，继续等待下一个
          }
            // 处理状态转换
          handle_state_transition(received_message);
          
          // 根据当前优先级状态处理电流设定
          switch (current_priority_state) {
            case PRIORITY_STATE_OVER_VOLTAGE:
              // 过压保护状态：强制电流为0
              current_ctrl.process_variable.target = 0.0;
              printf("\n[set_current_task] OVER_VOLTAGE state: forcing current to 0 mA");
              break;
              
            case PRIORITY_STATE_TESTING:
              // 负载测试状态：使用消息中的电流值（来自测试任务）
              current_ctrl.process_variable.target = received_message.data;
              printf("\n[set_current_task] TESTING state: set current to %.3f mA", received_message.data);
              break;
                case PRIORITY_STATE_NORMAL:
              // 正常状态：使用消息中的电流值，并更新last_normal_current
              // 特殊处理：如果是状态退出事件，恢复之前保存的电流值
              if (received_message.task_id == EVENT_OVER_VOLTAGE_CLEAR || 
                  received_message.task_id == EVENT_TESTING_COMPLETE) {
                // 状态退出，恢复到之前保存的正常电流值
                current_ctrl.process_variable.target = last_normal_current;
                printf("\n[set_current_task] NORMAL state: restored current to %.3f mA", last_normal_current);
              } else {
                // 正常的电流设定，使用消息中的电流值并更新保存值
                current_ctrl.process_variable.target = received_message.data;
                last_normal_current = received_message.data;
                printf("\n[set_current_task] NORMAL state: set current to %.3f mA", received_message.data);
              }
              break;
              
            case PRIORITY_STATE_EMERGENCY:
              // 紧急状态：强制电流为0
              current_ctrl.process_variable.target = 0.0;
              printf("\n[set_current_task] EMERGENCY state: forcing current to 0 mA");
              break;
              
            default:
              printf("\n[set_current_task] Unknown priority state, ignoring message");
              continue;
          }
          
          // 执行PID控制
          current_ctrl.pid_control_service();
        }
        
        // 检查是否可以退出过压保护状态
        if (can_exit_over_voltage_protection()) {
            current_priority_state = PRIORITY_STATE_NORMAL;
            // 可选：恢复到之前的电流值
            // current_ctrl.process_variable.target = last_normal_current;
            printf("\n[set_current_task] Exited over-voltage protection, returned to NORMAL state");
        }
        
        // 短暂延时防止任务独占CPU
        vTaskDelay(1 / portTICK_PERIOD_MS);
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

#endif

#ifdef USE_VOLTAGE_PROTECTION
  BaseType_t over_voltage_protection_flag = pdFALSE; // 过压保护标志位
  BaseType_t over_voltage_igonre_pid_flag = pdFALSE; // 过压保护忽略 PID 标志位
  SemaphoreHandle_t over_voltage_protection_xBinarySemaphore; // 过压保护二值信号量
#endif


/************************************** tasks ***************************************/

#ifdef USE_DUMMY_SENSOR
/**
 * @brief 模拟获得传感器数据的任务函数
 * @author skyswordx
 */
void get_dummy_sensor_data_task(void *pvParameters)
{
  QueueElement_t<double> queue_element(TASK_DUMMY_SENSOR); // 定义一个队列元素对象
  while(1)
  {
    // printf("\n[get_sensor_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));

    // 模拟传感器数据
    queue_element.data = 4.0 + (rand() % 100) / 100.0;
   

    int return_value = xQueueSend(LVGL_queue, (void *)&queue_element, 0);
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

  QueueElement_t<double> queue_element1(TASK_INA226, DATA_DESCRIPTION_CURRENT); 
  QueueElement_t<double> queue_element2(TASK_INA226, DATA_DESCRIPTION_VOLTAGE); 
  QueueElement_t<double> queue_element3(TASK_INA226, DATA_DESCRIPTION_POWER); 
  QueueElement_t<double> queue_element4(TASK_INA226, DATA_DESCRIPTION_RESISTANCE); 
  while(1)
  {
    // printf("\n[get_ina226_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    // printf("\n[get_ina226_data_task] now_time: %d", millis());
    
    // 读取 INA226 的数据(修正后)
    // 使用 plus 版本
    double measure_current_mA = INA226_device.getCurrent_mA_plus(); // 读取电流值（plus版）
    double measure_voltage_V = INA226_device.getBusVoltage_plus(); // 读取电压值（plus版）
    double measure_power_W = abs((measure_current_mA * measure_voltage_V) / 1000); // 功率值
    double measure_resistance_ohm = abs((measure_voltage_V )/ (measure_current_mA) * 1000);


    queue_element1.data = measure_current_mA; 
    queue_element2.data = measure_voltage_V; 
    queue_element3.data = measure_power_W; 
    queue_element4.data = measure_resistance_ohm; 

    double DAC_output_V = MCP4725_device.getVoltage();
    // printf("\n[get_sensors_task] INA226: %.3f mA, %.3f V, DAC set: %.3f V, I_target: %.3f A", measure_current_mA, measure_voltage_V, DAC_output_V, (DAC_output_V/(125 * RSHUNT)));
    printf("\n[get_ina226_data_task] INA226 V/mA: %.3f,%.3f", measure_voltage_V, measure_current_mA);     // 检查 INA226 电压是否超过警告值，如果超过则进行过压保护
    if(measure_voltage_V >= WARNING_VOLTAGE){
      // 电压过高，警告
    
      // 这里触发过压之后的保护程序
      xSemaphoreGive(over_voltage_protection_xBinarySemaphore);
    }
    
    // 检查过压状态是否可以解除
    static bool last_over_voltage_state = false;
    bool current_over_voltage = (measure_voltage_V >= WARNING_VOLTAGE);
    
    if (last_over_voltage_state && !current_over_voltage) {
      // 从过压状态转为正常状态，发送过压解除消息
      QueueElement_t<double> over_voltage_clear_message(EVENT_OVER_VOLTAGE_CLEAR, DATA_DESCRIPTION_CURRENT_SETPOINT, 0.0);
      xQueueSend(current_control_queue, &over_voltage_clear_message, 0);
      printf("\n[get_ina226_data_task] Over-voltage cleared, sent EVENT_OVER_VOLTAGE_CLEAR message");
    }
    last_over_voltage_state = current_over_voltage;if (measure_voltage_V < WARNING_VOLTAGE) {
      // 电压正常，过压状态将在接收端自动管理
      #ifdef USE_LCD_DISPLAY
        // 获取LVGL互斥锁，确保LVGL操作线程安全
        if (xSemaphoreTake(gui_xMutex, portMAX_DELAY) == pdTRUE) {
          // 隐藏过压保护警告控件
          lv_obj_add_flag(guider_ui.main_page_over_voltage_warning_msgbox, LV_OBJ_FLAG_HIDDEN);
          printf("\n[get_ina226_data_task] LVGL warning cleared");
          xSemaphoreGive(gui_xMutex); // 释放互斥锁
        }
      #endif
      // 移除标志位清除，状态管理统一在接收端处理
    }
 
    int return_value1 = xQueueSend(LVGL_queue, (void *)&queue_element1, 0); // 发送电流值到消息队列
    int return_value2 = xQueueSend(LVGL_queue, (void *)&queue_element2, 0); // 发送电压值到消息队列
    int return_value3 = xQueueSend(LVGL_queue, (void *)&queue_element3, 0); // 发送功率值到消息队列
    int return_value4 = xQueueSend(LVGL_queue, (void *)&queue_element4, 0); // 发送电阻值到消息队列



    if (return_value1 == pdTRUE && return_value2 == pdTRUE && return_value3 == pdTRUE && return_value4 == pdTRUE) {
      // printf("\n[get_ina226_data_task] sent message  to the queue successfully\n");
    } else if ( return_value1 == errQUEUE_FULL || return_value2 == errQUEUE_FULL || return_value3 == errQUEUE_FULL || return_value4 == errQUEUE_FULL) {
      // printf("\n[get_ina226_data_task] failed to send message to queue, queue is full\n");
    } else {
      // printf("\n[get_ina226_data_task] failed to send message to queue\n");
    }

    vTaskDelay( 200 / portTICK_PERIOD_MS ); // 延时 200 ms
  }
}


void get_load_changing_rate_task(void *pvParameters){
  /* 负载调整率计算
   *  到达 check_current1 = ->  记录 bus_voltage1
   *  到达 check_current2 = ->  记录 bus_voltage2
   * rate = (bus_V1 - bus_V0) / bus_V0
   */
  double check_voltage_L_V = 0.0;
  double check_voltage_H_V = 0.0;
  double check_current_L_A = 0.0;
  double check_current_H_A = 0.0; 
  BaseType_t check_L = pdFALSE;
  BaseType_t check_H = pdFALSE;
  double rate = 0.0;

  QueueElement_t<double> queue_element(EVENT_TESING_LOAD_RATE); // 定义一个队列元素对象
  while(1){
    #ifdef USE_IIC_DEVICE
      xSemaphoreTake(load_testing_xBinarySemaphore, portMAX_DELAY); // 总是保持阻塞等待二值信号量
      printf("\n[get_load_changing_rate_task] Load rate testing started");
      // 移除标志位设置，状态管理统一在接收端处理
      
      #ifdef USE_PID_CONTROLLER
          // 发送测试消息到电流控制队列，接收端会自动管理状态
          QueueElement_t<double> test_message_min(EVENT_TESING_LOAD_RATE, DATA_DESCRIPTION_CURRENT_SETPOINT, TESING_MIN_CURRENT_A * 1000);
          xQueueSend(current_control_queue, &test_message_min, 0);
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
          // 发送测试消息到电流控制队列，接收端会自动管理状态
          QueueElement_t<double> test_message_max(EVENT_TESING_LOAD_RATE, DATA_DESCRIPTION_CURRENT_SETPOINT, TESING_MAX_CURRENT_A * 1000);
          xQueueSend(current_control_queue, &test_message_max, 0);
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
      
      // #ifdef USE_PID_CONTROLLER
      //   current_ctrl.process_variable.target = 0.0;
      // #else
      //   MCP4725_device.setVoltage(0.0); // 设置输出电压为 0V
      // #endif
      // printf("\n[get_load_changing_rate_task] DAC output voltage set to 0V");
      
      if (check_L == pdTRUE && check_H == pdTRUE){
        // 计算负载变化率
        rate =(check_voltage_L_V - check_voltage_H_V ) / check_voltage_L_V;
        printf("\n[get_load_changing_rate_task] load changing rate: %.3f\n", rate);
        check_L = pdFALSE;
        check_H = pdFALSE;        queue_element.data = rate; // 发送负载变化率到消息队列
  
        int return_value = xQueueSend(LVGL_queue, (void *)&queue_element, 0);
        if (return_value == pdTRUE) {
          printf("\n[get_load_changing_rate_task] sent message to the queue successfully\n");
        } else if (return_value == errQUEUE_FULL) {
          printf("\n[get_load_changing_rate_task] failed to send message to queue, queue is full\n");
        } else {
          printf("\n[get_load_changing_rate_task] failed to send message to queue\n");
        }        // 移除标志位清除，状态管理统一在接收端处理
        printf("\n[get_load_changing_rate_task] Load rate testing completed");
        
        // 发送测试完成消息，通知接收端退出测试状态
        QueueElement_t<double> test_complete_message(EVENT_TESTING_COMPLETE, DATA_DESCRIPTION_CURRENT_SETPOINT, 0.0);
        xQueueSend(current_control_queue, &test_complete_message, 0);
        printf("\n[get_load_changing_rate_task] Sent EVENT_TESTING_COMPLETE message");
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
void over_voltage_protection_task(void *pvParameters){

    QueueElement_t<alert_type_t> queue_element(EVENT_OVER_VOLTAGE);

    while(1){
      
    
      printf("\n[over_voltage_protection_task] waiting");
      xSemaphoreTake(over_voltage_protection_xBinarySemaphore, portMAX_DELAY); // 总是保持阻塞等待二值信号量
      // 移除标志位设置，状态管理统一在接收端处理
      
      printf("\n[over_voltage_protection_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
  
      // vTaskDelete(encoder1_task_handle); // 删除编码器任务
      // 执行过压保护任务，关闭 DAC 输出

      #ifdef USE_LCD_DISPLAY
        // 获取LVGL互斥锁，确保LVGL操作线程安全
        if (xSemaphoreTake(gui_xMutex, portMAX_DELAY) == pdTRUE) {
          // 显示过压保护警告控件（假设guider_ui中有overvoltage_warning控件）
          lv_obj_clear_flag(guider_ui.main_page_over_voltage_warning_msgbox, LV_OBJ_FLAG_HIDDEN);
          // lv_obj_set_style_opa(guider_ui.main_page, LV_OPA_60, LV_PART_MAIN | LV_STATE_DEFAULT);
          printf("\n[over_voltage_protection_task] LVGL warning");
          xSemaphoreGive(gui_xMutex); // 释放互斥锁
        }
      #endif      
      #ifdef USE_PID_CONTROLLER
        // 发送紧急优先级消息到电流控制队列（使用统一的QueueElement_t结构）
        QueueElement_t<double> emergency_message(EVENT_OVER_VOLTAGE, DATA_DESCRIPTION_EMERGENCY_STOP, 0.0);
        
        if (xQueueSend(current_control_queue, &emergency_message, 0) == pdTRUE) {
          printf("\n[over_voltage_protection_task] Sent emergency current setpoint message: 0.0 mA");
        } else {
          printf("\n[over_voltage_protection_task] Failed to send emergency message - queue full");
          // 如果队列满了，直接设置目标值作为备用方案
          current_ctrl.process_variable.target = 0.0;
        }
        // for (int i = 0; i < 1000; i++){
        //   printf("\n[over_voltage_protection_task] block %d", i);
        // } 
      #else
        MCP4725_device.setVoltage(0.0); // 设置输出电压为 0V
      #endif

      
      // 切断功率板电源，让 MOSFET 关断
      /* todo */
      
      
      // int return_value = xQueueSend(LVGL_queue, (void *)&queue_element, 0); // 发送过压保护值到消息队列

      // if (return_value == pdTRUE) {
      //   printf("\n[over_voltage_protection_task] sent message to the queue successfully\n"); // 添加成功发送消息的打印
      // } else if (return_value == errQUEUE_FULL) {
      //   printf("\n[over_voltage_protection_task] failed to send message to queue, queue is full\n"); // 添加队列满的打印
      // } else {
      //   printf("\n[over_voltage_protection_task] failed to send message to queue\n"); // 添加发送失败的打印
      // }


      printf("\n[over_voltage_protection_task] DAC output voltage set to 0V");
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
  QueueElement_t queue_element(TASK_ADC1); // 定义一个队列元素对象
  while(1)
  {
    // printf("\n[ADC1_read_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    
 

    double adc_value_3 = MY_ADC_GPIO6.get_ADC1_voltage_average_mV();

    double adc_value_average = adc_value_3 / 1.0;

    queue_element.data = adc_value_average; // 发送电压值到消息队列

    printf("\n[ADC1_read_task] ADC1_CHANNEL_5: %.3f", adc_value_3);
    
    int return_value = xQueueSend(LVGL_queue, (void *)&queue_element, 0);
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
      if ( button_queue != NULL){ // 检查消息队列是否创建成功
        if (xQueueReceive(button_queue, &GPIO_PIN, portMAX_DELAY) == pdTRUE) {
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
  
           
            printf("\n[button_handler_task] duration: %d ms", time_ms);
          #ifdef USE_ENCODER1
            if (GPIO_PIN == encoder1_button.pin){
              // 按下编码器按键之后切换模式
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
                xSemaphoreGive(startup_xBinarySemaphore);
              
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
              // 这里模拟过压保护的触发
              xSemaphoreGive(over_voltage_protection_xBinarySemaphore);
       
            }
          #endif // USE_BUTTON3
          #ifdef USE_BUTTON4
            if(GPIO_PIN == button4.pin){
              /* 页面切换 */
              // 从 chart -> main sw
              // ui_load_scr_animation(&guider_ui, &guider_ui.main_page, guider_ui.main_page_del, &guider_ui.chart_page_del, setup_scr_main_page, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);

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

TaskHandle_t encoder1_task_handle; // 旋转编码器任务结构句柄

/**
 * @brief 获取编码器数据的任务函数，用来设置电流
 * @author skyswordx
 * @param pvParameters 任务参数
 * @details 仅在 setup 调用一次即可，该任务会一直运行，获取编码器的数据（期望电流）转换所需相应的 DAC 电压输出
 *          并把期望电流数据发送到消息队列中给 LVGL 更新
 */
void get_encoder1_data_task(void *pvParameters)
{
  double value;
  QueueElement_t<double> queue_element1(TASK_ENCODER, DATA_DESCRIPTION_SET_CURRENT); 
  while(1)
  {
    // printf("\n[get_encoder1_data_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    
    // 获取旋转编码器数据
    value = encoder1.read_count_accum_clear(); // 获取编码器的总计数值
    printf("\n[get_encoder1_data_task] encoder1 value: %.3f", value);

    /* 实现电流调节 */
    #ifdef USE_IIC_DEVICE
      // 进行电流调节映射，把编码器的值进行限幅
      if (value > 1800.0) {
        value = 1800.0;
    
      } else if (value < 50) {
        value = 50;
      }

      printf("\n[get_encoder1_data_task] modified encoder1 value A: %.3f", value);
        // 移除发送端限制逻辑，统一在接收端进行优先级仲裁
      #ifdef USE_PID_CONTROLLER
        // 创建编码器电流控制消息 (使用统一的QueueElement_t结构)
        QueueElement_t<double> encoder_message(TASK_ENCODER, DATA_DESCRIPTION_CURRENT_SETPOINT, value);
        
        // 发送消息到电流控制队列（接收端会进行优先级仲裁）
        if (xQueueSend(current_control_queue, &encoder_message, 0) == pdTRUE) {
          printf("\n[get_encoder1_data_task] Sent current setpoint message: %.3f mA", value);
        } else {
          printf("\n[get_encoder1_data_task] Failed to send current setpoint message - queue full");
        }
        
        // 继续发送数据到 LVGL 队列用于显示
        queue_element1.data = value; // 把该值传递给 LVGL 队列的元素

      #else
        // 未启用 PID 控制器
        printf("\n[get_encoder1_data_task] setpoint current(mA): %.3f", value); // 修改为 value 变量
        MCP4725_device.setVoltage(from_set_current2voltage_V(value/1000.0)); // 设置 DAC 的目标值
      #endif

      // printf("\n[get_encoder1_data_task] setpoint voltage(V): %.3f", MCP4725_device.getVoltage());
    
    #endif
    
    printf("\n[get_encoder1_data_task] set_point %.3f", current_ctrl.process_variable.target);
    printf("\n[get_encoder1_data_task] queque_element1 data: %.3f", queue_element1.data);

    int return_value1 = xQueueSend(LVGL_queue, (void *)&queue_element1, 0);

    if (return_value1 == pdTRUE ) { // Updated to check return_value2
      // printf("\n[get_encoder1_data_task] sent message to the queue successfully\n");
    } else if (return_value1 == errQUEUE_FULL ) { // Updated to check return_value2 for queue full
      // printf("\n[get_encoder1_data_task] failed to send message to queue, queue is full\n");
    } else {
      // printf("\n[get_encoder1_data_task] failed to send message to queue\n");
    }

    vTaskDelay( 200 / portTICK_PERIOD_MS ); // 延时 200 ms
  }
}

/*********************** 辅助函数 ***********************/
#ifdef USE_IIC_DEVICE
  double from_set_current2voltage_V(double set_current_A){
    return (set_current_A * 125 * RSHUNT); // 计算电压值
  }

#endif

#endif // USE_ENCODER1