/**
 * @file main.cpp
 * @brief LQR控制器测试程序
 * @author skyswordx
 * @details 测试LQR控制器的基本功能，包括增益计算、控制响应等
 */

#include <Arduino.h>
#include <vector>
#include <cmath>

// 简化的LQR控制器测试类
class LQRTest {
private:
    // 系统参数
    double dt = 0.01;  // 采样时间 10ms
    double a = 0.95;   // 系统极点
    double b = 0.05;   // 控制增益
    
    // 状态空间模型
    std::vector<std::vector<double>> A;
    std::vector<std::vector<double>> B;
    std::vector<std::vector<double>> Q;
    std::vector<std::vector<double>> R;
    
    // LQR增益
    std::vector<double> K;
    
    // 系统状态
    std::vector<double> state;
    double target = 1000.0;  // 目标电流 1000mA
    double output = 0.0;     // 控制输出
    
public:
    LQRTest() {
        initializeMatrices();
        computeLQRGain();
        resetState();
    }
    
    void initializeMatrices() {
        // 初始化A矩阵 (3x3)
        A = {{1.0, dt, 0.0},
             {0.0, a,  0.0},
             {1.0, 0.0, 1.0}};
        
        // 初始化B矩阵 (3x1)
        B = {{0.0},
             {b},
             {0.0}};
        
        // 初始化Q矩阵 (3x3) - 状态权重
        Q = {{10.0, 0.0, 0.0},   // 误差权重
             {0.0,  1.0, 0.0},   // 误差变化率权重
             {0.0,  0.0, 0.1}};  // 积分权重
        
        // 初始化R矩阵 (1x1) - 控制权重
        R = {{0.1}};
        
        // 初始化状态向量
        state.resize(3, 0.0);
        K.resize(3, 0.0);
    }
    
    bool computeLQRGain() {
        // 简化的增益计算（解析解）
        // 对于这个特定系统，可以直接计算
        
        Serial.println("Computing LQR gain...");
        
        // 使用迭代法求解Riccati方程
        std::vector<std::vector<double>> P = Q;  // 初始化P=Q
        
        for (int iter = 0; iter < 50; ++iter) {
            // 计算 B^T * P * B + R
            double BT_P_B_R = B[1][0] * P[1][1] * B[1][0] + R[0][0];
            
            if (abs(BT_P_B_R) < 1e-10) {
                Serial.println("Error: Singular matrix in LQR computation");
                return false;
            }
            
            // 计算增益 K = (B^T*P*B + R)^-1 * B^T * P * A
            K[0] = (B[1][0] * P[1][0] * A[0][0] + B[1][0] * P[1][1] * A[1][0] + B[1][0] * P[1][2] * A[2][0]) / BT_P_B_R;
            K[1] = (B[1][0] * P[1][0] * A[0][1] + B[1][0] * P[1][1] * A[1][1] + B[1][0] * P[1][2] * A[2][1]) / BT_P_B_R;
            K[2] = (B[1][0] * P[1][0] * A[0][2] + B[1][0] * P[1][1] * A[1][2] + B[1][0] * P[1][2] * A[2][2]) / BT_P_B_R;
            
            // 更新P矩阵（简化版）
            auto P_old = P;
            
            // P = A^T*P*A - A^T*P*B*(R+B^T*P*B)^-1*B^T*P*A + Q
            // 这里使用简化的更新规则
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    P[i][j] = Q[i][j] + 0.95 * P_old[i][j];  // 简化更新
                }
            }
            
            // 检查收敛
            double max_diff = 0.0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    double diff = abs(P[i][j] - P_old[i][j]);
                    if (diff > max_diff) max_diff = diff;
                }
            }
            
            if (max_diff < 1e-6) {
                Serial.printf("LQR converged after %d iterations\n", iter + 1);
                break;
            }
        }
        
        Serial.printf("LQR Gains: K1=%.4f, K2=%.4f, K3=%.4f\n", K[0], K[1], K[2]);
        return true;
    }
    
    void resetState() {
        state[0] = 0.0;  // 误差
        state[1] = 0.0;  // 误差变化率
        state[2] = 0.0;  // 积分
        output = 0.0;
    }
    
    void simulateStep(double measurement) {
        // 更新状态
        double error = target - measurement;
        double prev_error = state[0];
        
        state[0] = error;                           // 当前误差
        state[1] = (error - prev_error) / dt;       // 误差变化率
        state[2] = state[2] + error * dt;           // 误差积分
        
        // 积分抗饱和
        if (state[2] > 1000.0) state[2] = 1000.0;
        if (state[2] < -1000.0) state[2] = -1000.0;
        
        // LQR控制律: u = -K*x
        output = -(K[0] * state[0] + K[1] * state[1] + K[2] * state[2]);
        
        // 输出限幅
        if (output > 3.3) output = 3.3;
        if (output < 0.0) output = 0.0;
    }
    
    double getOutput() const { return output; }
    
    void printStatus() {
        Serial.printf("State: [%.2f, %.2f, %.2f], Output: %.3f\n", 
                     state[0], state[1], state[2], output);
    }
    
    void setTarget(double new_target) {
        target = new_target;
        Serial.printf("Target set to %.1f mA\n", target);
    }
    
    std::vector<double> getState() const { return state; }
    std::vector<double> getGains() const { return K; }
};

// 系统仿真器
class SystemSimulator {
private:
    double current_output = 0.0;  // 当前电流输出
    double a = 0.95;              // 系统参数
    double b = 0.05;
    
public:
    double simulateSystem(double control_input) {
        // 一阶系统仿真: y(k+1) = a*y(k) + b*u(k)
        current_output = a * current_output + b * control_input * 1000.0;  // 转换为mA
        
        // 添加一些噪声
        double noise = (random(-50, 50)) / 100.0;  // ±0.5mA噪声
        
        return current_output + noise;
    }
    
    void reset() {
        current_output = 0.0;
    }
};

// 全局对象
LQRTest lqr;
SystemSimulator system_sim;

// 测试参数
unsigned long last_update = 0;
const unsigned long update_interval = 10;  // 10ms
int test_counter = 0;
bool test_running = false;

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("=== LQR Controller Test ===");
    Serial.println("Commands:");
    Serial.println("  's' - Start/Stop test");
    Serial.println("  'r' - Reset system");
    Serial.println("  't XXXX' - Set target (e.g., t 1500)");
    Serial.println("  'g' - Show gains");
    Serial.println("  'p' - Show parameters");
    Serial.println("==============================");
    
    // 显示初始增益
    auto gains = lqr.getGains();
    Serial.printf("Initial LQR Gains: [%.4f, %.4f, %.4f]\n", 
                 gains[0], gains[1], gains[2]);
}

void loop() {
    // 处理串口命令
    handleSerialCommands();
    
    // 运行控制循环
    if (test_running && (millis() - last_update >= update_interval)) {
        runControlLoop();
        last_update = millis();
    }
}

void handleSerialCommands() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command == "s") {
            test_running = !test_running;
            if (test_running) {
                Serial.println("Test STARTED");
                system_sim.reset();
                lqr.resetState();
                test_counter = 0;
            } else {
                Serial.println("Test STOPPED");
            }
        }
        else if (command == "r") {
            Serial.println("System RESET");
            system_sim.reset();
            lqr.resetState();
            test_counter = 0;
        }
        else if (command.startsWith("t ")) {
            double target = command.substring(2).toDouble();
            if (target > 0 && target <= 2000) {
                lqr.setTarget(target);
            } else {
                Serial.println("Invalid target (range: 1-2000 mA)");
            }
        }
        else if (command == "g") {
            auto gains = lqr.getGains();
            Serial.printf("LQR Gains: K1=%.6f, K2=%.6f, K3=%.6f\n", 
                         gains[0], gains[1], gains[2]);
        }
        else if (command == "p") {
            printSystemParameters();
        }
        else if (command == "h") {
            printHelp();
        }
        else {
            Serial.println("Unknown command. Type 'h' for help.");
        }
    }
}

void runControlLoop() {
    // 获取系统输出（仿真）
    double control_input = lqr.getOutput();
    double measurement = system_sim.simulateSystem(control_input);
    
    // 运行LQR控制器
    lqr.simulateStep(measurement);
    
    test_counter++;
    
    // 每100次循环打印一次状态（每秒）
    if (test_counter % 100 == 0) {
        auto state = lqr.getState();
        Serial.printf("Time: %ds, Current: %.1f mA, Error: %.1f, Output: %.3f V\n",
                     test_counter / 100, measurement, state[0], lqr.getOutput());
    }
    
    // 测试不同的目标值
    if (test_counter == 500) {  // 5秒后
        lqr.setTarget(1500.0);
        Serial.println("--- Target changed to 1500 mA ---");
    }
    else if (test_counter == 1000) {  // 10秒后
        lqr.setTarget(800.0);
        Serial.println("--- Target changed to 800 mA ---");
    }
    else if (test_counter == 1500) {  // 15秒后
        lqr.setTarget(1200.0);
        Serial.println("--- Target changed to 1200 mA ---");
    }
}

void printSystemParameters() {
    Serial.println("=== System Parameters ===");
    Serial.println("Sampling time: 10 ms");
    Serial.println("System model: y(k+1) = 0.95*y(k) + 0.05*u(k)");
    Serial.println("State vector: [error, error_rate, integral]");
    
    Serial.println("Q matrix (state weights):");
    Serial.println("  [10.0  0.0  0.0]");
    Serial.println("  [ 0.0  1.0  0.0]");
    Serial.println("  [ 0.0  0.0  0.1]");
    
    Serial.println("R matrix (control weight): [0.1]");
    
    auto gains = lqr.getGains();
    Serial.printf("Computed gains: [%.6f, %.6f, %.6f]\n", 
                 gains[0], gains[1], gains[2]);
    Serial.println("========================");
}

void printHelp() {
    Serial.println("=== LQR Test Commands ===");
    Serial.println("s       - Start/Stop simulation");
    Serial.println("r       - Reset system state");
    Serial.println("t XXXX  - Set target current (mA)");
    Serial.println("g       - Show LQR gains");
    Serial.println("p       - Show system parameters");
    Serial.println("h       - Show this help");
    Serial.println("=========================");
}
