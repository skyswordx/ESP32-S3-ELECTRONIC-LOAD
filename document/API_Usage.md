# 使用事件驱动型系统的 API

## 概述
事件驱动型系统通过 `EventBus` 实现，支持事件的发布和订阅机制。用户可以通过订阅特定类型的事件来响应系统中的变化。

## 核心组件
1. **Event**: 所有事件的基类，定义了事件类型和基本接口。
2. **EventBus**: 事件总线，负责事件的发布和分发。
3. **EventListener**: 事件监听器，用户需要实现该接口以处理事件。
4. **TaskManager**: 任务管理器，负责管理FreeRTOS任务的创建、调度和销毁。

记得在 `freeRTOS/task.h` 中定义
```cpp
 #define configTASKLIST_INCLUDE_COREID 1
```

## C++单例类介绍

在继续了解具体API之前，我们需要理解"单例类"的概念。单例类是一种特殊的类，它在整个程序中只有一个实例（对象）。

### 什么是单例类？

想象一下图书馆的借书系统：整个图书馆只需要一个借书系统，多个借书系统会导致混乱（比如重复借阅）。单例类就像这个借书系统，确保全局只有一个实例存在。

在本系统中，`EventBus`和`TaskManager`都是单例类。

### getInstance() 方法的作用

`static TaskManager& getInstance()`方法是获取单例对象的标准方式，它的作用是：

1. 如果单例对象还不存在，它会创建这个对象
2. 如果单例对象已经存在，它会返回已存在的对象

这样无论你在程序中的哪里调用`getInstance()`，得到的都是同一个对象。

### 为什么要使用单例？

1. **资源共享**：某些资源(如任务管理器)全局只需要一个
2. **避免冲突**：防止多个实例相互干扰(比如两个任务管理器同时操作一个任务)
3. **统一访问点**：提供全局访问点，不需要传递对象引用

### 如何使用单例类

使用单例类非常简单，只需要通过`getInstance()`方法获取实例，然后调用它的方法：

```cpp
// 获取TaskManager单例
TaskManager& taskManager = TaskManager::getInstance();

// 使用这个单例
taskManager.startTask("MyTask", myTaskFunction, nullptr, 4096, 1, 0);
```

通过这种方式，你不需要担心如何创建`TaskManager`对象，也不需要跟踪它的生命周期，整个程序中只会有一个`TaskManager`实例。

## 使用步骤

### 1. 定义事件

#### Event 基类简介
`Event` 是所有事件的基类，定义了事件的基本结构和接口。以下是 `Event` 的主要内容：
```cpp
class Event {
public:
    explicit Event(EventType type) : type_(type) {} // 构造函数，初始化事件类型

    EventType getType() const { return type_; } // 获取事件类型

    virtual String toString() const { // 虚函数，返回事件的字符串表示
        return "Event[type=" + String(static_cast<int>(type_)) + "]";
    }

    virtual ~Event() = default; // 虚析构函数，确保子类正确析构

private:
    EventType type_; // 事件类型
};
```
**解释**：
- `EventType type` 是事件的类型，用于区分不同的事件。
- `toString` 是一个虚函数，子类可以重写它以提供更详细的事件信息。
- `~Event` 是虚析构函数，确保子类对象被正确销毁。

#### 编码器事件定义示例
我们以编码器为例，定义一个事件类 `EncoderValueChangedEvent`，表示编码器的值发生了变化。
```cpp
class EncoderValueChangedEvent : public Event { // 继承 Event 基类
public:
    // 构造函数，用于创建事件对象
    explicit EncoderValueChangedEvent(double value)
        : Event(EventType::ENCODER_CHANGED), // 调用基类构造函数，初始化事件类型
          value_(value) {} // 初始化子类特有的成员变量

    // 获取编码器值
    double getValue() const { return value_; }

    // 获取事件的字符串表示（用于调试）
    String toString() const override { // override 表示重写基类的虚函数
        return "EncoderValueChangedEvent[value=" + String(value_) + "]";
    }

private:
    double value_; // 编码器值（子类特有的成员变量）
};
```
**解释**：
- `: Event(EventType::ENCODER_CHANGED)` 调用了基类的构造函数，初始化了事件类型，这是基类的职责。
- `value_(value)` 是子类的扩展部分，用于初始化子类特有的成员变量 `value_`。
- 子类可以根据需要添加自己的成员变量和方法，例如 `value_` 和 `getValue`。

### 2. 订阅事件

#### EventBus 订阅功能简介
`EventBus` 类提供了订阅事件的功能，通过 `subscribe` 方法实现：
```cpp
void subscribe(EventType type, EventListener* listener); // 订阅事件
```
**解释**：
- `type` 参数是要订阅的事件类型。
- `listener` 参数是处理该事件的监听器指针。
- 订阅后，当有匹配类型的事件被发布时，监听器的 `onEvent` 方法会被调用。

#### 编码器事件订阅示例
以下是订阅编码器值变化事件的代码示例：
```cpp
// 获取 EventBus 单例
EventBus& bus = EventBus::getInstance();

// 创建监听器对象（假设已经定义了 MyEncoderListener 类）
MyEncoderListener listener;

// 订阅编码器值变化事件
bus.subscribe(EventType::ENCODER_CHANGED, &listener);
```
**解释**：
- `EventBus::getInstance()` 返回事件总线的单例对象。
- `bus.subscribe` 方法将监听器与编码器值变化事件类型关联。
- 当编码器值变化事件发生时，`listener` 的 `onEvent` 方法将被调用。

### 3. 发布事件

#### EventBus 发布功能简介
`EventBus` 类提供了发布事件的功能，通过 `publish` 方法实现：
```cpp
bool publish(const Event& event); // 发布事件
bool publish(const std::shared_ptr<Event>& event); // 发布事件（智能指针版本）
```
**解释**：
- 第一个版本接收事件对象的引用。
- 第二个版本接收事件对象的智能指针。
- 如果有至少一个监听器处理了该事件，则返回 `true`，否则返回 `false`。

#### 编码器事件发布示例
以下是发布编码器值变化事件的代码示例：
```cpp
// 创建一个编码器值变化事件，值为 42.0
EncoderValueChangedEvent event(42.0);

// 获取 EventBus 单例并发布事件
EventBus& bus = EventBus::getInstance();
bool handled = bus.publish(event);

// 检查事件是否被处理
if (handled) {
    Serial.println("编码器事件已被处理");
} else {
    Serial.println("没有监听器处理编码器事件");
}
```
**解释**：
- `EncoderValueChangedEvent event(42.0)` 创建了一个编码器值变化事件。
- `bus.publish(event)` 将事件分发给所有订阅了该事件类型的监听器。
- 返回值 `handled` 表示是否有监听器处理了该事件。

### 4. 实现监听器

#### EventListener 接口简介
`EventListener` 是一个接口（抽象基类），定义了处理事件的基本方法：
```cpp
class EventListener {
public:
    virtual bool onEvent(const Event& event) = 0; // 纯虚函数，子类必须实现
    virtual std::string getListenerId() const { return "EventListener"; } // 获取监听器 ID
    virtual ~EventListener() = default; // 虚析构函数
};
```
**解释**：
- `onEvent` 是一个纯虚函数（用 `= 0` 表示），子类必须实现此方法。
- `getListenerId` 返回监听器的标识符，便于调试。
- 虚析构函数确保子类对象被正确销毁。

#### 编码器事件监听器实现示例
以下是实现编码器事件监听器的代码示例：
```cpp
class MyEncoderListener : public EventListener {
public:
    // 处理事件的方法
    bool onEvent(const Event& event) override {
        // 检查是否为编码器值变化事件
        if (event.getType() == EventType::ENCODER_CHANGED) {
            // 将基类引用转换为子类引用
            const auto& encoderEvent = static_cast<const EncoderValueChangedEvent&>(event);
            
            // 获取并打印编码器值
            double value = encoderEvent.getValue();
            Serial.println("编码器新值: " + String(value));
            
            // 返回 true 表示事件已处理
            return true;
        }
        
        // 返回 false 表示该监听器不处理此类型的事件
        return false;
    }
    
    // 返回监听器标识符
    std::string getListenerId() const override {
        return "MyEncoderListener";
    }
};
```
**解释**：
- `onEvent` 方法首先检查事件类型是否为编码器值变化事件。
- 如果是，则使用 `static_cast` 将基类引用转换为子类引用，以便访问子类特有的成员。
- `encoderEvent.getValue()` 获取编码器的值。
- 最后返回 `true` 表示事件已被处理。

### 5. 使用任务管理器

#### TaskManager 简介
`TaskManager` 是一个单例类，用于管理 FreeRTOS 任务。它提供了创建、停止、暂停和恢复任务的方法。以下是 `TaskManager` 的主要功能：

```cpp
class TaskManager {
public:
    // 获取单例 - 这确保了整个程序中只有一个任务管理器实例
    static TaskManager& getInstance();
    
    // 创建并启动任务
    TaskHandle_t startTask(const String& name, TaskFunction_t function, void* params,
                           uint32_t stackSize = 1024 * 4, UBaseType_t priority = 1,
                           BaseType_t coreId = tskNO_AFFINITY);
    
    // 停止任务
    bool stopTask(TaskHandle_t handle);
    
    // 暂停任务
    bool suspendTask(TaskHandle_t handle);
    
    // 恢复任务
    bool resumeTask(TaskHandle_t handle);
    
    // 获取任务统计信息
    TaskStats getTaskStats(TaskHandle_t handle);
    
    // 获取所有任务的统计信息
    std::vector<TaskStats> getAllTaskStats();
    
    // 根据名称查找任务句柄
    TaskHandle_t getTaskHandleByName(const String& name);
};
```
**解释**：
- `getInstance()` 返回 `TaskManager` 的单例对象，保证整个程序只有一个任务管理器
- `startTask()` 创建并启动一个 FreeRTOS 任务，返回任务句柄
- `stopTask()`、`suspendTask()` 和 `resumeTask()` 分别用于停止、暂停和恢复任务
- `getTaskStats()` 和 `getAllTaskStats()` 用于获取任务的统计信息

#### 在 FreeRTOS 任务中使用事件驱动系统

创建一个处理编码器事件的任务：
```cpp
// 定义编码器事件处理任务
void encoderTaskFunction(void* params) {
    // 创建事件监听器
    class EncoderTaskListener : public EventListener {
    public:
        bool onEvent(const Event& event) override {
            if (event.getType() == EventType::ENCODER_CHANGED) {
                const auto& encoderEvent = static_cast<const EncoderValueChangedEvent&>(event);
                Serial.printf("任务中处理编码器值: %.2f\n", encoderEvent.getValue());
                return true;
            }
            return false;
        }
        
        std::string getListenerId() const override {
            return "EncoderTaskListener";
        }
    };
    
    // 创建监听器实例
    static EncoderTaskListener listener;
    
    // 订阅事件
    EventBus::getInstance().subscribe(EventType::ENCODER_CHANGED, &listener);
    
    // 任务主循环
    while (true) {
        // 如果有需要定期执行的代码，可以放在这里
        vTaskDelay(pdMS_TO_TICKS(100)); // 等待100毫秒
    }
}

// 定义编码器模拟任务
void encoderSimulationTaskFunction(void* params) {
    double value = 0.0;
    
    while (true) {
        // 模拟编码器值变化
        value += 1.0;
        
        // 创建并发布事件
        EncoderValueChangedEvent event(value);
        bool handled = EventBus::getInstance().publish(event);
        
        if (handled) {
            Serial.printf("编码器值 %.2f 已被处理\n", value);
        }
        
        // 等待一段时间
        vTaskDelay(pdMS_TO_TICKS(1000)); // 每秒发布一次事件
    }
}
```
**解释**：
- `encoderTaskFunction` 是一个处理编码器事件的任务函数。
- `encoderSimulationTaskFunction` 是一个模拟编码器值变化的任务函数。
- 两个任务通过 `EventBus` 进行通信，一个发布事件，一个处理事件。

#### 创建和启动任务

在主程序中创建和启动任务：
```cpp
#include <Arduino.h>
#include "core/EventBus.h"
#include "core/TaskManager.h"
#include "devices/EncoderEvent.h"

void setup() {
    // 初始化串口
    Serial.begin(115200);
    Serial.println("RTOS 事件驱动系统示例启动");
    
    // 获取任务管理器
    TaskManager& taskManager = TaskManager::getInstance();
    
    // 创建并启动处理编码器事件的任务
    TaskHandle_t encoderTaskHandle = taskManager.startTask(
        "EncoderTask",            // 任务名称
        encoderTaskFunction,      // 任务函数
        nullptr,                 // 任务参数
        4096,                    // 栈大小
        1,                       // 优先级
        1                        // 核心ID (1表示在第二个核心上运行)
    );
    
    // 创建并启动模拟编码器值变化的任务
    TaskHandle_t simulationTaskHandle = taskManager.startTask(
        "EncoderSimulationTask",  // 任务名称
        encoderSimulationTaskFunction, // 任务函数
        nullptr,                 // 任务参数
        4096,                    // 栈大小
        1,                       // 优先级
        0                        // 核心ID (0表示在第一个核心上运行)
    );
    
    if (encoderTaskHandle && simulationTaskHandle) {
        Serial.println("所有任务已成功启动");
    } else {
        Serial.println("任务启动失败");
    }
}

void loop() {
    // 在主循环中可以处理其他逻辑，或者不做任何事
    delay(1000);
}
```
**解释**：
- `TaskManager::getInstance()` 获取任务管理器的单例。
- `startTask()` 方法用于创建和启动任务，指定任务名称、函数、参数、栈大小、优先级和运行的核心。
- 各个任务通过事件总线相互通信，实现松耦合的系统架构。

### 6. 任务之间的事件通信

在基于 FreeRTOS 的系统中，任务之间可以通过事件总线进行通信，这比直接使用队列或信号量更加灵活。

#### 多任务事件通信示例

```cpp
// 定义温度监控任务
void temperatureMonitorTaskFunction(void* params) {
    float temperature = 25.0f;
    
    while (true) {
        // 模拟温度变化
        temperature += (random(100) - 50) / 100.0f;
        
        // 当温度超过阈值时发布事件
        if (temperature > 30.0f) {
            // 假设系统中定义了 TemperatureAlarmEvent
            TemperatureAlarmEvent event(temperature);
            EventBus::getInstance().publish(event);
            
            // 降低温度回到安全范围
            temperature = 25.0f;
        }
        
        // 等待一段时间
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// 定义报警处理任务
void alarmHandlerTaskFunction(void* params) {
    // 定义报警事件监听器
    class AlarmListener : public EventListener {
    public:
        bool onEvent(const Event& event) override {
            if (event.getType() == EventType::TEMPERATURE_ALARM) {
                const auto& tempEvent = static_cast<const TemperatureAlarmEvent&>(event);
                Serial.printf("温度警报! 当前温度: %.2f\n", tempEvent.getTemperature());
                
                // 这里可以添加报警处理逻辑，如控制风扇、蜂鸣器等
                
                return true;
            }
            return false;
        }
    };
    
    // 创建监听器并订阅事件
    static AlarmListener listener;
    EventBus::getInstance().subscribe(EventType::TEMPERATURE_ALARM, &listener);
    
    // 任务主循环
    while (true) {
        vTaskDelay(portMAX_DELAY); // 无限等待，只在事件发生时被唤醒
    }
}
```
**解释**：
- `temperatureMonitorTaskFunction` 监控温度并在超过阈值时发布事件。
- `alarmHandlerTaskFunction` 定义了一个监听器来处理温度报警事件。
- 两个任务在不同的上下文中运行，但通过事件总线进行通信。

## 完整示例：基于 RTOS 的事件驱动系统

以下是一个完整的示例，展示如何在 FreeRTOS 环境下使用事件驱动系统处理编码器事件：

```cpp
#include <Arduino.h>
#include "core/EventBus.h"
#include "core/TaskManager.h"
#include "devices/EncoderEvent.h"

// 编码器事件处理任务
void encoderTaskFunction(void* params) {
    class EncoderTaskListener : public EventListener {
    public:
        bool onEvent(const Event& event) override {
            if (event.getType() == EventType::ENCODER_CHANGED) {
                const auto& encoderEvent = static_cast<const EncoderValueChangedEvent&>(event);
                Serial.printf("编码器值: %.2f\n", encoderEvent.getValue());
                return true;
            }
            return false;
        }
        
        std::string getListenerId() const override {
            return "EncoderTaskListener";
        }
    };
    
    static EncoderTaskListener listener;
    EventBus::getInstance().subscribe(EventType::ENCODER_CHANGED, &listener);
    
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// 编码器模拟任务
void encoderSimulationTaskFunction(void* params) {
    double value = 0.0;
    
    while (true) {
        value += 1.0;
        EncoderValueChangedEvent event(value);
        EventBus::getInstance().publish(event);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("RTOS 事件驱动系统示例启动");
    
    TaskManager& taskManager = TaskManager::getInstance();
    
    taskManager.startTask("EncoderTask", encoderTaskFunction, nullptr, 4096, 1, 1);
    taskManager.startTask("EncoderSimulationTask", encoderSimulationTaskFunction, nullptr, 4096, 1, 0);
    
    Serial.println("所有任务已启动");
}

void loop() {
    delay(1000);
}
```
该示例展示了如何在 ESP32 的 FreeRTOS 环境中创建任务，并使用事件驱动机制实现任务间通信。