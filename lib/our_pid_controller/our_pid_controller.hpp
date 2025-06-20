/**
 * @file our_pid_controller.hpp
 * @brief 自定义 PID 控制器类的头文件
 * @author skyswordx
 * @details 该文件包含了自定义 PID 控制器类的定义和相关函数的声明
 *          使用了模板类和函数指针或 std::function 来实现灵活的控制器
 *          在 PID 的计算中预防了微分冲击的问题、还进行了合理的积分限幅处理
 *          集成了卡尔曼滤波器来对电流测量值进行滤波，提高控制精度
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

        // 配置卡尔曼滤波器参数 (可选)
        pid.setKalmanFilterParams(0.001, 0.1); // 过程噪声, 测量噪声
        pid.enableKalmanFilter(true); // 启用卡尔曼滤波器
        
        // 如果电流测量噪声较大，可以增加测量噪声参数：
        // pid.setKalmanFilterParams(0.001, 0.5);
        
        // 如果需要更快的响应，可以减少测量噪声参数：
        // pid.setKalmanFilterParams(0.001, 0.05);

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

// 以后删掉
#define RSHUNT 0.020 // 20mΩ 电阻
inline double from_set_current_mA2voltage_V(double set_current_mA){
    return (set_current_mA * 125 * RSHUNT / 1000); // 计算电压值
}

/**
 * @brief 卡尔曼滤波器类
 * @author Triwalt
 * @details 一维卡尔曼滤波器，用于对电流测量值进行滤波
 */
template<typename T>
class KalmanFilter_t {
private:
    T q; // 过程噪声协方差
    T r; // 测量噪声协方差
    T x; // 状态估计值
    T p; // 估计误差协方差
    T k; // 卡尔曼增益
    bool initialized; // 初始化标志

public:
    // 构造函数
    KalmanFilter_t(T process_noise = 0.001, T measurement_noise = 0.1) 
        : q(process_noise), r(measurement_noise), x(T{}), p(1.0), k(T{}), initialized(false) {}
    
    /**
     * @brief 卡尔曼滤波更新
     * @param measurement 测量值
     * @return 滤波后的估计值
     */
    T update(T measurement) {
        if (!initialized) {
            // 第一次测量时，直接用测量值初始化状态
            x = measurement;
            initialized = true;
            return x;
        }
        
        // 预测步骤
        // x_pred = x (假设状态转移矩阵为1，没有控制输入)
        // p_pred = p + q
        p = p + q;
        
        // 更新步骤
        // 计算卡尔曼增益: k = p_pred / (p_pred + r)
        k = p / (p + r);
        
        // 更新状态估计: x = x_pred + k * (measurement - x_pred)
        x = x + k * (measurement - x);
        
        // 更新估计误差协方差: p = (1 - k) * p_pred
        p = (1 - k) * p;
        
        return x;
    }
    
    /**
     * @brief 重置滤波器
     */
    void reset() {
        x = T{};
        p = 1.0;
        initialized = false;
    }
    
    /**
     * @brief 设置过程噪声协方差
     * @param process_noise 过程噪声协方差
     */
    void setProcessNoise(T process_noise) {
        q = process_noise;
    }
    
    /**
     * @brief 设置测量噪声协方差
     * @param measurement_noise 测量噪声协方差
     */
    void setMeasurementNoise(T measurement_noise) {
        r = measurement_noise;
    }
};

template<typename T>
class process_data_t {
    public:
        T measure; // 测量值
        T last_measure; // 上一次测量值
        T last_output; // 上一次输出值
        T target; // 期望值
        T error; // 误差

        // 默认构造函数，初始化测量值、期望值和误差为 0
        process_data_t() : measure(T()), last_measure(T()), target(T()), error(T()) {} 
        
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

        // 卡尔曼滤波器
        KalmanFilter_t<T> kalman_filter; // 用于电流测量值滤波
        bool enable_kalman_filter; // 是否启用卡尔曼滤波器

        // 默认构造函数， controller_output 为 0
        PID_controller_t() : controller_output(T{}),  CONTROLLER_OUTPUT_MAX(T{}), CONTROLLER_OUTPUT_MIN(T{}),
                              kp(0), ki(0), kd(0),
                              kalman_filter(0.002, 0.0005), enable_kalman_filter(true),
                              read_sensor(nullptr), convert_output(nullptr) {}

        PID_controller_t(T output_max, T output_min, double kp, double ki, double kd) : 
                            CONTROLLER_OUTPUT_MAX(output_max), CONTROLLER_OUTPUT_MIN(output_min),
                            kp(kp), ki(ki), kd(kd),
                            kalman_filter(0.002, 0.0005), enable_kalman_filter(true),
                            read_sensor(nullptr), convert_output(nullptr) {
                                controller_output = T{}; // 初始化控制器输出为 0
                            }

        // 带参数的构造函数，初始化控制器输出为 0
        PID_controller_t(T output_max, T output_min, double kp, double ki, double kd, 
                         std::function<T()> read_sensor, std::function<T(T)> convert_output) : 
                            
                            CONTROLLER_OUTPUT_MAX(output_max), CONTROLLER_OUTPUT_MIN(output_min),
                            kp(kp), ki(ki), kd(kd),
                            kalman_filter(0.002, 0.0005), enable_kalman_filter(true),
                            read_sensor(read_sensor), convert_output(convert_output)
                            {
                                controller_output = T{}; // 初始化控制器输出为 0
                            }  


        // 函数指针或 std::function，用于绑定外部函数
        std::function<T()> read_sensor; // 读取传感器数据的函数
        std::function<T(T)> convert_output; // 转换控制器输出的函数

        void pid_control_service();
        
        /**
         * @brief 设置卡尔曼滤波器参数
         * @param process_noise 过程噪声协方差 (建议值: 0.001-0.01)
         * @param measurement_noise 测量噪声协方差 (建议值: 0.1-1.0)
         */
        void setKalmanFilterParams(T process_noise, T measurement_noise) {
            kalman_filter.setProcessNoise(process_noise);
            kalman_filter.setMeasurementNoise(measurement_noise);
        }
        
        /**
         * @brief 启用或禁用卡尔曼滤波器
         * @param enable true为启用，false为禁用
         */
        void enableKalmanFilter(bool enable) {
            enable_kalman_filter = enable;
            if (!enable) {
                kalman_filter.reset(); // 禁用时重置滤波器
            }
        }
                            


};


/**
 * @brief PID 控制器函数
 * @author skyswordx
 * @details 该函数应该在任务中循环调用，进行 PID 控制器的计算和输出
 *          预防了微分冲击的问题、还进行了合理的积分限幅处理
 *          集成了卡尔曼滤波器对电流测量值进行滤波
 */
template<typename T>
void PID_controller_t<T>::pid_control_service() {
    /* 第一次进入时初始化process_variable.last_output */
    if (process_variable.last_output == 0) {
        process_variable.last_output = CONTROLLER_OUTPUT_MIN; // 初始化上一次输出值为最小值
    }
    /* 确保控制器输出在计算之前默认值是 0 */
    controller_output = T{};
    // printf("\n[PID_controller_t] Before, DAC_OP: %.3f(V), I_target %.3f ", controller_output, process_variable.target);
    
    /* 获取过程变量的测量值 */
    if (read_sensor) { // 检查传感器读取函数是否存在
        T raw_measurement = read_sensor(); // 调用用户提供的传感器读取函数获取原始测量值
        
        // 应用卡尔曼滤波器
        if (enable_kalman_filter) {
            process_variable.measure = kalman_filter.update(raw_measurement); // 使用卡尔曼滤波后的值
            // printf("[PID_controller_t] Raw: %.3f(mA), Filtered: %.3f(mA)\n", raw_measurement, process_variable.measure);
        } else {
            process_variable.measure = raw_measurement; // 直接使用原始测量值
        }
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

        controller_output = process_variable.last_output + p_term + i_term + d_term;
        // printf("\nraw op: %.3f", controller_output);
        /* 积分限幅和输出限幅 */

        // 限幅控制器输出
        double controller_output_limited = min(3 * from_set_current_mA2voltage_V(process_variable.target), CONTROLLER_OUTPUT_MAX); // 3倍的电流值对应的电压值
       
        if (controller_output > controller_output_limited) { 
            // 核心在于 controller_output 计算完毕但在被限幅之前调整 i_term
            // 这样比直接把 i_term 和 controller_output 限幅在同一个值更加合理 
            i_term -= controller_output - controller_output_limited;
            controller_output = controller_output_limited;
        } else if (controller_output < CONTROLLER_OUTPUT_MIN) {
            i_term += CONTROLLER_OUTPUT_MIN - controller_output;
            controller_output = CONTROLLER_OUTPUT_MIN;
        }
        
        /* 为下一次的算法计算记住一些变量 */
        process_variable.last_measure = process_variable.measure;
        process_variable.last_output = controller_output; // 上一次输出值
    }

    /* 转换控制器输出 */
    if (convert_output) { // 检查输出转换函数是否存在
        // controller_output = convert_output(process_variable.target * 0.00002 * 125); // 调用用户提供的输出转换函数
        controller_output = convert_output(controller_output); // 调用用户提供的输出转换函数
        // printf("DAC_OP/I_t/I_m/error:%.3f,%.3f,%.3f,%.3f\n", controller_output, process_variable.target, process_variable.measure, process_variable.error);
    }
}


#endif // OUR_PID_CONTROLLER_HPP