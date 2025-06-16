/**
 * @file our_lqr_controller.hpp
 * @brief LQR线性二次调节器控制器类的头文件
 * @author skyswordx
 * @details 该文件包含了自定义LQR控制器类的定义和相关函数的声明
 *          实现了基于状态空间的线性二次最优控制算法
 *          专门针对电子负载恒流控制进行优化
 *          支持多状态反馈和最优增益计算
 */
#ifndef OUR_LQR_CONTROLLER_HPP
#define OUR_LQR_CONTROLLER_HPP

/* 使用示例：
    LQR_controller_t<double> lqr;
    
    void my_lqr_control_task(void *pvParameters) {
        LQR_controller_t<double> *lqr = (LQR_controller_t<double> *)pvParameters;
        lqr->lqr_control_service(); // 调用 LQR 控制器服务函数
    }

    void setup(){
        // 配置系统模型参数
        lqr.setSystemModel(A_matrix, B_matrix, C_matrix);
        
        // 配置权重矩阵
        lqr.setWeights(Q_matrix, R_matrix); // 状态权重, 控制权重
        
        // 计算最优增益
        lqr.computeLQRGain();
        
        // 绑定输入输出函数
        lqr.read_sensor = []() -> double {
            return INA226_device.getCurrent_mA();
        };
        lqr.convert_output = [](double output) -> double {
            MCP4725_device.setVoltage(output);
            return output;
        };

        xTaskCreatePinnedToCore(
            my_lqr_control_task,
            "lqr_control_task",
            1024*6, // LQR需要中等大小的堆栈
            &lqr,
            3, // 高优先级
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
 * @brief LQR过程数据结构，存储LQR控制器相关的状态信息
 */
template<typename T>
class lqr_process_data_t {
public:
    T target;                           // 设定目标值
    T measure;                          // 当前测量值
    T output;                           // 控制器输出
    T last_output;                      // 上一次输出
    
    // 状态变量
    std::vector<T> state;               // 状态向量 [error, error_rate, integral]
    std::vector<T> last_state;          // 上一次状态向量
    
    // 系统参数
    std::vector<std::vector<T>> A;      // 状态转移矩阵
    std::vector<std::vector<T>> B;      // 控制输入矩阵
    std::vector<std::vector<T>> C;      // 输出矩阵
    
    // LQR参数
    std::vector<std::vector<T>> Q;      // 状态权重矩阵
    std::vector<std::vector<T>> R;      // 控制权重矩阵
    std::vector<T> K;                   // LQR增益向量
    
    // 性能监控
    T cost_function;                    // 当前性能指标值
    T tracking_error;                   // 跟踪误差
    
    // 构造函数
    lqr_process_data_t() : 
        target(T{0}), measure(T{0}), output(T{0}), last_output(T{0}),
        cost_function(T{0}), tracking_error(T{0}) {
        // 初始化为3状态系统 [error, error_rate, integral]
        state.resize(3, T{0});
        last_state.resize(3, T{0});
        K.resize(3, T{0});
        
        // 初始化矩阵
        A.resize(3, std::vector<T>(3, T{0}));
        B.resize(3, std::vector<T>(1, T{0}));
        C.resize(1, std::vector<T>(3, T{0}));
        Q.resize(3, std::vector<T>(3, T{0}));
        R.resize(1, std::vector<T>(1, T{0}));
    }
};

/**
 * @brief LQR控制器主类
 * @details 实现基于状态空间的线性二次最优控制算法
 *          针对电子负载恒流控制优化
 */
template<typename T>
class LQR_controller_t {
private:
    // 控制器约束
    const T CONTROLLER_OUTPUT_MAX;      // 控制器输出上限
    const T CONTROLLER_OUTPUT_MIN;      // 控制器输出下限
    const T CONTROLLER_OUTPUT_RATE_MAX; // 控制器输出变化率上限
    const T CONTROLLER_OUTPUT_RATE_MIN; // 控制器输出变化率下限
    
    // 采样参数
    double sampling_time;               // 采样时间(s)
    unsigned long last_update_time;     // 上次更新时间
    
    // 状态估计器参数
    bool enable_state_estimator;        // 是否启用状态估计器
    T estimator_gain;                   // 状态估计器增益
    
    // 积分抗饱和参数
    T integral_max;                     // 积分项上限
    T integral_min;                     // 积分项下限
    bool enable_integral_windup_protection; // 积分抗饱和使能
    
    // LQR求解参数
    int max_iterations;                 // Riccati方程最大迭代次数
    T convergence_tolerance;            // 收敛容差
    
    // 内部方法
    void updateStates();                // 更新状态向量
    void computeLQRControl();           // 计算LQR控制量
    T evaluateCostFunction();           // 评估性能指标
    bool solveLQRRiccati();            // 求解LQR Riccati方程
    void applyConstraints();            // 应用输出约束
    
    // 矩阵运算辅助函数
    std::vector<std::vector<T>> matrixMultiply(
        const std::vector<std::vector<T>>& A,
        const std::vector<std::vector<T>>& B);
    std::vector<std::vector<T>> matrixTranspose(
        const std::vector<std::vector<T>>& matrix);
    std::vector<std::vector<T>> matrixInverse(
        const std::vector<std::vector<T>>& matrix);
    
public:
    lqr_process_data_t<T> process_variable; // 过程变量数据
    
    // 函数指针，用于绑定外部函数
    std::function<T()> read_sensor;         // 读取传感器数据
    std::function<T(T)> convert_output;     // 转换控制器输出
    
    // 构造函数
    LQR_controller_t() : 
        CONTROLLER_OUTPUT_MAX(T{3.3}), CONTROLLER_OUTPUT_MIN(T{0.0}),
        CONTROLLER_OUTPUT_RATE_MAX(T{0.5}), CONTROLLER_OUTPUT_RATE_MIN(T{-0.5}),
        sampling_time(0.01), last_update_time(0),
        enable_state_estimator(false), estimator_gain(T{0.1}),
        integral_max(T{1000}), integral_min(T{-1000}), 
        enable_integral_windup_protection(true),
        max_iterations(100), convergence_tolerance(T{1e-6}) {
        
        // 初始化默认系统模型 (电子负载一阶系统)
        initializeDefaultModel();
        
        // 初始化默认权重矩阵
        initializeDefaultWeights();
    }
    
    // 析构函数
    ~LQR_controller_t() = default;
    
    /**
     * @brief 主控制服务函数
     * @details 在任务中循环调用此函数进行LQR控制
     */
    void lqr_control_service();
    
    /**
     * @brief 设置系统模型参数
     * @param A 状态转移矩阵
     * @param B 控制输入矩阵  
     * @param C 输出矩阵
     */
    void setSystemModel(
        const std::vector<std::vector<T>>& A,
        const std::vector<std::vector<T>>& B,
        const std::vector<std::vector<T>>& C);
    
    /**
     * @brief 设置LQR权重矩阵
     * @param Q 状态权重矩阵
     * @param R 控制权重矩阵
     */
    void setWeights(
        const std::vector<std::vector<T>>& Q,
        const std::vector<std::vector<T>>& R);
    
    /**
     * @brief 计算LQR最优增益
     * @return true 计算成功, false 计算失败
     */
    bool computeLQRGain();
    
    /**
     * @brief 设置采样时间
     * @param dt 采样时间(秒)
     */
    void setSamplingTime(double dt) { sampling_time = dt; }
    
    /**
     * @brief 设置目标值
     * @param target_value 目标设定值
     */
    void setTarget(T target_value) { process_variable.target = target_value; }
    
    /**
     * @brief 获取当前输出
     * @return 控制器输出值
     */
    T getOutput() const { return process_variable.output; }
    
    /**
     * @brief 获取当前状态向量
     * @return 状态向量的引用
     */
    const std::vector<T>& getState() const { return process_variable.state; }
    
    /**
     * @brief 获取LQR增益向量
     * @return LQR增益向量的引用
     */
    const std::vector<T>& getLQRGain() const { return process_variable.K; }
    
    /**
     * @brief 获取当前性能指标
     * @return 性能指标值
     */
    T getCostFunction() const { return process_variable.cost_function; }
    
    /**
     * @brief 重置控制器状态
     */
    void reset();
    
    /**
     * @brief 启用/禁用状态估计器
     * @param enable 是否启用
     * @param gain 估计器增益
     */
    void enableStateEstimator(bool enable, T gain = T{0.1}) {
        enable_state_estimator = enable;
        estimator_gain = gain;
    }
    
    /**
     * @brief 设置积分抗饱和参数
     * @param enable 是否启用积分抗饱和
     * @param max_val 积分上限
     * @param min_val 积分下限
     */
    void setIntegralWindupProtection(bool enable, T max_val, T min_val) {
        enable_integral_windup_protection = enable;
        integral_max = max_val;
        integral_min = min_val;
    }
    
    /**
     * @brief 设置Riccati方程求解参数
     * @param max_iter 最大迭代次数
     * @param tolerance 收敛容差
     */
    void setRiccatiSolverParams(int max_iter, T tolerance) {
        max_iterations = max_iter;
        convergence_tolerance = tolerance;
    }
    
private:
    /**
     * @brief 初始化默认系统模型
     */
    void initializeDefaultModel();
    
    /**
     * @brief 初始化默认权重矩阵
     */
    void initializeDefaultWeights();
};

// 类型别名，方便使用
using LQR_controller = LQR_controller_t<double>;
using LQR_controller_f = LQR_controller_t<float>;

#endif // OUR_LQR_CONTROLLER_HPP
