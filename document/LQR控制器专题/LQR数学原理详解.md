# LQR数学原理详解：从理论到实现

## 📖 目录
1. [数学基础](#-数学基础)
2. [最优控制理论](#-最优控制理论)
3. [Riccati方程求解](#-riccati方程求解)
4. [电子负载建模](#-电子负载建模)
5. [稳定性分析](#-稳定性分析)
6. [代码实现对照](#-代码实现对照)

## 📐 数学基础

### 状态空间表示

线性时不变系统的状态空间模型：

```
连续时间系统：
ẋ(t) = Ax(t) + Bu(t)
y(t) = Cx(t)

离散时间系统：
x(k+1) = Ax(k) + Bu(k)
y(k) = Cx(k)
```

**符号含义：**
- `x(k)` ∈ ℝⁿ：状态向量
- `u(k)` ∈ ℝᵐ：控制输入向量
- `y(k)` ∈ ℝᵖ：输出向量
- `A` ∈ ℝⁿˣⁿ：状态转移矩阵
- `B` ∈ ℝⁿˣᵐ：控制输入矩阵
- `C` ∈ ℝᵖˣⁿ：输出矩阵

### 电子负载状态空间建模

对于电子负载恒流控制系统，我们定义：

#### 状态变量选择
```
x₁(k) = e(k) = r(k) - y(k)           // 电流误差
x₂(k) = ė(k) = [e(k) - e(k-1)]/dt    // 误差变化率
x₃(k) = ∫e(τ)dτ                      // 误差积分
```

#### 状态方程推导

**误差动态方程：**
```
e(k+1) = r(k+1) - y(k+1)
```

假设参考值变化缓慢：`r(k+1) ≈ r(k)`

根据系统动态特性：
```
y(k+1) = ay(k) + bu(k)
```

因此：
```
e(k+1) = r(k) - ay(k) - bu(k)
       = r(k) - a[r(k) - e(k)] - bu(k)
       = (1-a)r(k) + ae(k) - bu(k)
```

在平衡点附近线性化（r为常数）：
```
e(k+1) ≈ ae(k) - bu(k)
```

**完整状态方程：**
```
[e(k+1)    ]   [1  dt  0] [e(k)    ]   [0 ]
[ė(k+1)    ] = [0  a   0] [ė(k)    ] + [b ] u(k)
[∫e(k+1)   ]   [1  0   1] [∫e(k)   ]   [0 ]
```

其中：
- `dt`：采样周期
- `a`：系统极点（0 < a < 1）
- `b`：控制增益（b > 0）

## 🎯 最优控制理论

### 性能指标函数

LQR的目标是最小化二次型性能指标：

**无限时域LQR：**
```
J = Σ(k=0 to ∞) [x(k)ᵀQx(k) + u(k)ᵀRu(k)]
```

**有限时域LQR：**
```
J = x(N)ᵀSx(N) + Σ(k=0 to N-1) [x(k)ᵀQx(k) + u(k)ᵀRu(k)]
```

**权重矩阵要求：**
- `Q ≥ 0`（半正定）：状态权重矩阵
- `R > 0`（正定）：控制权重矩阵
- `S ≥ 0`（半正定）：终端权重矩阵

### 电子负载性能指标设计

```
J = Σ(k=0 to ∞) [q₁e(k)² + q₂ė(k)² + q₃(∫e)² + ru(k)²]
```

**物理意义：**
- `q₁e²`：惩罚电流跟踪误差
- `q₂ė²`：惩罚电流变化率（增加阻尼）
- `q₃(∫e)²`：惩罚稳态误差
- `ru²`：惩罚控制能量消耗

**权重矩阵形式：**
```
Q = [q₁  0   0 ]     R = [r]
    [0   q₂  0 ]
    [0   0   q₃]
```

### 最优控制律推导

使用**动态规划原理**，定义值函数：
```
V(x,k) = min{u(k),...,u(∞)} Σ(j=k to ∞) [x(j)ᵀQx(j) + u(j)ᵀRu(j)]
```

**Bellman方程：**
```
V(x,k) = min{u(k)} [x(k)ᵀQx(k) + u(k)ᵀRu(k) + V(Ax(k)+Bu(k), k+1)]
```

假设值函数为二次型：
```
V(x,k) = x(k)ᵀP(k)x(k)
```

将此形式代入Bellman方程，通过变分法求最优控制：
```
∂/∂u [x(k)ᵀQx(k) + u(k)ᵀRu(k) + (Ax(k)+Bu(k))ᵀP(k+1)(Ax(k)+Bu(k))] = 0
```

得到最优控制律：
```
u*(k) = -K(k)x(k)
```

其中增益矩阵：
```
K(k) = (R + BᵀP(k+1)B)⁻¹BᵀP(k+1)A
```

## 🔄 Riccati方程求解

### 离散代数Riccati方程（DARE）

对于无限时域问题，P矩阵收敛到常数P∞，满足：

```
P = AᵀPA - AᵀPB(R + BᵀPB)⁻¹BᵀPA + Q
```

这就是**离散代数Riccati方程（DARE）**。

### 迭代求解算法

**算法1：直接迭代法**
```
1. 初始化：P₀ = Q
2. 迭代：Pₖ₊₁ = AᵀPₖA - AᵀPₖB(R + BᵀPₖB)⁻¹BᵀPₖA + Q
3. 收敛判断：||Pₖ₊₁ - Pₖ|| < ε
```

**代码实现：**
```cpp
bool solveLQRRiccati() {
    std::vector<std::vector<T>> P = Q;  // 初始化
    
    for (int iter = 0; iter < max_iterations; ++iter) {
        // 计算 BᵀPB + R
        auto BT_P_B_R = computeBTPB_R(P);
        
        // 计算 (BᵀPB + R)⁻¹
        auto inv_BT_P_B_R = matrixInverse(BT_P_B_R);
        
        // 计算 AᵀPA
        auto AT_P_A = computeATPB(P);
        
        // 计算修正项
        auto correction = computeCorrection(P, inv_BT_P_B_R);
        
        // 更新 P
        auto P_new = AT_P_A - correction + Q;
        
        // 检查收敛
        if (isConverged(P, P_new)) {
            computeGain(P_new, inv_BT_P_B_R);
            return true;
        }
        
        P = P_new;
    }
    return false;
}
```

### 增益矩阵计算

一旦Riccati方程收敛，计算增益矩阵：
```
K = (R + BᵀPB)⁻¹BᵀPA
```

**电子负载系统的增益向量：**
```cpp
// 对于3×1系统，K是1×3向量
K = [k₁  k₂  k₃]

// 控制律
u(k) = -k₁e(k) - k₂ė(k) - k₃∫e(k)
```

这相当于一个**增强型PID控制器**：
- `k₁`：比例增益（类似Kp）
- `k₂`：微分增益（类似Kd）
- `k₃`：积分增益（类似Ki）

但与传统PID不同，这些增益是通过**数学优化**得到的最优值！

## 📊 电子负载建模

### 系统辨识

电子负载系统可建模为一阶系统：
```
G(s) = b/(s - a)
```

**离散化（零阶保持器）：**
```
G(z) = b(1-e^(aT))/(z - e^(aT))
```

其中T为采样周期。

### 参数估计

**方法1：阶跃响应**
```cpp
void systemIdentification() {
    // 施加阶跃输入
    setDAC(2.0);  // 2V阶跃
    
    // 记录响应
    std::vector<double> response;
    for (int i = 0; i < 100; ++i) {
        response.push_back(readCurrent());
        delay(10);  // 10ms采样
    }
    
    // 拟合一阶模型
    auto [a, b] = fitFirstOrderModel(response);
    
    Serial.printf("Identified parameters: a=%.4f, b=%.4f\n", a, b);
}
```

**方法2：最小二乘辨识**
```
设：y(k) = ay(k-1) + bu(k-1) + ε(k)

矩阵形式：Y = Φθ + E

其中：
Y = [y(2), y(3), ..., y(N)]ᵀ
Φ = [y(1) u(1)]
    [y(2) u(2)]
    [... ...]
    [y(N-1) u(N-1)]
θ = [a, b]ᵀ

最小二乘解：θ̂ = (ΦᵀΦ)⁻¹ΦᵀY
```

### 模型验证

```cpp
bool validateModel(double a, double b) {
    // 检查稳定性
    if (abs(a) >= 1.0) {
        Serial.println("Warning: System unstable!");
        return false;
    }
    
    // 检查增益合理性
    if (b <= 0 || b > 1.0) {
        Serial.println("Warning: Unreasonable gain!");
        return false;
    }
    
    // 检查可控性
    // 对于电子负载系统，通常是可控的
    
    return true;
}
```

## 📈 稳定性分析

### 闭环系统稳定性

LQR闭环系统：
```
x(k+1) = (A - BK)x(k)
```

**稳定性条件：**
矩阵(A - BK)的所有特征值都在单位圆内：
```
|λᵢ(A - BK)| < 1,  ∀i
```

### LQR稳定性保证

**定理：** 如果系统(A,B)可控，则LQR控制器保证闭环系统稳定。

**证明思路：**
1. 可控性保证Riccati方程有唯一正定解
2. 正定解保证增益矩阵使闭环系统稳定
3. LQR的最优性保证了最佳的稳定裕度

### 鲁棒性分析

LQR具有优秀的鲁棒性：

**增益裕度：** 无穷大
**相位裕度：** 至少60°

```cpp
void analyzeRobustness() {
    // 计算闭环极点
    auto A_cl = A - B * K;
    auto poles = eigenvalues(A_cl);
    
    Serial.println("Closed-loop poles:");
    for (auto pole : poles) {
        Serial.printf("  %.4f + %.4fj (magnitude: %.4f)\n", 
                     pole.real(), pole.imag(), abs(pole));
    }
    
    // 检查稳定性
    bool stable = true;
    for (auto pole : poles) {
        if (abs(pole) >= 1.0) {
            stable = false;
            break;
        }
    }
    
    Serial.printf("System is %s\n", stable ? "STABLE" : "UNSTABLE");
}
```

## 💻 代码实现对照

### 数学公式与代码对照

#### 1. 状态更新

**数学公式：**
```
x(k+1) = Ax(k) + Bu(k)
```

**代码实现：**
```cpp
void updateStates() {
    // 计算新的误差
    T current_error = process_variable.target - process_variable.measure;
    
    // 状态向量更新
    std::vector<T> prev_state = process_variable.state;
    
    // x₁ = error
    process_variable.state[0] = current_error;
    
    // x₂ = error_rate = (e(k) - e(k-1))/dt
    T dt = static_cast<T>(sampling_time);
    process_variable.state[1] = (current_error - prev_state[0]) / dt;
    
    // x₃ = integral = integral(k-1) + e(k)*dt
    process_variable.state[2] = prev_state[2] + current_error * dt;
}
```

#### 2. 控制律计算

**数学公式：**
```
u(k) = -Kx(k) = -[k₁ k₂ k₃][e(k), ė(k), ∫e(k)]ᵀ
```

**代码实现：**
```cpp
void computeLQRControl() {
    T control_output = T{0};
    
    // u = -K * x
    for (size_t i = 0; i < process_variable.K.size(); ++i) {
        control_output -= process_variable.K[i] * process_variable.state[i];
    }
    
    process_variable.output = control_output;
}
```

#### 3. Riccati方程求解

**数学公式：**
```
P = AᵀPA - AᵀPB(R + BᵀPB)⁻¹BᵀPA + Q
```

**代码实现：**
```cpp
bool solveLQRRiccati() {
    // 初始化 P = Q
    std::vector<std::vector<T>> P = process_variable.Q;
    
    for (int iter = 0; iter < max_iterations; ++iter) {
        // 计算 B^T * P * B + R
        auto BT_P_B_R = computeBTPB_plusR(P);
        
        // 计算逆矩阵
        auto inv_BT_P_B_R = matrixInverse(BT_P_B_R);
        
        // 计算 A^T * P * A
        auto AT_P_A = computeATPB(P);
        
        // 计算修正项
        auto correction = computeCorrection(P, inv_BT_P_B_R);
        
        // 更新 P_new = A^T*P*A - correction + Q
        auto P_new = matrixAdd(matrixSubtract(AT_P_A, correction), Q);
        
        // 检查收敛
        if (checkConvergence(P, P_new)) {
            computeLQRGain(P_new, inv_BT_P_B_R);
            return true;
        }
        
        P = P_new;
    }
    
    return false; // 未收敛
}
```

### 数值计算注意事项

#### 1. 数值稳定性
```cpp
// 避免矩阵奇异
if (std::abs(determinant(BT_P_B_R)) < 1e-12) {
    Serial.println("Warning: Matrix near singular!");
    return false;
}

// 使用数值稳定的算法
auto inv = matrixInverse_SVD(BT_P_B_R);  // SVD分解求逆
```

#### 2. 收敛判断
```cpp
bool checkConvergence(const Matrix& P_old, const Matrix& P_new) {
    T max_diff = T{0};
    
    for (size_t i = 0; i < P_old.size(); ++i) {
        for (size_t j = 0; j < P_old[i].size(); ++j) {
            T diff = std::abs(P_new[i][j] - P_old[i][j]);
            max_diff = std::max(max_diff, diff);
        }
    }
    
    return max_diff < convergence_tolerance;
}
```

#### 3. 数值精度
```cpp
// 使用double而非float
using T = double;  // 推荐

// 设置合适的容差
const T convergence_tolerance = 1e-8;

// 限制迭代次数防止无限循环
const int max_iterations = 100;
```

## 🎯 总结

LQR控制器的数学基础体现了现代控制理论的优雅：

### 🔬 理论优势
1. **最优性保证**：数学上的最优解
2. **稳定性保证**：可控系统必然稳定
3. **鲁棒性优秀**：天然的增益和相位裕度
4. **设计系统**：基于状态空间的科学设计

### 💻 实现特点
1. **计算复杂度**：O(n³)的初始化 + O(n)的运行时
2. **存储需求**：主要是增益向量K
3. **数值稳定**：需要注意矩阵运算的数值问题
4. **参数调节**：通过Q、R矩阵调节性能

### 🎯 应用前景
在电子负载系统中，LQR提供了PID和MPC之间的最佳平衡：
- 比PID更科学（最优性）
- 比MPC更简单（无约束优化）
- 实时性优秀（无在线优化）

通过深入理解LQR的数学原理，我们可以更好地调参和优化控制器性能！
