# MPC数学公式与代码实现对照

## 🧮 数学公式 ↔ 代码实现对照表

### 1. 系统模型

**数学公式：**
```
y(k+1) = a·y(k) + b·u(k)
```

**物理含义：**
- 下一时刻的电流 = 0.95×当前电流 + 0.05×控制电压

**代码实现：**
```cpp
// 在 predictFutureStates() 函数中
process_variable.predicted_states[i] = 
    model_a * process_variable.predicted_states[i-1] +  // a·y(k)
    model_b * control_input;                           // b·u(k)

// 参数值
double model_a = 0.95;  // 系统惯性系数
double model_b = 0.05;  // 控制增益系数
```

**为什么是0.95和0.05？**
- 电流有惯性，不会瞬间变化 → a接近1
- 控制电压的影响相对较小 → b较小
- a + b接近1保证系统稳定性

---

### 2. 成本函数（目标函数）

**数学公式：**
```
J = Σ[i=1 to Np] [Q·(r-y(k+i))² + R·u(k+i-1)² + S·Δu(k+i-1)²]
```

**各项含义：**
- **Q项**：跟踪误差惩罚 - "我多想准确跟踪目标"
- **R项**：控制能耗惩罚 - "我多想节约控制能量"
- **S项**：控制变化惩罚 - "我多想让控制平稳"

**代码实现：**
```cpp
// 在 solveQPProblem() 函数中

// Q项：跟踪误差项的梯度
for (int j = i; j < prediction_horizon; j++) {
    double predicted_output = process_variable.predicted_outputs[j];
    double tracking_error = process_variable.target - predicted_output;  // r-y(k+j)
    
    // ∂J/∂u = -2·Q·(r-y)·∂y/∂u
    grad += -2.0 * weight_tracking * tracking_error * model_b * pow(model_a, j-i);
}

// R项：控制量权重项
// ∂J/∂u = 2·R·u
grad += 2.0 * weight_control * process_variable.control_sequence[i];

// S项：控制增量权重项  
double delta_u = process_variable.control_sequence[i] - process_variable.control_sequence[i-1];
// ∂J/∂u = 2·S·Δu
grad += 2.0 * weight_rate * delta_u;
```

**为什么有系数2？**
- 对 x² 求导得到 2x
- 这来自数学求导规则

**为什么有pow(model_a, j-i)？**
- 因为第i步的控制对第j步输出的影响会衰减
- 衰减系数就是 a^(j-i)

---

### 3. 梯度下降更新

**数学公式：**
```
u(k+1) = u(k) - α·∇J(u(k))
```

**含义：**
- 沿着成本函数下降最快的方向更新控制量
- α是学习率（步长）

**代码实现：**
```cpp
// 梯度下降更新
const double step_size = 0.1;  // α = 0.1
for (int i = 0; i < control_horizon; i++) {
    process_variable.control_sequence[i] -= step_size * gradient[i];
    //           u(k+1)                =    u(k)    - α·∇J
}
```

**为什么选择α=0.1？**
- 太大（如α=1）：可能振荡，不稳定
- 太小（如α=0.01）：收敛太慢
- 0.1是经验上的折中值

---

### 4. 约束条件

**数学公式：**
```
u_min ≤ u(k) ≤ u_max                    (输出约束)
|u(k) - u(k-1)| ≤ Δu_max               (变化率约束)
y_min ≤ y(k+i) ≤ y_max                 (状态约束)
```

**代码实现：**
```cpp
// 在 applyConstraints() 函数中

// 输出约束：0V ≤ u ≤ 3.3V
if (process_variable.control_sequence[i] > CONTROLLER_OUTPUT_MAX) {
    process_variable.control_sequence[i] = CONTROLLER_OUTPUT_MAX;  // u = u_max
}
if (process_variable.control_sequence[i] < CONTROLLER_OUTPUT_MIN) {
    process_variable.control_sequence[i] = CONTROLLER_OUTPUT_MIN;  // u = u_min
}

// 变化率约束：|Δu| ≤ 0.5V
T change = process_variable.control_sequence[i] - process_variable.control_sequence[i-1];
if (change > max_output_change) {
    process_variable.control_sequence[i] = 
        process_variable.control_sequence[i-1] + max_output_change;
}
if (change < -max_output_change) {
    process_variable.control_sequence[i] = 
        process_variable.control_sequence[i-1] - max_output_change;
}
```

---

### 5. 预测方程组

**数学公式：**
```
y(k+1) = a·y(k) + b·u(k)
y(k+2) = a·y(k+1) + b·u(k+1) = a²·y(k) + a·b·u(k) + b·u(k+1)
y(k+3) = a³·y(k) + a²·b·u(k) + a·b·u(k+1) + b·u(k+2)
...
```

**矩阵形式：**
```
[y(k+1)]   [a ]       [b  0  0  0  0 ] [u(k)  ]
[y(k+2)] = [a²] y(k) + [ab b  0  0  0 ] [u(k+1)]
[y(k+3)]   [a³]       [a²b ab b 0  0 ] [u(k+2)]
[y(k+4)]   [a⁴]       [a³b a²b ab b 0] [u(k+3)]
[y(k+5)]   [a⁵]       [a⁴b a³b a²b ab b] [u(k+4)]
```

**代码实现：**
```cpp
// 在 predictFutureStates() 函数中
for (int i = 1; i <= prediction_horizon; i++) {
    // 选择控制输入
    T control_input = (i <= control_horizon) ? 
                     process_variable.control_sequence[i-1] :
                     process_variable.control_sequence[control_horizon-1];
    
    // 递推计算：y(k+i) = a·y(k+i-1) + b·u(k+i-1)
    process_variable.predicted_states[i] = 
        model_a * process_variable.predicted_states[i-1] + 
        model_b * control_input;
}
```

**为什么用递推而不是矩阵？**
- 递推计算简单，内存占用少
- 对于单输入单输出系统，矩阵运算过于复杂

---

### 6. 权重矩阵的选择

**数学原理：**
```
Q >> R >> S  (重要性递减)
```

**典型比例：**
```
Q : R : S = 100 : 10 : 1
```

**代码实现：**
```cpp
// 权重参数设置
double weight_tracking = 1.0;   // Q权重：跟踪性能
double weight_control = 0.1;    // R权重：控制能耗
double weight_rate = 0.01;      // S权重：控制平滑
```

**调参指导：**
- **Q增大** → 跟踪更精确，但可能振荡
- **R增大** → 控制更节能，但响应变慢  
- **S增大** → 控制更平稳，但响应变慢

---

### 7. 时域参数的数学意义

**预测时域 Np：**
```
数学：能预测多远的未来
代码：prediction_horizon = 10
物理：能看到未来10个采样周期的行为
```

**控制时域 Nc：**
```  
数学：优化多少步的控制
代码：control_horizon = 5
物理：精确计算前5步，后5步用固定策略
```

**为什么 Nc < Np？**
- 减少计算量：5个变量比10个变量好优化
- 实际考虑：远期的精确控制意义不大

---

## 🎯 完整的MPC数学模型

**优化问题的标准形式：**
```
minimize:   J = Σ[i=1 to Np] [Q·(r-y(k+i))² + R·u(k+i-1)² + S·Δu(k+i-1)²]

subject to: y(k+i) = a·y(k+i-1) + b·u(k+i-1)    (系统模型)
           u_min ≤ u(k+i-1) ≤ u_max             (输出约束)
           |Δu(k+i-1)| ≤ Δu_max                (变化率约束)
           
variables:  u(k), u(k+1), ..., u(k+Nc-1)       (控制序列)
```

**对应的代码结构：**
```cpp
class MPC_controller_t {
    // 目标函数权重
    double weight_tracking, weight_control, weight_rate;  // Q, R, S
    
    // 系统模型
    double model_a, model_b;  // a, b
    
    // 约束参数
    T CONTROLLER_OUTPUT_MAX, CONTROLLER_OUTPUT_MIN;  // u_min, u_max
    double max_output_change;  // Δu_max
    
    // 时域参数
    static const int prediction_horizon = 10;  // Np
    static const int control_horizon = 5;      // Nc
    
    // 核心算法
    void predictFutureStates();  // 实现系统模型
    void solveQPProblem();       // 求解优化问题
    void applyConstraints();     // 处理约束条件
};
```

这样，数学公式和代码实现就完全对应起来了！每一行代码都有明确的数学含义，每个数学公式都有具体的代码实现。

理解了这个对照关系，你就能：
1. 从数学角度理解代码在做什么
2. 从代码角度验证数学推导是否正确
3. 根据应用需求调整参数和算法

这就是工程实现与理论基础的完美结合！
