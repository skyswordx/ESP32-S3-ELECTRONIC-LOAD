#ifndef OUR_QUEUE_HPP
#define OUR_QUEUE_HPP

#include <Arduino.h>

// 在实际 main.cpp 代码中更新使用的消息队列
extern QueueHandle_t button_queue; // 按键消息队列句柄
extern QueueHandle_t LVGL_queue;
extern QueueHandle_t current_control_queue; // 电流控制器队列句柄


// 定义一个枚举类型，作为队列中的一种数据类型表示警报
enum alert_type_t {
    ALERT_TYPE_NONE = 0, // 无警报
    ALERT_TYPE_VOLTAGE = 1, // 电压过高
    ALERT_TYPE_CURRENT = 2, // 电流过高
    ALERT_TYPE_POWER = 3, // 功率过高
    ALERT_TYPE_UNKNOWN = 99 // 未知类型
};


// 定义一个枚举类型，表示有把数据写入队列需求的任务 ID
// 数值越小表示优先级越高（用于电流控制队列的优先级排序）
enum task_id_t {
    EVENT_OVER_VOLTAGE = 0,      // 紧急事件：过压保护（最高优先级）
    TASK_ENCODER = 1,            // 编码器任务（中等优先级）
    TASK_DUMMY_SENSOR = 2,       // 传感器任务（普通优先级）
    TASK_INA226 = 3,             // INA226任务（普通优先级）
    TASK_ADC1 = 4,               // ADC任务（普通优先级）
    EVENT_TESING_LOAD_RATE = 5,  // 负载测试事件（普通优先级）
    EVENT_OVER_VOLTAGE_CLEAR = 6, // 过压解除事件
    EVENT_TESTING_COMPLETE = 7,   // 负载测试完成事件
    TASK_UNKNOWN = 99            // 未知任务（最低优先级）
};

// 定义一个枚举类型，表示数据描述
// 适用于一个任务中要发送多种数据的情况
enum data_description_t {
// FOR INA226
    DATA_DESCRIPTION_NONE = 0,
    DATA_DESCRIPTION_VOLTAGE = 1,
    DATA_DESCRIPTION_CURRENT = 2,
    DATA_DESCRIPTION_POWER = 3,
    DATA_DESCRIPTION_RESISTANCE = 4,
    
// FOR ENCODER
    DATA_DESCRIPTION_SET_CURRENT = 5,

// FOR CURRENT CONTROL (电流控制相关)
    DATA_DESCRIPTION_CURRENT_SETPOINT = 6,     // 电流设定值
    DATA_DESCRIPTION_EMERGENCY_STOP = 7,       // 紧急停止

// FOR ...
    DATA_DESCRIPTION_UNKNOWN = 99
};

// 定义优先级状态枚举，用于接收端状态管理
enum priority_state_t {
    PRIORITY_STATE_NORMAL = 0,           // 正常状态，接受所有消息
    PRIORITY_STATE_OVER_VOLTAGE = 1,     // 过压保护状态，只接受过压相关和更高优先级消息
    PRIORITY_STATE_TESTING = 2,          // 负载测试状态，暂停普通编码器控制
    PRIORITY_STATE_EMERGENCY = 3         // 紧急状态，只接受紧急消息
};

// 定义一个模板类 QueueElement_t，表示队列中的元素，支持不同类型的数据
template <typename T>
class QueueElement_t {
public:
    task_id_t task_id; // 任务 ID (也表示优先级，数值越小优先级越高)
    T data;
    data_description_t data_description; // 数据描述，适用于一个任务中要发送多种数据的情况
    
    QueueElement_t() : task_id(TASK_UNKNOWN), data(T()), data_description(DATA_DESCRIPTION_NONE) {} // 默认构造函数
    QueueElement_t(task_id_t id) : task_id(id), data(T()), data_description(DATA_DESCRIPTION_NONE) {} // 构造函数
    QueueElement_t(task_id_t id, data_description_t desc) : task_id(id), data(T()), data_description(desc) {}
    QueueElement_t(task_id_t id, data_description_t desc, T value) : task_id(id), data(value), data_description(desc) {}
};

#endif