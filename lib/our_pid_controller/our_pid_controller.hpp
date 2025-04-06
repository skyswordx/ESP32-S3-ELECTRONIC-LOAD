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
        void open_loop_step_test(); // 开环阶跃测试函数，以便获取整定 Lambda 参数所需数据


};


#endif // OUR_PID_CONTROLLER_HPP