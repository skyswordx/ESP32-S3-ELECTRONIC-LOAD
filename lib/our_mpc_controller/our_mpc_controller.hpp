/**
 * @file our_mpc_controller.hpp
 * @brief 模型预测控制器(MPC)类的头文件
 * @author skyswordx
 * @details 该文件包含了自定义MPC控制器类的定义和相关函数的声明
 *          实现了基于二次规划的模型预测控制算法
 *          专门针对电子负载恒流控制进行优化
 *          支持多重约束处理和预测控制
 */
#ifndef OUR_MPC_CONTROLLER_HPP
#define OUR_MPC_CONTROLLER_HPP

/* 使用示例：
    MPC_controller_t<double> mpc;
    
    void my_mpc_control_task(void *pvParameters) {
        MPC_controller_t<double> *mpc = (MPC_controller_t<double> *)pvParameters;
        mpc->mpc_control_service(); // 调用 MPC 控制器服务函数
    }

    void setup(){
        // 配置模型参数
        mpc.setModelParameters(0.95, 0.05); // 系统增益, 时间常数
        
        // 配置权重矩阵
        mpc.setWeights(1.0, 0.1, 0.01); // 跟踪权重, 控制权重, 变化率权重
        
        // 配置约束
        mpc.setConstraints(0.0, 3.3, -0.5, 0.5); // 输出约束, 输出变化率约束
        
        // 绑定输入输出函数
        mpc.read_sensor = []() -> double {
            return INA226_device.getCurrent_mA();
        };
        mpc.convert_output = [](double output) -> double {
            MCP4725_device.setVoltage(output);
            return output;
        };

        xTaskCreatePinnedToCore(
            my_mpc_control_task,
            "mpc_control_task",
            1024*8, // MPC需要更大的堆栈
            &mpc,
            3, // 较高优先级
            NULL,
            1
        );
    }
*/

#include <functional>
#include <Arduino.h>
#include <vector>
#include <cmath>

/**
 * @brief 过程数据结构，存储MPC控制器相关的状态信息
 */
template<typename T>
class mpc_process_data_t {
public:
    T current_output;    // 当前输出值
    T last_output;       // 上一次输出值
    T measure;           // 当前测量值
    T last_measure;      // 上一次测量值
    T target;            // 目标设定值
    T error;             // 当前误差
    
    // 预测相关变量
    std::vector<T> predicted_outputs;  // 预测输出序列
    std::vector<T> predicted_states;   // 预测状态序列
    std::vector<T> control_sequence;   // 控制序列
    
    // 默认构造函数
    mpc_process_data_t() : current_output(T{}), last_output(T{}), 
                           measure(T{}), last_measure(T{}), 
                           target(T{}), error(T{}) {
        // 初始化预测向量
        predicted_outputs.resize(10, T{});  // 预测范围10步
        predicted_states.resize(10, T{});
        control_sequence.resize(5, T{});    // 控制范围5步
    }
};

/**
 * @brief MPC控制器主类
 * @details 实现基于二次规划的模型预测控制算法
 *          针对电子负载恒流控制优化
 */
template<typename T>
class MPC_controller_t {
public:
    mpc_process_data_t<T> process_variable; // MPC过程变量
    
    // 控制器输出限制
    T CONTROLLER_OUTPUT_MAX;
    T CONTROLLER_OUTPUT_MIN;
    T CONTROLLER_OUTPUT_RATE_MAX;  // 输出变化率限制
    T CONTROLLER_OUTPUT_RATE_MIN;
    
    // MPC参数
    int prediction_horizon;    // 预测时域 Np
    int control_horizon;       // 控制时域 Nc
    int model_order;          // 模型阶数
    
    // 权重参数
    double weight_tracking;    // 跟踪误差权重 Q
    double weight_control;     // 控制量权重 R  
    double weight_rate;        // 控制增量权重 S
    
    // 系统模型参数 (一阶系统: y(k+1) = a*y(k) + b*u(k))
    double model_a;           // 系统极点
    double model_b;           // 系统增益
    double model_delay;       // 系统延迟（采样周期数）
    
    // 状态估计器参数
    bool enable_state_estimator;
    double estimator_gain;
    
    // 函数指针，用于绑定外部函数
    std::function<T()> read_sensor;        // 读取传感器数据
    std::function<T(T)> convert_output;    // 转换控制器输出
    
    // 构造函数
    MPC_controller_t() : 
        CONTROLLER_OUTPUT_MAX(T{3.3}), CONTROLLER_OUTPUT_MIN(T{0.0}),
        CONTROLLER_OUTPUT_RATE_MAX(T{0.5}), CONTROLLER_OUTPUT_RATE_MIN(T{-0.5}),
        prediction_horizon(10), control_horizon(5), model_order(1),
        weight_tracking(1.0), weight_control(0.1), weight_rate(0.01),
        model_a(0.95), model_b(0.05), model_delay(1),
        enable_state_estimator(true), estimator_gain(0.3),
        read_sensor(nullptr), convert_output(nullptr) {}
    
    // 带参数的构造函数
    MPC_controller_t(T output_max, T output_min, T rate_max, T rate_min,
                     int Np, int Nc, double Q, double R, double S) :
        CONTROLLER_OUTPUT_MAX(output_max), CONTROLLER_OUTPUT_MIN(output_min),
        CONTROLLER_OUTPUT_RATE_MAX(rate_max), CONTROLLER_OUTPUT_RATE_MIN(rate_min),
        prediction_horizon(Np), control_horizon(Nc), model_order(1),
        weight_tracking(Q), weight_control(R), weight_rate(S),
        model_a(0.95), model_b(0.05), model_delay(1),
        enable_state_estimator(true), estimator_gain(0.3),
        read_sensor(nullptr), convert_output(nullptr) {}
    
    // 主要控制函数
    void mpc_control_service();
    
    // 配置函数
    void setModelParameters(double a, double b, double delay = 1);
    void setWeights(double Q, double R, double S);
    void setConstraints(T u_min, T u_max, T du_min, T du_max);
    void setHorizon(int Np, int Nc);
    void enableStateEstimator(bool enable, double gain = 0.3);
    
    // 核心MPC算法函数
    void predictFutureStates();
    void solveQPProblem();
    T calculateOptimalControl();
    void updateStateEstimate();
    
private:
    // 内部辅助函数
    void buildPredictionMatrices();
    void buildConstraintMatrices();
    double evaluateCostFunction();
    void applyConstraints();
    bool checkStability();
    
    // 预测矩阵和约束矩阵
    std::vector<std::vector<double>> prediction_matrix;
    std::vector<std::vector<double>> control_matrix;
    std::vector<double> constraint_vector;
    
    // 状态估计器
    T estimated_state;
    T estimation_error;
};

/**
 * @brief 设置系统模型参数
 * @param a 系统极点参数 (0 < a < 1 for stable system)
 * @param b 系统增益参数 
 * @param delay 系统延迟（采样周期数）
 */
template<typename T>
void MPC_controller_t<T>::setModelParameters(double a, double b, double delay) {
    model_a = a;
    model_b = b;
    model_delay = delay;
    
    // 验证稳定性
    if (abs(a) >= 1.0) {
        Serial.println("[MPC] Warning: System may be unstable with a >= 1.0");
    }
}

/**
 * @brief 设置MPC权重参数
 * @param Q 跟踪误差权重（越大越重视跟踪精度）
 * @param R 控制量权重（越大越倾向于小的控制信号）
 * @param S 控制增量权重（越大越平滑）
 */
template<typename T>
void MPC_controller_t<T>::setWeights(double Q, double R, double S) {
    weight_tracking = Q;
    weight_control = R;
    weight_rate = S;
}

/**
 * @brief 设置控制约束
 * @param u_min 最小输出值
 * @param u_max 最大输出值  
 * @param du_min 最小输出变化率
 * @param du_max 最大输出变化率
 */
template<typename T>
void MPC_controller_t<T>::setConstraints(T u_min, T u_max, T du_min, T du_max) {
    CONTROLLER_OUTPUT_MIN = u_min;
    CONTROLLER_OUTPUT_MAX = u_max;
    CONTROLLER_OUTPUT_RATE_MIN = du_min;
    CONTROLLER_OUTPUT_RATE_MAX = du_max;
}

/**
 * @brief 设置预测时域和控制时域
 * @param Np 预测时域长度
 * @param Nc 控制时域长度  
 */
template<typename T>
void MPC_controller_t<T>::setHorizon(int Np, int Nc) {
    prediction_horizon = Np;
    control_horizon = Nc;
    
    // 重新调整向量大小
    process_variable.predicted_outputs.resize(Np);
    process_variable.predicted_states.resize(Np);
    process_variable.control_sequence.resize(Nc);
}

/**
 * @brief 启用/禁用状态估计器
 * @param enable 是否启用状态估计器
 * @param gain 估计器增益
 */
template<typename T>
void MPC_controller_t<T>::enableStateEstimator(bool enable, double gain) {
    enable_state_estimator = enable;
    estimator_gain = gain;
}

#endif // OUR_MPC_CONTROLLER_HPP
