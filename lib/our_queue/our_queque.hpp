#ifndef OUR_QUEUE_HPP
#define OUR_QUEUE_HPP

#include <Arduino.h>

// GUI 更新使用的消息队列

extern QueueHandle_t button_queue; // 按键消息队列句柄
extern QueueHandle_t LVGL_queue; 


enum alert_type_t {
    ALERT_TYPE_NONE = 0, // 无警报
    ALERT_TYPE_VOLTAGE = 1, // 电压过高
    ALERT_TYPE_CURRENT = 2, // 电流过高
    ALERT_TYPE_POWER = 3, // 功率过高
    ALERT_TYPE_UNKNOWN = 99 // 未知类型
};


// 定义一个枚举类型，表示有把数据写入队列需求的任务 ID
enum task_id_t {
    TASK_DUMMY_SENSOR = 0,
    TASK_ENCODER = 1,
    TASK_INA226 = 2,
    TASK_ADC1 = 3,
    EVENT_TESING_LOAD_RATE = 4,
    EVENT_OVER_VOLTAGE = 5,
    TASK_UNKNOWN = 99  
};

enum data_description_t {
// FOT INA226
    DATA_DESCRIPTION_NONE = 0,
    DATA_DESCRIPTION_VOLTAGE = 1,
    DATA_DESCRIPTION_CURRENT = 2,
    DATA_DESCRIPTION_POWER = 3,
    DATA_DESCRIPTION_RESISTANCE = 4,
    
// FOR ENCODER
    DATA_DESCRIPTION_SET_CURRENT = 5,

// FOR ...
    DATA_DESCRIPTION_UNKNOWN = 99
};

template <typename T>
class QueueElement_t {
public:
    task_id_t task_id; // 任务 ID
    T data;
    data_description_t data_description; // 数据描述，适用于一个任务中要发送多种数据的情况
    QueueElement_t() : task_id(TASK_UNKNOWN), data(T()), data_description(DATA_DESCRIPTION_NONE) {} // 默认构造函数
    QueueElement_t(task_id_t id) : task_id(id), data(T()), data_description(DATA_DESCRIPTION_NONE) {} // 构造函数
    QueueElement_t(task_id_t id, data_description_t desc) : task_id(id), data(T()), data_description(desc) {}
   
};

#endif