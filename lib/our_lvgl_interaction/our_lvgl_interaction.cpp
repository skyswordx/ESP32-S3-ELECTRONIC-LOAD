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
 *          所谓的互斥锁就是在同一时刻只能有一个线程访问共享资源，其他线程需要等待
 *          实现的方式就是利用一个二值信号量，一个线程要运行下面的代码，就要先 take 这个信号量
 *          其他线程因为之前的线程已经 take 了这个信号量，所以只能等待
 *          直到之前的线程 give 这个信号量，其他线程才能继续运行
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

}

/**
 * @brief 更新 GUI 的任务函数，接收传感器数据并更新 GUI
 * @author skyswordx 
 * @details 建议仅在这个线程中更新 GUI，防止多线程同时更新 GUI 导致数据错乱
 *          使用 task_id 来区分不同的传感器数据
 */
void update_gui_task(void *pvParameters)
{

  QueueElement_t<double> queue_element; // 定义一个队列元素对象
  while(1)
  {
    // printf("\n[update_gui_task] running on core: %d, Free stack space: %d\n", xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
    if (LVGL_queue != NULL){ // 检查消息队列是否创建成功
      if (xQueueReceive(LVGL_queue, &queue_element, portMAX_DELAY) == pdTRUE) {
        // printf("\n[update_gui_task] received message from queue\n");

        if (xSemaphoreTake(gui_xMutex, portMAX_DELAY) == pdTRUE) { // 获取互斥锁
          // 检测元素是否准备齐全并更新 GUI

          switch (queue_element.task_id){
            
            case TASK_INA226:
              // 显示 INA226 测量的电流
              if (guider_ui.main_page_measure_current_label != NULL && queue_element.data_description == DATA_DESCRIPTION_CURRENT){ lv_label_set_text_fmt(guider_ui.main_page_measure_current_label, "%.3f", queue_element.data); }
              if (guider_ui.main_page_measure_voltage_label != NULL && queue_element.data_description == DATA_DESCRIPTION_VOLTAGE){ lv_label_set_text_fmt(guider_ui.main_page_measure_voltage_label, "%.3f", queue_element.data); }
              if (guider_ui.main_page_measure_power_label != NULL && queue_element.data_description == DATA_DESCRIPTION_POWER){ lv_label_set_text_fmt(guider_ui.main_page_measure_power_label, "%.3f", queue_element.data); }
              if (guider_ui.main_page_measure_resistance_label != NULL && queue_element.data_description == DATA_DESCRIPTION_RESISTANCE){ lv_label_set_text_fmt(guider_ui.main_page_measure_resistance_label, "%.1f", queue_element.data); }
              break;
            case TASK_ENCODER:
              // 显示编码器的电流值
              if (guider_ui.main_page_set_current_box != NULL && queue_element.data_description == DATA_DESCRIPTION_SET_CURRENT){ lv_spinbox_set_value(guider_ui.main_page_set_current_box, queue_element.data); }
              lv_spinbox_set_value(guider_ui.main_page_over_voltage_box, WARNING_VOLTAGE); //TODO: 以后要删掉
              break;
            case TASK_DUMMY_SENSOR:

              break;
            case EVENT_TESING_LOAD_RATE:
              if (guider_ui.main_page_start_load_regulation_button_label != NULL){ lv_label_set_text_fmt(guider_ui.main_page_load_regulation_label, "%.3f", queue_element.data); }
              break;

            case EVENT_OVER_VOLTAGE:
              printf("\n[update_gui_task] over voltage event");

              break;
            default:
              break;
          }
          xSemaphoreGive(gui_xMutex); // 释放互斥锁
        }
        
        // printf("\n[update_gui_task] using msg.value: %.3f", msg.value);
      } else {
        // printf("\n[update_gui_task] failed to receive message from queue\n");
      }
    }
    
  }
}
