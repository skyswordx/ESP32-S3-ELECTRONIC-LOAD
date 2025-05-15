/**
 * @file EncoderTask.cpp
 * @brief 编码器任务类实现
 * @author Triwalt
 * @date 2025-05-15
 * @details 负责周期性更新编码器状态
 */

#include "EncoderTask.h"

EncoderTask::EncoderTask(const String& name,
                       std::shared_ptr<Encoder> encoder,
                       uint32_t updateInterval)
    : Task(name),
      encoder_(encoder),
      updateInterval_(updateInterval)
{
}

void EncoderTask::run() {
    Serial.printf("[%s] Started encoder task for device '%s'\n", 
                 getName().c_str(), encoder_->getName().c_str());
    
    // 确保编码器已初始化
    if (!encoder_->isInitialized()) {
        if (!encoder_->init()) {
            Serial.printf("[%s] Failed to initialize encoder '%s'\n", 
                         getName().c_str(), encoder_->getName().c_str());
            return; // 任务终止
        }
    }
    
    // 主任务循环
    while (true) {
        // 更新编码器状态
        encoder_->update();
        
        // 周期性延时
        vTaskDelay(pdMS_TO_TICKS(updateInterval_));
    }
}