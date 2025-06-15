# MPC代码实现：参数命名含义详解

## 🏗️ 类和变量命名解释

### 1. 主要类名含义

```cpp
template<typename T>
class MPC_controller_t {
    // ...
};
```

**为什么叫 `MPC_controller_t`？**
- **MPC** = Model Predictive Control（模型预测控制）
- **controller** = 控制器
- **_t** = type（类型），C++编程惯例，表示这是一个类型定义

### 2. 过程数据结构

```cpp
template<typename T>
class mpc_process_data_t {
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
};
```

**参数命名含义：**

**基础变量：**
- `current_output` → 当前控制器输出的电压值（如2.5V）
- `last_output` → 上一次的输出值，用于计算变化率
- `measure` → 传感器测量的电流值（如1200mA）
- `last_measure` → 上一次的测量值
- `target` → 目标电流值（如1500mA）
- `error` → 误差 = target - measure

**预测变量：**
- `predicted_outputs` → 预测的未来10步输出值 [y(k+1), y(k+2), ..., y(k+10)]
- `predicted_states` → 预测的未来10步状态值（在我们的系统中等于输出）
- `control_sequence` → 计算的未来5步控制动作 [u(k), u(k+1), ..., u(k+4)]

### 3. 系统模型参数

```cpp
// 系统模型：y(k+1) = model_a * y(k) + model_b * u(k)
double model_a = 0.95;  // 状态转移矩阵
double model_b = 0.05;  // 输入矩阵
```

**为什么叫model_a和model_b？**

这来自控制理论的标准状态空间模型：
```
x(k+1) = A*x(k) + B*u(k)  // 状态方程
y(k)   = C*x(k) + D*u(k)  // 输出方程
```

在我们的简化系统中：
- `model_a` 对应矩阵 **A**（状态转移矩阵）
- `model_b` 对应矩阵 **B**（输入影响矩阵）
- 由于是单输入单输出系统，A和B都是标量

**物理含义：**
- `model_a = 0.95` → 系统有95%的"惯性"，当前状态对下一时刻的影响
- `model_b = 0.05` → 控制输入有5%的"效果"，控制信号对系统的影响

### 4. 时域参数

```cpp
static const int prediction_horizon = 10;  // Np
static const int control_horizon = 5;      // Nc
```

**为什么用这些名字？**
- `prediction_horizon` → "预测地平线"，能看多远的未来
- `control_horizon` → "控制地平线"，实际计算多少步控制

**为什么用horizon这个词？**
- horizon = 地平线，视野范围
- 在控制理论中用来表示"时间窗口"的概念
- 就像船长用望远镜看到的海平线，表示能看到多远

### 5. 权重参数

```cpp
double weight_Q = 1.0;   // 跟踪性能权重
double weight_R = 0.1;   // 控制能耗权重  
double weight_S = 0.01;  // 控制平滑权重
```

**为什么用Q、R、S？**

这来自线性二次调节器（LQR）理论的标准命名：

**Q矩阵（State weighting）：**
- Q = Quality，质量权重
- 衡量状态偏差的重要性
- 在我们的系统中，Q越大越想精确跟踪目标电流

**R矩阵（Input weighting）：**
- R = Regulation，调节权重  
- 衡量控制输入大小的代价
- R越大越想节省控制能量（输出更小的电压）

**S权重（Delta U weighting）：**
- S = Smoothness，平滑权重
- 衡量控制变化的代价
- S越大控制变化越缓慢（电压变化更平稳）

### 6. 约束参数

```cpp
T CONTROLLER_OUTPUT_MAX = 3.3;   // 最大输出约束
T CONTROLLER_OUTPUT_MIN = 0.0;   // 最小输出约束
double max_output_change = 0.5;  // 最大变化率约束
```

**命名解释：**
- `CONTROLLER_OUTPUT_MAX/MIN` → 硬约束，DAC输出不能超过3.3V
- `max_output_change` → 软约束，保护被测设备，电压变化不能太快

---

## 🔧 核心算法函数命名解释

### 1. 预测函数

```cpp
void predictFutureStates() {
    // 预测未来状态的函数
}
```

**为什么叫predictFutureStates？**
- `predict` → 预测
- `Future` → 未来的
- `States` → 状态（系统的状态变量）
- 合起来就是"预测未来状态"

### 2. 优化函数

```cpp
void solveQPProblem() {
    // 求解二次规划问题
}
```

**为什么叫solveQPProblem？**
- `solve` → 求解
- `QP` → Quadratic Programming（二次规划）
- `Problem` → 问题
- MPC的核心是求解一个二次规划优化问题

### 3. 约束函数

```cpp
void applyConstraints() {
    // 应用约束条件
}
```

**为什么叫applyConstraints？**
- `apply` → 应用，执行
- `Constraints` → 约束条件
- 确保控制序列满足所有约束

### 4. 状态估计函数

```cpp
void updateStateEstimate() {
    // 更新状态估计
}
```

**为什么叫updateStateEstimate？**
- `update` → 更新
- `State` → 状态
- `Estimate` → 估计值
- 类似卡尔曼滤波，估计系统真实状态

---

## 🧮 数学公式与代码对应关系

### 1. 系统模型

**数学公式：**
```
y(k+1) = a*y(k) + b*u(k)
```

**代码实现：**
```cpp
process_variable.predicted_states[i] = 
    model_a * process_variable.predicted_states[i-1] +  // a*y(k)
    model_b * control_input;                           // b*u(k)
```

### 2. 成本函数

**数学公式：**
```
J = Σ[Q*(r-y)² + R*u² + S*Δu²]
```

**代码实现：**
```cpp
// Q项：跟踪误差
double tracking_error = process_variable.target - predicted_output;
grad += -2.0 * weight_Q * tracking_error * model_b;

// R项：控制能耗
grad += 2.0 * weight_R * control_sequence[i];

// S项：控制平滑
double control_change = control_sequence[i] - control_sequence[i-1];
grad += 2.0 * weight_S * control_change;
```

### 3. 约束条件

**数学公式：**
```
u_min ≤ u(k) ≤ u_max
|Δu(k)| ≤ Δu_max
```

**代码实现：**
```cpp
// 输出约束
control_sequence[i] = std::clamp(control_sequence[i], 
                                CONTROLLER_OUTPUT_MIN, 
                                CONTROLLER_OUTPUT_MAX);

// 变化率约束
if (abs(change) > max_output_change) {
    control_sequence[i] = control_sequence[i-1] + 
                         sign(change) * max_output_change;
}
```

---

## 🎯 总结：为什么要这样命名？

### 1. 遵循学术传统
- Q、R、S矩阵来自经典LQR理论
- A、B矩阵来自状态空间理论
- Np、Nc来自MPC文献

### 2. 便于理解和维护
- 直观的英文命名（如predicted_outputs）
- 统一的命名风格（如_t后缀表示类型）
- 注释说明每个参数的物理含义

### 3. 国际化标准
- 使用控制理论的标准术语
- 便于与学术文献对照
- 利于团队协作和代码移植

这样的命名体系让代码既专业又易懂，初学者通过参数名就能大致了解其作用，而专业人士也能快速理解算法实现。
