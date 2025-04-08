/**
 * @file our_pid_controller.hpp
 * @brief 自定义 PID 控制器类的头文件
 * @author skyswordx
 * @details 该文件包含了自定义 PID 控制器类的定义和相关函数的声明
 *          使用了模板类和函数指针或 std::function 来实现灵活的控制器
 *          在 PID 的计算中预防了微分冲击的问题、还进行了合理的积分限幅处理
 */
#ifndef OUR_PID_CONTROLLER_HPP
#define OUR_PID_CONTROLLER_HPP


/* 使用示例：
    PID_controller_t<double> pid;
    
    void my_sensors_pid_task(void *pvParameters) {
        PID_controller_t<double> *pid = (PID_controller_t<double> *)pvParameters;
        pid->pid_control_service(); // 调用 PID 控制器服务函数
    }

    void setup(){
        pid.read_sensor = []() -> double {
            // 用户定义的传感器读取逻辑
            return 42.0; // 示例值
        };
        pid.convert_output = [](double output) -> double {
            // 用户定义的输出转换逻辑
            return output * 2; // 示例转换
        };

        xTaskCreatePinnedToCore(
            my_sensors_pid_task, // PID 控制器服务函数
            "my_sensors_pid_task", // 任务名称
            1024*4, // 堆栈大小
            &pid, // 传递 PID 控制器对象的指针
            2, // 任务优先级
            NULL, // 任务句柄
            1 // 运行在核心 1 上
        );
    }
*/

#include <functional> // 用于 std::function 绑定用户函数
#include <Arduino.h> 
#include "MCP4725.h"
template<typename T>
class process_data_t {
    public:
        T measure; // 测量值
        T last_measure; // 上一次测量值
        T target; // 期望值
        T error; // 误差

        // 默认构造函数，初始化测量值、期望值和误差为 0
        process_data_t() : measure(0), last_measure(0), target(0), error(0) {} 
        
};

template<typename T>
class PID_controller_t {
    public:
        process_data_t<T> process_variable; // 要控制的变量
        T controller_output; // 控制器输出

        T CONTROLLER_OUTPUT_MAX; // 控制器输出最大值
        T CONTROLLER_OUTPUT_MIN; // 控制器输出最小值

        double kp; 
        double ki; 
        double kd; 

        // 默认构造函数， controller_output 为 0
        PID_controller_t() : controller_output(T{}),  CONTROLLER_OUTPUT_MAX(T{}), CONTROLLER_OUTPUT_MIN(T{}),
                              kp(0), ki(0), kd(0),
                              read_sensor(nullptr), convert_output(nullptr) {}

        PID_controller_t(T output_max, T output_min, double kp, double ki, double kd) : 
                            CONTROLLER_OUTPUT_MAX(output_max), CONTROLLER_OUTPUT_MIN(output_min),
                            kp(kp), ki(ki), kd(kd),
                            read_sensor(nullptr), convert_output(nullptr) {
                                controller_output = T{}; // 初始化控制器输出为 0
                            }

        // 带参数的构造函数，初始化控制器输出为 0
        PID_controller_t(T output_max, T output_min, double kp, double ki, double kd, 
                         std::function<T()> read_sensor, std::function<T(T)> convert_output) : 
                            
                            CONTROLLER_OUTPUT_MAX(output_max), CONTROLLER_OUTPUT_MIN(output_min),
                            kp(kp), ki(ki), kd(kd),
                            read_sensor(read_sensor), convert_output(convert_output)
                            {
                                controller_output = T{}; // 初始化控制器输出为 0
                            }  


        // 函数指针或 std::function，用于绑定外部函数
        std::function<T()> read_sensor; // 读取传感器数据的函数
        std::function<T(T)> convert_output; // 转换控制器输出的函数

        void pid_control_service();
                            


};


/**
 * @brief PID 控制器函数
 * @author skyswordx
 * @details 该函数应该在任务中循环调用，进行 PID 控制器的计算和输出
 *          预防了微分冲击的问题、还进行了合理的积分限幅处理
 */
template<typename T>
void PID_controller_t<T>::pid_control_service() {
    /* 确保控制器输出在计算之前默认值是 0 */
    controller_output = T{};
    // printf("\n[PID_controller_t] Before, DAC_OP: %.3f(V), I_target %.3f ", controller_output, process_variable.target);
    /* 获取过程变量的测量值 */
    if (read_sensor) { // 检查传感器读取函数是否存在
        process_variable.measure = read_sensor(); // 调用用户提供的传感器读取函数
        // printf("[PID_controller_t] I_measure: %.3f(mA)\n", process_variable.measure);
    }

    /* 控制器终止条件 */
    if ((process_variable.target == 0) && (abs(process_variable.measure) < 15)) {
        // 停止输出 pid
    } else {
        /* 计算 pid 控制器的输出 */ 
        // error = target - measure
        process_variable.error = process_variable.target - process_variable.measure;

        double p_term = kp * process_variable.error;
        double i_term = ki * process_variable.error; 
        double d_term = kd * (process_variable.last_measure - process_variable.measure); 
        // d_term = kd * (error - last_error) 但是防止微分冲击 kd * (last_measure - measure)

        controller_output = p_term + i_term + d_term;
        // printf("\nraw op: %.3f", controller_output);
        /* 积分限幅和输出限幅 */
        if (controller_output > CONTROLLER_OUTPUT_MAX) { 
            // 核心在于 controller_output 计算完毕但在被限幅之前调整 i_term
            // 这样比直接把 i_term 和 controller_output 限幅在同一个值更加合理 
            i_term -= controller_output - CONTROLLER_OUTPUT_MAX;
            controller_output = CONTROLLER_OUTPUT_MAX;
        } else if (controller_output < CONTROLLER_OUTPUT_MIN) {
            i_term += CONTROLLER_OUTPUT_MIN - controller_output;
            controller_output = CONTROLLER_OUTPUT_MIN;
        }
        /* 为下一次的算法计算记住一些变量 */
        process_variable.last_measure = process_variable.measure;
    }

    /* 转换控制器输出 */
    if (convert_output) { // 检查输出转换函数是否存在
        controller_output = convert_output(controller_output); // 调用用户提供的输出转换函数
        printf("[PID_controller_t]DAC_OP/I_tagret/I_measure:%.3f,%.3f,%.3f\n", controller_output, process_variable.target, process_variable.measure);
    }
}


#endif // OUR_PID_CONTROLLER_HPP