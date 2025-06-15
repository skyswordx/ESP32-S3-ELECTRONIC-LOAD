/**
 * @file our_mpc_controller.cpp
 * @brief MPC控制器实现文件
 * @author skyswordx  
 * @details 实现MPC控制器的核心算法，包括预测、优化和约束处理
 */

#include "our_mpc_controller.hpp"

/**
 * @brief MPC控制器主服务函数
 * @details 该函数应该在任务中循环调用，进行MPC控制器的计算和输出
 *          包含预测、优化和约束处理的完整MPC算法
 */
template<typename T>
void MPC_controller_t<T>::mpc_control_service() {
    /* 第一次进入时初始化 */
    if (process_variable.last_output == T{}) {
        process_variable.last_output = CONTROLLER_OUTPUT_MIN;
        estimated_state = T{};
    }
    
    /* 获取过程变量的测量值 */
    if (read_sensor) {
        process_variable.measure = read_sensor();
        process_variable.error = process_variable.target - process_variable.measure;
    }
    
    /* 状态估计器更新 */
    if (enable_state_estimator) {
        updateStateEstimate();
    }
    
    /* 控制器终止条件检查 */
    if (process_variable.target == 0 && abs(process_variable.measure) < 15) {
        // 目标为0且当前值很小时，停止控制
        process_variable.current_output = CONTROLLER_OUTPUT_MIN;
    } else {
        /* 执行MPC主算法 */
        
        // 1. 预测未来状态
        predictFutureStates();
        
        // 2. 求解二次规划问题
        solveQPProblem();
        
        // 3. 计算最优控制量
        process_variable.current_output = calculateOptimalControl();
        
        // 4. 应用约束
        applyConstraints();
    }
    
    /* 更新历史变量 */
    process_variable.last_measure = process_variable.measure;
    process_variable.last_output = process_variable.current_output;
    
    /* 转换控制器输出 */
    if (convert_output) {
        convert_output(process_variable.current_output);
    }
}

/**
 * @brief 预测未来状态
 * @details 基于系统模型预测未来Np步的系统输出
 */
template<typename T>
void MPC_controller_t<T>::predictFutureStates() {
    T current_state = enable_state_estimator ? estimated_state : process_variable.measure;
    
    // 使用一阶系统模型: y(k+1) = a*y(k) + b*u(k)
    for (int i = 0; i < prediction_horizon; i++) {
        if (i == 0) {
            // 第一步预测
            process_variable.predicted_states[i] = model_a * current_state + 
                                                  model_b * process_variable.last_output;
        } else {
            // 后续预测步
            T control_input = (i < control_horizon) ? 
                             process_variable.control_sequence[i] : 
                             process_variable.control_sequence[control_horizon-1];
                             
            process_variable.predicted_states[i] = model_a * process_variable.predicted_states[i-1] + 
                                                  model_b * control_input;
        }
        
        process_variable.predicted_outputs[i] = process_variable.predicted_states[i];
    }
}

/**
 * @brief 求解二次规划问题
 * @details 使用简化的解析解法求解MPC的二次规划问题
 *          成本函数: J = Σ[Q*(r-y)² + R*u² + S*Δu²]
 */
template<typename T>
void MPC_controller_t<T>::solveQPProblem() {
    // 简化的解析解法，适用于单输入单输出系统
    
    // 初始化控制序列为当前输出值
    for (int i = 0; i < control_horizon; i++) {
        process_variable.control_sequence[i] = process_variable.last_output;
    }
    
    // 使用梯度下降法求解（简化实现）
    const int max_iterations = 10;
    const double step_size = 0.1;
    
    for (int iter = 0; iter < max_iterations; iter++) {
        // 计算成本函数梯度
        std::vector<double> gradient(control_horizon, 0.0);
        
        for (int i = 0; i < control_horizon; i++) {
            double grad = 0.0;
            
            // 计算跟踪误差项的梯度
            for (int j = i; j < prediction_horizon; j++) {
                double predicted_output = process_variable.predicted_outputs[j];
                double tracking_error = process_variable.target - predicted_output;
                
                // 简化的梯度计算
                grad += -2.0 * weight_tracking * tracking_error * model_b * pow(model_a, j-i);
            }
            
            // 控制量权重项
            grad += 2.0 * weight_control * process_variable.control_sequence[i];
            
            // 控制增量权重项
            if (i == 0) {
                double delta_u = process_variable.control_sequence[i] - process_variable.last_output;
                grad += 2.0 * weight_rate * delta_u;
            } else {
                double delta_u = process_variable.control_sequence[i] - process_variable.control_sequence[i-1];
                grad += 2.0 * weight_rate * delta_u;
            }
            
            gradient[i] = grad;
        }
        
        // 梯度下降更新
        for (int i = 0; i < control_horizon; i++) {
            process_variable.control_sequence[i] -= step_size * gradient[i];
            
            // 简单约束处理
            if (process_variable.control_sequence[i] > CONTROLLER_OUTPUT_MAX) {
                process_variable.control_sequence[i] = CONTROLLER_OUTPUT_MAX;
            }
            if (process_variable.control_sequence[i] < CONTROLLER_OUTPUT_MIN) {
                process_variable.control_sequence[i] = CONTROLLER_OUTPUT_MIN;
            }
        }
        
        // 重新预测状态
        predictFutureStates();
    }
}

/**
 * @brief 计算最优控制量
 * @details 返回控制序列的第一个元素作为当前时刻的控制输出
 */
template<typename T>
T MPC_controller_t<T>::calculateOptimalControl() {
    return process_variable.control_sequence[0];
}

/**
 * @brief 更新状态估计
 * @details 使用观测器估计系统真实状态
 */
template<typename T>
void MPC_controller_t<T>::updateStateEstimate() {
    // 状态预测
    T predicted_state = model_a * estimated_state + model_b * process_variable.last_output;
    
    // 预测误差
    estimation_error = process_variable.measure - predicted_state;
    
    // 状态更新 (类似卡尔曼滤波的更新步骤)
    estimated_state = predicted_state + estimator_gain * estimation_error;
}

/**
 * @brief 应用控制约束
 * @details 确保控制输出和变化率都在允许范围内
 */
template<typename T>
void MPC_controller_t<T>::applyConstraints() {
    // 输出幅值约束
    if (process_variable.current_output > CONTROLLER_OUTPUT_MAX) {
        process_variable.current_output = CONTROLLER_OUTPUT_MAX;
    }
    if (process_variable.current_output < CONTROLLER_OUTPUT_MIN) {
        process_variable.current_output = CONTROLLER_OUTPUT_MIN;
    }
    
    // 输出变化率约束
    T delta_output = process_variable.current_output - process_variable.last_output;
    if (delta_output > CONTROLLER_OUTPUT_RATE_MAX) {
        process_variable.current_output = process_variable.last_output + CONTROLLER_OUTPUT_RATE_MAX;
    }
    if (delta_output < CONTROLLER_OUTPUT_RATE_MIN) {
        process_variable.current_output = process_variable.last_output + CONTROLLER_OUTPUT_RATE_MIN;
    }
}

/**
 * @brief 评估成本函数值
 * @details 计算当前控制序列下的成本函数值，用于调试和性能评估
 */
template<typename T>
double MPC_controller_t<T>::evaluateCostFunction() {
    double cost = 0.0;
    
    // 跟踪误差成本
    for (int i = 0; i < prediction_horizon; i++) {
        double tracking_error = process_variable.target - process_variable.predicted_outputs[i];
        cost += weight_tracking * tracking_error * tracking_error;
    }
    
    // 控制量成本
    for (int i = 0; i < control_horizon; i++) {
        cost += weight_control * process_variable.control_sequence[i] * process_variable.control_sequence[i];
    }
    
    // 控制增量成本
    for (int i = 0; i < control_horizon; i++) {
        double delta_u;
        if (i == 0) {
            delta_u = process_variable.control_sequence[i] - process_variable.last_output;
        } else {
            delta_u = process_variable.control_sequence[i] - process_variable.control_sequence[i-1];
        }
        cost += weight_rate * delta_u * delta_u;
    }
    
    return cost;
}

/**
 * @brief 检查系统稳定性
 * @details 检查当前模型参数是否会导致不稳定
 */
template<typename T>
bool MPC_controller_t<T>::checkStability() {
    // 对于一阶系统，稳定性条件是 |a| < 1
    return abs(model_a) < 1.0;
}

// 显式实例化常用类型
template class MPC_controller_t<double>;
template class MPC_controller_t<float>;
