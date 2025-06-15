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

// 定义电路开关状态变量，默认关闭
bool circuit_enabled = false; // 电路开关状态，开机后默认关闭
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

/****************************** Safty Setup ***********************************/

uint8_t Warning_Voltage = 12; // 过压保护阈值，单位为 V

/*********************************** PID Setup ***********************************/
// 包含自定义的 PID 控制器类和 VOFA 下位机
#ifdef USE_PID_CONTROLLER
    #include "our_pid_controller.hpp"
    PID_controller_t<double> current_ctrl(DAC_OUTPUT_V_MAX, DAC_OUTPUT_V_MIN, CURRENT_TASK_KP, CURRENT_TASK_KI, CURRENT_TASK_KD);  

    /**
     * @brief 初始化PID控制器，设置线程安全的传感器读取函数
     * @author skyswordx
     */    
    void init_pid_controller() {
        // 设置线程安全的传感器读取函数
        current_ctrl.read_sensor = []() -> double {
            return safe_read_ina226_current_mA();
        };
        
        // 设置输出转换函数
        current_ctrl.convert_output = [](double output_voltage) -> double {
            // 如果电路关闭，则强制输出为0
            if (!circuit_enabled) {
                output_voltage = 0.0;
                printf("\n[convert_output] Circuit disabled, forcing output to 0V");
            }
            
            // 限幅保护
            double safe_voltage = constrain(output_voltage, 0.0, 5.0);
            #ifdef USE_IIC_DEVICE
                // MCP4725也需要I2C互斥锁保护
                if (xSemaphoreTake(i2c_device_mutex, 50 / portTICK_PERIOD_MS) == pdTRUE) {
                    MCP4725_device.setVoltage(safe_voltage);
                    xSemaphoreGive(i2c_device_mutex);
                } else {
                    printf("\n[ERROR] Failed to acquire I2C mutex for DAC output");
                }
            #endif
            return safe_voltage;
        };
        
        printf("\n[init_pid_controller] PID controller initialized with thread-safe I/O");
    }    void set_current_task(void *pvParameters) {
        // 该任务用于根据电流控制器对象的 target 设定值数据设置电流值
        while (1) {
            // 首先检查电路开关状态
            if (!circuit_enabled) {
                // 在电路关闭状态下强制停止输出
                if (xSemaphoreTake(i2c_device_mutex, 50 / portTICK_PERIOD_MS) == pdTRUE) {
                    MCP4725_device.setVoltage(0.0);
                    xSemaphoreGive(i2c_device_mutex);
                }
                current_ctrl.process_variable.target = 0.0;
                vTaskDelay(100 / portTICK_PERIOD_MS);
                continue;
            }
            
            // 检查保护状态
            if (over_voltage_protection_flag == pdTRUE) {
                // 在保护状态下强制停止输出
                if (xSemaphoreTake(i2c_device_mutex, 50 / portTICK_PERIOD_MS) == pdTRUE) {
                    MCP4725_device.setVoltage(0.0);
                    xSemaphoreGive(i2c_device_mutex);
                }
                current_ctrl.process_variable.target = 0.0;
                vTaskDelay(100 / portTICK_PERIOD_MS);
                continue;
            }
            
            // 根据当前负载模式更新目标电流
            // 只有在恒功率和恒阻模式下才需要动态计算目标电流
            // 恒流模式的目标电流由编码器或其他方式直接设置
            if (current_load_mode == CONSTANT_POWER || current_load_mode == CONSTANT_RESISTANCE) {
                double calculated_target = calculate_target_current_for_mode();
                // 如果计算出的目标电流与当前目标相差较大才更新，避免频繁抖动
                if (abs(calculated_target - current_ctrl.process_variable.target) > 5.0) { // 5mA的死区
                    current_ctrl.process_variable.target = calculated_target;
                    printf("\n[set_current_task] Mode-based target updated to %.3f mA", calculated_target);
                }
            }
            // 恒流模式下目标电流由编码器任务或其他地方设置，这里不需要改变
            
            // 电路开启且无保护状态时正常执行PID控制
            current_ctrl.pid_control_service();
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }

    #ifdef USE_CURRENT_OPEN_LOOP_TEST
      SemaphoreHandle_t open_loop_test_xBinarySemaphore; // 开环测试二值信号量
      double pv_data[OPEN_LOOP_TEST_LENGTH] = {}; // 开环测试数据
      double op_data[OPEN_LOOP_TEST_LENGTH] = {}; // 开环测试数据

      /**
       * @brief PID 控制器的开环阶跃测试函数
       * @author Triwalt
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
  
  // 编码器任务的当前电流设定值，使用互斥锁保护
  static double encoder_current_setpoint = 100.0; // 初始值100mA
  static SemaphoreHandle_t encoder_setpoint_mutex = NULL;
  
  /**
   * @brief 设置编码器任务的当前电流设定值
   * @author skyswordx
   * @param setpoint_mA 要设置的电流值（毫安）
   * @details 线程安全的方式设置编码器任务内部维护的电流设定值
   */
  void set_encoder_current_setpoint(double setpoint_mA) {
    if (encoder_setpoint_mutex == NULL) {
      encoder_setpoint_mutex = xSemaphoreCreateMutex();
    }
    
    if (xSemaphoreTake(encoder_setpoint_mutex, 100 / portTICK_PERIOD_MS) == pdTRUE) {
      encoder_current_setpoint = setpoint_mA;
      // 限幅处理
      if (encoder_current_setpoint > 1800.0) {
        encoder_current_setpoint = 1800.0;
      } else if (encoder_current_setpoint < 50.0) {
        encoder_current_setpoint = 50.0;
      }
      printf("\n[set_encoder_current_setpoint] Setpoint updated to: %.3f mA", encoder_current_setpoint);
      xSemaphoreGive(encoder_setpoint_mutex);
    }
  }
  
  /**
   * @brief 获取编码器任务的当前电流设定值
   * @author skyswordx
   * @return double 当前的电流设定值（毫安）
   * @details 线程安全的方式获取编码器任务内部维护的电流设定值
   */
  double get_encoder_current_setpoint() {
    double setpoint = 0.0;
    
    if (encoder_setpoint_mutex == NULL) {
      encoder_setpoint_mutex = xSemaphoreCreateMutex();
    }
    
    if (xSemaphoreTake(encoder_setpoint_mutex, 100 / portTICK_PERIOD_MS) == pdTRUE) {
      setpoint = encoder_current_setpoint;
      xSemaphoreGive(encoder_setpoint_mutex);
    }
    
    return setpoint;
  }
#endif



/*********************************** INA226 & MCP4725 Setup *************************/
#ifdef USE_IIC_DEVICE
  INA226 INA226_device(0x40); // INA226 电流传感器
  MCP4725 MCP4725_device(0x60); // MCP4725 DAC 芯片

  // I2C设备互斥锁，防止并发访问冲突
  SemaphoreHandle_t i2c_device_mutex;

  /**
   * @brief 线程安全的INA226电流读取函数
   * @return 电流值(mA)
   */
  double safe_read_ina226_current_mA() {
    double current_value = 0.0;
    if (xSemaphoreTake(i2c_device_mutex, portMAX_DELAY) == pdTRUE) {
      current_value = INA226_device.getCurrent_mA_plus();
      xSemaphoreGive(i2c_device_mutex);
    } else {
      printf("\n[ERROR] Failed to acquire I2C mutex for current reading");
    }
    return current_value;
  }

  /**
   * @brief 线程安全的INA226电压读取函数
   * @return 电压值(V)
   */
  double safe_read_ina226_voltage_V() {
    double voltage_value = 0.0;
    if (xSemaphoreTake(i2c_device_mutex, portMAX_DELAY) == pdTRUE) {
      voltage_value = INA226_device.getBusVoltage_plus();
      xSemaphoreGive(i2c_device_mutex);
    } else {
      printf("\n[ERROR] Failed to acquire I2C mutex for voltage reading");
    }
    return voltage_value;
  }

  /**
   * @brief 线程安全的INA226功率读取函数
   * @return 功率值(W)
   */
  double safe_read_ina226_power_W() {
    double current_mA = 0.0, voltage_V = 0.0, power_W = 0.0;
    if (xSemaphoreTake(i2c_device_mutex, portMAX_DELAY) == pdTRUE) {
      current_mA = INA226_device.getCurrent_mA_plus();
      voltage_V = INA226_device.getBusVoltage_plus();
      power_W = abs((current_mA * voltage_V) / 1000);
      xSemaphoreGive(i2c_device_mutex);
    } else {
      printf("\n[ERROR] Failed to acquire I2C mutex for power reading");
    }
    return power_W;
  }

  /*********************************** Current Measurement Setup *************************/
  SemaphoreHandle_t  load_testing_xBinarySemaphore;
  BaseType_t testing_load_flag = pdFALSE; // 测试负载标志位
#endif

/***************************************** ADC1 Setup *******************************/
#ifdef USE_ADC1
  ADC_channel_handler_t MY_ADC_GPIO6(ADC1_CHANNEL_5, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 64, ADC_UNIT_1);
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
    
    // 使用线程安全的读取函数，防止与PID任务并发冲突
    double measure_current_mA = safe_read_ina226_current_mA(); 
    double measure_voltage_V = safe_read_ina226_voltage_V();
    double measure_power_W = safe_read_ina226_power_W();
    double measure_resistance_ohm = abs((measure_voltage_V )/ (measure_current_mA) * 1000);


    queue_element1.data = measure_current_mA; 
    queue_element2.data = measure_voltage_V; 
    queue_element3.data = measure_power_W; 
    queue_element4.data = measure_resistance_ohm; 

    double DAC_output_V = MCP4725_device.getVoltage();
    // printf("\n[get_sensors_task] INA226: %.3f mA, %.3f V, DAC set: %.3f V, I_target: %.3f A", measure_current_mA, measure_voltage_V, DAC_output_V, (DAC_output_V/(125 * RSHUNT)));
    // printf("\n[get_ina226_data_task] INA226 V/mA: %.3f,%.3f", measure_voltage_V, measure_current_mA);

    // 检查 INA226 电压是否超过警告值，如果超过则进行过压保护
    if(measure_voltage_V >= Warning_Voltage){
      // 电压过高，触发过压之后的保护程序
      xSemaphoreGive(over_voltage_protection_xBinarySemaphore);
    }

    if (measure_voltage_V < Warning_Voltage && over_voltage_protection_flag == pdTRUE) {
      // 电压正常，清除过压保护标志位
      // 显示测量到的电压
      // printf("\n[get_ina226_data_task] Voltage is normal, measure_voltage_V: %.3f V", measure_voltage_V);
      over_voltage_protection_flag = pdFALSE;
      #ifdef USE_LCD_DISPLAY
        // 获取LVGL互斥锁，确保LVGL操作线程安全
        if (xSemaphoreTake(gui_xMutex, portMAX_DELAY) == pdTRUE) {
          if (circuit_enabled) {
            lv_obj_set_style_text_color(guider_ui.main_page_measure_current_label, lv_color_hex(0xff9600), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_add_state(guider_ui.main_page_ONOFF, LV_STATE_CHECKED);
          }
          // 清除过压保护警告控件（假设guider_ui中有overvoltage_warning控件）
          lv_obj_add_flag(guider_ui.main_page_over_voltage_warning_msgbox, LV_OBJ_FLAG_HIDDEN);
          // lv_obj_set_style_opa(guider_ui.main_page, LV_OPA_60, LV_PART_MAIN | LV_STATE_DEFAULT);
          // printf("\n[over_voltage_protection_task] LVGL warning");
          xSemaphoreGive(gui_xMutex); // 释放互斥锁
        }
      #endif
      // printf("\n[over_voltage_protection_task] Voltage is normal, over_voltage_protection_flag: %d, over_voltage_igonre_pid_flag: %d", over_voltage_protection_flag);
    }
 
    int return_value1 = xQueueSend(LVGL_queue, (void *)&queue_element1, 0); // 发送电流值到消息队列
    int return_value2 = xQueueSend(LVGL_queue, (void *)&queue_element2, 0); // 发送电压值到消息队列
    int return_value3 = xQueueSend(LVGL_queue, (void *)&queue_element3, 0); // 发送功率值到消息队列
    int return_value4 = xQueueSend(LVGL_queue, (void *)&queue_element4, 0); // 发送电阻值到消息队列


    // 打印当前队列的占用情况
    // printf("\n[get_ina226_data_task] Queue length: %d, Queue space: %d", uxQueueMessagesWaiting(LVGL_queue), uxQueueSpacesAvailable(LVGL_queue));

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

      check_current_L_A = safe_read_ina226_current_mA() / 1000; // 读取电流值
      printf("\n[get_load_changing_rate_task] check_current_L: %.3f (A)", check_current_L_A);
      if (abs(check_current_L_A - TESING_MIN_CURRENT_A) < THRESHOLD_A){
        check_voltage_L_V = safe_read_ina226_voltage_V(); // 读取电压值
        printf("\n[get_load_changing_rate_task] check_voltage_L: %.3f (V), check_current_L: %.3f (A)", check_voltage_L_V, check_current_L_A);
        check_L = pdTRUE;
      }      
      #ifdef USE_PID_CONTROLLER
          // 这里是 PID 控制器的电流值
          current_ctrl.process_variable.target = (load_test_high_current_mA / 1000.0) * 1000; // 使用动态高电流值设置 PID 控制器的目标值
      #else
          // 这里是 DAC 的电流值
          MCP4725_device.setVoltage(from_set_current2voltage_V(load_test_high_current_mA / 1000.0)); // 使用动态高电流值设置输出电压
      #endif  // 设置输出电压为第二个电流值
      vTaskDelay(200 / portTICK_PERIOD_MS); // 延时 200 ms
      printf("\n From TEST_HIGH_CURRENT to set DAC: %.3f (V)", MCP4725_device.getVoltage());

      check_current_H_A = safe_read_ina226_current_mA() / 1000; // 读取电流值
      printf("\n[get_load_changing_rate_task] check_current_H: %.3f (A)", check_current_H_A);
      if ( abs(check_current_H_A - (load_test_high_current_mA / 1000.0)) < THRESHOLD_A){
        check_voltage_H_V = safe_read_ina226_voltage_V(); // 读取电压值
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
        check_H = pdFALSE;

        queue_element.data = rate; // 发送负载变化率到消息队列
  
        int return_value = xQueueSend(LVGL_queue, (void *)&queue_element, 0);
        if (return_value == pdTRUE) {
          printf("\n[get_load_changing_rate_task] sent message to the queue successfully\n");
        } else if (return_value == errQUEUE_FULL) {
          printf("\n[get_load_changing_rate_task] failed to send message to queue, queue is full\n");
        } else {
          printf("\n[get_load_changing_rate_task] failed to send message to queue\n");
        }

        testing_load_flag = pdFALSE; // 清除测试负载标志位
      }
      else {
        printf("\n[get_load_changing_rate_task] check_L or check_H failed, check_L: %d, check_H: %d", check_L, check_H);
        // 如果检查失败，重置标志位
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
void over_voltage_protection_task(void *pvParameters){

    QueueElement_t<alert_type_t> queue_element(EVENT_OVER_VOLTAGE);

    while(1){
      
    
      printf("\n[over_voltage_protection_task] waiting");
      xSemaphoreTake(over_voltage_protection_xBinarySemaphore, portMAX_DELAY); // 总是保持阻塞等待二值信号量
      over_voltage_protection_flag = pdTRUE; // 设置过压保护标志位

      printf("\n[over_voltage_protection_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
  
      // vTaskDelete(encoder1_task_handle); // 删除编码器任务
      // 执行过压保护任务，关闭 DAC 输出

      #ifdef USE_LCD_DISPLAY
        // 获取LVGL互斥锁，确保LVGL操作线程安全
        if (xSemaphoreTake(gui_xMutex, portMAX_DELAY) == pdTRUE) {
          // 显示过压保护警告控件（假设guider_ui中有overvoltage_warning控件）
          lv_obj_set_style_text_color(guider_ui.main_page_measure_current_label, lv_color_hex(0xc4c4c4), LV_PART_MAIN|LV_STATE_DEFAULT);
          lv_obj_clear_state(guider_ui.main_page_ONOFF, LV_STATE_CHECKED); // 取消电路开关的选中状态
          lv_obj_clear_flag(guider_ui.main_page_over_voltage_warning_msgbox, LV_OBJ_FLAG_HIDDEN);
          // lv_obj_set_style_opa(guider_ui.main_page, LV_OPA_60, LV_PART_MAIN | LV_STATE_DEFAULT);
          printf("\n[over_voltage_protection_task] LVGL warning");
          xSemaphoreGive(gui_xMutex); // 释放互斥锁
        }
      #endif

      #ifdef USE_PID_CONTROLLER
        current_ctrl.process_variable.target = 0.0;
        // 触发过压保护时同时关闭电路开关
        circuit_enabled = false;
        printf("\n[over_voltage_protection_task] Circuit disabled due to overvoltage protection");
        // for (int i = 0; i < 1000; i++){
        //   printf("\n[over_voltage_protection_task] block %d", i);
        // } 
      #else
        MCP4725_device.setVoltage(0.0); // 设置输出电压为 0V
        // 触发过压保护时同时关闭电路开关
        circuit_enabled = false;
        printf("\n[over_voltage_protection_task] Circuit disabled due to overvoltage protection");
      #endif

      
      // 切断功率板电源，让 MOSFET 关断
      /* todo */
      
      
      // int return_value = xQueueSend(LVGL_queue, (void *)&queue_element, 0); // 发送过压保护值到消息队列

      // if (return_value == pdTRUE) {
      //   printf("\n[over_voltage_protection_task] sent message to the queue successfully\n"); // 添加成功发送消息的打印
      // } else if ( return_value == errQUEUE_FULL) {
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
  QueueElement_t<double> queue_element(TASK_ADC1); // 定义一个队列元素对象，存储温度数据(float类型)
  while(1)
  {
    // printf("\n[ADC1_read_task] running on core: %d, Free stack space: %d", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    
    // 获取ADC电压值
    double adc_voltage_mV = MY_ADC_GPIO6.get_ADC1_voltage_average_mV();
    
    // 计算温度值
    float temperature_celsius = MY_ADC_GPIO6.calculate_temperature_from_voltage(adc_voltage_mV);

    // 将温度值发送到消息队列
    queue_element.data = temperature_celsius;

    // 通过串口输出电压值和温度值
    printf("\n[ADC1_read_task] NTC Voltage: %.3f mV, Temperature: %.2f °C", adc_voltage_mV, temperature_celsius);
    
    int return_value = xQueueSend(LVGL_queue, (void *)&queue_element, 0);
    if (return_value == pdTRUE) {
      // printf("\n[ADC1_read_task] sent temperature data to the queue successfully\n");
    } else if (return_value == errQUEUE_FULL) {
      // printf("\n[ADC1_read_task] failed to send message to queue, queue is full\n");
    } else {
      // printf("\n[ADC1_read_task] failed to send message to queue\n");
    }

    vTaskDelay( 1000 ); // 每秒更新一次温度
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
              long_press = pdFALSE; // 短按时清除长按标志位
  
            } else {
              long_press = pdTRUE;
              printf("\n[button_handler_task] GPIO %d long press", GPIO_PIN);
              short_press = pdFALSE; // 长按时清除短按标志位
            }
  
           
            printf("\n[button_handler_task] duration: %d ms", time_ms);
          #ifdef USE_ENCODER1
            if (GPIO_PIN == encoder1_button.pin){
              // 按下编码器按键之后切换模式
              // TODO:不良耦合的步长spinbox显示切换
              if (encoder1.mode == HUNDRED) {
                encoder1.mode = SINGLE;
                lv_spinbox_set_step (guider_ui.main_page_set_current_box, 1); // 设置步长为 1
                printf("\n[button_handler_task] encoder1 mode changed to SINGLE");
              } else if (encoder1.mode == SINGLE) {
                encoder1.mode = TEN;
                lv_spinbox_set_step (guider_ui.main_page_set_current_box, 10); // 设置步长为 10
                printf("\n[button_handler_task] encoder1 mode changed to TEN");
              } else if (encoder1.mode == TEN) {
                encoder1.mode = HUNDRED;
                lv_spinbox_set_step (guider_ui.main_page_set_current_box, 100); // 设置步长为 100
                printf("\n[button_handler_task] encoder1 mode changed to HUNDRED");
              }
            }
          #endif // USE_ENCODER1
          #ifdef USE_BUTTON1
            if(GPIO_PIN == button1.pin){
              // 按键1：短按 - 电路开关功能，长按 - 负载模式切换
              if (long_press == pdFALSE) {
                // 短按：电路开关功能
                circuit_enabled = !circuit_enabled; // 切换电路开关状态
                
                if (!circuit_enabled) {
                  // 电路关闭时，强制设置DAC为0V
                  printf("\n[button_handler_task] Circuit disabled, setting DAC to 0V");
                  lv_obj_set_style_text_color(guider_ui.main_page_measure_current_label, lv_color_hex(0xc4c4c4), LV_PART_MAIN|LV_STATE_DEFAULT);
                  lv_obj_clear_state(guider_ui.main_page_ONOFF, LV_STATE_CHECKED); // 取消电路开关的选中状态
                  #ifdef USE_IIC_DEVICE
                    if (xSemaphoreTake(i2c_device_mutex, 50 / portTICK_PERIOD_MS) == pdTRUE) {
                      MCP4725_device.setVoltage(0.0); // 设置输出电压为0V
                      xSemaphoreGive(i2c_device_mutex);
                    }
                  #endif
                  
                  #ifdef USE_PID_CONTROLLER
                    current_ctrl.process_variable.target = 0.0; // 设置目标电流为0
                  #endif
                } else {
                  // 电路开启时，输出日志
                  printf("\n[button_handler_task] Circuit enabled");
                  if (over_voltage_protection_flag == pdFALSE) {
                    lv_obj_set_style_text_color(guider_ui.main_page_measure_current_label, lv_color_hex(0xff9600), LV_PART_MAIN|LV_STATE_DEFAULT);
                    lv_obj_add_state(guider_ui.main_page_ONOFF, LV_STATE_CHECKED);  
                  }
                }
                
                #ifdef USE_CURRENT_OPEN_LOOP_TEST
                  // 仅在电路开启时才进行开环测试
                  if (circuit_enabled) {
                    xSemaphoreGive(open_loop_test_xBinarySemaphore); // 释放二值信号量，触发开环测试
                  }
                #endif
                
                #ifdef USE_OUTPUT_CALIBRATION
                  // 仅在电路开启时才进行输出标定
                  if (circuit_enabled) {
                    xSemaphoreGive(output_calibration_xBinarySemaphore); // 释放二值信号量，触发输出标定
                  }
                #endif
                
                // 原有功能保留
                if (circuit_enabled) {
                  xSemaphoreGive(startup_xBinarySemaphore);
                }
              } else {
                // 长按：负载模式切换功能
                printf("\n[button_handler_task] Long press detected - switching load mode");
                switch_load_mode(); // 切换负载模式
                
                // 更新当前目标电流以适应新模式
                #ifdef USE_PID_CONTROLLER
                  if (circuit_enabled) {
                    double new_target = calculate_target_current_for_mode();
                    current_ctrl.process_variable.target = new_target;
                    printf("\n[button_handler_task] Target current updated to %.3f mA for new mode", new_target);
                  }
                #endif
              }
            }
          #endif // USE_BUTTON1
          #ifdef USE_BUTTON2
            if(GPIO_PIN == button2.pin){
              // 按键2：短按 - 负载测试功能，长按 - button3/4功能模式切换
              if (long_press == pdFALSE) {
                // 短按：负载测试功能（保持原有功能）
                #ifdef USE_IIC_DEVICE
                  xSemaphoreGive(load_testing_xBinarySemaphore); // 释放二值信号量，触发负载测试
                #endif // IIC_DEVICE
                printf("\n[button_handler_task] Button2 short press - Load testing triggered");
              } else {
                // 长按：button3/4功能模式切换
                printf("\n[button_handler_task] Long press detected - switching button3/4 function mode");
                switch_button34_mode(); // 切换button3/4功能模式
              }
            }
          #endif // USE_BUTTON2
          
          #ifdef USE_BUTTON3
            if(GPIO_PIN == button3.pin){
              // button3功能根据当前模式而定
              switch(current_button34_mode) {
                case VOLTAGE_ADJUSTMENT:
                  // 过压阈值调节模式：提高过压阈值
                  if (Warning_Voltage < 20.0) {
                    Warning_Voltage += 1; // 增加过压阈值
                    printf("\n[button_handler_task] Warning_Voltage increased to: %.1f V", (double)Warning_Voltage);
                    // 更新GUI显示
                    #ifdef USE_LCD_DISPLAY
                    if (xSemaphoreTake(gui_xMutex, 50 / portTICK_PERIOD_MS) == pdTRUE) {
                      if (guider_ui.main_page_over_voltage_box != NULL) {
                        lv_spinbox_set_value(guider_ui.main_page_over_voltage_box, (int32_t)Warning_Voltage);
                      }
                      xSemaphoreGive(gui_xMutex);
                    }
                    #endif
                  } else {
                    printf("\n[button_handler_task] Warning_Voltage already at maximum: %.1f V", (double)Warning_Voltage);
                  }
                  break;
                  
                case CURRENT_ADJUSTMENT:
                  // 负载测试电流调节模式：增加高电流值
                  if (load_test_high_current_mA < 1800.0) {
                    load_test_high_current_mA += 100.0; // 增加100mA
                    if (load_test_high_current_mA > 1800.0) load_test_high_current_mA = 1800.0; // 限制上限
                    printf("\n[button_handler_task] Load test high current increased to: %.0f mA", load_test_high_current_mA);
                    // 更新GUI显示
                    #ifdef USE_LCD_DISPLAY
                    if (xSemaphoreTake(gui_xMutex, 50 / portTICK_PERIOD_MS) == pdTRUE) {
                      if (guider_ui.main_page_over_voltage_box != NULL) {
                        lv_spinbox_set_value(guider_ui.main_page_over_voltage_box, (int32_t)load_test_high_current_mA);
                      }
                      xSemaphoreGive(gui_xMutex);
                    }
                    #endif
                  } else {
                    printf("\n[button_handler_task] Load test high current already at maximum: %.0f mA", load_test_high_current_mA);
                  }
                  break;
              }
            }
          #endif // USE_BUTTON3
          #ifdef USE_BUTTON4
            if(GPIO_PIN == button4.pin){
              // button4功能根据当前模式而定
              switch(current_button34_mode) {
                case VOLTAGE_ADJUSTMENT:
                  // 过压阈值调节模式：降低过压阈值
                  if (Warning_Voltage > 5.0) {
                    Warning_Voltage -= 1; // 减少过压阈值
                    printf("\n[button_handler_task] Warning_Voltage decreased to: %.1f V", (double)Warning_Voltage);
                    // 更新GUI显示
                    #ifdef USE_LCD_DISPLAY
                    if (xSemaphoreTake(gui_xMutex, 50 / portTICK_PERIOD_MS) == pdTRUE) {
                      if (guider_ui.main_page_over_voltage_box != NULL) {
                        lv_spinbox_set_value(guider_ui.main_page_over_voltage_box, (int32_t)Warning_Voltage);
                      }
                      xSemaphoreGive(gui_xMutex);
                    }
                    #endif
                  } else {
                    printf("\n[button_handler_task] Warning_Voltage already at minimum: %.1f V", (double)Warning_Voltage);
                  }
                  break;
                  
                case CURRENT_ADJUSTMENT:
                  // 负载测试电流调节模式：减少高电流值
                  if (load_test_high_current_mA > 200.0) {
                    load_test_high_current_mA -= 100.0; // 减少100mA
                    if (load_test_high_current_mA < 200.0) load_test_high_current_mA = 200.0; // 限制下限
                    printf("\n[button_handler_task] Load test high current decreased to: %.0f mA", load_test_high_current_mA);
                    // 更新GUI显示
                    #ifdef USE_LCD_DISPLAY
                    if (xSemaphoreTake(gui_xMutex, 50 / portTICK_PERIOD_MS) == pdTRUE) {
                      if (guider_ui.main_page_over_voltage_box != NULL) {
                        lv_spinbox_set_value(guider_ui.main_page_over_voltage_box, (int32_t)load_test_high_current_mA);
                      }
                      xSemaphoreGive(gui_xMutex);
                    }
                    #endif
                  } else {
                    printf("\n[button_handler_task] Load test high current already at minimum: %.0f mA", load_test_high_current_mA);
                  }
                  break;
              }
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
 * @brief 获取编码器数据的任务函数，用来设置负载参数
 * @author skyswordx
 * @param pvParameters 任务参数
 * @details 根据当前负载模式调整相应的设定值：恒流模式调整电流，恒功率模式调整功率，恒阻模式调整电阻
 *          使用增量方式避免设置值与编码器传递过来的值不一致的问题
 */
void get_encoder1_data_task(void *pvParameters)
{
  double increment;
  QueueElement_t<double> queue_element1(TASK_ENCODER, DATA_DESCRIPTION_SET_CURRENT); 
  
  while(1)
  {
    increment = encoder1.read_count_increment(); // 获取编码器相对于上次的增量值
    
    // 只有当有增量时才更新设定值
    if (increment != 0.0) {
      // 根据当前负载模式调整相应的设定值
      switch(current_load_mode) {
        case CONSTANT_CURRENT:
          {
            // 恒流模式：调整电流设定值
            double current_setpoint = get_encoder_current_setpoint();
            current_setpoint += increment;
            set_encoder_current_setpoint(current_setpoint);
            load_setpoint_current_mA = current_setpoint; // 同步到负载模式设定值
            printf("\n[get_encoder1_data_task] CC mode - current setpoint: %.3f mA", current_setpoint);
          }
          break;
          
        case CONSTANT_POWER:
          {
            // 恒功率模式：调整功率设定值
            load_setpoint_power_W += (increment * 0.001); // 每增量0.001W
            if (load_setpoint_power_W > 50.0) load_setpoint_power_W = 50.0;   // 限制最大功率
            if (load_setpoint_power_W < 0.1) load_setpoint_power_W = 0.1;     // 限制最小功率
            printf("\n[get_encoder1_data_task] CP mode - power setpoint: %.3f W", load_setpoint_power_W);
          }
          break;
          
        case CONSTANT_RESISTANCE:
          {
            // 恒阻模式：调整电阻设定值
            load_setpoint_resistance_ohm += (increment * 1); // 每增量1Ω
            if (load_setpoint_resistance_ohm > 1000.0) load_setpoint_resistance_ohm = 1000.0; // 限制最大电阻
            if (load_setpoint_resistance_ohm < 1.0) load_setpoint_resistance_ohm = 1.0;       // 限制最小电阻
            printf("\n[get_encoder1_data_task] CR mode - resistance setpoint: %.3f Ω", load_setpoint_resistance_ohm);
          }
          break;
      }
    }

    /* 实现负载调节 */
    #ifdef USE_IIC_DEVICE
      // 根据模式获取当前显示值和队列数据
      double display_value = 0.0;
      switch(current_load_mode) {
        case CONSTANT_CURRENT:
          display_value = load_setpoint_current_mA;
          break;
        case CONSTANT_POWER:
          display_value = load_setpoint_power_W * 1000; // 转换为mW显示
          break;
        case CONSTANT_RESISTANCE:
          display_value = load_setpoint_resistance_ohm;
          break;
      }
      
      queue_element1.data = display_value; // 把当前设定值传递给 LVGL 队列的元素

      if (testing_load_flag == pdFALSE && circuit_enabled == true) {
        #ifdef USE_PID_CONTROLLER
          // 在恒流模式下直接设置目标电流，其他模式由set_current_task计算
          if (current_load_mode == CONSTANT_CURRENT) {
            current_ctrl.process_variable.target = load_setpoint_current_mA;
            if (increment != 0.0) {
              printf("\n[get_encoder1_data_task] CC mode - PID target updated to: %.3f mA", load_setpoint_current_mA);
            }
          }
          // 恒功率和恒阻模式的目标电流由set_current_task动态计算
        #else
          // 未启用 PID 控制器的情况
          if (current_load_mode == CONSTANT_CURRENT && increment != 0.0) {
            printf("\n[get_encoder1_data_task] setpoint current(mA): %.3f", load_setpoint_current_mA);
            MCP4725_device.setVoltage(from_set_current2voltage_V(load_setpoint_current_mA/1000.0));
          }
        #endif
      } else {
        if (!circuit_enabled) {
          //TODO: 每次都灰色化当前电流显示（无法初始化为灰色的妥协）
          lv_obj_set_style_text_color(guider_ui.main_page_measure_current_label, lv_color_hex(0xc4c4c4), LV_PART_MAIN|LV_STATE_DEFAULT);
          lv_obj_clear_state(guider_ui.main_page_ONOFF, LV_STATE_CHECKED); // 取消电路开关的选中状态
          
          // 电路关闭时，确保目标值为0，但保持设定值不变以便重新开启时恢复
          #ifdef USE_PID_CONTROLLER
            current_ctrl.process_variable.target = 0.0;
          #else
            MCP4725_device.setVoltage(0.0);
          #endif
            if (increment != 0.0) {
            printf("\n[get_encoder1_data_task] Circuit disabled, encoder input stored but not applied");
          }
        }
      }

      // printf("\n[get_encoder1_data_task] setpoint voltage(V): %.3f", MCP4725_device.getVoltage());
    
    #endif
    
    // printf("\n[get_encoder1_data_task] set_point %.3f", current_ctrl.process_variable.target);
    // printf("\n[get_encoder1_data_task] queque_element1 data: %.3f", queue_element1.data);

    int return_value1 = xQueueSend(LVGL_queue, (void *)&queue_element1, 0);

    // if (return_value1 == pdTRUE ) { // Updated to check return_value2
    //   printf("\n[get_encoder1_data_task] sent message to the queue successfully\n");
    // } else if (return_value1 == errQUEUE_FULL ) { // Updated to check return_value2 for queue full
    //   printf("\n[get_encoder1_data_task] failed to send message to queue, queue is full\n");
    // } else {
    //   printf("\n[get_encoder1_data_task] failed to send message to queue\n");
    // }

    vTaskDelay( 100 / portTICK_PERIOD_MS ); // 延时 100 ms
  }
}

/*********************** 辅助函数 ***********************/
#ifdef USE_IIC_DEVICE
  double from_set_current2voltage_V(double set_current_A){
    return (set_current_A * 125 * RSHUNT); // 计算电压值
  }

#endif

#endif // USE_ENCODER1

/*********************************** Load Mode Setup *********************************/
LoadMode current_load_mode = CONSTANT_CURRENT; // 默认恒流模式
double load_setpoint_current_mA = 100.0;       // 恒流模式设定值，默认100mA
double load_setpoint_power_W = 1.0;            // 恒功率模式设定值，默认1W
double load_setpoint_resistance_ohm = 100.0;    // 恒阻模式设定值，默认100Ω

/**
 * @brief 切换负载模式
 * @author skyswordx
 * @details 在三种负载模式之间循环切换：恒流 -> 恒功率 -> 恒阻 -> 恒流
 */
void switch_load_mode() {
    switch(current_load_mode) {
        case CONSTANT_CURRENT:
            current_load_mode = CONSTANT_POWER;
            printf("\n[switch_load_mode] Switched to CONSTANT_POWER mode");
            break;
        case CONSTANT_POWER:
            current_load_mode = CONSTANT_RESISTANCE;
            printf("\n[switch_load_mode] Switched to CONSTANT_RESISTANCE mode");
            break;
        case CONSTANT_RESISTANCE:
            current_load_mode = CONSTANT_CURRENT;
            printf("\n[switch_load_mode] Switched to CONSTANT_CURRENT mode");
            break;
    }
    update_load_mode_display();
}

/**
 * @brief 更新负载模式显示
 * @author skyswordx
 * @details 更新GUI上的负载模式显示
 */
void update_load_mode_display() {
    #ifdef USE_LCD_DISPLAY
    if (xSemaphoreTake(gui_xMutex,portMAX_DELAY) == pdTRUE) {
        switch(current_load_mode) {
            case CONSTANT_CURRENT:
                // 更新显示为恒流模式，可以在这里更新相关GUI元素
                printf("\n[update_load_mode_display] Display updated for CONSTANT_CURRENT");
                lv_label_set_text(guider_ui.main_page_set_current_description, "设置电流(mA)");
                lv_dropdown_set_selected(guider_ui.main_page_ddlist_1, 0); // 0是恒流模式的索引
                break;
            case CONSTANT_POWER:
                // 更新显示为恒功率模式
                printf("\n[update_load_mode_display] Display updated for CONSTANT_POWER");
                lv_label_set_text(guider_ui.main_page_set_current_description, "设置功率(mW)");
                lv_dropdown_set_selected(guider_ui.main_page_ddlist_1, 1); // 0是恒功率模式的索引
                break;
            case CONSTANT_RESISTANCE:
                // 更新显示为恒阻模式
                printf("\n[update_load_mode_display] Display updated for CONSTANT_RESISTANCE");
                lv_label_set_text(guider_ui.main_page_set_current_description, "设置电阻(Ohm)");
                
                lv_dropdown_set_selected(guider_ui.main_page_ddlist_1, 2); // 0是恒阻模式的索引
                break;
        }
        xSemaphoreGive(gui_xMutex);
    } else {
        printf("\n[ERROR] Failed to acquire GUI mutex for mode display update");
    }
    #endif
}

/**
 * @brief 根据当前模式计算目标电流
 * @author skyswordx
 * @return 目标电流值(mA)
 * @details 根据不同的负载模式和当前测量值计算所需的目标电流
 */
double calculate_target_current_for_mode() {
    double target_current_mA = 0.0;
    
    switch(current_load_mode) {
        case CONSTANT_CURRENT:
            // 恒流模式：直接返回设定电流
            target_current_mA = load_setpoint_current_mA;
            break;
            
        case CONSTANT_POWER:
            {
                // 恒功率模式：P = U * I，因此 I = P / U
                double current_voltage_V = safe_read_ina226_voltage_V();
                if (current_voltage_V > 0.1) { // 避免除零错误
                    target_current_mA = (load_setpoint_power_W / current_voltage_V) * 1000; // 转换为mA
                    // 限制电流范围，避免过大电流
                    if (target_current_mA > 2000.0) target_current_mA = 2000.0;
                    if (target_current_mA < 10.0) target_current_mA = 10.0;
                } else {
                    target_current_mA = 10.0; // 电压过低时设为最小电流
                }
            }
            break;
            
        case CONSTANT_RESISTANCE:
            {
                // 恒阻模式：I = U / R
                double current_voltage_V = safe_read_ina226_voltage_V();
                if (load_setpoint_resistance_ohm > 0.1) { // 避免除零错误
                    target_current_mA = (current_voltage_V / load_setpoint_resistance_ohm) * 1000; // 转换为mA
                    // 限制电流范围
                    if (target_current_mA > 2000.0) target_current_mA = 2000.0;
                    if (target_current_mA < 10.0) target_current_mA = 10.0;
                } else {
                    target_current_mA = 10.0; // 电阻过小时设为最小电流
                }
            }
            break;
    }
    
    return target_current_mA;
}

/*********************************** Button34 Function Mode Setup *******************/
Button34Mode current_button34_mode = VOLTAGE_ADJUSTMENT; // 默认过压阈值调节模式
double load_test_high_current_mA = 1000.0;               // 负载测试高电流值，默认1000mA

/**
 * @brief 切换button3/4功能模式
 * @author skyswordx
 * @details 在两种功能模式之间切换：过压阈值调节 <-> 负载测试电流调节
 */
void switch_button34_mode() {
    switch(current_button34_mode) {
        case VOLTAGE_ADJUSTMENT:
            current_button34_mode = CURRENT_ADJUSTMENT;
            printf("\n[switch_button34_mode] Switched to CURRENT_ADJUSTMENT mode");
            break;
        case CURRENT_ADJUSTMENT:
            current_button34_mode = VOLTAGE_ADJUSTMENT;
            printf("\n[switch_button34_mode] Switched to VOLTAGE_ADJUSTMENT mode");
            break;
    }
    update_button34_mode_display();
}

/**
 * @brief 更新button3/4功能模式显示
 * @author skyswordx
 * @details 更新GUI上的button3/4功能模式显示和相关控件的单位、范围等
 */
void update_button34_mode_display() {
    #ifdef USE_LCD_DISPLAY
    if (xSemaphoreTake(gui_xMutex, 100 / portTICK_PERIOD_MS) == pdTRUE) {
        switch(current_button34_mode) {
            case VOLTAGE_ADJUSTMENT:
                // 更新显示为过压阈值调节模式
                if (guider_ui.main_page_over_volatge_description != NULL) {
                    lv_label_set_text(guider_ui.main_page_over_volatge_description, "过压阈值(V)");
                }
                if (guider_ui.main_page_over_voltage_box != NULL) {
                    lv_spinbox_set_value(guider_ui.main_page_over_voltage_box, (int32_t)Warning_Voltage);
                    lv_spinbox_set_step(guider_ui.main_page_over_voltage_box, 1);       // 步长1V
                    lv_spinbox_set_range(guider_ui.main_page_over_voltage_box, 5, 20);  // 范围5-20V
                }
                printf("\n[update_button34_mode_display] Display updated for VOLTAGE_ADJUSTMENT");
                break;
                
            case CURRENT_ADJUSTMENT:
                // 更新显示为负载测试电流调节模式
                if (guider_ui.main_page_over_volatge_description != NULL) {
                    lv_label_set_text(guider_ui.main_page_over_volatge_description, "测试电流(mA)");
                }
                if (guider_ui.main_page_over_voltage_box != NULL) {
                    lv_spinbox_set_value(guider_ui.main_page_over_voltage_box, (int32_t)load_test_high_current_mA);
                    lv_spinbox_set_step(guider_ui.main_page_over_voltage_box, 100);       // 步长100mA
                    lv_spinbox_set_range(guider_ui.main_page_over_voltage_box, 200, 1800); // 范围200-1800mA
                }
                printf("\n[update_button34_mode_display] Display updated for CURRENT_ADJUSTMENT");
                break;
        }
        xSemaphoreGive(gui_xMutex);
    } else {
        printf("\n[ERROR] Failed to acquire GUI mutex for button34 mode display update");
    }
    #endif
}

/******************************  ESP32S3 Setup ***********************************/