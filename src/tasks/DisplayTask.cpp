/**
 * @file DisplayTask.cpp
 * @brief 显示任务类实现
 * @author Triwalt
 * @date 2025-05-16
 * @details 基于LVGL的显示处理任务，负责界面更新和处理
 */

#include "DisplayTask.h"
#include "../devices/EncoderEvent.h"
#include "../devices/Button.h"
#include <lvgl.h>

// LVGL相关头文件（根据实际项目调整）
#include "../../lib/lvgl_port/lv_port_disp.h"
#include "../../lib/lvgl_port/lv_port_indev.h"
#include "../../lib/generated/gui_guider.h"

DisplayTask::DisplayTask(const String& name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId)
    : Task(name, stackSize, priority, coreId),
      displayInitialized_(false),
      lastUpdateTime_(0),
      updateInterval_(5), // 5ms的更新间隔，通常LVGL推荐的刷新率
      currentMode_(DisplayMode::MAIN),
      currentSensor_(nullptr),
      protectionController_(nullptr)
{
}

DisplayTask::~DisplayTask() {
    // 取消事件监听
    EventBus::getInstance().unregisterListener(this);
}

bool DisplayTask::initDisplay() {
    // 避免重复初始化
    if (displayInitialized_) {
        return true;
    }
    
    // 初始化LVGL库
    lv_init();
      // 初始化显示设备
    lv_port_disp_init();  // 返回类型是void
    
    // 初始化输入设备（如果有触摸屏）
    lv_port_indev_init();
    
    // 注册事件监听
    registerEventListeners();
    
    // 加载主界面
    loadMainScreen();
    
    displayInitialized_ = true;
    Serial.println("Display initialization successful!");
    return true;
}

void DisplayTask::run() {
    // 初始化显示
    if (!initDisplay()) {
        Serial.println("Failed to initialize display, task terminating");
        return;
    }
    
    // 显示任务主循环
    while (isRunning()) {
        unsigned long currentTime = millis();
        
        // 根据指定的更新间隔更新显示
        if (currentTime - lastUpdateTime_ >= updateInterval_) {
            updateDisplay();
            lastUpdateTime_ = currentTime;
        }
        
        // 适当延时，减少CPU负担
        vTaskDelay(pdMS_TO_TICKS(1)); // 1ms延迟
    }
}

void DisplayTask::updateDisplay() {
    // 处理LVGL任务和绘制
    lv_task_handler();
    
    // 更新系统状态显示
    static unsigned long lastStatusUpdateTime = 0;
    unsigned long currentTime = millis();
    
    // 每秒更新一次系统状态
    if (currentTime - lastStatusUpdateTime >= 1000) {
        lastStatusUpdateTime = currentTime;
        
        // 获取UI元素
        extern lv_ui guider_ui;
        
        // 更新运行时间
        static uint32_t runningSeconds = 0;
        runningSeconds++;
        
        uint32_t hours = runningSeconds / 3600;
        uint32_t minutes = (runningSeconds % 3600) / 60;
        uint32_t seconds = runningSeconds % 60;
        
        if (guider_ui.main_page_running_time) {
            char timeStr[16];
            snprintf(timeStr, sizeof(timeStr), "%02lu:%02lu:%02lu", hours, minutes, seconds);
            lv_label_set_text(guider_ui.main_page_running_time, timeStr);
        }
        
        // 如果有电流传感器，更新测量值
        if (currentSensor_) {
            // 读取测量值
            double current = currentSensor_->getCurrent();
            double voltage = currentSensor_->getVoltage();
            double power = currentSensor_->getPower();
            double resistance = currentSensor_->getResistance();
            
            // 创建队列元素并发送到GUI更新队列
            if (LVGL_queue != NULL) {
                // 更新电流值
                QueueElement_t<double> currentElement(TASK_INA226, DATA_DESCRIPTION_CURRENT);
                currentElement.data = current;
                xQueueSend(LVGL_queue, &currentElement, 0);
                
                // 更新电压值
                QueueElement_t<double> voltageElement(TASK_INA226, DATA_DESCRIPTION_VOLTAGE);
                voltageElement.data = voltage;
                xQueueSend(LVGL_queue, &voltageElement, 0);
                
                // 更新功率值
                QueueElement_t<double> powerElement(TASK_INA226, DATA_DESCRIPTION_POWER);
                powerElement.data = power;
                xQueueSend(LVGL_queue, &powerElement, 0);
                
                // 更新电阻值
                QueueElement_t<double> resistanceElement(TASK_INA226, DATA_DESCRIPTION_RESISTANCE);
                resistanceElement.data = resistance;
                xQueueSend(LVGL_queue, &resistanceElement, 0);
            }
            
            // 如果有保护控制器，检查保护状态
            if (protectionController_) {
                // 检查各种保护状态
                if (protectionController_->isOverVoltageProtectionTriggered()) {
                    handleProtectionEvent(Event(EventType::OVER_VOLTAGE));
                }
                
                if (protectionController_->isOverCurrentProtectionTriggered()) {
                    handleProtectionEvent(Event(EventType::OVER_CURRENT));
                }
                
                if (protectionController_->isOverPowerProtectionTriggered()) {
                    handleProtectionEvent(Event(EventType::OVER_POWER));
                }
            }
        } else {
            // 如果没有电流传感器，使用模拟值（仅用于测试）
            static float temperature = 25.0f;
            temperature += (random(10) - 5) * 0.1f; // 在上次温度基础上随机波动
            temperature = constrain(temperature, 20.0f, 80.0f); // 限制温度范围
            
            if (guider_ui.main_page_temperature_label) {
                char tempStr[8];
                snprintf(tempStr, sizeof(tempStr), "%.1f", temperature);
                lv_label_set_text(guider_ui.main_page_temperature_label, tempStr);
            }
            
            // 更新图表数据（如果需要）
            static int chartUpdateCounter = 0;
            if (++chartUpdateCounter >= 5) { // 每5秒更新一次图表
                chartUpdateCounter = 0;
                
                // 电流图表更新
                if (guider_ui.main_page_measure_current_chart && guider_ui.main_page_measure_current_chart_0) {
                    lv_chart_set_next(guider_ui.main_page_measure_current_chart, 
                                    guider_ui.main_page_measure_current_chart_0, 
                                    random(100)); // 实际项目中应替换为真实电流值
                }
                
                // 电压图表更新
                if (guider_ui.main_page_measure_voltage_chart && guider_ui.main_page_measure_voltage_chart_0) {
                    lv_chart_set_next(guider_ui.main_page_measure_voltage_chart, 
                                    guider_ui.main_page_measure_voltage_chart_0, 
                                    random(100)); // 实际项目中应替换为真实电压值
                }
            }
        }
    }
}

void DisplayTask::registerEventListeners() {
    // 注册为事件监听器
    EventBus& eventBus = EventBus::getInstance();
    
    // 监听所有事件类型
    eventBus.registerListener(this);
}

void DisplayTask::onEvent(const Event& event) {
    // 根据事件类型分发到对应的处理函数
    EventType type = event.getType();
    
    // 编码器事件
    if (type == EventType::ENCODER_CHANGED) {
        handleEncoderEvent(event);
        return;
    }
    
    // 按钮事件
    if (type == EventType::BUTTON) {
        handleButtonEvent(event);
        return;
    }
    
    // 测量事件处理
    if (type == EventType::CURRENT_CHANGED || 
        type == EventType::VOLTAGE_CHANGED || 
        type == EventType::POWER_CHANGED || 
        type == EventType::RESISTANCE_CHANGED) {
        handleMeasurementEvent(event);
        return;
    }
    
    // 保护事件处理
    if (type == EventType::OVER_VOLTAGE || 
        type == EventType::OVER_CURRENT || 
        type == EventType::OVER_POWER) {
        handleProtectionEvent(event);
        return;
    }
    
    // 控制事件处理
    if (type == EventType::TARGET_CURRENT_CHANGED) {
        handleControlEvent(event);
        return;
    }
    
    // UI事件处理
    if (type == EventType::UI_PAGE_CHANGED) {
        handleUIEvent(event);
        return;
    }
}

void DisplayTask::handleEncoderEvent(const Event& event) {
    // 转换为编码器事件
    const EncoderValueChangedEvent& encoderEvent = static_cast<const EncoderValueChangedEvent&>(event);
    
    // 获取编码器值
    double value = encoderEvent.getValue();
    
    // 获取当前激活的界面元素
    extern lv_ui guider_ui;
    
    // 将编码器值更新到系统总线
    // 创建队列元素
    QueueElement_t<double> queue_element(TASK_ENCODER, DATA_DESCRIPTION_SET_CURRENT);
    queue_element.data = value;
    
    // 发送到GUI更新队列
    if (LVGL_queue != NULL) {
        xQueueSend(LVGL_queue, &queue_element, portMAX_DELAY);
        Serial.printf("Encoder value sent to GUI queue: %.2f\n", value);
    } else {
        // 如果队列不可用，直接更新UI
        if (guider_ui.main_page_set_current_box) {
            lv_spinbox_set_value(guider_ui.main_page_set_current_box, (int16_t)value);
            Serial.printf("Encoder value directly updated: %.2f\n", value);
        }
    }
}

void DisplayTask::handleButtonEvent(const Event& event) {
    // 处理按钮事件
    const ButtonEvent& buttonEvent = static_cast<const ButtonEvent&>(event);
    ButtonEventType buttonType = buttonEvent.getEventType();
    const String& buttonName = buttonEvent.getButtonName();
    
    extern lv_ui guider_ui;
    
    Serial.printf("Button event: %s, type: %d\n", buttonName.c_str(), static_cast<int>(buttonType));
    
    if (buttonName == "PowerButton") {
        // 处理电源按钮事件
        if (buttonType == ButtonEventType::CLICKED) {
            // 切换电子负载开关状态
            static bool powerOn = false;
            powerOn = !powerOn;
            
            // 更新UI元素状态
            if (guider_ui.main_page_ONOFF_btn) {
                lv_obj_add_state(guider_ui.main_page_ONOFF_btn, powerOn ? LV_STATE_CHECKED : 0);
                lv_obj_clear_state(guider_ui.main_page_ONOFF_btn, powerOn ? 0 : LV_STATE_CHECKED);
                
                if (guider_ui.main_page_ONOFF_label) {
                    lv_label_set_text(guider_ui.main_page_ONOFF_label, powerOn ? "ON" : "OFF");
                }
            }
            
            // 设置保护状态LED
            if (guider_ui.main_page_status_led) {
                lv_led_set_color(guider_ui.main_page_status_led, powerOn ? lv_color_hex(0x00FF00) : lv_color_hex(0x808080));
                lv_led_set_brightness(guider_ui.main_page_status_led, powerOn ? 255 : 100);
                lv_led_on(guider_ui.main_page_status_led);
            }
        }
    } else if (buttonName == "ModeButton") {
        // 处理模式切换按钮事件
        if (buttonType == ButtonEventType::CLICKED) {
            // 在不同模式之间循环切换
            static int currentMode = 0;
            currentMode = (currentMode + 1) % 3; // 假设有3种模式
            
            if (guider_ui.main_page_mode_btn && guider_ui.main_page_mode_label) {
                // 根据模式设置显示文本
                switch (currentMode) {
                    case 0:
                        lv_label_set_text(guider_ui.main_page_mode_label, "CC");
                        break;
                    case 1:
                        lv_label_set_text(guider_ui.main_page_mode_label, "CV");
                        break;
                    case 2:
                        lv_label_set_text(guider_ui.main_page_mode_label, "CR");
                        break;
                }
            }
        }
    } else if (buttonName == "MenuButton") {
        // 处理菜单按钮事件
        if (buttonType == ButtonEventType::CLICKED) {
            // 切换到设置屏幕
            switchDisplayMode(DisplayMode::SETTINGS);
        }
    }
}

void DisplayTask::loadMainScreen() {
    // 初始化GUI界面
    extern lv_ui guider_ui;
    
    // 设置界面
    setup_ui(&guider_ui);
    
    // 加载主页面
    setup_scr_main_page(&guider_ui);
    lv_scr_load(guider_ui.main_page);
    
    // 初始化界面默认值
    if (guider_ui.main_page_set_current_box) {
        lv_spinbox_set_value(guider_ui.main_page_set_current_box, 0); // 设置默认电流为0mA
    }
    
    if (guider_ui.main_page_over_voltage_box) {
        lv_spinbox_set_value(guider_ui.main_page_over_voltage_box, 12); // 设置默认过压保护为12V
    }
    
    // 设置ONOFF按钮初始状态为OFF
    if (guider_ui.main_page_ONOFF) {
        lv_btn_set_state(guider_ui.main_page_ONOFF, LV_BTN_STATE_RELEASED);
        if (guider_ui.main_page_ONOFF_label) {
            lv_label_set_text(guider_ui.main_page_ONOFF_label, "OFF");
        }
    }
    
    Serial.println("Main screen loaded successfully");
}

void DisplayTask::handleMeasurementEvent(const Event& event) {
    EventType type = event.getType();
    
    extern lv_ui guider_ui;
    
    // 根据事件类型处理不同的测量值更新
    switch (type) {
        case EventType::CURRENT_CHANGED: {
            // 处理电流变化事件
            double current = event.getDoubleValue();
            char buffer[16];
            snprintf(buffer, sizeof(buffer), "%.3f A", current);
            
            if (guider_ui.main_page_measure_current_label) {
                lv_label_set_text(guider_ui.main_page_measure_current_label, buffer);
            }
            
            // 更新电流图表
            if (guider_ui.main_page_measure_current_chart && guider_ui.main_page_measure_current_chart_0) {
                // 将电流值映射到图表范围
                int32_t chart_value = (int32_t)(current * 100); // 假设图表范围是0-100
                lv_chart_set_next(guider_ui.main_page_measure_current_chart, 
                                  guider_ui.main_page_measure_current_chart_0, 
                                  chart_value);
            }
            break;
        }
        
        case EventType::VOLTAGE_CHANGED: {
            // 处理电压变化事件
            double voltage = event.getDoubleValue();
            char buffer[16];
            snprintf(buffer, sizeof(buffer), "%.3f V", voltage);
            
            if (guider_ui.main_page_measure_voltage_label) {
                lv_label_set_text(guider_ui.main_page_measure_voltage_label, buffer);
            }
            
            // 更新电压图表
            if (guider_ui.main_page_measure_voltage_chart && guider_ui.main_page_measure_voltage_chart_0) {
                // 将电压值映射到图表范围
                int32_t chart_value = (int32_t)(voltage * 10); // 假设图表范围是0-100
                lv_chart_set_next(guider_ui.main_page_measure_voltage_chart, 
                                  guider_ui.main_page_measure_voltage_chart_0, 
                                  chart_value);
            }
            break;
        }
        
        case EventType::POWER_CHANGED: {
            // 处理功率变化事件
            double power = event.getDoubleValue();
            char buffer[16];
            snprintf(buffer, sizeof(buffer), "%.3f W", power);
            
            if (guider_ui.main_page_measure_power_label) {
                lv_label_set_text(guider_ui.main_page_measure_power_label, buffer);
            }
            
            // 更新功率显示
            // 这里可以增加更多功率相关显示的更新
            break;
        }
        
        case EventType::RESISTANCE_CHANGED: {
            // 处理电阻变化事件
            double resistance = event.getDoubleValue();
            char buffer[16];
            
            // 根据电阻值大小选择合适的单位
            if (resistance >= 1000) {
                snprintf(buffer, sizeof(buffer), "%.2f kΩ", resistance / 1000.0);
            } else {
                snprintf(buffer, sizeof(buffer), "%.2f Ω", resistance);
            }
            
            if (guider_ui.main_page_measure_resistance_label) {
                lv_label_set_text(guider_ui.main_page_measure_resistance_label, buffer);
            }
            break;
        }
        
        default:
            break;
    }
}

void DisplayTask::handleProtectionEvent(const Event& event) {
    EventType eventType = event.getType();
    
    extern lv_ui guider_ui;
    
    // 根据保护事件类型处理
    switch (eventType) {
        case EventType::OVER_VOLTAGE:
            Serial.println("Protection: Over Voltage detected!");
            
            // 更新UI状态为过压保护
            if (guider_ui.main_page_status_label) {
                lv_label_set_text(guider_ui.main_page_status_label, "OVP!");
            }
            
            // 设置警告LED为红色
            if (guider_ui.main_page_status_led) {
                lv_led_set_color(guider_ui.main_page_status_led, lv_color_hex(0xFF0000));
                lv_led_set_brightness(guider_ui.main_page_status_led, 255);
                lv_led_on(guider_ui.main_page_status_led);
            }
            break;
            
        case EventType::OVER_CURRENT:
            Serial.println("Protection: Over Current detected!");
            
            // 更新UI状态为过流保护
            if (guider_ui.main_page_status_label) {
                lv_label_set_text(guider_ui.main_page_status_label, "OCP!");
            }
            
            // 设置警告LED
            if (guider_ui.main_page_status_led) {
                lv_led_set_color(guider_ui.main_page_status_led, lv_color_hex(0xFF4000));
                lv_led_set_brightness(guider_ui.main_page_status_led, 255);
                lv_led_on(guider_ui.main_page_status_led);
            }
            break;
            
        case EventType::OVER_POWER:
            Serial.println("Protection: Over Power detected!");
            
            // 更新UI状态为过功率保护
            if (guider_ui.main_page_status_label) {
                lv_label_set_text(guider_ui.main_page_status_label, "OPP!");
            }
            
            // 设置警告LED
            if (guider_ui.main_page_status_led) {
                lv_led_set_color(guider_ui.main_page_status_led, lv_color_hex(0xFF8000));
                lv_led_set_brightness(guider_ui.main_page_status_led, 255);
                lv_led_on(guider_ui.main_page_status_led);
            }
            break;
            
        case EventType::OVER_TEMPERATURE:
            Serial.println("Protection: Over Temperature detected!");
            
            // 更新UI状态为过温保护
            if (guider_ui.main_page_status_label) {
                lv_label_set_text(guider_ui.main_page_status_label, "OTP!");
            }
            
            // 设置警告LED
            if (guider_ui.main_page_status_led) {
                lv_led_set_color(guider_ui.main_page_status_led, lv_color_hex(0xFF00FF));
                lv_led_set_brightness(guider_ui.main_page_status_led, 255);
                lv_led_on(guider_ui.main_page_status_led);
            }
            break;
            
        default:
            break;
    }
    
    // 如果保护控制器可用，检查是否需要切换到保护界面
    if (protectionController_ && protectionController_->isAnyProtectionTriggered()) {
        // 自动切换到保护界面
        switchDisplayMode(DisplayMode::PROTECTION);
    }
}

void DisplayTask::handleControlEvent(const Event& event) {
    EventType type = event.getType();
    
    extern lv_ui guider_ui;
    
    // 处理目标电流变化事件
    if (type == EventType::TARGET_CURRENT_CHANGED) {
        double targetCurrent = event.getDoubleValue();
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "%.3f A", targetCurrent);
        
        // 更新设置电流值显示
        if (guider_ui.main_page_set_current_label) {
            lv_label_set_text(guider_ui.main_page_set_current_label, buffer);
        }
        
        // 如果有滑块控件，更新滑块位置
        if (guider_ui.main_page_set_current_slider) {
            // 假设滑块范围是0-100，需要将电流值映射到此范围
            int32_t slider_value = (int32_t)(targetCurrent * 100.0 / 5.0); // 假设最大电流是5A
            lv_slider_set_value(guider_ui.main_page_set_current_slider, slider_value, LV_ANIM_ON);
        }
    }
    // 处理其他控制事件类型
    // ...
}

void DisplayTask::handleUIEvent(const Event& event) {
    // 获取事件类型
    EventType type = event.getType();
    
    // 获取UI元素
    extern lv_ui guider_ui;
    
    // 处理UI事件
    if (type == EventType::UI_PAGE_CHANGED) {
        // 在实际应用中，应该从具体的事件子类中获取页面ID
        // 暂时使用固定值来演示功能
        int pageId = 0; // 主页面
        
        if (guider_ui.main_page_main_tileview) {
            lv_tabview_set_tab_act(guider_ui.main_page_main_tileview, pageId, LV_ANIM_ON);
            Serial.printf("UI page changed to: %d\n", pageId);
        }
    }
}

void DisplayTask::switchDisplayMode(DisplayMode mode) {
    // 如果模式没有变化，直接返回
    if (mode == currentMode_) {
        return;
    }
    
    // 更新当前模式
    currentMode_ = mode;
    
    // 获取UI元素
    extern lv_ui guider_ui;
    
    // 根据不同的显示模式切换页面
    switch (mode) {
    case DisplayMode::MAIN:
        // 切换到主界面
        if (guider_ui.main_page_main_tileview) {
            lv_tabview_set_tab_act(guider_ui.main_page_main_tileview, 0, LV_ANIM_ON);
        }
        break;
        
    case DisplayMode::SETTINGS:
        // 切换到设置界面
        if (guider_ui.main_page_main_tileview) {
            lv_tabview_set_tab_act(guider_ui.main_page_main_tileview, 1, LV_ANIM_ON);
        }
        break;
        
    case DisplayMode::CHART:
        // 切换到图表界面
        // 实际项目中可能需要创建专门的图表页面
        break;
        
    case DisplayMode::PROTECTION:
        // 切换到保护设置界面
        if (guider_ui.main_page_main_tileview) {
            lv_tabview_set_tab_act(guider_ui.main_page_main_tileview, 2, LV_ANIM_ON);
        }
        break;
        
    case DisplayMode::CALIBRATION:
        // 切换到校准界面
        // 实际项目中可能需要创建专门的校准页面
        break;
    }
    
    Serial.printf("Display mode switched to: %d\n", static_cast<int>(mode));
}
