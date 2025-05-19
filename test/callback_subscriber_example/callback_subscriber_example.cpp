/**
 * @file callback_subscriber_example.cpp
 * @brief 基于回调的订阅者示例
 * @author AI Assistant
 * @date 2025-05-19
 * @details 展示如何使用基于回调的订阅机制
 */

#include <Arduino.h>
#include "our.h"
#include "callback_subscriber.h"

// 定义一个示例消息类型
struct SensorData {
    float temperature;
    float humidity;
    int light;
    uint32_t timestamp;
};

// 全局变量
OUR::Publisher temperaturePublisher("sensor_data");
std::shared_ptr<OUR::CallbackSubscriber<SensorData>> sensorSubscriber;

// 回调函数示例
void temperatureCallback(const SensorData& data, task_id_t taskId, data_description_t dataDesc) {
    Serial.println("================================");
    Serial.printf("[温度回调] 接收到温度数据: %.2f°C\n", data.temperature);
    Serial.printf("来源任务ID: %d, 数据描述: %d\n", taskId, dataDesc);
    Serial.printf("湿度: %.2f%%, 光照: %d, 时间戳: %u\n", 
                 data.humidity, data.light, data.timestamp);
    Serial.println("================================\n");
}

// 湿度数据回调
void humidityCallback(const SensorData& data, task_id_t taskId, data_description_t dataDesc) {
    Serial.printf("[湿度回调] 湿度: %.2f%%\n", data.humidity);
    
    // 根据湿度值执行不同操作
    if (data.humidity > 80.0f) {
        Serial.println("警告: 湿度过高");
    }
}

// 自定义过滤器回调
void highTemperatureCallback(const SensorData& data, task_id_t taskId, data_description_t dataDesc) {
    Serial.printf("[高温警报] 检测到高温: %.2f°C\n", data.temperature);
    // 在实际应用中，这里可以触发告警或执行其他操作
}

// 自定义过滤器函数
bool highTemperatureFilter(const SensorData& data, task_id_t taskId, data_description_t dataDesc) {
    // 只有当温度超过30度时才返回true
    return data.temperature > 30.0f;
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
        bool success = temperaturePublisher.publish(data, taskId, dataDesc);
        
        if (success) {
            Serial.printf("[发布] 发布消息 #%d: 温度=%.2f, 湿度=%.2f\n", 
                         counter, data.temperature, data.humidity);
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
    
    Serial.println("\n基于回调的订阅示例启动");
    
    // 初始化OUR框架
    OUR::init();
    
    // 创建传感器数据话题的发布者
    temperaturePublisher = OUR::pub<SensorData>("sensor_data");
    
    // 创建基于回调的订阅者，处理间隔为50ms
    sensorSubscriber = OUR::createCallbackSubscriber<SensorData>("sensor_data", 50);
    
    if (!sensorSubscriber) {
        Serial.println("错误: 创建回调订阅者失败");
        return;
    }
    
    Serial.println("回调订阅者创建成功");
    
    // 添加各种订阅
    
    // 1. 无过滤的订阅 - 接收所有消息
    int subId1 = sensorSubscriber->subscribe(temperatureCallback);
    Serial.printf("添加了全局温度回调, ID=%d\n", subId1);
    
    // 2. 基于任务ID的过滤订阅 - 只接收INA226传感器的消息
    int subId2 = sensorSubscriber->subscribeTaskId(
        [](const SensorData& data, task_id_t taskId, data_description_t dataDesc) {
            Serial.printf("[INA226任务回调] 温度: %.2f°C\n", data.temperature);
        },
        TASK_INA226
    );
    Serial.printf("添加了INA226任务过滤回调, ID=%d\n", subId2);
    
    // 3. 基于数据描述的过滤订阅 - 只接收温度类型的数据
    int subId3 = sensorSubscriber->subscribeDataDesc(
        humidityCallback,
        DATA_DESCRIPTION_TEMPERATURE
    );
    Serial.printf("添加了温度描述过滤回调, ID=%d\n", subId3);
    
    // 4. 自定义过滤器订阅 - 使用自定义逻辑过滤消息
    int subId4 = sensorSubscriber->subscribeCustom(
        highTemperatureCallback,
        highTemperatureFilter
    );
    Serial.printf("添加了高温自定义过滤回调, ID=%d\n", subId4);
    
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
    // 每30秒取消一个订阅，展示动态调整订阅的能力
    static unsigned long lastToggleTime = 0;
    static int toggleState = 0;
    
    if (millis() - lastToggleTime > 30000) {
        lastToggleTime = millis();
        
        switch (toggleState) {
            case 0:
                Serial.println("\n[演示] 取消全局温度回调");
                sensorSubscriber->unsubscribe(0);
                break;
                
            case 1:
                Serial.println("\n[演示] 取消INA226任务过滤回调");
                sensorSubscriber->unsubscribe(1);
                break;
                
            case 2:
                Serial.println("\n[演示] 重新添加全局温度回调");
                sensorSubscriber->subscribe(temperatureCallback);
                break;
        }
        
        toggleState = (toggleState + 1) % 3;
        
        // 打印当前活跃订阅数量
        Serial.printf("当前活跃订阅数量: %d\n\n", 
                     sensorSubscriber->getActiveSubscriptionsCount());
    }
    
    delay(100);  // 防止CPU过度使用
}
