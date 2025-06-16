/**
 * @file our_lqr_controller.cpp
 * @brief LQR线性二次调节器控制器的实现
 * @author skyswordx
 * @details 实现LQR控制器的核心算法，包括Riccati方程求解、状态更新、最优控制计算等
 */

#include "our_lqr_controller.hpp"
#include <algorithm>
#include <cstring>

/**
 * @brief 初始化默认系统模型（电子负载一阶系统）
 */
template<typename T>
void LQR_controller_t<T>::initializeDefaultModel() {
    // 状态向量：[error, error_rate, integral]
    // 系统采样时间
    T dt = static_cast<T>(sampling_time);
    
    // A矩阵 - 状态转移矩阵
    process_variable.A[0][0] = T{1.0};    // error(k+1) = error(k) + dt*error_rate(k)
    process_variable.A[0][1] = dt;
    process_variable.A[0][2] = T{0.0};
    
    process_variable.A[1][0] = T{0.0};    // error_rate(k+1) = 0.95*error_rate(k) + 0.05*u(k)
    process_variable.A[1][1] = T{0.95};   // 系统衰减系数
    process_variable.A[1][2] = T{0.0};
    
    process_variable.A[2][0] = T{1.0};    // integral(k+1) = integral(k) + error(k)
    process_variable.A[2][1] = T{0.0};
    process_variable.A[2][2] = T{1.0};
    
    // B矩阵 - 控制输入矩阵
    process_variable.B[0][0] = T{0.0};    // 控制量不直接影响误差
    process_variable.B[1][0] = T{0.05};   // 控制量影响误差变化率
    process_variable.B[2][0] = T{0.0};    // 控制量不直接影响积分
    
    // C矩阵 - 输出矩阵
    process_variable.C[0][0] = T{1.0};    // 输出 = 误差
    process_variable.C[0][1] = T{0.0};
    process_variable.C[0][2] = T{0.0};
}

/**
 * @brief 初始化默认权重矩阵
 */
template<typename T>
void LQR_controller_t<T>::initializeDefaultWeights() {
    // Q矩阵 - 状态权重矩阵
    process_variable.Q[0][0] = T{10.0};   // 误差权重（最重要）
    process_variable.Q[0][1] = T{0.0};
    process_variable.Q[0][2] = T{0.0};
    
    process_variable.Q[1][0] = T{0.0};
    process_variable.Q[1][1] = T{1.0};    // 误差变化率权重
    process_variable.Q[1][2] = T{0.0};
    
    process_variable.Q[2][0] = T{0.0};
    process_variable.Q[2][1] = T{0.0};
    process_variable.Q[2][2] = T{0.1};    // 积分项权重
    
    // R矩阵 - 控制权重矩阵
    process_variable.R[0][0] = T{0.1};    // 控制量权重
}

/**
 * @brief 主控制服务函数
 */
template<typename T>
void LQR_controller_t<T>::lqr_control_service() {
    // 检查采样时间
    unsigned long current_time = millis();
    if (current_time - last_update_time < static_cast<unsigned long>(sampling_time * 1000)) {
        return; // 还没到采样时间
    }
    last_update_time = current_time;
    
    // 读取传感器数据
    if (read_sensor) {
        process_variable.measure = read_sensor();
    }
    
    // 更新状态向量
    updateStates();
    
    // 计算LQR控制量
    computeLQRControl();
    
    // 应用输出约束
    applyConstraints();
    
    // 输出控制量
    if (convert_output) {
        convert_output(process_variable.output);
    }
    
    // 计算性能指标
    process_variable.cost_function = evaluateCostFunction();
    
    // 更新历史状态
    process_variable.last_state = process_variable.state;
    process_variable.last_output = process_variable.output;
}

/**
 * @brief 更新状态向量
 */
template<typename T>
void LQR_controller_t<T>::updateStates() {
    // 计算跟踪误差
    T current_error = process_variable.target - process_variable.measure;
    process_variable.tracking_error = current_error;
    
    // 保存上一次状态
    std::vector<T> prev_state = process_variable.state;
    
    // 状态向量：[error, error_rate, integral]
    process_variable.state[0] = current_error;
    
    // 计算误差变化率（数值微分）
    T dt = static_cast<T>(sampling_time);
    if (dt > T{0}) {
        process_variable.state[1] = (current_error - prev_state[0]) / dt;
    } else {
        process_variable.state[1] = T{0};
    }
    
    // 计算误差积分（数值积分）
    process_variable.state[2] = prev_state[2] + current_error * dt;
      // 积分抗饱和处理
    if (enable_integral_windup_protection) {
        if (process_variable.state[2] > integral_max) {
            process_variable.state[2] = integral_max;
        } else if (process_variable.state[2] < integral_min) {
            process_variable.state[2] = integral_min;
        }
    }
    
    // 状态估计器（可选）
    if (enable_state_estimator) {
        // 简单的Luenberger观测器
        T estimation_error = process_variable.measure - 
                            process_variable.C[0][0] * process_variable.state[0];
        
        for (size_t i = 0; i < process_variable.state.size(); ++i) {
            process_variable.state[i] += estimator_gain * estimation_error;
        }
    }
}

/**
 * @brief 计算LQR控制量
 */
template<typename T>
void LQR_controller_t<T>::computeLQRControl() {
    // LQR控制律：u = -K * x
    T control_output = T{0};
    
    for (size_t i = 0; i < process_variable.K.size(); ++i) {
        control_output -= process_variable.K[i] * process_variable.state[i];
    }
    
    process_variable.output = control_output;
}

/**
 * @brief 评估性能指标
 */
template<typename T>
T LQR_controller_t<T>::evaluateCostFunction() {
    T cost = T{0};
    
    // 状态代价：x^T * Q * x
    for (size_t i = 0; i < process_variable.state.size(); ++i) {
        for (size_t j = 0; j < process_variable.state.size(); ++j) {
            cost += process_variable.state[i] * process_variable.Q[i][j] * process_variable.state[j];
        }
    }
    
    // 控制代价：u^T * R * u
    cost += process_variable.output * process_variable.R[0][0] * process_variable.output;
    
    return cost;
}

/**
 * @brief 求解LQR Riccati方程
 */
template<typename T>
bool LQR_controller_t<T>::solveLQRRiccati() {
    // 使用迭代法求解离散时间代数Riccati方程
    // A^T * P * A - P - A^T * P * B * (R + B^T * P * B)^-1 * B^T * P * A + Q = 0
    
    const size_t n = process_variable.A.size();     // 状态维数
    const size_t m = process_variable.B[0].size();  // 控制维数
    
    // 初始化P矩阵（解矩阵）
    std::vector<std::vector<T>> P(n, std::vector<T>(n, T{0}));
    std::vector<std::vector<T>> P_new(n, std::vector<T>(n, T{0}));
    
    // 初始P = Q
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            P[i][j] = process_variable.Q[i][j];
        }
    }
    
    // 迭代求解
    for (int iter = 0; iter < max_iterations; ++iter) {
        // 计算 B^T * P * B + R
        std::vector<std::vector<T>> BT_P_B_R(m, std::vector<T>(m, T{0}));
        
        // B^T * P
        std::vector<std::vector<T>> BT_P(m, std::vector<T>(n, T{0}));
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                for (size_t k = 0; k < n; ++k) {
                    BT_P[i][j] += process_variable.B[k][i] * P[k][j];
                }
            }
        }
        
        // (B^T * P) * B + R
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < m; ++j) {
                for (size_t k = 0; k < n; ++k) {
                    BT_P_B_R[i][j] += BT_P[i][k] * process_variable.B[k][j];
                }
                if (i == j) {
                    BT_P_B_R[i][j] += process_variable.R[i][j];
                }
            }
        }
        
        // 计算 (B^T * P * B + R)^-1
        std::vector<std::vector<T>> inv_BT_P_B_R = matrixInverse(BT_P_B_R);
        
        // 计算 A^T * P * A
        std::vector<std::vector<T>> AT_P_A(n, std::vector<T>(n, T{0}));
        std::vector<std::vector<T>> AT = matrixTranspose(process_variable.A);
        std::vector<std::vector<T>> AT_P = matrixMultiply(AT, P);
        AT_P_A = matrixMultiply(AT_P, process_variable.A);
        
        // 计算 A^T * P * B * (R + B^T * P * B)^-1 * B^T * P * A
        std::vector<std::vector<T>> AT_P_B = matrixMultiply(AT_P, process_variable.B);
        std::vector<std::vector<T>> temp1 = matrixMultiply(AT_P_B, inv_BT_P_B_R);
        std::vector<std::vector<T>> temp2 = matrixMultiply(temp1, BT_P);
        std::vector<std::vector<T>> correction = matrixMultiply(temp2, process_variable.A);
        
        // 更新P: P_new = A^T * P * A - correction + Q
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                P_new[i][j] = AT_P_A[i][j] - correction[i][j] + process_variable.Q[i][j];
            }
        }
        
        // 检查收敛性
        T max_diff = T{0};
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                T diff = std::abs(P_new[i][j] - P[i][j]);
                if (diff > max_diff) {
                    max_diff = diff;
                }
            }
        }
        
        if (max_diff < convergence_tolerance) {
            // 收敛，计算LQR增益
            // K = (R + B^T * P * B)^-1 * B^T * P * A
            std::vector<std::vector<T>> K_matrix = matrixMultiply(inv_BT_P_B_R, BT_P);
            K_matrix = matrixMultiply(K_matrix, process_variable.A);
            
            // 将增益矩阵转换为增益向量（假设单输入系统）
            for (size_t i = 0; i < n; ++i) {
                process_variable.K[i] = K_matrix[0][i];
            }
            
            return true; // 求解成功
        }
        
        // 更新P矩阵
        P = P_new;
    }
    
    return false; // 未收敛
}

/**
 * @brief 计算LQR最优增益
 */
template<typename T>
bool LQR_controller_t<T>::computeLQRGain() {
    return solveLQRRiccati();
}

/**
 * @brief 应用输出约束
 */
template<typename T>
void LQR_controller_t<T>::applyConstraints() {
    // 输出限幅
    if (process_variable.output > CONTROLLER_OUTPUT_MAX) {
        process_variable.output = CONTROLLER_OUTPUT_MAX;
    } else if (process_variable.output < CONTROLLER_OUTPUT_MIN) {
        process_variable.output = CONTROLLER_OUTPUT_MIN;
    }
    
    // 输出变化率限制
    T output_rate = process_variable.output - process_variable.last_output;
    T dt = static_cast<T>(sampling_time);
    
    if (dt > T{0}) {
        T max_change = CONTROLLER_OUTPUT_RATE_MAX * dt;
        T min_change = CONTROLLER_OUTPUT_RATE_MIN * dt;
        
        if (output_rate > max_change) {
            output_rate = max_change;
        } else if (output_rate < min_change) {
            output_rate = min_change;
        }
        process_variable.output = process_variable.last_output + output_rate;
        
        // 再次应用输出限幅
        if (process_variable.output > CONTROLLER_OUTPUT_MAX) {
            process_variable.output = CONTROLLER_OUTPUT_MAX;
        } else if (process_variable.output < CONTROLLER_OUTPUT_MIN) {
            process_variable.output = CONTROLLER_OUTPUT_MIN;
        }
    }
}

/**
 * @brief 设置系统模型参数
 */
template<typename T>
void LQR_controller_t<T>::setSystemModel(
    const std::vector<std::vector<T>>& A,
    const std::vector<std::vector<T>>& B,
    const std::vector<std::vector<T>>& C) {
    
    process_variable.A = A;
    process_variable.B = B;
    process_variable.C = C;
}

/**
 * @brief 设置LQR权重矩阵
 */
template<typename T>
void LQR_controller_t<T>::setWeights(
    const std::vector<std::vector<T>>& Q,
    const std::vector<std::vector<T>>& R) {
    
    process_variable.Q = Q;
    process_variable.R = R;
}

/**
 * @brief 重置控制器状态
 */
template<typename T>
void LQR_controller_t<T>::reset() {
    std::fill(process_variable.state.begin(), process_variable.state.end(), T{0});
    std::fill(process_variable.last_state.begin(), process_variable.last_state.end(), T{0});
    process_variable.output = T{0};
    process_variable.last_output = T{0};
    process_variable.cost_function = T{0};
    process_variable.tracking_error = T{0};
    last_update_time = 0;
}

// 矩阵运算辅助函数实现

/**
 * @brief 矩阵乘法
 */
template<typename T>
std::vector<std::vector<T>> LQR_controller_t<T>::matrixMultiply(
    const std::vector<std::vector<T>>& A,
    const std::vector<std::vector<T>>& B) {
    
    size_t rows_A = A.size();
    size_t cols_A = A[0].size();
    size_t cols_B = B[0].size();
    
    std::vector<std::vector<T>> result(rows_A, std::vector<T>(cols_B, T{0}));
    
    for (size_t i = 0; i < rows_A; ++i) {
        for (size_t j = 0; j < cols_B; ++j) {
            for (size_t k = 0; k < cols_A; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    return result;
}

/**
 * @brief 矩阵转置
 */
template<typename T>
std::vector<std::vector<T>> LQR_controller_t<T>::matrixTranspose(
    const std::vector<std::vector<T>>& matrix) {
    
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();
    
    std::vector<std::vector<T>> result(cols, std::vector<T>(rows));
    
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[j][i] = matrix[i][j];
        }
    }
    
    return result;
}

/**
 * @brief 矩阵求逆（使用高斯-约旦消元法）
 */
template<typename T>
std::vector<std::vector<T>> LQR_controller_t<T>::matrixInverse(
    const std::vector<std::vector<T>>& matrix) {
    
    size_t n = matrix.size();
    std::vector<std::vector<T>> aug(n, std::vector<T>(2 * n, T{0}));
    
    // 构造增广矩阵 [A | I]
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            aug[i][j] = matrix[i][j];
        }
        aug[i][i + n] = T{1}; // 单位矩阵
    }
    
    // 高斯-约旦消元
    for (size_t i = 0; i < n; ++i) {
        // 寻找主元
        size_t pivot_row = i;
        for (size_t k = i + 1; k < n; ++k) {
            if (std::abs(aug[k][i]) > std::abs(aug[pivot_row][i])) {
                pivot_row = k;
            }
        }
        
        // 交换行
        if (pivot_row != i) {
            std::swap(aug[i], aug[pivot_row]);
        }
        
        // 主元归一化
        T pivot = aug[i][i];
        if (std::abs(pivot) < T{1e-10}) {
            // 矩阵奇异，返回零矩阵
            return std::vector<std::vector<T>>(n, std::vector<T>(n, T{0}));
        }
        
        for (size_t j = 0; j < 2 * n; ++j) {
            aug[i][j] /= pivot;
        }
        
        // 消元
        for (size_t k = 0; k < n; ++k) {
            if (k != i) {
                T factor = aug[k][i];
                for (size_t j = 0; j < 2 * n; ++j) {
                    aug[k][j] -= factor * aug[i][j];
                }
            }
        }
    }
    
    // 提取逆矩阵
    std::vector<std::vector<T>> result(n, std::vector<T>(n));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            result[i][j] = aug[i][j + n];
        }
    }
    
    return result;
}

// 显式实例化模板类
template class LQR_controller_t<float>;
template class LQR_controller_t<double>;
