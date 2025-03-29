#ifndef _OUR_MOTOR_CONTROLLER_H
#define _OUR_MOTOR_CONTROLLER_H

#include <Arduino.h>
#include "our_robot_config.h"
// #include "PID_v1.h" 没包含，但是参考了思路

#include "ESP32Encoder.h"
// #include "InterruptEncoder.h"
// 暂时用不到

class Motor{
    public:// 给外部接口的声明
        Motor(); // 构造函数，在创建对象的时候会被调用

    /* 电机的硬件属性初始化 */
        ESP32Encoder encoder; // 声明电机的编码器
        int pin_pwm; // 减速电机的 PWM 引脚
        bool flag_of_motor_direction;   // 减速电机的驱动方向引脚
        int pin_encoder_A;   // 减速电机的编码器 A 相引脚
        int pin_encoder_B;   // 减速电机的编码器 B 相引脚
        // A 和 B 相的引脚是用来检测电机的转向的，用来检测电机的转速
        // 两个引脚的波形是正交的，相差 90 度
        // 以上变量的初始化放在下面的 attach 函数里面
        int attach(int pin_pwm, bool flag_of_motor_direction, int pin_encoder_A, int pin_encoder_B); // 初始化电机的引脚、电机的转向和电机的编码器


        // 由于电机的减速组，编码器码盘，还有空载和负载的区别，
        // 编码器在一段时间测量的脉冲（measure_encoder_count_in_velocity）在不同的情况下会有不同的值
        // 所以设计一系列比例系数，用来调整 
        double ratio_of_load = 1.0; // 空载和负载的比例系数
        double ppr = 11; // 电机编码器转一圈的脉冲数
        double ratio_of_reduction = 30; // 电机的减速比

    /* 电机的控制 */
        int64_t read_encoder_count_and_clear(); // 读取当前编码器的脉冲数并清零

        // 普通并联 pid 控制器，用于控制电机的转速
        double target_encoder_count_in_velocity = 0.0; // 速度环的期望转速，是编码器脉冲数单位
        double measure_encoder_count_in_velocity = 0.0; // 测量到的转速，是编码器脉冲数单位，由一段时间内编码器采样值得到
        double output_in_velocity = 0.0; // 速度环输出的 pwm 信号
        double error_in_velocity; // 速度环期望转速和测量值的误差
        double i_term_in_velocity = 0.0; // 速度环的积分项
        double last_error_in_velocity = 0.0; // 速度环的上一次误差
        double last_input_in_velocity = 0.0; // 速度环的上一次测量值
        double last_last_input_in_velocity = 0.0; // 速度环的上上次测量值
        double kp_in_velocity, ki_in_velocity, kd_in_velocity; // 速度环的 pid 控制器的参数
        void pid_velocity_controller(); // 速度 PID 控制器，对电机的转速进行控制

        // 普通并联 pid 控制器，用于控制电机的转的圈数
        double target_encoder_count_in_position = 0.0; // 位置环的期望转的圈数，对应位置，是编码器脉冲数单位，把一段时间内编码器脉冲数不断累加得到
        double measure_encoder_count_in_position = 0.0; // 测量到的转圈位置，是编码器脉冲数单位
        double output_in_position = 0.0; // 位置环输出的 pwm 信号
        double error_in_position; // 位置环期望转圈数和测量值的误差
        double last_error_in_position = 0.0; // 位置环的上一次误差
        double last_input_in_position = 0.0; // 位置环的上一次测量值
        double i_term_in_position = 0.0; // 位置环的积分项
        double kp_in_position, ki_in_position, kd_in_position; // 位置环的 pid 控制器的参数
        void pid_position_controller(); // 位置 PID 控制器，对电机的转的圈数进行控制

        // 增量式 pid 控制器，暂时不用
        void incremental_pid_controller();

        // 开环阶跃测试，用于整定位置式 PID 控制器
        void open_loop_step_test(int flag); // 在里面打印 pid 控制器输出值和过程变量（测量值）

        // 重新开始 pid 控制器的时候更新配置参数，暂时不用
        void update_config_when_restart_pid_controller(); 
        
    /* 运动信息和电机编码器之间的转换接口 */
        double from_count_toget_rpm(double encoder_count); //根据测量到的一个采样时间内的编码器脉冲数计算对应的电机转速
        double from_revolution_toget_count(double number_of_revolution); // 根据电机转多少圈计算需要对应总计多少的编码器脉冲数
        double from_rpm_toget_count(double rpm); // 根据电机的转速计算对应一个采样时间内应该要得到的编码器脉冲数
        
        double output_final = 0.0; // 为了同时使用多个环控制，把所有环的输出值叠加在这个成员变量
        void from_output_final_to_set_pwm(); // 把 output_final 的值转换成 pwm 信号并加上死区电压输出到电机
        void set_motor_direction(); // 设置电机的转向，主要被 from_output_final_to_set_pwm 调用

        // 利用控制器的 demo 演示 
        void position_in_serial_connection_to_velocity(); // 位置环串联速度环，未测试，暂时不用
        void only_velocity_control_service();
        void only_position_control_service();
        
    /* 用于测试预留的成员空间 
        void test_service();
        void pid_test_controller();
        double test_variable1 = 0.0;
        double test_variable2 = 0.0;
        double test_variable3 = 0.0; */

    private:
        // 没想好要把上面的谁放过来，我还是希望使用者对这个对象有足够的控制权，所以把上面的都放在 public 里面
};

#define FOR_POSITION_PID 0 // 开环测试时，使用位置 PID 控制器，搞这个的原因主要是串口的通信协议要区分
#define FOR_VELOCITY_PID 1 // 开环测试时，使用速度 PID 控制器，搞这个的原因主要是串口的通信协议要区分

#define SAMPLE_TIME_ms 10 // 10ms 采样周期
#define CONVERT_ms_TO_us(ms) ((ms) * 1000) // 把毫秒转换成微秒的宏定义，主要是给定时器设定用的

void backforward_kinematics(double vx, double vy, double omega, double* wheel_omega_speed1, double* wheel_omega_speed2, double* wheel_omega_speed3, double* wheel_omega_speed4);
//麦克纳姆轮的逆运动学方程解算
//由车身速度推导成四个轮子的转速

void forward_kinematics(double wheel_omega_speed1, double wheel_omega_speed2, double wheel_omega_speed3, double wheel_omega_speed4, double* vx, double* vy, double* omega);
//麦克纳姆轮的正运动学方程解算
//由四个轮子的转速推导成车身速度

void timer0_init_with_ISR(); // 初始化定时器 0，用于定时器中断，定时器中断用于电机的控制
void IRAM_ATTR timer0_itr_callback(); // 定时器 0 的中断服务函数

void motors_and_board_init(); // 初始化所有的电机，包括 L298N 的转向设置

#endif