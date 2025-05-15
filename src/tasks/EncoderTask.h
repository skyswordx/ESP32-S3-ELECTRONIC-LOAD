/**
 * @file EncoderTask.h
 * @brief 编码器任务类定义
 * @author Triwalt
 * @date 2025-05-15
 * @details 负责周期性更新编码器状态
 */

#ifndef ENCODER_TASK_H
#define ENCODER_TASK_H

#include "../core/Task.h"
#include "../devices/Encoder.h"
#include <memory>

/**
 * @brief 编码器任务类
 */
class EncoderTask : public Task {
public:
    /**
     * @brief 构造函数
     * @param name 任务名称
     * @param encoder 编码器设备
     * @param updateInterval 更新间隔(毫秒)
     */
    EncoderTask(const String& name, 
               std::shared_ptr<Encoder> encoder,
               uint32_t updateInterval = 20);
               
    /**
     * @brief 析构函数
     */
    virtual ~EncoderTask() = default;
    
protected:
    /**
     * @brief 任务运行函数
     */
    void run() override;
    
private:
    std::shared_ptr<Encoder> encoder_; ///< 编码器设备
    uint32_t updateInterval_;         ///< 更新间隔(毫秒)
};

#endif // ENCODER_TASK_H