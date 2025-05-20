
# OUR 框架学习手册与API规范（精简重构版）


## 第一部分：由浅入深的实用案例引导

### 1. 框架简介与快速上手

OUR 框架是基于 FreeRTOS 的轻量级消息通信与服务调用系统，适用于 ESP32 等嵌入式场景。它支持“发布/订阅”与“请求/响应”两种通信模式，解耦任务间的数据流。

#### 1.1 最小可运行示例

```cpp
#include "our.h"

struct MyData { int value; };

void publisherTask(void*) {
    auto pub = OUR::pub<MyData>("my_topic");
    MyData d{42};
    pub.publish(d);
    vTaskDelete(NULL);
}

void subscriberTask(void*) {
    auto sub = OUR::sub<MyData>("my_topic");
    QueueElement_t<MyData> elem;
    if (sub->receive(elem, 1000)) {
        Serial.printf("收到: %d\n", elem.data.value);
    }
    vTaskDelete(NULL);
}

void setup() {
    Serial.begin(115200);
    OUR::init();
    xTaskCreate(publisherTask, "pub", 2048, NULL, 1, NULL);
    xTaskCreate(subscriberTask, "sub", 2048, NULL, 1, NULL);
}
```
**规范说明**：  
- 话题名建议用英文小写+下划线，表达数据含义。
- 发布/订阅类型需一致，否则行为未定义。

---

### 2. 进阶案例：带过滤的订阅与服务调用

#### 2.1 任务ID过滤订阅

```cpp
auto sub = OUR::sub<MyData>("my_topic");
MyData d;
if (sub->receiveWithTaskId(d, TASK_SENSOR, 1000)) {
    // 只处理来自 TASK_SENSOR 的消息
}
```

#### 2.2 服务调用

```cpp
struct Req { int a; };
struct Resp { int b; };

bool myService(const Req& req, Resp& resp) {
    resp.b = req.a * 2;
    return true;
}

void setup() {
    OUR::init();
    auto server = OUR::advertiseService<Req, Resp>("double", myService);
    auto client = OUR::serviceClient<Req, Resp>("double");
    Req req{21}; Resp resp;
    if (client->call(req, resp, 1000)) {
        Serial.printf("结果: %d\n", resp.b);
    }
}
```
**规范说明**：  
- 服务名同样建议用英文小写+下划线。
- 服务回调应保证线程安全和高效。

---

### 3. 高级案例：回调订阅者与过滤队列订阅者

#### 3.1 回调订阅者

```cpp
auto cbsub = OUR::createCallbackSubscriber<MyData>("my_topic");
cbsub->subscribe([](const MyData& d, task_id_t, data_description_t) {
    Serial.printf("回调收到: %d\n", d.value);
});
```

#### 3.2 过滤队列订阅者

```cpp
auto filter = OUR::createTaskIdFilter<MyData>("my_topic");
filter->addFilterQueue(TASK_SENSOR, "sensor_data", 5);
auto sub = OUR::sub<MyData>("sensor_data");
MyData d;
if (sub->receive(d)) {
    // 只收到 TASK_SENSOR 的消息
}
```
**规范说明**：  
- 回调/过滤器建议用 lambda 或静态函数，避免捕获外部状态。
- 过滤队列命名应反映过滤条件。

---

## 第二部分：功能模块与接口详解

### 4. 话题与队列管理

- **核心文件**：`common.h/cpp`, `our_queue_helpers.hpp`
- **接口**：
  - `OUR::pub<T>(topic, size)` 创建发布者
  - `OUR::sub<T>(topic)` 创建订阅者
- **维护建议**：  
  - 话题注册与查找均受互斥锁保护，避免多任务并发破坏全局表。
  - 队列长度和消息体大小需合理预估，避免溢出或浪费。

### 5. 发布者与订阅者

- **核心文件**：`publisher.h/cpp`, `subscriber.h`
- **接口**：
  - `Publisher::publish(data, taskId, dataDesc)`
  - `Subscriber::receive(...)`
- **使用场景**：  
  - 适合一对多、数据流简单的场合。
- **维护规范**：  
  - 发布失败要有日志和重试机制。
  - 订阅者应及时处理队列，避免消息堆积。

### 6. 服务端与客户端

- **核心文件**：`server.h`, `client.h`
- **接口**：
  - `OUR::advertiseService<Req, Resp>(name, cb)`
  - `OUR::serviceClient<Req, Resp>(name)`
- **使用场景**：  
  - 适合请求-响应、参数配置、控制命令等。
- **维护规范**：  
  - 服务回调应快速返回，避免阻塞。
  - 客户端超时应有容错处理。

### 7. 回调订阅者

- **核心文件**：callback_subscriber.h
- **接口**：
  - `OUR::createCallbackSubscriber<T>(topic)`
  - `CallbackSubscriber::subscribe/callback/filter`
- **使用场景**：  
  - 需要对同一话题多种条件分发、事件驱动处理。
- **维护规范**：  
  - 回调函数应高效、无阻塞。
  - 动态添加/移除回调需注意线程安全。

### 8. 过滤队列订阅者

- **核心文件**：`filter_queue.h`, `task_id_filter.h`, `data_desc_filter.h`, `custom_filter.h`
- **接口**：
  - `OUR::createTaskIdFilter<T>(topic)`
  - `TaskIdFilter::addFilterQueue(taskId, queueName, size)`
  - `OUR::createDataDescFilter<T>(topic)`
  - `DataDescFilter::addFilterQueue(dataDesc, queueName, size)`
  - `OUR::createCustomFilter<T>(topic)`
  - `CustomFilter::addFilterQueue(filterFunc, queueName, size)`
- **使用场景**：  
  - 多任务并行处理、实时性和隔离性要求高。
- **维护规范**：  
  - 每个子队列都要有独立订阅者及时消费。
  - 过滤器任务和子队列资源需在析构时释放。

---

## 9. 订阅模式对比与选型建议

| 模式         | 适用场景           | 优点                | 缺点                |
|--------------|--------------------|---------------------|---------------------|
| 基础订阅者   | 简单/资源极限      | 低资源、易理解      | 需手动轮询和过滤    |
| 回调订阅者   | 多条件/事件驱动    | 代码简洁、灵活      | 回调串行、易阻塞    |
| 过滤队列订阅 | 并行/高实时/隔离   | 并行、隔离、灵活    | 占用资源较多        |

**选型建议**：  
- 资源极限、简单场景优先用基础订阅者。
- 需要多条件分发、事件驱动优先用回调订阅者。
- 多任务并行、实时性和隔离性要求高用过滤队列订阅者。

---

## 10. 维护与扩展规范

- 所有接口均应遵循头文件声明、实现分离，便于维护。
- 新增话题/服务/过滤器时，需在全局注册表登记，避免命名冲突。
- 代码注释需包含用途、参数、返回值、异常情况说明。
- 变更接口时，需同步更新本手册和示例代码。

---

**本手册为OUR框架官方规范文档，建议每次开发/维护/扩展前先查阅本手册，严格按照示例和接口规范进行实现，避免“虚空搓方法”与不规范用法。**

如需进一步扩展或有疑问，请参考各功能模块头文件或联系维护者。

---

如需导出为附件，建议直接使用本md文档，或在新聊天中上传本文件以便AI辅助开发与代码规范校验。
