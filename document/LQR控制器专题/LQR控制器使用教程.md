# LQR控制器使用教程：从理论到实践

## 📖 目录
1. [快速入门](#-快速入门)
2. [LQR核心概念](#-lqr核心概念)
3. [参数配置指南](#-参数配置指南)
4. [代码实例](#-代码实例)
5. [性能调优](#-性能调优)
6. [故障排除](#-故障排除)

## 🚀 快速入门

### 第一步：包含头文件
```cpp
#include "our_lqr_controller.hpp"
```

### 第二步：创建LQR控制器
```cpp
LQR_controller_t<double> lqr_controller;
```

### 第三步：配置系统模型
```cpp
// 使用默认模型（推荐新手）
lqr_controller.computeLQRGain();

// 或者自定义模型（高级用户）
std::vector<std::vector<double>> A = {
    {1.0, 0.01, 0.0},   // dt=0.01s
    {0.0, 0.95, 0.0},   // 衰减系数
    {1.0, 0.0,  1.0}    // 积分
};
lqr_controller.setSystemModel(A, B, C);
```

### 第四步：绑定传感器和执行器
```cpp
lqr_controller.read_sensor = []() -> double {
    return INA226_device.getCurrent_mA();
};

lqr_controller.convert_output = [](double output) -> double {
    MCP4725_device.setVoltage(output);
    return output;
};
```

### 第五步：启动控制任务
```cpp
xTaskCreatePinnedToCore(
    lqr_control_task,
    "lqr_control",
    1024*6,
    &lqr_controller,
    3,
    NULL,
    1
);
```

### 控制任务实现
```cpp
void lqr_control_task(void *pvParameters) {
    LQR_controller_t<double> *lqr = (LQR_controller_t<double> *)pvParameters;
    
    while (true) {
        lqr->lqr_control_service();
        vTaskDelay(10 / portTICK_PERIOD_MS); // 10ms采样
    }
}
```

## 🧠 LQR核心概念

### 状态空间表示

LQR控制器基于**状态空间模型**：

```
状态方程：x(k+1) = A*x(k) + B*u(k)
输出方程：y(k) = C*x(k)
```

**电子负载系统的状态定义：**
```cpp
state[0] = error;        // 电流误差 (目标值 - 测量值)
state[1] = error_rate;   // 电流误差变化率
state[2] = integral;     // 电流误差积分
```

### 最优控制律

LQR的控制律非常简单：
```cpp
u(k) = -K * x(k)  // 控制输出 = -增益向量 × 状态向量
```

其中K是通过求解**Riccati方程**得到的最优增益向量。

### 性能指标

LQR最小化以下性能指标：
```
J = Σ[x(k)ᵀ*Q*x(k) + u(k)ᵀ*R*u(k)]
```

- **Q矩阵**：状态权重，越大越重视该状态的控制精度
- **R矩阵**：控制权重，越大越节约控制能量

## ⚙️ 参数配置指南

### 1. Q矩阵配置（状态权重）

```cpp
std::vector<std::vector<double>> Q = {
    {10.0, 0.0, 0.0},   // 误差权重（最重要）
    {0.0,  1.0, 0.0},   // 误差变化率权重
    {0.0,  0.0, 0.1}    // 积分项权重
};
```

**调参指南：**
- `Q[0][0]` ↑ → 提高跟踪精度，可能增加超调
- `Q[1][1]` ↑ → 增加阻尼，减少振荡
- `Q[2][2]` ↑ → 消除稳态误差，注意积分饱和

### 2. R矩阵配置（控制权重）

```cpp
std::vector<std::vector<double>> R = {
    {0.1}  // 控制量权重
};
```

**调参指南：**
- `R[0][0]` ↑ → 控制更节能，响应变慢
- `R[0][0]` ↓ → 响应更快，控制更激进

### 3. 系统模型配置

#### A矩阵（状态转移）
```cpp
std::vector<std::vector<double>> A = {
    {1.0,  dt,   0.0},   // dt = 采样时间
    {0.0,  a,    0.0},   // a = 系统极点 (0.9~0.99)
    {1.0,  0.0,  1.0}    // 积分环节
};
```

#### B矩阵（控制输入）
```cpp
std::vector<std::vector<double>> B = {
    {0.0},
    {b},     // b = 控制增益 (0.01~0.1)
    {0.0}
};
```

### 4. 采样时间设置

```cpp
lqr_controller.setSamplingTime(0.01);  // 10ms采样
```

**选择原则：**
- 系统带宽的10-20倍
- 电子负载推荐：5-20ms

## 💻 代码实例

### 完整的电子负载LQR控制实现

```cpp
#include "our_lqr_controller.hpp"
#include "our_tasks_config.hpp"

// 全局LQR控制器
LQR_controller_t<double> lqr_controller;

void setup_lqr_controller() {
    // 1. 设置采样时间
    lqr_controller.setSamplingTime(0.01);  // 10ms
    
    // 2. 配置权重矩阵
    std::vector<std::vector<double>> Q = {
        {10.0, 0.0, 0.0},   // 重视跟踪精度
        {0.0,  1.0, 0.0},   // 适度阻尼
        {0.0,  0.0, 0.1}    // 少量积分
    };
    
    std::vector<std::vector<double>> R = {
        {0.1}  // 适度控制代价
    };
    
    lqr_controller.setWeights(Q, R);
    
    // 3. 计算最优增益
    if (!lqr_controller.computeLQRGain()) {
        Serial.println("LQR gain computation failed!");
        return;
    }
    
    // 4. 绑定硬件接口
    lqr_controller.read_sensor = []() -> double {
        if (xSemaphoreTake(i2c_device_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            double current = INA226_device.getCurrent_mA();
            xSemaphoreGive(i2c_device_mutex);
            return current;
        }
        return 0.0;
    };
    
    lqr_controller.convert_output = [](double output) -> double {
        if (xSemaphoreTake(i2c_device_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            MCP4725_device.setVoltage(output);
            xSemaphoreGive(i2c_device_mutex);
        }
        return output;
    };
    
    // 5. 设置目标值
    lqr_controller.setTarget(1000.0);  // 1000mA
    
    // 6. 启用状态估计器（可选）
    lqr_controller.enableStateEstimator(true, 0.1);
    
    // 7. 启用积分抗饱和
    lqr_controller.setIntegralWindupProtection(true, 1000.0, -1000.0);
    
    Serial.println("LQR controller initialized successfully!");
    
    // 打印增益向量
    const auto& K = lqr_controller.getLQRGain();
    Serial.printf("LQR Gains: K1=%.4f, K2=%.4f, K3=%.4f\n", 
                  K[0], K[1], K[2]);
}

void lqr_control_task(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 10ms
    
    while (true) {
        // 执行LQR控制
        lqr_controller.lqr_control_service();
        
        // 性能监控（调试时使用）
        #ifdef DEBUG_LQR
        static int counter = 0;
        if (++counter >= 100) { // 每秒打印一次
            const auto& state = lqr_controller.getState();
            double cost = lqr_controller.getCostFunction();
            double output = lqr_controller.getOutput();
            
            Serial.printf("LQR: Error=%.2f, Rate=%.2f, Integral=%.2f, "
                         "Output=%.3f, Cost=%.3f\n",
                         state[0], state[1], state[2], output, cost);
            counter = 0;
        }
        #endif
        
        // 精确定时
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void setup() {
    // ...其他初始化代码...
    
    // 初始化LQR控制器
    setup_lqr_controller();
    
    // 创建LQR控制任务
    xTaskCreatePinnedToCore(
        lqr_control_task,
        "lqr_control",
        1024*6,
        NULL,
        3,  // 高优先级
        NULL,
        1   // Core 1
    );
}
```

### 模式切换支持

```cpp
enum ControlMode {
    MODE_PID,
    MODE_LQR,
    MODE_MPC
};

ControlMode current_mode = MODE_LQR;

void switch_control_mode(ControlMode new_mode) {
    // 停止当前控制器
    switch (current_mode) {
        case MODE_PID:
            // 停止PID任务
            break;
        case MODE_LQR:
            lqr_controller.reset();
            break;
        case MODE_MPC:
            // 停止MPC任务
            break;
    }
    
    current_mode = new_mode;
    
    // 启动新控制器
    switch (new_mode) {
        case MODE_LQR:
            lqr_controller.setTarget(get_current_target());
            break;
        // ...其他模式
    }
    
    Serial.printf("Switched to %s mode\n", 
                  (new_mode == MODE_LQR) ? "LQR" : "Other");
}
```

## 🎯 性能调优

### 1. 响应速度调优

**问题：响应太慢**
```cpp
// 解决方案1：增加误差权重
Q[0][0] = 20.0;  // 原来10.0

// 解决方案2：减少控制权重
R[0][0] = 0.05;  // 原来0.1

// 解决方案3：调整系统模型
A[1][1] = 0.9;   // 原来0.95，系统响应更快
B[1][0] = 0.08;  // 原来0.05，控制更有效
```

### 2. 稳定性调优

**问题：系统振荡**
```cpp
// 解决方案1：增加阻尼
Q[1][1] = 2.0;   // 原来1.0

// 解决方案2：增加控制权重
R[0][0] = 0.2;   // 原来0.1

// 解决方案3：启用状态估计器
lqr_controller.enableStateEstimator(true, 0.2);
```

### 3. 稳态精度调优

**问题：稳态误差大**
```cpp
// 解决方案1：增加积分权重
Q[2][2] = 0.5;   // 原来0.1

// 解决方案2：调整积分限制
lqr_controller.setIntegralWindupProtection(true, 2000.0, -2000.0);

// 解决方案3：减少控制权重
R[0][0] = 0.05;  // 原来0.1
```

### 4. 性能监控代码

```cpp
void monitor_lqr_performance() {
    static unsigned long last_print = 0;
    
    if (millis() - last_print > 1000) { // 每秒监控
        const auto& state = lqr_controller.getState();
        const auto& gains = lqr_controller.getLQRGain();
        double cost = lqr_controller.getCostFunction();
        double output = lqr_controller.getOutput();
        
        // 计算性能指标
        double rms_error = sqrt(state[0] * state[0]);
        double control_effort = abs(output);
        double settling_indicator = abs(state[1]); // 误差变化率
        
        Serial.printf("LQR Performance Monitor:\n");
        Serial.printf("  RMS Error: %.3f mA\n", rms_error);
        Serial.printf("  Control Effort: %.3f V\n", control_effort);
        Serial.printf("  Settling: %.3f (mA/s)\n", settling_indicator);
        Serial.printf("  Cost Function: %.3f\n", cost);
        Serial.printf("  Gains: [%.3f, %.3f, %.3f]\n", 
                     gains[0], gains[1], gains[2]);
        Serial.println();
        
        last_print = millis();
    }
}
```

## 🔧 故障排除

### 常见问题1：控制器不工作

**症状**：输出始终为0
```cpp
// 检查清单：
1. 确认传感器绑定：lqr_controller.read_sensor != nullptr
2. 确认执行器绑定：lqr_controller.convert_output != nullptr  
3. 确认增益计算：lqr_controller.computeLQRGain() == true
4. 确认目标值设置：lqr_controller.setTarget(target_value)
5. 检查约束限制：CONTROLLER_OUTPUT_MAX/MIN是否合理
```

### 常见问题2：系统不稳定

**症状**：输出振荡或发散
```cpp
// 诊断代码：
void diagnose_instability() {
    const auto& gains = lqr_controller.getLQRGain();
    
    // 检查增益是否合理
    if (gains[0] > 10.0) {
        Serial.println("Warning: K1 gain too high, may cause instability");
    }
    
    if (gains[1] < 0.1) {
        Serial.println("Warning: K2 gain too low, insufficient damping");
    }
    
    // 检查采样时间
    if (sampling_time > 0.05) {
        Serial.println("Warning: Sampling time too large");
    }
}
```

### 常见问题3：Riccati方程不收敛

**症状**：computeLQRGain()返回false
```cpp
// 解决方案：
1. 检查系统可控性
2. 调整Q、R矩阵，确保正定
3. 增加迭代次数：
   lqr_controller.setRiccatiSolverParams(200, 1e-8);
4. 检查A、B矩阵是否合理
```

### 调试工具

```cpp
void debug_lqr_matrices() {
    Serial.println("=== LQR Debug Information ===");
    
    // 打印系统矩阵
    Serial.println("A Matrix:");
    const auto& A = lqr_controller.process_variable.A;
    for (int i = 0; i < 3; i++) {
        Serial.printf("  [%.3f, %.3f, %.3f]\n", A[i][0], A[i][1], A[i][2]);
    }
    
    // 打印权重矩阵
    Serial.println("Q Matrix:");
    const auto& Q = lqr_controller.process_variable.Q;
    for (int i = 0; i < 3; i++) {
        Serial.printf("  [%.3f, %.3f, %.3f]\n", Q[i][0], Q[i][1], Q[i][2]);
    }
    
    // 打印增益向量
    const auto& K = lqr_controller.getLQRGain();
    Serial.printf("K Vector: [%.6f, %.6f, %.6f]\n", K[0], K[1], K[2]);
    
    Serial.println("============================");
}
```

## 🎯 总结

LQR控制器为电子负载系统提供了**理论最优**的控制方案：

### ✅ 优势
- **设计科学**：基于最优控制理论
- **参数少**：主要调Q、R矩阵
- **稳定性好**：理论保证稳定
- **实时性强**：计算复杂度低

### ⚠️ 注意事项
- 需要准确的系统模型
- 不直接处理约束（需要额外设计）
- 对模型误差敏感

### 🔄 与其他控制器配合
- **日常控制**：LQR（最优、快速）
- **复杂约束**：MPC（预测、约束）
- **应急备份**：PID（简单、鲁棒）

通过合理的参数调整和系统建模，LQR可以为电子负载提供出色的控制性能！
