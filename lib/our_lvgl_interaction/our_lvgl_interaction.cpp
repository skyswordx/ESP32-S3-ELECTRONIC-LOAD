/**
 * @file our_lvgl_interaction.cpp
 * @brief LVGL GUI 任务调度接口和 GUI 数据更新接口
 * @author skyswordx
 */

#include "our_lvgl_interaction.h"


/**
 * @brief LVGL 任务调度函数，在一个 CPU 中运行即可调度所有的 LVGL 页面显示
 * @author skyswordx 
 * @details 仅在 setup() 中创建一次即可，LVGL 任务会一直运行，配合 GUI guider 的高级接口可以实现 LVGL 的页面显示和自动管理
 *          由于 LVGL 的接口是线程不安全的，所以需要加锁来保护 LVGL 的接口调用
 * 
 */
void lvgl_task(void *pvParameters){
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

/**
 * @brief 更新 GUI 的任务函数，接收传感器数据并更新 GUI
 * @author skyswordx 
 * @details 建议仅在这个线程中更新 GUI，防止多线程同时更新 GUI 导致数据错乱
 *          使用 DEVICE_ID 来区分不同的传感器数据
 */
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
