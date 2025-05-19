/**
 * @file ButtonTask.h
 * @brief 按键任务类定义
 * @author Triwalt
 * @date 2025-05-16
 * @details 定义处理按键输入的任务
 */

#pragma once

#include "../core/Task.h"
#include "../devices/Button.h"
#include <vector>

// 按键任务类
class ButtonTask : public Task {
public:
    /**
     * @brief 构造函数
     * @param name 任务名称
     * @param stackSize 任务栈大小
     * @param priority 任务优先级
     * @param coreId 运行核心ID
     */
    ButtonTask(const String& name = "ButtonTask",
              uint32_t stackSize = 4096,
              UBaseType_t priority = 2,
              BaseType_t coreId = 1);
    
    virtual ~ButtonTask();
    
    /**
     * @brief 添加按键设备到任务
     * @param button 按键设备指针
     * @return 成功返回true，失败返回false
     */
    bool addButton(Button* button);
    
    /**
     * @brief 删除按键设备
     * @param button 按键设备指针
     * @return 成功返回true，失败返回false
     */
    bool removeButton(Button* button);
    
    /**
     * @brief 根据名称获取按键设备
     * @param name 按键名称
     * @return 按键设备指针，如果未找到则返回nullptr
     */
    Button* getButton(const String& name) const;
    
protected:
    /**
     * @brief 任务运行函数
     */
    virtual void run() override;
    
private:
    std::vector<Button*> buttons_;    // 按键列表
};
