/**
 * @file our_button.cpp
 * @brief GPIO 按键类的实现文件
 * @author skyswordx
 * @details 该文件包含了 GPIO 按键类的实现文件，包含了 GPIO 按键的初始化和中断处理函数
 *          这里使用 RTOS 规范的中断处理流程，仅仅在中断函数中释放信号量，标记触发中断的 GPIO 引脚号
 *          具体的按键处理逻辑在辅助任务中进行
 */
#include "our_button.hpp"
#include "our_config.hpp"

/**
 * @brief GPIO 按键类的构造函数
 * @author skyswordx
 * @param pin GPIO 引脚号
 */
GPIO_button_handler_t::GPIO_button_handler_t(gpio_num_t pin) {
    this->pin = pin;
    this->button_state = false;
    
    // 绑定 GPIO 引脚至中断和中断处理函数
    // 配置GPIO，下降沿和上升沿触发中断
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = 1 << pin; // 为 GPIO pin 设置中断
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    // 注册中断服务
    gpio_set_intr_type((gpio_num_t)pin, GPIO_INTR_ANYEDGE);
    
    // 安装中断服务函数和中断函数 在 main 函数中进行
}

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


void IRAM_ATTR button1_press_ISR(void *arg ){ // 产生 GPIO 中断时调用
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; 
    // 初始化为 pdFALSE，以便最后把耗时的数据处理移出中断给辅助任务处理
    
    // 释放二值信号量，对应阻塞于此信号量的任务（辅助任务）将被唤醒
    xSemaphoreGiveFromISR(button_xBinarySemaphore, &xHigherPriorityTaskWoken);
  
    gpio_num_t GPIO_PIN = button1.pin; // 获取 GPIO 引脚号
    xQueueSendFromISR(button_queue_handle, &GPIO_PIN, &xHigherPriorityTaskWoken); // 发送消息到按键消息队列
  
   
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 
    // 如果有更高优先级的任务需要运行，立即切换到那个任务（辅助任务，因为此任务设定为最高优先级）
  }
  
  void IRAM_ATTR button2_press_ISR(void *arg ){ 
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; 
  
    xSemaphoreGiveFromISR(button_xBinarySemaphore, &xHigherPriorityTaskWoken);
  
    gpio_num_t GPIO_PIN = button2.pin; 
    xQueueSendFromISR(button_queue_handle, &GPIO_PIN, &xHigherPriorityTaskWoken); 
  
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 
    
  }
  
  void IRAM_ATTR button3_press_ISR(void *arg ){ 
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; 
  
    xSemaphoreGiveFromISR(button_xBinarySemaphore, &xHigherPriorityTaskWoken);
  
    gpio_num_t GPIO_PIN = button3.pin; 
    xQueueSendFromISR(button_queue_handle, &GPIO_PIN, &xHigherPriorityTaskWoken); 
  
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 
    
  }
  
  void IRAM_ATTR button4_press_ISR(void *arg ){ 
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; 
  
    xSemaphoreGiveFromISR(button_xBinarySemaphore, &xHigherPriorityTaskWoken);
  
    gpio_num_t GPIO_PIN = button4.pin; 
    xQueueSendFromISR(button_queue_handle, &GPIO_PIN, &xHigherPriorityTaskWoken); 
  
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 
    
  }

    void IRAM_ATTR encoder1_button_press_ISR(void *arg ){ 
        BaseType_t xHigherPriorityTaskWoken = pdFALSE; 
    
        xSemaphoreGiveFromISR(button_xBinarySemaphore, &xHigherPriorityTaskWoken);
    
        gpio_num_t GPIO_PIN = encoder1_button.pin; 
        xQueueSendFromISR(button_queue_handle, &GPIO_PIN, &xHigherPriorityTaskWoken); 
    
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 
        
    }