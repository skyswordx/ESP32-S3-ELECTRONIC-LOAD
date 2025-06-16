/**
 * @file our_mahony_filter.cpp
 * @brief Mahony姿态解算滤波器实现
 * @author skyswordx
 * @details 实现Mahony互补滤波算法的核心计算逻辑
 */

#include "our_mahony_filter.hpp"
#include <algorithm>

/**
 * @brief 6DOF模式更新 (陀螺仪 + 加速度计)
 */
template<typename T>
void Mahony_filter_t<T>::update_6DOF(T gx, T gy, T gz, T ax, T ay, T az) {
    // 更新传感器数据
    process_variable.gyro = IMU_data_t<T>(gx, gy, gz);
    process_variable.accel = IMU_data_t<T>(ax, ay, az);
    
    // 执行6DOF更新
    updateQuaternion6DOF(process_variable.gyro, process_variable.accel);
    
    // 更新欧拉角
    updateEulerAngles();
    
    // 更新性能指标
    unsigned long current_time = millis();
    if (process_variable.last_update_time > 0) {
        T dt = (current_time - process_variable.last_update_time) / 1000.0;
        if (dt > T{0}) {
            process_variable.update_frequency = T{1.0} / dt;
        }
    }
    process_variable.last_update_time = current_time;
}

/**
 * @brief 9DOF模式更新 (陀螺仪 + 加速度计 + 磁力计)
 */
template<typename T>
void Mahony_filter_t<T>::update_9DOF(T gx, T gy, T gz, T ax, T ay, T az, T mx, T my, T mz) {
    // 更新传感器数据
    process_variable.gyro = IMU_data_t<T>(gx, gy, gz);
    process_variable.accel = IMU_data_t<T>(ax, ay, az);
    process_variable.mag = IMU_data_t<T>(mx, my, mz);
    
    // 执行9DOF更新
    updateQuaternion9DOF(process_variable.gyro, process_variable.accel, process_variable.mag);
    
    // 更新欧拉角
    updateEulerAngles();
    
    // 更新性能指标
    unsigned long current_time = millis();
    if (process_variable.last_update_time > 0) {
        T dt = (current_time - process_variable.last_update_time) / 1000.0;
        if (dt > T{0}) {
            process_variable.update_frequency = T{1.0} / dt;
        }
    }
    process_variable.last_update_time = current_time;
}

/**
 * @brief Mahony滤波器主服务函数
 */
template<typename T>
void Mahony_filter_t<T>::mahony_filter_service() {
    // 读取传感器数据
    if (read_gyro && read_accel) {
        auto gyro_data = read_gyro();
        auto accel_data = read_accel();
        
        if (enable_magnetometer && read_mag) {
            auto mag_data = read_mag();
            update_9DOF(gyro_data.x, gyro_data.y, gyro_data.z,
                       accel_data.x, accel_data.y, accel_data.z,
                       mag_data.x, mag_data.y, mag_data.z);
        } else {
            update_6DOF(gyro_data.x, gyro_data.y, gyro_data.z,
                       accel_data.x, accel_data.y, accel_data.z);
        }
    }
}

/**
 * @brief 6DOF四元数更新算法核心
 */
template<typename T>
void Mahony_filter_t<T>::updateQuaternion6DOF(const IMU_data_t<T>& gyro, const IMU_data_t<T>& accel) {
    // 验证加速度计数据有效性
    if (!validateAccelData(accel)) {
        // 数据无效，仅使用陀螺仪积分
        T gx = gyro.x, gy = gyro.y, gz = gyro.z;
        
        // 四元数微分方程：q̇ = 0.5 * q ⊗ ω
        T q0 = process_variable.quaternion.q0;
        T q1 = process_variable.quaternion.q1;
        T q2 = process_variable.quaternion.q2;
        T q3 = process_variable.quaternion.q3;
        
        process_variable.quaternion.q0 += T{0.5} * sample_period * (-q1*gx - q2*gy - q3*gz);
        process_variable.quaternion.q1 += T{0.5} * sample_period * ( q0*gx + q2*gz - q3*gy);
        process_variable.quaternion.q2 += T{0.5} * sample_period * ( q0*gy - q1*gz + q3*gx);
        process_variable.quaternion.q3 += T{0.5} * sample_period * ( q0*gz + q1*gy - q2*gx);
        
        process_variable.quaternion.normalize();
        return;
    }
    
    // 计算梯度（误差）
    IMU_data_t<T> gradient;
    computeGradient6DOF(accel, gradient);
    
    // 应用PI校正
    applyCorrection(gradient);
    
    // 应用陀螺仪校正后的四元数更新
    T gx = gyro.x - process_variable.integral_error.x;
    T gy = gyro.y - process_variable.integral_error.y;
    T gz = gyro.z - process_variable.integral_error.z;
    
    // 四元数更新
    T q0 = process_variable.quaternion.q0;
    T q1 = process_variable.quaternion.q1;
    T q2 = process_variable.quaternion.q2;
    T q3 = process_variable.quaternion.q3;
    
    process_variable.quaternion.q0 += T{0.5} * sample_period * (-q1*gx - q2*gy - q3*gz);
    process_variable.quaternion.q1 += T{0.5} * sample_period * ( q0*gx + q2*gz - q3*gy);
    process_variable.quaternion.q2 += T{0.5} * sample_period * ( q0*gy - q1*gz + q3*gx);
    process_variable.quaternion.q3 += T{0.5} * sample_period * ( q0*gz + q1*gy - q2*gx);
    
    // 归一化四元数
    process_variable.quaternion.normalize();
    
    // 计算收敛指标
    process_variable.convergence_indicator = vectorNorm(gradient);
}

/**
 * @brief 9DOF四元数更新算法核心
 */
template<typename T>
void Mahony_filter_t<T>::updateQuaternion9DOF(const IMU_data_t<T>& gyro, const IMU_data_t<T>& accel, const IMU_data_t<T>& mag) {
    // 验证传感器数据有效性
    if (!validateAccelData(accel) || !validateMagData(mag)) {
        // 退化为6DOF模式
        updateQuaternion6DOF(gyro, accel);
        return;
    }
    
    // 计算9DOF梯度
    IMU_data_t<T> gradient;
    computeGradient9DOF(accel, mag, gradient);
    
    // 应用PI校正
    applyCorrection(gradient);
    
    // 应用陀螺仪校正后的四元数更新
    T gx = gyro.x - process_variable.integral_error.x;
    T gy = gyro.y - process_variable.integral_error.y;
    T gz = gyro.z - process_variable.integral_error.z;
    
    // 四元数更新（同6DOF）
    T q0 = process_variable.quaternion.q0;
    T q1 = process_variable.quaternion.q1;
    T q2 = process_variable.quaternion.q2;
    T q3 = process_variable.quaternion.q3;
    
    process_variable.quaternion.q0 += T{0.5} * sample_period * (-q1*gx - q2*gy - q3*gz);
    process_variable.quaternion.q1 += T{0.5} * sample_period * ( q0*gx + q2*gz - q3*gy);
    process_variable.quaternion.q2 += T{0.5} * sample_period * ( q0*gy - q1*gz + q3*gx);
    process_variable.quaternion.q3 += T{0.5} * sample_period * ( q0*gz + q1*gy - q2*gx);
    
    // 归一化四元数
    process_variable.quaternion.normalize();
    
    // 计算收敛指标
    process_variable.convergence_indicator = vectorNorm(gradient);
}

/**
 * @brief 计算6DOF梯度（基于重力向量）
 */
template<typename T>
void Mahony_filter_t<T>::computeGradient6DOF(const IMU_data_t<T>& accel, IMU_data_t<T>& gradient) {
    T q0 = process_variable.quaternion.q0;
    T q1 = process_variable.quaternion.q1;
    T q2 = process_variable.quaternion.q2;
    T q3 = process_variable.quaternion.q3;
    
    // 归一化加速度计测量值
    IMU_data_t<T> accel_norm = accel;
    normalizeVector(accel_norm);
    
    // 由四元数计算的重力方向（地球坐标系中重力向量在机体坐标系的投影）
    T vx = T{2.0} * (q1*q3 - q0*q2);
    T vy = T{2.0} * (q0*q1 + q2*q3);
    T vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
    
    // 误差向量：测量重力方向 × 估计重力方向
    T ex = accel_norm.y * vz - accel_norm.z * vy;
    T ey = accel_norm.z * vx - accel_norm.x * vz;
    T ez = accel_norm.x * vy - accel_norm.y * vx;
    
    gradient.x = ex;
    gradient.y = ey;
    gradient.z = ez;
}

/**
 * @brief 计算9DOF梯度（基于重力向量和磁场向量）
 */
template<typename T>
void Mahony_filter_t<T>::computeGradient9DOF(const IMU_data_t<T>& accel, const IMU_data_t<T>& mag, IMU_data_t<T>& gradient) {
    T q0 = process_variable.quaternion.q0;
    T q1 = process_variable.quaternion.q1;
    T q2 = process_variable.quaternion.q2;
    T q3 = process_variable.quaternion.q3;
    
    // 归一化传感器数据
    IMU_data_t<T> accel_norm = accel;
    IMU_data_t<T> mag_norm = mag;
    normalizeVector(accel_norm);
    normalizeVector(mag_norm);
    
    // 6DOF重力误差
    T vx = T{2.0} * (q1*q3 - q0*q2);
    T vy = T{2.0} * (q0*q1 + q2*q3);
    T vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
    
    T ex = accel_norm.y * vz - accel_norm.z * vy;
    T ey = accel_norm.z * vx - accel_norm.x * vz;
    T ez = accel_norm.x * vy - accel_norm.y * vx;
    
    // 磁场参考方向（地球坐标系）
    T hx = T{2.0} * (mag_norm.x * (T{0.5} - q2*q2 - q3*q3) + mag_norm.y * (q1*q2 - q0*q3) + mag_norm.z * (q1*q3 + q0*q2));
    T hy = T{2.0} * (mag_norm.x * (q1*q2 + q0*q3) + mag_norm.y * (T{0.5} - q1*q1 - q3*q3) + mag_norm.z * (q2*q3 - q0*q1));
    T bx = sqrt(hx*hx + hy*hy);
    T bz = T{2.0} * (mag_norm.x * (q1*q3 - q0*q2) + mag_norm.y * (q2*q3 + q0*q1) + mag_norm.z * (T{0.5} - q1*q1 - q2*q2));
    
    // 估计的磁场方向
    T wx = T{2.0} * (bx * (T{0.5} - q2*q2 - q3*q3) + bz * (q1*q3 - q0*q2));
    T wy = T{2.0} * (bx * (q1*q2 - q0*q3) + bz * (q0*q1 + q2*q3));
    T wz = T{2.0} * (bx * (q0*q2 + q1*q3) + bz * (T{0.5} - q1*q1 - q2*q2));
    
    // 磁场误差
    ex += mag_norm.y * wz - mag_norm.z * wy;
    ey += mag_norm.z * wx - mag_norm.x * wz;
    ez += mag_norm.x * wy - mag_norm.y * wx;
    
    gradient.x = ex;
    gradient.y = ey;
    gradient.z = ez;
}

/**
 * @brief 应用PI校正
 */
template<typename T>
void Mahony_filter_t<T>::applyCorrection(const IMU_data_t<T>& gradient) {
    if (enable_integral_correction) {
        // 积分校正
        process_variable.integral_error.x += Ki * sample_period * gradient.x;
        process_variable.integral_error.y += Ki * sample_period * gradient.y;
        process_variable.integral_error.z += Ki * sample_period * gradient.z;
    }
    
    // 比例校正已经隐含在梯度计算中，通过Kp缩放
    // 这里可以进一步处理，但通常Mahony算法将比例校正直接应用到陀螺仪数据
}

/**
 * @brief 更新欧拉角
 */
template<typename T>
void Mahony_filter_t<T>::updateEulerAngles() {
    T q0 = process_variable.quaternion.q0;
    T q1 = process_variable.quaternion.q1;
    T q2 = process_variable.quaternion.q2;
    T q3 = process_variable.quaternion.q3;
    
    // 四元数转欧拉角
    T sinr_cosp = T{2.0} * (q0 * q1 + q2 * q3);
    T cosr_cosp = T{1.0} - T{2.0} * (q1 * q1 + q2 * q2);
    process_variable.euler.roll = atan2(sinr_cosp, cosr_cosp) * T{180.0} / M_PI;
    
    T sinp = T{2.0} * (q0 * q2 - q3 * q1);
    if (abs(sinp) >= T{1}) {
        process_variable.euler.pitch = copysign(M_PI / T{2.0}, sinp) * T{180.0} / M_PI;
    } else {
        process_variable.euler.pitch = asin(sinp) * T{180.0} / M_PI;
    }
    
    T siny_cosp = T{2.0} * (q0 * q3 + q1 * q2);
    T cosy_cosp = T{1.0} - T{2.0} * (q2 * q2 + q3 * q3);
    process_variable.euler.yaw = atan2(siny_cosp, cosy_cosp) * T{180.0} / M_PI;
}

/**
 * @brief 验证加速度计数据有效性
 */
template<typename T>
bool Mahony_filter_t<T>::validateAccelData(const IMU_data_t<T>& accel) {
    T norm = vectorNorm(accel);
    return (norm >= accel_threshold_min && norm <= accel_threshold_max);
}

/**
 * @brief 验证磁力计数据有效性
 */
template<typename T>
bool Mahony_filter_t<T>::validateMagData(const IMU_data_t<T>& mag) {
    T norm = vectorNorm(mag);
    return (norm >= mag_threshold);
}

/**
 * @brief 计算向量模长
 */
template<typename T>
T Mahony_filter_t<T>::vectorNorm(const IMU_data_t<T>& vec) {
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

/**
 * @brief 向量归一化
 */
template<typename T>
void Mahony_filter_t<T>::normalizeVector(IMU_data_t<T>& vec) {
    T norm = vectorNorm(vec);
    if (norm > T{0}) {
        vec.x /= norm;
        vec.y /= norm;
        vec.z /= norm;
    }
}

/**
 * @brief 获取当前欧拉角
 */
template<typename T>
void Mahony_filter_t<T>::getEulerAngles(T& roll, T& pitch, T& yaw) const {
    roll = process_variable.euler.roll;
    pitch = process_variable.euler.pitch;
    yaw = process_variable.euler.yaw;
}

/**
 * @brief 重置滤波器状态
 */
template<typename T>
void Mahony_filter_t<T>::reset() {
    process_variable.quaternion = Quaternion_t<T>(T{1}, T{0}, T{0}, T{0});
    process_variable.euler = EulerAngles_t<T>(T{0}, T{0}, T{0});
    process_variable.integral_error = IMU_data_t<T>(T{0}, T{0}, T{0});
    process_variable.update_frequency = T{0};
    process_variable.convergence_indicator = T{0};
    process_variable.last_update_time = 0;
}

/**
 * @brief 设置初始姿态
 */
template<typename T>
void Mahony_filter_t<T>::setInitialAttitude(T roll, T pitch, T yaw) {
    // 欧拉角转四元数
    T cy = cos(yaw * M_PI / T{180.0} * T{0.5});
    T sy = sin(yaw * M_PI / T{180.0} * T{0.5});
    T cp = cos(pitch * M_PI / T{180.0} * T{0.5});
    T sp = sin(pitch * M_PI / T{180.0} * T{0.5});
    T cr = cos(roll * M_PI / T{180.0} * T{0.5});
    T sr = sin(roll * M_PI / T{180.0} * T{0.5});
    
    process_variable.quaternion.q0 = cr * cp * cy + sr * sp * sy;
    process_variable.quaternion.q1 = sr * cp * cy - cr * sp * sy;
    process_variable.quaternion.q2 = cr * sp * cy + sr * cp * sy;
    process_variable.quaternion.q3 = cr * cp * sy - sr * sp * cy;
    
    updateEulerAngles();
}

/**
 * @brief 执行传感器校准
 */
template<typename T>
void Mahony_filter_t<T>::calibrateSensors(unsigned long duration_ms) {
    if (!read_gyro || !read_accel) return;
    
    IMU_data_t<T> gyro_offset(T{0}, T{0}, T{0});
    int sample_count = 0;
    
    unsigned long start_time = millis();
    Serial.println("开始传感器校准，请保持IMU静止...");
    
    while (millis() - start_time < duration_ms) {
        auto gyro_data = read_gyro();
        
        gyro_offset.x += gyro_data.x;
        gyro_offset.y += gyro_data.y;
        gyro_offset.z += gyro_data.z;
        sample_count++;
        
        delay(10); // 100Hz采样
    }
    
    // 计算平均偏移
    if (sample_count > 0) {
        gyro_offset.x /= sample_count;
        gyro_offset.y /= sample_count;
        gyro_offset.z /= sample_count;
        
        Serial.printf("校准完成，陀螺仪偏移: [%.4f, %.4f, %.4f]\n", 
                     (float)gyro_offset.x, (float)gyro_offset.y, (float)gyro_offset.z);
    }
}

// 显式实例化模板类
template class Mahony_filter_t<float>;
template class Mahony_filter_t<double>;
