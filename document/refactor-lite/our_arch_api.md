# OUR框架学习手册

## 1. 框架概述

OUR框架是一个轻量级的基于FreeRTOS的消息传递和服务调用框架，设计用于ESP32等嵌入式系统。它提供了类似于ROS（机器人操作系统）的发布/订阅模式和客户端/服务器模式，但更适合资源受限的微控制器。该框架允许不同任务之间进行结构化通信，而无需直接共享内存或使用复杂的同步原语。

## 2. 核心概念

### 2.1 命名空间

框架的所有组件都封装在`OUR`命名空间内，以避免名称冲突：

```cpp
namespace OUR {
    // 框架组件...
}
```

### 2.2 基本通信模式

OUR框架支持两种主要的通信模式：

1. **话题（发布/订阅）**：一对多的异步通信
2. **服务（请求/响应）**：一对一的同步通信

## 3. 话题系统详解

### 3.1 话题映射表

话题映射表(`g_topicQueues`)是整个框架的核心组件之一，它是一个全局的映射表，关联话题名称和其对应的消息队列：

```cpp
// 定义在common.h中
typedef std::map<std::string, QueueHandle_t> TopicQueueMap;
// 实例化在common.cpp中
TopicQueueMap g_topicQueues;
```

这个映射表允许通过话题名称（如"temperature"）查找对应的FreeRTOS队列句柄，从而实现消息的发布和订阅。

### 3.2 话题互斥锁

`g_topicQueuesMutex`是保护话题映射表的互斥锁。它确保在多任务环境下对映射表的安全访问：

```cpp
// 定义在common.h中
extern SemaphoreHandle_t g_topicQueuesMutex;
// 实例化在common.cpp中
SemaphoreHandle_t g_topicQueuesMutex = nullptr;
```

**注意**：这个互斥锁只用于保护话题映射表的访问（创建、查找话题），而不是直接保护消息的发布/接收。消息的发布/接收由FreeRTOS队列自身的线程安全机制保护。

### 3.3 消息元素

框架使用`QueueElement_t`模板类（定义在`our_queue.hpp`中）来封装消息数据：

```cpp
template <typename T>
class QueueElement_t {
public:
    task_id_t task_id;             // 任务ID
    T data;                        // 实际数据
    data_description_t data_description;  // 数据描述
    // ...构造函数和其他方法...
};
```

这种设计允许消息携带额外的元数据，如来源任务ID和数据类型描述，便于接收方进行过滤和处理。

## 4. 发布者（Publisher）

### 4.1 发布者类

发布者类是向特定话题发布消息的接口：

```cpp
class Publisher {
private:
    std::string topicName;    // 话题名称
    QueueHandle_t queueHandle; // 队列句柄
    size_t maxItemSize;       // 最大消息大小

public:
    // ...方法...
    template <typename T>
    bool publish(const T& data, task_id_t taskId = TASK_UNKNOWN, 
                 data_description_t dataDesc = DATA_DESCRIPTION_NONE);
};
```

### 4.2 创建发布者

创建发布者的过程展示了互斥锁的使用方式：

```cpp
Publisher createPublisher(const std::string& topicName, const QueueConfig& config) {
    // 尝试获取互斥锁
    if (xSemaphoreTake(g_topicQueuesMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        // 失败处理...
    }
    
    // 在保护下访问映射表
    QueueHandle_t queueHandle = nullptr;
    auto it = g_topicQueues.find(topicName);
    if (it == g_topicQueues.end()) {
        // 话题不存在，创建新队列...
        // 保存到映射表...
    } else {
        // 话题已存在，获取队列句柄...
    }
    
    // 释放互斥锁
    xSemaphoreGive(g_topicQueuesMutex);
    
    // 返回发布者对象...
}
```

## 5. 订阅者（Subscriber）

### 5.1 订阅者类模板

订阅者类模板允许从特定话题接收特定类型的消息：

```cpp
template <typename T>
class Subscriber {
private:
    std::string topicName;       // 话题名称
    QueueHandle_t queueHandle;   // 队列句柄

public:
    // ...接收消息的方法...
    bool receive(QueueElement_t<T>& element, uint32_t timeout = portMAX_DELAY);
    bool receiveWithTaskId(T& data, task_id_t taskId, uint32_t timeout = portMAX_DELAY);
    bool receiveWithDataDesc(T& data, data_description_t dataDesc, uint32_t timeout = portMAX_DELAY);
};
```

订阅者提供了三种接收消息的方法：
- `receive`: 接收任何消息
- `receiveWithTaskId`: 只接收特定任务ID的消息
- `receiveWithDataDesc`: 只接收特定数据描述的消息

## 6. 服务系统

### 6.1 服务映射表

与话题系统类似，服务系统也有一个全局映射表和保护它的互斥锁：

```cpp
typedef std::map<std::string, SemaphoreHandle_t> ServiceSemaphoreMap;
extern ServiceSemaphoreMap g_serviceSemaphores;
extern SemaphoreHandle_t g_serviceSemaphoresMutex;
```

### 6.2 服务器类

服务器类处理客户端请求并返回响应：

```cpp
template <typename ReqT, typename RespT>
class Server {
public:
    using ServiceCallbackType = std::function<bool(const ReqT&, RespT&)>;
    // ...其他成员和方法...
};
```

### 6.3 客户端类

客户端类发送请求并等待响应：

```cpp
template <typename ReqT, typename RespT>
class Client {
    // ...成员和方法...
    bool call(const ReqT& request, RespT& response, uint32_t timeout = 5000);
};
```

## 7. 实际应用示例

### 7.1 温度监控系统示例

假设我们要构建一个简单的温度监控系统，包含以下组件：
- 温度传感器任务：读取温度并发布
- 显示任务：订阅温度并显示
- 告警任务：订阅温度并检查是否超限
- 控制服务：提供设置温度阈值的服务

#### 步骤1: 定义消息类型

```cpp
// 温度数据结构
struct TemperatureData {
    float value;          // 温度值
    uint32_t timestamp;   // 时间戳
};

// 温度阈值请求和响应
struct SetThresholdRequest {
    float minValue;
    float maxValue;
};

struct SetThresholdResponse {
    bool success;
    String message;
};
```

#### 步骤2: 初始化系统

```cpp
void setup() {
    Serial.begin(115200);
    
    // 初始化框架
    OUR::init();
    
    // 启动任务
    xTaskCreate(sensorTask, "TempSensor", 2048, NULL, 2, NULL);
    xTaskCreate(displayTask, "Display", 2048, NULL, 1, NULL);
    xTaskCreate(alertTask, "Alert", 2048, NULL, 3, NULL);
    xTaskCreate(controlTask, "Control", 2048, NULL, 2, NULL);
}
```

#### 步骤3: 实现传感器任务(发布者)

```cpp
void sensorTask(void* parameter) {
    // 创建温度话题发布者
    auto tempPublisher = OUR::pub<TemperatureData>("temperature");
    
    while(true) {
        // 读取温度
        float temp = readTemperature();
        
        // 创建消息并发布
        TemperatureData data;
        data.value = temp;
        data.timestamp = millis();
        
        // 发布消息，指定任务ID和数据描述
        tempPublisher.publish(data, TASK_INA226, DATA_DESCRIPTION_TEMPERATURE);
        
        // 延时
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

#### 步骤4: 实现显示任务(订阅者)

```cpp
void displayTask(void* parameter) {
    // 创建温度话题订阅者
    auto tempSubscriber = OUR::sub<TemperatureData>("temperature");
    
    while(true) {
        // 接收温度消息
        QueueElement_t<TemperatureData> element;
        if (tempSubscriber->receive(element, pdMS_TO_TICKS(5000))) {
            // 显示温度
            Serial.printf("温度: %.2f°C (来自任务%d)\n", 
                        element.data.value, 
                        element.task_id);
        }
    }
}
```

#### 步骤5: 实现告警任务(有过滤的订阅者)

```cpp
void alertTask(void* parameter) {
    // 创建温度话题订阅者
    auto tempSubscriber = OUR::sub<TemperatureData>("temperature");
    float threshold = 30.0f;
    
    while(true) {
        // 只接收来自INA226传感器的温度数据
        TemperatureData data;
        if (tempSubscriber->receiveWithTaskId(data, TASK_INA226, pdMS_TO_TICKS(5000))) {
            // 检查温度是否超过阈值
            if (data.value > threshold) {
                Serial.println("警告: 温度过高!");
            }
        }
    }
}
```

#### 步骤6: 实现控制任务(服务器)

```cpp
// 阈值设置服务回调函数
bool setThresholdCallback(const SetThresholdRequest& req, SetThresholdResponse& resp) {
    static float minThreshold = 0.0f;
    static float maxThreshold = 100.0f;
    
    // 验证请求
    if (req.minValue >= req.maxValue) {
        resp.success = false;
        resp.message = "最小值必须小于最大值";
        return false;
    }
    
    // 设置阈值
    minThreshold = req.minValue;
    maxThreshold = req.maxValue;
    
    // 返回成功
    resp.success = true;
    resp.message = "阈值已更新";
    return true;
}

void controlTask(void* parameter) {
    // 创建阈值设置服务
    auto server = OUR::advertiseService<SetThresholdRequest, SetThresholdResponse>(
        "set_threshold", setThresholdCallback);
    
    // 服务器任务保持活动
    while(true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

#### 步骤7: 客户端示例(设置阈值)

```cpp
void setNewThreshold(float min, float max) {
    // 创建服务客户端
    auto client = OUR::serviceClient<SetThresholdRequest, SetThresholdResponse>("set_threshold");
    
    // 准备请求
    SetThresholdRequest req;
    req.minValue = min;
    req.maxValue = max;
    
    // 发送请求并等待响应
    SetThresholdResponse resp;
    if (client->call(req, resp, 1000)) {
        Serial.printf("设置结果: %s\n", resp.message.c_str());
    } else {
        Serial.println("调用服务失败");
    }
}
```

## 8. 架构解析

### 8.1 互斥锁与线程安全

从示例中可以看出，互斥锁`g_topicQueuesMutex`只在以下操作中使用：
- 创建新的发布者（查找或创建话题队列）
- 创建新的订阅者（查找现有话题队列）

而在实际发布消息(`publisher.publish()`)和接收消息(`subscriber->receive()`)时，并不需要获取互斥锁，因为FreeRTOS队列本身就是线程安全的。

#### 8.1.1 为什么需要互斥锁保护话题映射表

使用互斥锁保护话题表(`g_topicQueues`)的主要原因是为了解决多任务环境下的并发访问问题：

1. **防止数据结构损坏**：
   - `g_topicQueues` 是一个STL的`std::map`，它不是线程安全的
   - 多个任务同时修改这个映射表可能导致内部数据结构损坏
   - 特别是当多个任务同时添加新话题时，可能导致map的重平衡出现错误

2. **确保操作原子性**：
   - 话题的创建或查找不是一个原子操作，通常包含多个步骤：检查话题是否存在、创建新队列、保存队列句柄
   - 互斥锁确保这一系列操作不会被其他任务中断

3. **一致性保证**：
   - 保证任务A看到的话题表状态与任务B修改完话题表后的状态是一致的
   - 避免读取到"中间状态"的话题表

4. **避免资源竞争**：
   - 防止多个任务尝试同时创建相同话题导致的资源浪费或队列句柄覆盖

#### 8.1.2 为什么发布/订阅操作不需要互斥锁

发布和订阅操作不需要互斥锁保护的原因：

1. **FreeRTOS队列的线程安全性**：
   - FreeRTOS队列(`QueueHandle_t`)本身是线程安全的
   - `xQueueSend`和`xQueueReceive`等API已经包含了必要的互斥保护

2. **性能考虑**：
   - 发布/订阅是高频操作，每次都获取互斥锁会显著降低性能
   - 使用队列的内置线程安全机制避免了额外的同步开销

3. **架构设计**：
   - 互斥锁保护的是"元数据"(话题表)，而不是"数据"(消息内容)
   - 这种分离允许数据流动的高效性和元数据管理的安全性同时得到保证

这种设计有以下优点：
1. 减少锁的使用频率，提高并发性能
2. 避免在高频的发布/接收操作中引入锁开销
3. 简化代码，减少死锁风险

### 8.2 消息元数据

通过`QueueElement_t`模板，每条消息都包含了额外的元数据：
- `task_id`: 发送消息的任务标识
- `data_description`: 消息数据的类型描述

这些元数据使订阅者能够：
1. 过滤特定来源的消息
2. 区分同一话题下的不同类型数据
3. 实现更复杂的消息处理逻辑

### 8.3 系统初始化流程

系统初始化时，`OUR::init()`函数创建必要的互斥锁：

```cpp
bool init() {
    // 创建互斥锁
    if (!g_topicQueuesMutex) {
        g_topicQueuesMutex = xSemaphoreCreateMutex();
        // ...错误处理...
    }
    
    if (!g_serviceSemaphoresMutex) {
        g_serviceSemaphoresMutex = xSemaphoreCreateMutex();
        // ...错误处理...
    }
    
    return true;
}
```

这确保了框架可以安全地创建话题和服务。

## 9. 常见问题与最佳实践

### 9.1 话题命名

使用描述性的话题名称，反映数据的内容而不是用途：
- 好：`"temperature"`, `"humidity"`, `"motor_speed"`
- 差：`"display_data"`, `"task1_output"`, `"my_topic"`

### 9.2 内存管理

消息队列在创建时就分配固定大小，所以：
- 合理估计队列长度(queueSize)和消息大小(maxItemSize)
- 避免发送过大的消息
- 对于大数据，考虑只发送指针或引用

### 9.3 超时处理

接收消息时总是指定合理的超时时间：
- 使用`0`进行非阻塞检查
- 使用`portMAX_DELAY`进行无限期等待
- 使用具体的毫秒数进行有限期等待

### 9.4 错误处理

始终检查发布和接收操作的返回值，处理可能的失败情况：
```cpp
if (!publisher.publish(data)) {
    // 处理发布失败
}
```

## 10. 框架局限性与扩展

### 10.1 当前局限性

- 没有自动化的话题发现机制
- 缺乏消息类型检查(需用户自行确保类型匹配)
- 不支持跨设备通信

### 10.2 可能的扩展

- 添加消息序列化支持
- 实现跨设备话题桥接
- 增加消息过滤和转换能力
- 添加事件和回调机制

## 11. 总结

OUR框架提供了一个简单但功能强大的消息传递系统，适用于ESP32等嵌入式系统。它的核心价值在于：

1. **解耦任务**：发布者和订阅者无需直接知道对方的存在
2. **结构化通信**：通过定义明确的话题和服务接口实现通信
3. **灵活性**：支持一对多(话题)和一对一(服务)通信模式
4. **效率**：最小化锁的使用，利用FreeRTOS队列的内置线程安全机制
5. **可扩展**：易于添加新的话题和服务

通过理解这些概念和示例，您应该能够有效地使用OUR框架构建复杂的嵌入式应用程序。

## 12. 回调订阅者架构

传统的订阅者模式需要用户创建一个任务，不断轮询队列来获取消息，然后根据消息内容进行处理。这种模式下，用户需要自行编写消息过滤和分发逻辑，容易出现重复代码和错误处理遗漏。为了简化这一过程，OUR框架提供了一种基于回调的订阅机制。

### 12.1 基本概念

回调订阅者（`CallbackSubscriber`）是一种更为高级的订阅实现，它与传统订阅者的主要区别是：

1. **自动轮询**：自动创建一个任务来轮询队列，无需用户手动编写轮询代码
2. **基于回调**：使用注册的回调函数处理接收到的消息，而非直接返回消息
3. **多重过滤**：支持多种方式过滤消息，包括任务ID、数据描述和自定义过滤器
4. **多重订阅**：允许同一个话题上注册多个不同的回调函数，可以并行处理消息

### 12.2 架构设计

回调订阅者的核心架构包括以下几个关键部分：

1. **处理任务**：一个独立的FreeRTOS任务，负责轮询队列和分发消息
2. **订阅表**：维护所有注册的回调函数及其过滤条件
3. **互斥保护**：使用C++11的`std::mutex`保护订阅表的并发访问
4. **消息分发器**：根据过滤条件将消息分发给合适的回调函数

```
+-----------------+     +----------------+     +------------------+
| 话题队列(Queue) |---->| 处理任务(Task) |---->| 消息过滤与分发器  |
+-----------------+     +----------------+     +------------------+
                                                      |
                                                      V
                        +--------------------------------------------------+
                        |                    订阅表                         |
                        |  +-------------+  +-------------+  +----------+  |
                        |  | 回调函数 A  |  | 回调函数 B  |  |    ...   |  |
                        |  | 过滤条件    |  | 过滤条件    |  |          |  |
                        |  +-------------+  +-------------+  +----------+  |
                        +--------------------------------------------------+
```

### 12.3 过滤机制

回调订阅者支持四种不同的过滤机制：

1. **无过滤**（`FilterType::NONE`）：接收所有消息
2. **任务ID过滤**（`FilterType::TASK_ID`）：只接收特定任务发送的消息
3. **数据描述过滤**（`FilterType::DATA_DESC`）：只接收特定数据类型的消息
4. **自定义过滤器**（`FilterType::CUSTOM`）：通过用户提供的函数决定是否接收消息

这种多层过滤机制使得处理复杂消息流变得简单，用户可以为不同类型的消息注册不同的处理逻辑，而不必编写复杂的条件判断代码。

### 12.4 与传统订阅者的对比

| 特性 | 传统订阅者 (`Subscriber<T>`) | 回调订阅者 (`CallbackSubscriber<T>`) |
|-----|------------------------|------------------------------|
| 获取消息方式 | 主动调用receive方法 | 被动接收通过回调函数 |
| 轮询任务 | 需用户创建 | 自动创建 |
| 多重订阅 | 需创建多个订阅者 | 单个实例可注册多个回调 |
| 过滤机制 | 基本过滤 (任务ID/数据描述) | 多种过滤 + 自定义过滤器 |
| 内存开销 | 较低 | 较高 (存储回调函数) |
| 使用灵活性 | 需要更多用户代码 | 更简洁，代码更少 |
| 适用场景 | 简单场景，资源受限 | 复杂场景，需多种处理逻辑 |

### 12.5 使用示例

以下是一个使用回调订阅者的完整示例：

```cpp
// 定义传感器数据类型
struct SensorData {
    float temperature;
    float humidity;
};

// 回调函数
void temperatureHandler(const SensorData& data, task_id_t taskId, data_description_t dataDesc) {
    Serial.printf("温度: %.2f°C\n", data.temperature);
}

void highHumidityAlert(const SensorData& data, task_id_t taskId, data_description_t dataDesc) {
    if (data.humidity > 80.0f) {
        Serial.println("警告: 湿度过高!");
    }
}

// 自定义过滤器
bool isHighTemperature(const SensorData& data, task_id_t taskId, data_description_t dataDesc) {
    return data.temperature > 30.0f;
}

void setup() {
    // 初始化框架
    OUR::init();
    
    // 创建发布者
    auto publisher = OUR::pub<SensorData>("sensor_data");
    
    // 创建回调订阅者
    auto subscriber = OUR::createCallbackSubscriber<SensorData>("sensor_data");
    
    // 添加多种订阅
    // 1. 普通订阅 - 接收所有消息
    subscriber->subscribe(temperatureHandler);
    
    // 2. 基于任务ID的过滤
    subscriber->subscribeTaskId(
        [](const SensorData& data, task_id_t taskId, data_description_t dataDesc) {
            Serial.println("来自INA226的数据");
        },
        TASK_INA226
    );
    
    // 3. 基于数据描述的过滤
    subscriber->subscribeDataDesc(highHumidityAlert, DATA_DESCRIPTION_HUMIDITY);
    
    // 4. 自定义过滤器
    subscriber->subscribeCustom(
        [](const SensorData& data, task_id_t taskId, data_description_t dataDesc) {
            Serial.println("高温警报!");
        },
        isHighTemperature
    );
}
```

### 12.6 实现细节与优化

回调订阅者在实现上有一些关键的技术细节：

1. **非阻塞轮询**：处理任务使用非阻塞方式检查队列，确保其他任务有足够的CPU时间
2. **可调整间隔**：`processingInterval`参数控制轮询频率，可以根据需要调整
3. **动态订阅管理**：支持运行时添加和移除订阅
4. **订阅ID**：每次添加订阅都返回一个ID，可用于后续取消特定订阅
5. **智能指针管理**：使用`std::shared_ptr`自动管理回调订阅者的生命周期

```cpp
// 调整处理间隔
subscriber->setProcessingInterval(20);  // 设置为20毫秒

// 取消特定订阅
int subId = subscriber->subscribe(myCallback);
// ... 稍后 ...
subscriber->unsubscribe(subId);

// 获取活跃订阅数量
size_t count = subscriber->getActiveSubscriptionsCount();
```

### 12.7 注意事项与最佳实践

使用回调订阅者时，需要注意以下几点：

1. **回调函数效率**：回调函数在处理任务的上下文中执行，应保持简短高效，避免长时间阻塞
2. **内存考虑**：每个回调函数和自定义过滤器都会占用内存，在资源受限的系统上需谨慎使用
3. **并发安全**：多个回调可能并发访问共享资源，需确保资源访问的线程安全
4. **避免死锁**：回调函数中不要尝试再订阅或取消订阅，可能导致死锁
5. **调整处理间隔**：根据消息频率和系统负载调整处理间隔，过短会占用过多CPU，过长会增加消息延迟

通过使用回调订阅者，可以显著简化复杂消息处理逻辑的编写，特别是在需要根据不同条件执行不同处理的场景下。这种设计模式促进了关注点分离，使代码更加模块化和可维护。

## 13. 过滤队列订阅者架构

过滤队列订阅者是一种高级订阅模式，它通过创建专门的过滤任务将消息分发到不同的子队列，每个子队列对应一个特定的过滤条件。这种设计使得各个订阅者可以从专用队列中获取已经过滤好的消息，无需再进行额外的过滤操作，提高了实时性和错误隔离性。

### 13.1 基本概念

过滤队列订阅者与传统订阅者和回调订阅者的主要区别在于：

1. **专用过滤任务**：创建独立的任务专门处理消息过滤和分发
2. **子队列机制**：为不同过滤条件创建独立的子队列
3. **队列复制**：符合条件的消息会被复制到对应的子队列
4. **任务隔离**：每个订阅者任务只从自己关注的子队列获取消息，相互独立

### 13.2 架构设计

过滤队列订阅者的核心架构包括以下几个关键组件：

1. **过滤器基类**：定义了通用的过滤和分发机制
2. **具体过滤器**：实现特定的过滤逻辑（如任务ID、数据描述或自定义条件）
3. **子队列管理**：创建和管理多个子队列
4. **过滤任务**：持续从源队列获取消息并分发到子队列

```
                      +-------------------+
                      |     源话题队列    |
                      +-------------------+
                              |
                              v
                      +-------------------+
                      |    过滤器任务     |
                      +-------------------+
                              |
                 +------------+------------+
                 |            |            |
        +----------------+ +----------------+ +----------------+
        |    子队列 A    | |    子队列 B    | |    子队列 C    |
        +----------------+ +----------------+ +----------------+
                 |            |            |
        +----------------+ +----------------+ +----------------+
        |   订阅者任务 A  | |   订阅者任务 B  | |   订阅者任务 C  |
        +----------------+ +----------------+ +----------------+
```

### 13.3 过滤器类型

OUR框架提供了三种类型的过滤器：

1. **任务ID过滤器**（`TaskIdFilter<T>`）：根据消息的任务ID将消息分发到不同子队列
2. **数据描述过滤器**（`DataDescFilter<T>`）：根据消息的数据描述将消息分发到不同子队列
3. **自定义过滤器**（`CustomFilter<T>`）：使用用户提供的过滤函数进行灵活的过滤和分发

这些过滤器都继承自通用的`FilterQueueBase<T>`基类，共享相同的基础功能。

### 13.4 工作流程

过滤队列订阅者的典型工作流程如下：

1. 创建并启动过滤器，连接到源话题队列
2. 为每个过滤条件创建子队列，并配置映射关系
3. 过滤任务不断从源队列获取消息
4. 根据过滤条件将消息分发到对应的子队列
5. 各个订阅者任务从自己的子队列获取已过滤的消息

由于消息在分发时已经过滤，订阅者任务可以直接处理消息，无需额外的过滤逻辑。这简化了订阅者的代码，并提高了消息处理的效率。

### 13.5 使用示例

以下是一个使用过滤队列的完整示例：

```cpp
// ===== 创建过滤器 =====
// 1. 基于任务ID的过滤器
auto taskIdFilter = OUR::createTaskIdFilter<SensorData>("sensor_data");

// 添加过滤规则：将TASK_INA226的消息分发到专门的队列
taskIdFilter->addFilterQueue(TASK_INA226, "ina226_data", 5);

// 2. 基于数据描述的过滤器
auto dataDescFilter = OUR::createDataDescFilter<SensorData>("sensor_data");

// 添加过滤规则：将温度类型的消息分发到专门的队列
dataDescFilter->addFilterQueue(DATA_DESCRIPTION_TEMPERATURE, "temperature_data", 5);

// 3. 自定义过滤器
auto customFilter = OUR::createCustomFilter<SensorData>("sensor_data");

// 添加过滤规则：将湿度大于80%的消息分发到专门的队列
customFilter->addFilterQueue(
    [](const OUR::QueueElement_t<SensorData>& element) {
        return element.data.humidity > 80.0f;
    },
    "high_humidity_data", 
    5
);

// ===== 创建订阅者任务 =====
void ina226DataTask(void* parameter) {
    // 创建订阅者，直接从过滤后的子队列获取消息
    auto subscriber = OUR::sub<SensorData>("ina226_data");
    
    while(true) {
        // 接收消息，注意无需再过滤任务ID
        QueueElement_t<SensorData> element;
        if (subscriber->receive(element)) {
            // 处理来自INA226的传感器数据
            Serial.printf("处理INA226数据: 温度=%.2f\n", element.data.temperature);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

### 13.6 子队列管理

过滤队列订阅者会自动将创建的子队列注册到全局话题表中，使它们可以像普通话题一样被订阅。这种设计有以下几个优点：

1. **简化接口**：订阅子队列使用与普通订阅相同的API
2. **兼容性**：所有订阅者类型（传统、回调、过滤）都可以使用这些子队列
3. **动态性**：可以在运行时添加或移除过滤规则和子队列

子队列的命名应当反映其过滤条件，例如：
- `"ina226_data"` - 包含来自INA226任务的消息
- `"temperature_data"` - 包含温度类型的消息
- `"high_humidity_data"` - 包含湿度大于80%的消息

### 13.7 实现细节与优化

过滤队列订阅者在实现上有一些关键的技术细节：

1. **非阻塞过滤**：过滤任务使用非阻塞方式检查源队列，以避免长时间占用CPU
2. **参数化间隔**：可以通过`setProcessingInterval()`方法调整过滤任务的处理频率
3. **多级互斥保护**：使用不同的互斥锁保护不同的共享资源（子队列表、映射表等）
4. **安全启停**：支持安全地启动和停止过滤任务
5. **资源清理**：在析构函数中自动清理创建的队列和互斥锁

```cpp
// 调整过滤器处理间隔
filter->setProcessingInterval(20);  // 设置为20毫秒

// 获取过滤器状态信息
Serial.printf("子队列数量: %d\n", filter->getSubQueueCount());
```

### 13.8 注意事项与最佳实践

使用过滤队列订阅者时，需要注意以下几点：

1. **资源开销**：每个子队列都需要额外的内存，在资源受限的系统上要谨慎使用
2. **过滤性能**：复杂的过滤条件可能会增加处理延迟，特别是在高频消息流的情况下
3. **队列大小**：子队列大小应根据预期的消息流量和处理速度进行调整
4. **多过滤器协作**：当使用多个过滤器处理同一个源话题时，它们之间是相互独立的
5. **消息一致性**：消息会被复制到子队列，因此需要注意多个订阅者可能会处理同一消息的副本

过滤队列特别适合以下场景：
- 多个任务需要处理同一话题的不同子集
- 消息处理逻辑复杂或耗时，需要与过滤逻辑分离
- 系统对实时性和错误隔离有较高要求

## 14. 订阅模式对比

OUR框架提供了三种不同的订阅模式，每种模式各有优缺点和适用场景。通过了解它们的区别，可以根据具体需求选择最合适的模式。

### 14.1 基本订阅模式对比

| 特性 | 基础订阅者 (`Subscriber<T>`) | 回调订阅者 (`CallbackSubscriber<T>`) | 过滤队列订阅者 (`FilterQueueSubscriber<T>`) |
|-----|------------------------|------------------------------|--------------------------------|
| 获取消息方式 | 主动调用receive方法 | 被动接收通过回调函数 | 被动接收过滤后的消息 |
| 轮询任务 | 需用户创建 | 自动创建 | 自动创建过滤任务 |
| 多重订阅 | 需创建多个订阅者 | 单个实例可注册多个回调 | 单个实例可注册多个子队列 |
| 过滤机制 | 基本过滤 (任务ID/数据描述) | 多种过滤 + 自定义过滤器 | 高度灵活的多级过滤 |
| 内存开销 | 较低 | 较高 (存储回调函数) | 较高 (存储子队列) |
| 使用灵活性 | 需要更多用户代码 | 更简洁，代码更少 | 适中，需配置过滤规则 |
| 适用场景 | 简单场景，资源受限 | 复杂场景，需多种处理逻辑 | 多任务并行处理复杂数据流 |

### 14.2 基础订阅者

基础订阅者是最简单的订阅模式，直接从话题队列获取消息。

```cpp
auto subscriber = OUR::sub<SensorData>("sensor_data");

while(true) {
    SensorData data;
    if (subscriber->receiveWithTaskId(data, TASK_INA226)) {
        // 处理来自INA226的数据
    }
}
```

**优点：**
- 简单直接，易于理解
- 资源占用最少
- 完全控制消息处理时机

**缺点：**
- 需要手动创建处理任务
- 重复编写轮询和过滤代码
- 一个任务只能处理一种过滤条件

**适用场景：**
- 资源极度受限的环境
- 简单订阅需求
- 对消息处理时机有严格控制需求

### 14.3 回调订阅者

回调订阅者提供基于回调函数的订阅机制，自动创建处理任务轮询队列并分发消息。

```cpp
auto subscriber = OUR::createCallbackSubscriber<SensorData>("sensor_data");

subscriber->subscribeTaskId([](const SensorData& data, task_id_t id, data_description_t desc) {
    // 处理来自INA226的数据
}, TASK_INA226);

subscriber->subscribeDataDesc([](const SensorData& data, task_id_t id, data_description_t desc) {
    // 处理温度数据
}, DATA_DESCRIPTION_TEMPERATURE);
```

**优点：**
- 编程模型简洁，事件驱动风格
- 资源占用适中（只需一个任务）
- 高度动态性，可随时添加/移除处理逻辑
- 支持多种过滤条件

**缺点：**
- 所有回调在同一任务中串行执行，相互影响
- 长耗时回调会阻塞其他回调执行
- 回调出错可能影响整个处理任务

**适用场景：**
- 资源有限但需要多样化订阅
- 处理逻辑简单，执行时间短
- 需要频繁动态调整订阅

### 14.4 过滤队列订阅者

过滤队列订阅者创建专门的过滤任务，根据过滤条件将消息分发到不同的子队列，订阅者从子队列接收消息。

```cpp
// 创建一个过滤器，根据任务ID分发消息
auto filter = OUR::createTaskIdFilter<SensorData>("sensor_data");

// 添加过滤条件和对应的子队列
filter->addFilterQueue(TASK_INA226, "ina226_data", 5);
filter->addFilterQueue(TASK_ENCODER, "encoder_data", 5);

// 从特定子队列接收消息
auto ina226Sub = OUR::sub<SensorData>("ina226_data");
while(true) {
    SensorData data;
    if (ina226Sub->receive(data)) {
        // 处理INA226数据（无需再过滤，已经预先过滤）
    }
}
```

**优点：**
- 完全隔离不同类型消息的处理
- 更好的实时性和错误隔离
- 简化订阅者逻辑，无需再实现过滤
- 适合重量级消息处理

**缺点：**
- 资源占用高（多个队列和任务）
- 系统复杂度增加
- 消息被复制多次，增加内存开销

**适用场景：**
- 资源充足的系统
- 重量级消息处理逻辑
- 对实时性和错误隔离要求高
- 多个任务需要处理相同类型但不同条件的消息

### 14.5 选择指南

1. **系统资源极度受限？** 选择基础订阅者
2. **需要简单、轻量的多样化订阅？** 选择回调订阅者
3. **需要处理重量级消息且要求高实时性和错误隔离？** 选择过滤队列订阅者
4. **未来可能需求变化？** 从简单到复杂依次是：基础订阅者 → 回调订阅者 → 过滤队列订阅者

### 14.6 组合使用

在复杂系统中，可以组合使用不同的订阅模式：

- 对于简单的、低频率更新的传感器数据，使用回调订阅者
- 对于关键控制回路或需要实时响应的部分，使用基础订阅者
- 对于需要多任务并行处理的复杂数据流，使用过滤队列订阅者

通过正确选择和组合这些订阅模式，可以在系统复杂度和资源效率之间取得平衡。

## 15. 总结

OUR框架提供了一个简单但功能强大的消息传递系统，适用于ESP32等嵌入式系统。它的核心价值在于：

1. **解耦任务**：发布者和订阅者无需直接知道对方的存在
2. **结构化通信**：通过定义明确的话题和服务接口实现通信
3. **灵活性**：支持一对多(话题)和一对一(服务)通信模式
4. **效率**：最小化锁的使用，利用FreeRTOS队列的内置线程安全机制
5. **可扩展**：易于添加新的话题和服务
6. **多样化订阅模式**：支持传统订阅者和基于回调的订阅者，适应不同应用场景

通过理解这些概念和示例，您应该能够有效地使用OUR框架构建复杂的嵌入式应用程序。