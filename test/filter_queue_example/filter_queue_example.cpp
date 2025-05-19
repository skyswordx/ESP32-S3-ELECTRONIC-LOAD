/**
 * @file filter_queue_example.cpp
 * @brief 过滤队列订阅示例
 * @author AI Assistant
 * @date 2025-05-19
 * @details 展示如何使用过滤队列架构进行消息订阅
 */

#include <Arduino.h>
#include "our.h"
#include "task_id_filter.h"
#include "data_desc_filter.h"
#include "custom_filter.h"

// 定义一个示例消息类型
struct SensorData {
    float temperature;
    float humidity;
    int light;
    uint32_t timestamp;
};

// 全局变量
OUR::Publisher sensorPublisher("sensor_data");
std::shared_ptr<OUR::TaskIdFilter<SensorData>> taskIdFilter;
std::shared_ptr<OUR::DataDescFilter<SensorData>> dataDescFilter;
std::shared_ptr<OUR::CustomFilter<SensorData>> customFilter;

// 处理来自INA226的数据
void ina226DataTask(void* parameter) {
    // 创建订阅者，直接从过滤后的子队列获取消息
    auto subscriber = OUR::sub<SensorData>("ina226_sensor_data");
    
    while(true) {
        // 接收消息，注意无需再过滤任务ID，因为已经在过滤器中完成了过滤
        QueueElement_t<SensorData> element;
        if (subscriber->receive(element)) {
            Serial.println("================================");
            Serial.printf("[INA226处理] 收到来自INA226的传感器数据\n");
            Serial.printf("温度: %.2f°C, 湿度: %.2f%%\n", 
                      element.data.temperature, element.data.humidity);
            Serial.println("================================\n");
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));  // 短暂延时
    }
}

// 处理温度数据
void temperatureDataTask(void* parameter) {
    // 创建订阅者，直接从过滤后的子队列获取消息
    auto subscriber = OUR::sub<SensorData>("temperature_data");
    
    while(true) {
        // 接收消息，注意无需再过滤数据描述
        QueueElement_t<SensorData> element;
        if (subscriber->receive(element)) {
            Serial.println("================================");
            Serial.printf("[温度处理] 收到温度类型数据\n");
            Serial.printf("温度: %.2f°C (来自任务ID: %d)\n", 
                      element.data.temperature, element.task_id);
            Serial.println("================================\n");
            
            // 检查温度告警
            if (element.data.temperature > 30.0f) {
                Serial.println("** 警告: 温度过高! **");
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));  // 短暂延时
    }
}

// 处理高湿度数据
void highHumidityTask(void* parameter) {
    // 创建订阅者，直接从过滤后的子队列获取消息
    auto subscriber = OUR::sub<SensorData>("high_humidity_data");
    
    while(true) {
        // 接收消息，注意无需再进行自定义过滤
        QueueElement_t<SensorData> element;
        if (subscriber->receive(element)) {
            Serial.println("================================");
            Serial.printf("[湿度告警] 检测到高湿度: %.2f%%\n", 
                      element.data.humidity);
            Serial.println("** 湿度过高，可能需要除湿 **");
            Serial.println("================================\n");
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));  // 短暂延时
    }
}

// 发布者任务
void publisherTask(void* parameter) {
    int counter = 0;
    
    while (true) {
        // 创建传感器数据
        SensorData data;
        data.temperature = 25.0f + (counter % 10);  // 温度在25-34之间变化
        data.humidity = 50.0f + (counter % 5) * 10.0f;  // 湿度在50-90之间变化
        data.light = 100 * (counter % 10 + 1);
        data.timestamp = millis();
        
        // 根据计数器值选择不同的任务ID和数据描述
        task_id_t taskId = (counter % 2 == 0) ? TASK_INA226 : TASK_ENCODER;
        data_description_t dataDesc = (counter % 3 == 0) ? 
            DATA_DESCRIPTION_TEMPERATURE : DATA_DESCRIPTION_CURRENT;
        
        // 发布消息
        bool success = sensorPublisher.publish(data, taskId, dataDesc);
        
        if (success) {
            Serial.printf("[发布] 发布消息 #%d: 温度=%.2f, 湿度=%.2f, 任务ID=%d, 数据描述=%d\n", 
                         counter, data.temperature, data.humidity, taskId, dataDesc);
        } else {
            Serial.printf("[发布] 消息 #%d 发布失败\n", counter);
        }
        
        counter++;
        vTaskDelay(pdMS_TO_TICKS(2000));  // 每2秒发布一次
    }
}

void setup() {
    // 初始化串口
    Serial.begin(115200);
    delay(2000);  // 等待串口稳定
    
    Serial.println("\n过滤队列订阅示例启动");
    
    // 初始化OUR框架
    OUR::init();
    
    // 创建传感器数据话题的发布者
    sensorPublisher = OUR::pub<SensorData>("sensor_data");
    
    // ===== 创建基于任务ID的过滤器 =====
    Serial.println("创建基于任务ID的过滤器...");
    taskIdFilter = OUR::createTaskIdFilter<SensorData>("sensor_data");
    
    if (!taskIdFilter) {
        Serial.println("错误: 无法创建任务ID过滤器");
    } else {
        // 添加过滤规则：将TASK_INA226的消息分发到专门的队列
        taskIdFilter->addFilterQueue(TASK_INA226, "ina226_sensor_data", 5);
        
        Serial.println("已创建INA226数据过滤子队列");
    }
    
    // ===== 创建基于数据描述的过滤器 =====
    Serial.println("创建基于数据描述的过滤器...");
    dataDescFilter = OUR::createDataDescFilter<SensorData>("sensor_data");
    
    if (!dataDescFilter) {
        Serial.println("错误: 无法创建数据描述过滤器");
    } else {
        // 添加过滤规则：将温度类型的消息分发到专门的队列
        dataDescFilter->addFilterQueue(DATA_DESCRIPTION_TEMPERATURE, "temperature_data", 5);
        
        Serial.println("已创建温度数据过滤子队列");
    }
    
    // ===== 创建自定义过滤器 =====
    Serial.println("创建自定义过滤器...");
    customFilter = OUR::createCustomFilter<SensorData>("sensor_data");
    
    if (!customFilter) {
        Serial.println("错误: 无法创建自定义过滤器");
    } else {
        // 添加过滤规则：将湿度大于80%的消息分发到专门的队列
        customFilter->addFilterQueue(
            [](const OUR::QueueElement_t<SensorData>& element) {
                return element.data.humidity > 80.0f;
            },
            "high_humidity_data", 
            5
        );
        
        Serial.println("已创建高湿度数据过滤子队列");
    }
    
    // 创建处理任务
    xTaskCreate(
        ina226DataTask,
        "INA226_Task",
        4096,
        NULL,
        1,
        NULL
    );
    
    xTaskCreate(
        temperatureDataTask,
        "Temp_Task",
        4096,
        NULL,
        1,
        NULL
    );
    
    xTaskCreate(
        highHumidityTask,
        "Humidity_Task",
        4096,
        NULL,
        1,
        NULL
    );
    
    // 创建发布者任务
    xTaskCreate(
        publisherTask,
        "Publisher",
        4096,
        NULL,
        1,
        NULL
    );
    
    Serial.println("初始化完成，开始发布和订阅...");
}

void loop() {
    // 每30秒打印一次过滤器状态
    static unsigned long lastStatusTime = 0;
    
    if (millis() - lastStatusTime > 30000) {
        lastStatusTime = millis();
        
        Serial.println("\n===== 过滤器状态 =====");
        Serial.printf("任务ID过滤器: 源话题='%s', 子队列数量=%d\n", 
                     taskIdFilter->getSourceTopic().c_str(), 
                     taskIdFilter->getSubQueueCount());
        
        Serial.printf("数据描述过滤器: 源话题='%s', 子队列数量=%d\n", 
                     dataDescFilter->getSourceTopic().c_str(), 
                     dataDescFilter->getSubQueueCount());
        
        Serial.printf("自定义过滤器: 源话题='%s', 过滤条件数量=%d\n", 
                     customFilter->getSourceTopic().c_str(), 
                     customFilter->getFilterCount());
        Serial.println("======================\n");
    }
    
    delay(100);  // 防止CPU过度使用
}
