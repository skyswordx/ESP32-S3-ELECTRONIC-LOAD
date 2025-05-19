/**
 * @file ButtonTask.cpp
 * @brief 按键任务类实现
 * @author Triwalt
 * @date 2025-05-16
 * @details 实现处理按键输入的任务
 */

#include "ButtonTask.h"
#include <algorithm>

ButtonTask::ButtonTask(const String& name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId)
    : Task(name, stackSize, priority, coreId)
{
}

ButtonTask::~ButtonTask() {
    // 不需要删除按键对象，因为它们是外部传入的
}

bool ButtonTask::addButton(Button* button) {
    if (button == nullptr) {
        return false;
    }
    
    // 检查是否已经存在
    auto it = std::find(buttons_.begin(), buttons_.end(), button);
    if (it != buttons_.end()) {
        return false; // 已存在
    }
    
    buttons_.push_back(button);
    return true;
}

bool ButtonTask::removeButton(Button* button) {
    if (button == nullptr) {
        return false;
    }
    
    auto it = std::find(buttons_.begin(), buttons_.end(), button);
    if (it == buttons_.end()) {
        return false; // 不存在
    }
    
    buttons_.erase(it);
    return true;
}

Button* ButtonTask::getButton(const String& name) const {
    for (auto button : buttons_) {
        if (button->getName() == name) {
            return button;
        }
    }
    return nullptr;
}

void ButtonTask::run() {
    // 初始化所有按键
    for (auto button : buttons_) {
        if (!button->isInitialized()) {
            button->init();
        }
    }
    
    // 任务主循环
    while (isRunning()) {
        // 更新所有按键状态
        for (auto button : buttons_) {
            button->update();
        }
        
        // 适当延时，减轻CPU负担
        vTaskDelay(pdMS_TO_TICKS(10)); // 10ms的扫描间隔
    }
}
