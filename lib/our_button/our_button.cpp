#include "our_button.hpp"

extern SemaphoreHandle_t button_xBinarySemaphore; // 按键二值信号量
extern QueueHandle_t button_queue_handle; // 按键消息队列句柄
extern GPIO_button_handler_t button1; // 按键对象
extern GPIO_button_handler_t button2; // 按键对象  
extern GPIO_button_handler_t button3; // 按键对象
extern GPIO_button_handler_t button4; // 按键对象
extern GPIO_button_handler_t encoder1_button; // 旋转编码器的按键对象


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