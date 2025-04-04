#include "our_pid_controller.hpp"


/* 使用示例：
    PID_controller_t<double> pid;
    pid.read_sensor = []() -> double {
        // 用户定义的传感器读取逻辑
        return 42.0; // 示例值
    };
    pid.convert_output = [](double output) -> double {
        // 用户定义的输出转换逻辑
        return output * 2; // 示例转换
    };
*/

template<typename T>
void PID_controller_t<T>::pid_control_service() {
    /* 确保控制器输出在计算之前默认值是 0 */
    controller_output = T{};

    /* 获取过程变量的测量值 */
    if (read_sensor) {
        process_variable.measure = read_sensor(); // 调用用户提供的传感器读取函数
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
    if (convert_output) {
        controller_output = convert_output(controller_output); // 调用用户提供的输出转换函数
    }
}

template<typename T>
void PID_controller_t<T>::open_loop_step_test() {
  
    if(read_sensor) {
        process_variable.measure = read_sensor(); // 调用用户提供的传感器读取函数
    }
    if(convert_output) {
        controller_output = convert_output(controller_output); // 调用用户提供的输出转换函数
    }
    printf("\n[pid:open_loop_step_test] OP/PV:%f,%f\n", controller_output, process_variable.measure);
}
