#include "EventBus.h"
#include "EncoderEvent.h"
#include <Arduino.h>

class EncoderListener : public EventListener {
public:
    bool onEvent(const Event& event) override {
        if (event.getType() == EventType::ENCODER_CHANGED) {
            const auto& encoderEvent = static_cast<const EncoderValueChangedEvent&>(event);
            Serial.println("Encoder Value: " + String(encoderEvent.getValue()));
            return true;
        }
        return false;
    }
};

void setup() {
    Serial.begin(115200);
    EventBus& bus = EventBus::getInstance();

    // 创建并注册监听器
    static EncoderListener listener;
    bus.subscribe(EventType::ENCODER_CHANGED, &listener);

    // 模拟发布事件
    EncoderValueChangedEvent event(123.45);
    bus.publish(event);
}

void loop() {
    // 主循环中可以添加其他逻辑
}