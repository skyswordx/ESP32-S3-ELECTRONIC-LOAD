/**
 * @file our_fft_processor.hpp
 * @brief 全国大学生电子设计大赛专用FFT频域分析模块
 * @details 专为电源类和信号类赛题设计的高性能数字信号处理库
 *          支持实时FFT计算、功率谱分析、谐波分析等功能
 * @author ESP32-S3 Electronic Load Team
 * @date 2025-06-16
 * @version 1.0
 * 
 * 功能特性：
 * - 高效FFT/IFFT算法实现（基于Cooley-Tukey算法）
 * - 实时功率谱密度(PSD)分析
 * - 基频自动检测
 * - 谐波成分分析
 * - 总谐波失真(THD)精确计算
 * - 频谱峰值搜索与排序
 * - 窗函数支持（汉明、汉宁、布莱克曼等）
 * - 零相位滤波
 * - 内存优化与实时性保证
 * 
 * 适用场景：
 * - 电源质量分析
 * - 信号完整性检测
 * - 频率响应测试
 * - 噪声分析
 * - 滤波器设计验证
 */

#pragma once

#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>
#include <memory>
#include <functional>
#include <string>

namespace DesignContest {

/**
 * @brief 复数类型定义（兼容标准库）
 */
using Complex = std::complex<double>;

/**
 * @brief 窗函数类型枚举
 */
enum class WindowType {
    RECTANGULAR,    ///< 矩形窗（无窗函数）
    HANNING,       ///< 汉宁窗
    HAMMING,       ///< 汉明窗
    BLACKMAN,      ///< 布莱克曼窗
    KAISER,        ///< 凯泽窗
    BARTLETT       ///< 巴特利特窗
};

/**
 * @brief 频谱峰值信息结构体
 */
struct SpectralPeak {
    double frequency;    ///< 峰值频率 (Hz)
    double magnitude;    ///< 峰值幅度
    double phase;        ///< 峰值相位 (弧度)
    double power;        ///< 峰值功率
    size_t bin_index;    ///< 频率仓索引
    
    SpectralPeak() : frequency(0), magnitude(0), phase(0), power(0), bin_index(0) {}
    SpectralPeak(double freq, double mag, double ph, double pow, size_t idx)
        : frequency(freq), magnitude(mag), phase(ph), power(pow), bin_index(idx) {}
};

/**
 * @brief 谐波分析结果结构体
 */
struct HarmonicAnalysis {
    double fundamental_freq;              ///< 基频 (Hz)
    double fundamental_magnitude;         ///< 基波幅度
    std::vector<double> harmonic_freqs;   ///< 谐波频率列表
    std::vector<double> harmonic_mags;    ///< 谐波幅度列表
    std::vector<double> harmonic_phases;  ///< 谐波相位列表
    double thd_percentage;                ///< 总谐波失真百分比
    double thd_db;                        ///< 总谐波失真(dB)
    double snr_db;                        ///< 信噪比(dB)
    
    HarmonicAnalysis() : fundamental_freq(0), fundamental_magnitude(0), 
                        thd_percentage(0), thd_db(0), snr_db(0) {}
};

/**
 * @brief FFT频域分析处理器主类
 * @details 高性能、实时的FFT处理器，专为电赛信号分析优化
 */
class FFTProcessor {
private:
    size_t fft_size_;                    ///< FFT点数（必须为2的幂）
    double sample_rate_;                 ///< 采样率 (Hz)
    WindowType window_type_;             ///< 当前窗函数类型
    std::vector<double> window_func_;    ///< 窗函数系数
    std::vector<Complex> fft_buffer_;    ///< FFT计算缓冲区
    std::vector<double> magnitude_spectrum_; ///< 幅度谱缓冲区
    std::vector<double> phase_spectrum_;     ///< 相位谱缓冲区
    std::vector<double> power_spectrum_;     ///< 功率谱缓冲区
    
    // 位反转表（加速FFT计算）
    std::vector<size_t> bit_reverse_table_;
    
    // 旋转因子表（预计算，提升性能）
    std::vector<Complex> twiddle_factors_;
    
    // 性能优化参数
    bool use_zero_padding_;              ///< 是否使用零填充
    double frequency_resolution_;        ///< 频率分辨率 (Hz/bin)
    
public:
    /**
     * @brief 构造函数
     * @param fft_size FFT点数，必须为2的幂（推荐512, 1024, 2048, 4096）
     * @param sample_rate 采样率 (Hz)
     * @param window_type 窗函数类型，默认汉宁窗
     */
    explicit FFTProcessor(size_t fft_size, double sample_rate, 
                         WindowType window_type = WindowType::HANNING);
    
    /**
     * @brief 析构函数
     */
    ~FFTProcessor() = default;
    
    // ==================== 核心FFT算法 ====================
    
    /**
     * @brief 前向快速傅里叶变换
     * @param input_data 输入时域信号
     * @param output_spectrum 输出频域复数谱（可选，为空则使用内部缓冲区）
     * @return 频域复数谱的引用
     */
    const std::vector<Complex>& computeFFT(const std::vector<double>& input_data,
                                          std::vector<Complex>* output_spectrum = nullptr);
    
    /**
     * @brief 逆快速傅里叶变换
     * @param input_spectrum 输入频域复数谱
     * @param output_data 输出时域信号
     * @return 时域信号的引用
     */
    std::vector<double>& computeIFFT(const std::vector<Complex>& input_spectrum,
                                    std::vector<double>& output_data);
    
    // ==================== 频谱分析功能 ====================
    
    /**
     * @brief 计算幅度谱
     * @param spectrum 频域复数谱（为空则使用上次FFT结果）
     * @return 幅度谱（dB或线性）
     */
    const std::vector<double>& computeMagnitudeSpectrum(const std::vector<Complex>* spectrum = nullptr,
                                                       bool in_db = true);
    
    /**
     * @brief 计算相位谱
     * @param spectrum 频域复数谱（为空则使用上次FFT结果）
     * @return 相位谱（弧度）
     */
    const std::vector<double>& computePhaseSpectrum(const std::vector<Complex>* spectrum = nullptr);
    
    /**
     * @brief 计算功率谱密度(PSD)
     * @param spectrum 频域复数谱（为空则使用上次FFT结果）
     * @param normalize 是否归一化
     * @return 功率谱密度
     */
    const std::vector<double>& computePowerSpectralDensity(const std::vector<Complex>* spectrum = nullptr,
                                                          bool normalize = true);
    
    // ==================== 基频检测 ====================
    
    /**
     * @brief 自动基频检测
     * @param input_data 输入时域信号
     * @param freq_min 搜索频率下限 (Hz)
     * @param freq_max 搜索频率上限 (Hz)
     * @param use_autocorrelation 是否使用自相关算法辅助
     * @return 检测到的基频 (Hz)
     */
    double detectFundamentalFrequency(const std::vector<double>& input_data,
                                     double freq_min = 20.0, double freq_max = 2000.0,
                                     bool use_autocorrelation = true);
    
    /**
     * @brief 基于HPS（谐波乘积谱）的基频检测
     * @param input_data 输入时域信号
     * @param harmonics_count 考虑的谐波数量
     * @return 检测到的基频 (Hz)
     */
    double detectFundamentalHPS(const std::vector<double>& input_data, int harmonics_count = 5);
    
    // ==================== 谐波分析 ====================
    
    /**
     * @brief 完整谐波分析
     * @param input_data 输入时域信号
     * @param max_harmonics 最大谐波次数
     * @param fundamental_freq 已知基频（为0则自动检测）
     * @return 谐波分析结果
     */
    HarmonicAnalysis analyzeHarmonics(const std::vector<double>& input_data,
                                     int max_harmonics = 10,
                                     double fundamental_freq = 0.0);
    
    /**
     * @brief 计算总谐波失真(THD)
     * @param harmonic_result 谐波分析结果
     * @return THD百分比和dB值
     */
    std::pair<double, double> calculateTHD(const HarmonicAnalysis& harmonic_result);
    
    /**
     * @brief 单独计算THD（基于幅度谱）
     * @param magnitude_spectrum 幅度谱
     * @param fundamental_freq 基频
     * @param max_harmonics 最大谐波次数
     * @return {THD百分比, THD_dB}
     */
    std::pair<double, double> calculateTHDFromSpectrum(const std::vector<double>& magnitude_spectrum,
                                                      double fundamental_freq,
                                                      int max_harmonics = 10);
    
    // ==================== 频谱峰值搜索 ====================
    
    /**
     * @brief 频谱峰值搜索
     * @param magnitude_spectrum 幅度谱（为空则使用内部缓冲区）
     * @param peak_count 返回峰值数量
     * @param min_magnitude 最小峰值幅度阈值 (dB)
     * @param min_separation 峰值间最小频率间隔 (Hz)
     * @return 按幅度排序的峰值列表
     */
    std::vector<SpectralPeak> findSpectralPeaks(const std::vector<double>* magnitude_spectrum = nullptr,
                                               size_t peak_count = 10,
                                               double min_magnitude = -60.0,
                                               double min_separation = 10.0);
    
    /**
     * @brief 精确峰值频率估计（抛物线插值）
     * @param bin_index 粗略峰值所在频率仓
     * @param magnitude_spectrum 幅度谱
     * @return 精确频率估计 (Hz)
     */
    double refinePeakFrequency(size_t bin_index, const std::vector<double>& magnitude_spectrum);
    
    // ==================== 窗函数管理 ====================
    
    /**
     * @brief 设置窗函数类型
     * @param window_type 窗函数类型
     * @param beta 凯泽窗参数（仅对KAISER窗有效）
     */
    void setWindowFunction(WindowType window_type, double beta = 8.0);
    
    /**
     * @brief 获取当前窗函数系数
     * @return 窗函数系数向量
     */
    const std::vector<double>& getWindowFunction() const { return window_func_; }
    
    /**
     * @brief 计算窗函数的相干增益
     * @return 相干增益系数
     */
    double getWindowCoherentGain() const;
    
    // ==================== 实用工具函数 ====================
    
    /**
     * @brief 频率到频率仓索引转换
     * @param frequency 频率 (Hz)
     * @return 频率仓索引
     */
    size_t frequencyToBin(double frequency) const;
    
    /**
     * @brief 频率仓索引到频率转换
     * @param bin_index 频率仓索引
     * @return 频率 (Hz)
     */
    double binToFrequency(size_t bin_index) const;
    
    /**
     * @brief 获取频率分辨率
     * @return 频率分辨率 (Hz/bin)
     */
    double getFrequencyResolution() const { return frequency_resolution_; }
    
    /**
     * @brief 获取奈奎斯特频率
     * @return 奈奎斯特频率 (Hz)
     */
    double getNyquistFrequency() const { return sample_rate_ / 2.0; }
    
    /**
     * @brief 获取有效频率范围
     * @return {最小频率, 最大频率} (Hz)
     */
    std::pair<double, double> getEffectiveFrequencyRange() const;
    
    /**
     * @brief 重新配置FFT参数
     * @param new_fft_size 新的FFT点数
     * @param new_sample_rate 新的采样率
     * @return 是否配置成功
     */
    bool reconfigure(size_t new_fft_size, double new_sample_rate);
    
    /**
     * @brief 获取FFT配置信息
     * @return 配置信息字符串
     */
    std::string getConfigurationInfo() const;
    
    // ==================== 性能与内存管理 ====================
    
    /**
     * @brief 预热FFT计算（预分配内存，优化首次执行性能）
     */
    void warmUp();
    
    /**
     * @brief 清理内存（释放缓冲区）
     */
    void cleanup();
    
    /**
     * @brief 获取内存使用情况
     * @return 内存使用量（字节）
     */
    size_t getMemoryUsage() const;
    
private:
    // ==================== 内部算法实现 ====================
    
    /**
     * @brief 位反转排列
     * @param data 待排列的复数数组
     */
    void bitReverseReorder(std::vector<Complex>& data);
    
    /**
     * @brief 生成位反转表
     */
    void generateBitReverseTable();
    
    /**
     * @brief 生成旋转因子表
     */
    void generateTwiddleFactors();
    
    /**
     * @brief 原地FFT计算（Cooley-Tukey算法）
     * @param data 输入输出复数数组
     * @param inverse 是否为逆变换
     */
    void computeFFTInPlace(std::vector<Complex>& data, bool inverse = false);
    
    /**
     * @brief 生成指定类型的窗函数
     * @param window_type 窗函数类型
     * @param size 窗函数长度
     * @param beta 额外参数（如凯泽窗的β参数）
     * @return 窗函数系数向量
     */
    std::vector<double> generateWindow(WindowType window_type, size_t size, double beta = 8.0);
    
    /**
     * @brief 应用窗函数
     * @param data 输入信号
     * @param windowed_data 加窗后的信号
     */
    void applyWindow(const std::vector<double>& data, std::vector<double>& windowed_data);
    
    /**
     * @brief 检查FFT大小是否为2的幂
     * @param size 待检查的大小
     * @return 是否为2的幂
     */
    bool isPowerOfTwo(size_t size) const;
    
    /**
     * @brief 自相关算法辅助基频检测
     * @param data 输入信号
     * @param min_period 最小周期（样本数）
     * @param max_period 最大周期（样本数）
     * @return 检测到的周期（样本数）
     */
    size_t autocorrelationPitchDetection(const std::vector<double>& data,
                                        size_t min_period, size_t max_period);
    
    /**
     * @brief 峰值检测算法（局部最大值）
     * @param data 输入数据
     * @param min_peak_distance 峰值间最小距离
     * @param min_peak_height 峰值最小高度
     * @return 峰值索引列表
     */
    std::vector<size_t> findLocalMaxima(const std::vector<double>& data,
                                       size_t min_peak_distance = 1,
                                       double min_peak_height = 0.0);
    
    /**
     * @brief 计算信噪比
     * @param signal_power 信号功率
     * @param noise_power 噪声功率
     * @return 信噪比 (dB)
     */
    double calculateSNR(double signal_power, double noise_power);
};

// ==================== 静态工具函数 ====================

/**
 * @brief 分贝转换工具类
 */
class DecibelUtils {
public:
    /**
     * @brief 线性值转分贝
     * @param linear_value 线性值
     * @param reference 参考值（默认1.0）
     * @return 分贝值
     */
    static double linearToDb(double linear_value, double reference = 1.0);
    
    /**
     * @brief 分贝转线性值
     * @param db_value 分贝值
     * @param reference 参考值（默认1.0）
     * @return 线性值
     */
    static double dbToLinear(double db_value, double reference = 1.0);
    
    /**
     * @brief 功率转分贝
     * @param power 功率值
     * @param reference_power 参考功率（默认1.0）
     * @return 分贝值
     */
    static double powerToDb(double power, double reference_power = 1.0);
};

/**
 * @brief 信号预处理工具类
 */
class SignalUtils {
public:
    /**
     * @brief 零相位带通滤波器
     * @param input_data 输入信号
     * @param low_freq 低截止频率 (Hz)
     * @param high_freq 高截止频率 (Hz)
     * @param sample_rate 采样率 (Hz)
     * @param filter_order 滤波器阶数
     * @return 滤波后的信号
     */
    static std::vector<double> zeroPhaseBandpassFilter(const std::vector<double>& input_data,
                                                      double low_freq, double high_freq,
                                                      double sample_rate, int filter_order = 4);
    
    /**
     * @brief 信号去趋势（去除直流分量和线性趋势）
     * @param input_data 输入信号
     * @param remove_linear 是否去除线性趋势
     * @return 去趋势后的信号
     */
    static std::vector<double> detrend(const std::vector<double>& input_data, bool remove_linear = true);
    
    /**
     * @brief 信号归一化
     * @param input_data 输入信号
     * @param method 归一化方法：0-最大值归一化，1-标准化，2-能量归一化
     * @return 归一化后的信号
     */
    static std::vector<double> normalize(const std::vector<double>& input_data, int method = 0);
};

} // namespace DesignContest