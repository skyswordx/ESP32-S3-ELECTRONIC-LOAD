/**
 * @file our_mahony_filter.hpp
 * @brief Mahony姿态解算滤波器头文件
 * @author skyswordx
 * @details 实现Mahony互补滤波算法，用于IMU数据融合和姿态解算
 *          支持6DOF (陀螺仪+加速度计) 和 9DOF (陀螺仪+加速度计+磁力计) 模式
 *          基于四元数表示的姿态估计算法
 */
#ifndef OUR_MAHONY_FILTER_HPP
#define OUR_MAHONY_FILTER_HPP

/* 使用示例：
    Mahony_filter_t<double> mahony;
    
    void imu_task(void *pvParameters) {
        Mahony_filter_t<double> *filter = (Mahony_filter_t<double> *)pvParameters;
        
        while(true) {
            // 读取IMU数据
            filter->update_6DOF(gx, gy, gz, ax, ay, az);
            // 或者9DOF模式
            // filter->update_9DOF(gx, gy, gz, ax, ay, az, mx, my, mz);
            
            // 获取姿态角
            double roll, pitch, yaw;
            filter->getEulerAngles(roll, pitch, yaw);
            
            vTaskDelay(10 / portTICK_PERIOD_MS); // 100Hz更新频率
        }
    }

    void setup(){
        // 配置Mahony滤波器参数
        mahony.setKp(2.0);     // 比例增益
        mahony.setKi(0.01);    // 积分增益
        mahony.setSampleRate(100.0); // 采样频率100Hz
        
        // 绑定IMU数据读取函数
        mahony.read_gyro = []() -> IMU_data_t {
            return {MPU6050.getGyroX(), MPU6050.getGyroY(), MPU6050.getGyroZ()};
        };
        mahony.read_accel = []() -> IMU_data_t {
            return {MPU6050.getAccelX(), MPU6050.getAccelY(), MPU6050.getAccelZ()};
        };
        mahony.read_mag = []() -> IMU_data_t {
            return {HMC5883L.getMagX(), HMC5883L.getMagY(), HMC5883L.getMagZ()};
        };

        xTaskCreatePinnedToCore(
            imu_task,
            "mahony_filter",
            1024*6,
            &mahony,
            3,
            NULL,
            1
        );
    }
*/

#include <functional>
#include <Arduino.h>
#include <cmath>

/**
 * @brief IMU数据结构
 */
template<typename T>
struct IMU_data_t {
    T x, y, z;
    
    IMU_data_t() : x(T{0}), y(T{0}), z(T{0}) {}
    IMU_data_t(T x_val, T y_val, T z_val) : x(x_val), y(y_val), z(z_val) {}
};

/**
 * @brief 四元数结构
 */
template<typename T>
struct Quaternion_t {
    T q0, q1, q2, q3;  // w, x, y, z
    
    Quaternion_t() : q0(T{1}), q1(T{0}), q2(T{0}), q3(T{0}) {}
    Quaternion_t(T w, T x, T y, T z) : q0(w), q1(x), q2(y), q3(z) {}
    
    // 四元数归一化
    void normalize() {
        T norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
        if (norm > T{0}) {
            q0 /= norm;
            q1 /= norm;
            q2 /= norm;
            q3 /= norm;
        }
    }
};

/**
 * @brief 欧拉角结构（单位：度）
 */
template<typename T>
struct EulerAngles_t {
    T roll;     // 横滚角 (绕X轴)
    T pitch;    // 俯仰角 (绕Y轴)  
    T yaw;      // 偏航角 (绕Z轴)
    
    EulerAngles_t() : roll(T{0}), pitch(T{0}), yaw(T{0}) {}
    EulerAngles_t(T r, T p, T y) : roll(r), pitch(p), yaw(y) {}
};

/**
 * @brief Mahony滤波器过程数据
 */
template<typename T>
class mahony_process_data_t {
public:
    // 传感器原始数据
    IMU_data_t<T> gyro;         // 陀螺仪数据 (rad/s)
    IMU_data_t<T> accel;        // 加速度计数据 (m/s²)
    IMU_data_t<T> mag;          // 磁力计数据 (μT)
    
    // 姿态表示
    Quaternion_t<T> quaternion; // 四元数姿态
    EulerAngles_t<T> euler;     // 欧拉角姿态
    
    // 误差积分
    IMU_data_t<T> integral_error; // 积分误差
    
    // 性能监控
    T update_frequency;         // 实际更新频率
    T convergence_indicator;    // 收敛指标
    unsigned long last_update_time; // 上次更新时间
    
    mahony_process_data_t() : 
        update_frequency(T{0}), convergence_indicator(T{0}), 
        last_update_time(0) {}
};

/**
 * @brief Mahony姿态解算滤波器主类
 * @details 实现基于梯度下降和PI控制的姿态估计算法
 *          专门用于IMU数据融合和实时姿态解算
 */
template<typename T>
class Mahony_filter_t {
private:
    // Mahony算法参数
    T Kp;                       // 比例增益
    T Ki;                       // 积分增益
    T sample_rate;              // 采样频率 (Hz)
    T sample_period;            // 采样周期 (s)
    
    // 算法配置
    bool enable_magnetometer;   // 是否启用磁力计
    bool enable_integral_correction; // 是否启用积分校正
    T gyro_measurement_error;   // 陀螺仪测量误差 (rad/s)
    T beta;                     // Madgwick beta参数（备用）
    
    // 数据预处理参数
    T accel_threshold_min;      // 加速度阈值下限 (排除静止)
    T accel_threshold_max;      // 加速度阈值上限 (排除冲击)
    T mag_threshold;            // 磁力计数据有效性阈值
    
    // 内部计算函数
    void updateQuaternion6DOF(const IMU_data_t<T>& gyro, const IMU_data_t<T>& accel);
    void updateQuaternion9DOF(const IMU_data_t<T>& gyro, const IMU_data_t<T>& accel, const IMU_data_t<T>& mag);
    void computeGradient6DOF(const IMU_data_t<T>& accel, IMU_data_t<T>& gradient);
    void computeGradient9DOF(const IMU_data_t<T>& accel, const IMU_data_t<T>& mag, IMU_data_t<T>& gradient);
    void applyCorrection(const IMU_data_t<T>& gradient);
    void updateEulerAngles();
    bool validateAccelData(const IMU_data_t<T>& accel);
    bool validateMagData(const IMU_data_t<T>& mag);
    T vectorNorm(const IMU_data_t<T>& vec);
    void normalizeVector(IMU_data_t<T>& vec);
    
public:
    mahony_process_data_t<T> process_variable; // 过程变量数据
    
    // 函数指针，用于绑定外部IMU传感器
    std::function<IMU_data_t<T>()> read_gyro;   // 读取陀螺仪数据
    std::function<IMU_data_t<T>()> read_accel;  // 读取加速度计数据
    std::function<IMU_data_t<T>()> read_mag;    // 读取磁力计数据
    
    // 构造函数
    Mahony_filter_t() : 
        Kp(T{2.0}), Ki(T{0.01}), sample_rate(T{100.0}), sample_period(T{0.01}),
        enable_magnetometer(false), enable_integral_correction(true),
        gyro_measurement_error(T{0.1}), beta(T{0.1}),
        accel_threshold_min(T{0.5}), accel_threshold_max(T{2.0}), mag_threshold(T{1.0}),
        read_gyro(nullptr), read_accel(nullptr), read_mag(nullptr) {
        
        // 初始化四元数为单位四元数
        process_variable.quaternion = Quaternion_t<T>(T{1}, T{0}, T{0}, T{0});
    }
    
    // 析构函数
    ~Mahony_filter_t() = default;
    
    /**
     * @brief 6DOF模式更新 (陀螺仪 + 加速度计)
     * @param gx, gy, gz 陀螺仪数据 (rad/s)
     * @param ax, ay, az 加速度计数据 (m/s²)
     */
    void update_6DOF(T gx, T gy, T gz, T ax, T ay, T az);
    
    /**
     * @brief 9DOF模式更新 (陀螺仪 + 加速度计 + 磁力计)
     * @param gx, gy, gz 陀螺仪数据 (rad/s)
     * @param ax, ay, az 加速度计数据 (m/s²)
     * @param mx, my, mz 磁力计数据 (μT)
     */
    void update_9DOF(T gx, T gy, T gz, T ax, T ay, T az, T mx, T my, T mz);
    
    /**
     * @brief Mahony滤波器主服务函数
     * @details 自动读取传感器数据并更新姿态
     */
    void mahony_filter_service();
    
    /**
     * @brief 设置Mahony算法参数
     * @param kp 比例增益
     * @param ki 积分增益
     */
    void setGains(T kp, T ki) { Kp = kp; Ki = ki; }
    
    /**
     * @brief 设置采样频率
     * @param freq 采样频率 (Hz)
     */
    void setSampleRate(T freq) { 
        sample_rate = freq; 
        sample_period = T{1.0} / freq;
    }
    
    /**
     * @brief 设置比例增益
     * @param kp 比例增益值
     */
    void setKp(T kp) { Kp = kp; }
    
    /**
     * @brief 设置积分增益
     * @param ki 积分增益值
     */
    void setKi(T ki) { Ki = ki; }
    
    /**
     * @brief 启用/禁用磁力计
     * @param enable 是否启用磁力计
     */
    void enableMagnetometer(bool enable) { enable_magnetometer = enable; }
    
    /**
     * @brief 获取当前欧拉角
     * @param roll, pitch, yaw 输出欧拉角 (度)
     */
    void getEulerAngles(T& roll, T& pitch, T& yaw) const;
    
    /**
     * @brief 获取当前四元数
     * @return 当前四元数
     */
    const Quaternion_t<T>& getQuaternion() const { return process_variable.quaternion; }
    
    /**
     * @brief 获取当前更新频率
     * @return 实际更新频率 (Hz)
     */
    T getUpdateFrequency() const { return process_variable.update_frequency; }
    
    /**
     * @brief 重置滤波器状态
     */
    void reset();
    
    /**
     * @brief 设置初始姿态
     * @param roll, pitch, yaw 初始欧拉角 (度)
     */
    void setInitialAttitude(T roll, T pitch, T yaw);
    
    /**
     * @brief 设置数据有效性阈值
     * @param accel_min, accel_max 加速度阈值范围
     * @param mag_thresh 磁力计阈值
     */
    void setDataThresholds(T accel_min, T accel_max, T mag_thresh) {
        accel_threshold_min = accel_min;
        accel_threshold_max = accel_max;
        mag_threshold = mag_thresh;
    }
    
    /**
     * @brief 获取收敛指标
     * @return 收敛指标值 (越小表示越稳定)
     */
    T getConvergenceIndicator() const { return process_variable.convergence_indicator; }
    
    /**
     * @brief 执行传感器校准
     * @param duration_ms 校准持续时间 (毫秒)
     */
    void calibrateSensors(unsigned long duration_ms);
};

// 类型别名，方便使用
using Mahony_filter = Mahony_filter_t<double>;
using Mahony_filter_f = Mahony_filter_t<float>;

#endif // OUR_MAHONY_FILTER_HPP
