/**
 * @file our_sensors.cpp
 * @brief 包含封装好的各种传感器类和相关的任务函数
 * @author skyswordx
 */
#include "our_sensors.hpp"
#include "our_config.hpp"

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

    vTaskDelay( 1000 );
  }
}

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
  
    float measure_current_mA = INA226_device.getCurrent_mA();
    float measure_voltage_V = INA226_device.getBusVoltage();
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
  
