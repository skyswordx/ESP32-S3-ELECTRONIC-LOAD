/**
 * @file our_fft_processor.cpp
 * @brief 全国大学生电子设计大赛专用FFT频域分析模块实现
 * @author ESP32-S3 Electronic Load Team
 * @date 2025-06-16
 * @version 1.0
 */

#include "our_fft_processor.hpp"
#include <sstream>
#include <iomanip>
#include <cstring>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace DesignContest {

// ==================== FFTProcessor 实现 ====================

FFTProcessor::FFTProcessor(size_t fft_size, double sample_rate, WindowType window_type)
    : fft_size_(fft_size), sample_rate_(sample_rate), window_type_(window_type),
      use_zero_padding_(false) {
    
    // 检查FFT大小是否为2的幂
    if (!isPowerOfTwo(fft_size_)) {
        throw std::invalid_argument("FFT size must be a power of 2");
    }
    
    if (sample_rate_ <= 0) {
        throw std::invalid_argument("Sample rate must be positive");
    }
    
    // 计算频率分辨率
    frequency_resolution_ = sample_rate_ / static_cast<double>(fft_size_);
    
    // 初始化缓冲区
    fft_buffer_.resize(fft_size_);
    magnitude_spectrum_.resize(fft_size_ / 2 + 1);
    phase_spectrum_.resize(fft_size_ / 2 + 1);
    power_spectrum_.resize(fft_size_ / 2 + 1);
    
    // 生成优化表
    generateBitReverseTable();
    generateTwiddleFactors();
    
    // 设置窗函数
    setWindowFunction(window_type);
}

const std::vector<Complex>& FFTProcessor::computeFFT(const std::vector<double>& input_data,
                                                    std::vector<Complex>* output_spectrum) {
    if (input_data.size() > fft_size_) {
        throw std::invalid_argument("Input data size exceeds FFT size");
    }
    
    // 准备FFT输入数据
    std::vector<double> windowed_data(fft_size_, 0.0);
    
    // 复制输入数据并应用零填充
    for (size_t i = 0; i < input_data.size() && i < fft_size_; ++i) {
        windowed_data[i] = input_data[i];
    }
    
    // 应用窗函数
    applyWindow(windowed_data, windowed_data);
    
    // 转换为复数
    for (size_t i = 0; i < fft_size_; ++i) {
        fft_buffer_[i] = Complex(windowed_data[i], 0.0);
    }
    
    // 执行FFT
    computeFFTInPlace(fft_buffer_, false);
    
    // 如果提供了输出缓冲区，复制结果
    if (output_spectrum != nullptr) {
        *output_spectrum = fft_buffer_;
        return *output_spectrum;
    }
    
    return fft_buffer_;
}

std::vector<double>& FFTProcessor::computeIFFT(const std::vector<Complex>& input_spectrum,
                                              std::vector<double>& output_data) {
    if (input_spectrum.size() != fft_size_) {
        throw std::invalid_argument("Input spectrum size must match FFT size");
    }
    
    // 复制输入频谱
    fft_buffer_ = input_spectrum;
    
    // 执行逆FFT
    computeFFTInPlace(fft_buffer_, true);
    
    // 提取实部并归一化
    output_data.resize(fft_size_);
    for (size_t i = 0; i < fft_size_; ++i) {
        output_data[i] = fft_buffer_[i].real();
    }
    
    return output_data;
}

const std::vector<double>& FFTProcessor::computeMagnitudeSpectrum(const std::vector<Complex>* spectrum,
                                                                 bool in_db) {
    const std::vector<Complex>* spec_ptr = spectrum ? spectrum : &fft_buffer_;
    
    // 计算单边频谱（只取正频率部分）
    for (size_t i = 0; i < magnitude_spectrum_.size(); ++i) {
        double magnitude = std::abs((*spec_ptr)[i]);
        
        // DC和奈奎斯特频率特殊处理
        if (i > 0 && i < fft_size_ / 2) {
            magnitude *= 2.0; // 双边谱转单边谱
        }
        
        // 归一化
        magnitude /= static_cast<double>(fft_size_);
        
        if (in_db) {
            magnitude_spectrum_[i] = DecibelUtils::linearToDb(magnitude);
        } else {
            magnitude_spectrum_[i] = magnitude;
        }
    }
    
    return magnitude_spectrum_;
}

const std::vector<double>& FFTProcessor::computePhaseSpectrum(const std::vector<Complex>* spectrum) {
    const std::vector<Complex>* spec_ptr = spectrum ? spectrum : &fft_buffer_;
    
    for (size_t i = 0; i < phase_spectrum_.size(); ++i) {
        phase_spectrum_[i] = std::arg((*spec_ptr)[i]);
    }
    
    return phase_spectrum_;
}

const std::vector<double>& FFTProcessor::computePowerSpectralDensity(const std::vector<Complex>* spectrum,
                                                                    bool normalize) {
    const std::vector<Complex>* spec_ptr = spectrum ? spectrum : &fft_buffer_;
    
    double window_power = 0.0;
    if (normalize) {
        // 计算窗函数功率
        for (double w : window_func_) {
            window_power += w * w;
        }
    }
    
    for (size_t i = 0; i < power_spectrum_.size(); ++i) {
        double power = std::norm((*spec_ptr)[i]); // |X(k)|^2
        
        // DC和奈奎斯特频率特殊处理
        if (i > 0 && i < fft_size_ / 2) {
            power *= 2.0; // 双边谱转单边谱
        }
        
        if (normalize && window_power > 0) {
            power = power / (sample_rate_ * window_power);
        }
        
        power_spectrum_[i] = power;
    }
    
    return power_spectrum_;
}

double FFTProcessor::detectFundamentalFrequency(const std::vector<double>& input_data,
                                               double freq_min, double freq_max,
                                               bool use_autocorrelation) {
    // 先进行FFT分析
    computeFFT(input_data);
    computeMagnitudeSpectrum();
    
    // 确定搜索范围
    size_t bin_min = frequencyToBin(freq_min);
    size_t bin_max = frequencyToBin(freq_max);
    bin_max = std::min(bin_max, magnitude_spectrum_.size() - 1);
    
    // 在频域搜索最大峰值
    double max_magnitude = -std::numeric_limits<double>::infinity();
    size_t max_bin = bin_min;
    
    for (size_t i = bin_min; i <= bin_max; ++i) {
        if (magnitude_spectrum_[i] > max_magnitude) {
            max_magnitude = magnitude_spectrum_[i];
            max_bin = i;
        }
    }
    
    double fft_fundamental = binToFrequency(max_bin);
    
    // 如果启用自相关辅助验证
    if (use_autocorrelation && input_data.size() > 100) {
        size_t min_period = static_cast<size_t>(sample_rate_ / freq_max);
        size_t max_period = static_cast<size_t>(sample_rate_ / freq_min);
        max_period = std::min(max_period, input_data.size() / 2);
        
        size_t autocorr_period = autocorrelationPitchDetection(input_data, min_period, max_period);
        double autocorr_fundamental = sample_rate_ / static_cast<double>(autocorr_period);
        
        // 如果两种方法结果接近，使用更精确的结果
        if (std::abs(fft_fundamental - autocorr_fundamental) / fft_fundamental < 0.1) {
            return refinePeakFrequency(max_bin, magnitude_spectrum_);
        }
    }
    
    return refinePeakFrequency(max_bin, magnitude_spectrum_);
}

double FFTProcessor::detectFundamentalHPS(const std::vector<double>& input_data, int harmonics_count) {
    computeFFT(input_data);
    computeMagnitudeSpectrum(nullptr, false); // 使用线性刻度
    
    std::vector<double> hps_spectrum(magnitude_spectrum_.size());
    
    // 初始化HPS谱为第一次谐波（基波）
    hps_spectrum = magnitude_spectrum_;
    
    // 谐波乘积处理
    for (int h = 2; h <= harmonics_count; ++h) {
        for (size_t i = 0; i < hps_spectrum.size(); ++i) {
            size_t harmonic_bin = i * h;
            if (harmonic_bin < magnitude_spectrum_.size()) {
                hps_spectrum[i] *= magnitude_spectrum_[harmonic_bin];
            } else {
                hps_spectrum[i] = 0.0;
            }
        }
    }
    
    // 寻找HPS谱的最大值
    double max_value = 0.0;
    size_t max_bin = 0;
    
    for (size_t i = 1; i < hps_spectrum.size(); ++i) {
        if (hps_spectrum[i] > max_value) {
            max_value = hps_spectrum[i];
            max_bin = i;
        }
    }
    
    return refinePeakFrequency(max_bin, hps_spectrum);
}

HarmonicAnalysis FFTProcessor::analyzeHarmonics(const std::vector<double>& input_data,
                                               int max_harmonics,
                                               double fundamental_freq) {
    HarmonicAnalysis result;
    
    // 如果未提供基频，自动检测
    if (fundamental_freq <= 0.0) {
        fundamental_freq = detectFundamentalFrequency(input_data);
    }
    
    result.fundamental_freq = fundamental_freq;
    
    // 计算频谱
    computeFFT(input_data);
    computeMagnitudeSpectrum(nullptr, false); // 线性刻度用于THD计算
    computePhaseSpectrum();
    
    // 基波分析
    size_t fundamental_bin = frequencyToBin(fundamental_freq);
    if (fundamental_bin < magnitude_spectrum_.size()) {
        result.fundamental_magnitude = magnitude_spectrum_[fundamental_bin];
    }
    
    // 谐波分析
    double total_harmonic_power = 0.0;
    
    for (int h = 2; h <= max_harmonics + 1; ++h) {
        double harmonic_freq = fundamental_freq * h;
        size_t harmonic_bin = frequencyToBin(harmonic_freq);
        
        if (harmonic_bin < magnitude_spectrum_.size() && harmonic_freq <= getNyquistFrequency()) {
            result.harmonic_freqs.push_back(harmonic_freq);
            result.harmonic_mags.push_back(magnitude_spectrum_[harmonic_bin]);
            result.harmonic_phases.push_back(phase_spectrum_[harmonic_bin]);
            
            // 累积谐波功率（用于THD计算）
            if (h > 1) { // 排除基波
                total_harmonic_power += magnitude_spectrum_[harmonic_bin] * magnitude_spectrum_[harmonic_bin];
            }
        }
    }
    
    // 计算THD
    if (result.fundamental_magnitude > 0) {
        double fundamental_power = result.fundamental_magnitude * result.fundamental_magnitude;
        result.thd_percentage = 100.0 * std::sqrt(total_harmonic_power) / result.fundamental_magnitude;
        result.thd_db = DecibelUtils::powerToDb(total_harmonic_power, fundamental_power);
        
        // 估算SNR（简化计算）
        double noise_power = 0.0;
        size_t noise_samples = 0;
        for (size_t i = 10; i < magnitude_spectrum_.size() - 10; ++i) {
            bool is_harmonic = false;
            for (int h = 1; h <= max_harmonics + 1; ++h) {
                size_t harmonic_bin = frequencyToBin(fundamental_freq * h);
                if (std::abs(static_cast<int>(i) - static_cast<int>(harmonic_bin)) <= 2) {
                    is_harmonic = true;
                    break;
                }
            }
            if (!is_harmonic) {
                noise_power += magnitude_spectrum_[i] * magnitude_spectrum_[i];
                noise_samples++;
            }
        }
        
        if (noise_samples > 0) {
            noise_power /= noise_samples;
            result.snr_db = calculateSNR(fundamental_power, noise_power);
        }
    }
    
    return result;
}

std::pair<double, double> FFTProcessor::calculateTHD(const HarmonicAnalysis& harmonic_result) {
    return std::make_pair(harmonic_result.thd_percentage, harmonic_result.thd_db);
}

std::pair<double, double> FFTProcessor::calculateTHDFromSpectrum(const std::vector<double>& magnitude_spectrum,
                                                                double fundamental_freq,
                                                                int max_harmonics) {
    size_t fundamental_bin = frequencyToBin(fundamental_freq);
    
    if (fundamental_bin >= magnitude_spectrum.size()) {
        return std::make_pair(0.0, -std::numeric_limits<double>::infinity());
    }
    
    double fundamental_magnitude = magnitude_spectrum[fundamental_bin];
    double total_harmonic_power = 0.0;
    
    for (int h = 2; h <= max_harmonics + 1; ++h) {
        double harmonic_freq = fundamental_freq * h;
        size_t harmonic_bin = frequencyToBin(harmonic_freq);
        
        if (harmonic_bin < magnitude_spectrum.size() && harmonic_freq <= getNyquistFrequency()) {
            double harmonic_magnitude = magnitude_spectrum[harmonic_bin];
            total_harmonic_power += harmonic_magnitude * harmonic_magnitude;
        }
    }
    
    if (fundamental_magnitude > 0) {
        double fundamental_power = fundamental_magnitude * fundamental_magnitude;
        double thd_percentage = 100.0 * std::sqrt(total_harmonic_power) / fundamental_magnitude;
        double thd_db = DecibelUtils::powerToDb(total_harmonic_power, fundamental_power);
        
        return std::make_pair(thd_percentage, thd_db);
    }
    
    return std::make_pair(0.0, -std::numeric_limits<double>::infinity());
}

std::vector<SpectralPeak> FFTProcessor::findSpectralPeaks(const std::vector<double>* magnitude_spectrum,
                                                         size_t peak_count,
                                                         double min_magnitude,
                                                         double min_separation) {
    const std::vector<double>* spec_ptr = magnitude_spectrum ? magnitude_spectrum : &magnitude_spectrum_;
    
    // 查找所有局部最大值
    std::vector<size_t> peak_indices = findLocalMaxima(*spec_ptr, 
                                                      static_cast<size_t>(min_separation / frequency_resolution_),
                                                      min_magnitude);
    
    // 创建峰值对象并排序
    std::vector<SpectralPeak> peaks;
    peaks.reserve(peak_indices.size());
    
    for (size_t idx : peak_indices) {
        if (idx < spec_ptr->size()) {
            SpectralPeak peak;
            peak.bin_index = idx;
            peak.frequency = binToFrequency(idx);
            peak.magnitude = (*spec_ptr)[idx];
            peak.power = peak.magnitude * peak.magnitude;
            
            // 计算相位（如果可用）
            if (idx < phase_spectrum_.size()) {
                peak.phase = phase_spectrum_[idx];
            }
            
            peaks.push_back(peak);
        }
    }
    
    // 按幅度排序（降序）
    std::sort(peaks.begin(), peaks.end(), 
              [](const SpectralPeak& a, const SpectralPeak& b) {
                  return a.magnitude > b.magnitude;
              });
    
    // 返回指定数量的峰值
    if (peaks.size() > peak_count) {
        peaks.resize(peak_count);
    }
    
    return peaks;
}

double FFTProcessor::refinePeakFrequency(size_t bin_index, const std::vector<double>& magnitude_spectrum) {
    if (bin_index == 0 || bin_index >= magnitude_spectrum.size() - 1) {
        return binToFrequency(bin_index);
    }
    
    // 抛物线插值法精确估计峰值频率
    double y1 = magnitude_spectrum[bin_index - 1];
    double y2 = magnitude_spectrum[bin_index];
    double y3 = magnitude_spectrum[bin_index + 1];
    
    // 抛物线插值公式
    double delta = 0.5 * (y3 - y1) / (2.0 * y2 - y1 - y3);
    
    // 检查插值结果的合理性
    if (std::abs(delta) <= 0.5) {
        double refined_bin = static_cast<double>(bin_index) + delta;
        return refined_bin * frequency_resolution_;
    }
    
    return binToFrequency(bin_index);
}

void FFTProcessor::setWindowFunction(WindowType window_type, double beta) {
    window_type_ = window_type;
    window_func_ = generateWindow(window_type, fft_size_, beta);
}

double FFTProcessor::getWindowCoherentGain() const {
    double sum = 0.0;
    for (double w : window_func_) {
        sum += w;
    }
    return sum / static_cast<double>(window_func_.size());
}

size_t FFTProcessor::frequencyToBin(double frequency) const {
    size_t bin = static_cast<size_t>(std::round(frequency / frequency_resolution_));
    return std::min(bin, fft_size_ / 2);
}

double FFTProcessor::binToFrequency(size_t bin_index) const {
    return static_cast<double>(bin_index) * frequency_resolution_;
}

std::pair<double, double> FFTProcessor::getEffectiveFrequencyRange() const {
    return std::make_pair(frequency_resolution_, getNyquistFrequency());
}

bool FFTProcessor::reconfigure(size_t new_fft_size, double new_sample_rate) {
    if (!isPowerOfTwo(new_fft_size) || new_sample_rate <= 0) {
        return false;
    }
    
    fft_size_ = new_fft_size;
    sample_rate_ = new_sample_rate;
    frequency_resolution_ = sample_rate_ / static_cast<double>(fft_size_);
    
    // 重新分配缓冲区
    fft_buffer_.resize(fft_size_);
    magnitude_spectrum_.resize(fft_size_ / 2 + 1);
    phase_spectrum_.resize(fft_size_ / 2 + 1);
    power_spectrum_.resize(fft_size_ / 2 + 1);
    
    // 重新生成优化表
    generateBitReverseTable();
    generateTwiddleFactors();
    setWindowFunction(window_type_);
    
    return true;
}

std::string FFTProcessor::getConfigurationInfo() const {
    std::ostringstream oss;
    oss << "FFT Processor Configuration:\n";
    oss << "  FFT Size: " << fft_size_ << "\n";
    oss << "  Sample Rate: " << std::fixed << std::setprecision(2) << sample_rate_ << " Hz\n";
    oss << "  Frequency Resolution: " << std::fixed << std::setprecision(3) << frequency_resolution_ << " Hz/bin\n";
    oss << "  Nyquist Frequency: " << std::fixed << std::setprecision(2) << getNyquistFrequency() << " Hz\n";
    oss << "  Window Type: ";
    
    switch (window_type_) {
        case WindowType::RECTANGULAR: oss << "Rectangular"; break;
        case WindowType::HANNING: oss << "Hanning"; break;
        case WindowType::HAMMING: oss << "Hamming"; break;
        case WindowType::BLACKMAN: oss << "Blackman"; break;
        case WindowType::KAISER: oss << "Kaiser"; break;
        case WindowType::BARTLETT: oss << "Bartlett"; break;
    }
    oss << "\n";
    
    return oss.str();
}

void FFTProcessor::warmUp() {
    // 预热计算，分配内存并执行一次虚拟FFT
    std::vector<double> dummy_input(fft_size_, 0.0);
    computeFFT(dummy_input);
    computeMagnitudeSpectrum();
    computePhaseSpectrum();
    computePowerSpectralDensity();
}

void FFTProcessor::cleanup() {
    // 保留基本配置，清理缓冲区以节省内存
    std::vector<Complex>().swap(fft_buffer_);
    std::vector<double>().swap(magnitude_spectrum_);
    std::vector<double>().swap(phase_spectrum_);
    std::vector<double>().swap(power_spectrum_);
}

size_t FFTProcessor::getMemoryUsage() const {
    size_t usage = 0;
    usage += fft_buffer_.size() * sizeof(Complex);
    usage += magnitude_spectrum_.size() * sizeof(double);
    usage += phase_spectrum_.size() * sizeof(double);
    usage += power_spectrum_.size() * sizeof(double);
    usage += window_func_.size() * sizeof(double);
    usage += bit_reverse_table_.size() * sizeof(size_t);
    usage += twiddle_factors_.size() * sizeof(Complex);
    return usage;
}

// ==================== 私有方法实现 ====================

void FFTProcessor::bitReverseReorder(std::vector<Complex>& data) {
    for (size_t i = 0; i < fft_size_; ++i) {
        size_t j = bit_reverse_table_[i];
        if (i < j) {
            std::swap(data[i], data[j]);
        }
    }
}

void FFTProcessor::generateBitReverseTable() {
    bit_reverse_table_.resize(fft_size_);
    
    size_t log2n = 0;
    size_t temp = fft_size_;
    while (temp > 1) {
        temp >>= 1;
        log2n++;
    }
    
    for (size_t i = 0; i < fft_size_; ++i) {
        size_t reversed = 0;
        size_t temp_i = i;
        for (size_t bit = 0; bit < log2n; ++bit) {
            reversed = (reversed << 1) | (temp_i & 1);
            temp_i >>= 1;
        }
        bit_reverse_table_[i] = reversed;
    }
}

void FFTProcessor::generateTwiddleFactors() {
    twiddle_factors_.resize(fft_size_ / 2);
    
    for (size_t i = 0; i < fft_size_ / 2; ++i) {
        double angle = -2.0 * M_PI * static_cast<double>(i) / static_cast<double>(fft_size_);
        twiddle_factors_[i] = Complex(std::cos(angle), std::sin(angle));
    }
}

void FFTProcessor::computeFFTInPlace(std::vector<Complex>& data, bool inverse) {
    // 位反转重排
    bitReverseReorder(data);
    
    // Cooley-Tukey FFT算法
    for (size_t length = 2; length <= fft_size_; length *= 2) {
        size_t half_length = length / 2;
        
        for (size_t i = 0; i < fft_size_; i += length) {
            for (size_t j = 0; j < half_length; ++j) {
                size_t u_idx = i + j;
                size_t v_idx = i + j + half_length;
                
                // 计算旋转因子
                size_t twiddle_idx = (j * fft_size_) / length;
                Complex twiddle = twiddle_factors_[twiddle_idx];
                
                if (inverse) {
                    twiddle = std::conj(twiddle);
                }
                
                Complex u = data[u_idx];
                Complex v = data[v_idx] * twiddle;
                
                data[u_idx] = u + v;
                data[v_idx] = u - v;
            }
        }
    }
    
    // 逆变换需要归一化
    if (inverse) {
        double scale = 1.0 / static_cast<double>(fft_size_);
        for (Complex& val : data) {
            val *= scale;
        }
    }
}

std::vector<double> FFTProcessor::generateWindow(WindowType window_type, size_t size, double beta) {
    std::vector<double> window(size);
    
    switch (window_type) {
        case WindowType::RECTANGULAR:
            std::fill(window.begin(), window.end(), 1.0);
            break;
            
        case WindowType::HANNING:
            for (size_t i = 0; i < size; ++i) {
                window[i] = 0.5 * (1.0 - std::cos(2.0 * M_PI * static_cast<double>(i) / static_cast<double>(size - 1)));
            }
            break;
            
        case WindowType::HAMMING:
            for (size_t i = 0; i < size; ++i) {
                window[i] = 0.54 - 0.46 * std::cos(2.0 * M_PI * static_cast<double>(i) / static_cast<double>(size - 1));
            }
            break;
            
        case WindowType::BLACKMAN:
            for (size_t i = 0; i < size; ++i) {
                double factor = 2.0 * M_PI * static_cast<double>(i) / static_cast<double>(size - 1);
                window[i] = 0.42 - 0.5 * std::cos(factor) + 0.08 * std::cos(2.0 * factor);
            }
            break;
            
        case WindowType::KAISER:
            // 简化的凯泽窗实现
            for (size_t i = 0; i < size; ++i) {
                double x = 2.0 * static_cast<double>(i) / static_cast<double>(size - 1) - 1.0;
                double bessel_arg = beta * std::sqrt(1.0 - x * x);
                // 简化的修正贝塞尔函数I0近似
                double i0_beta = 1.0 + (bessel_arg * bessel_arg) / 4.0;
                double i0_beta_x = 1.0 + (beta * beta) / 4.0;
                window[i] = i0_beta / i0_beta_x;
            }
            break;
            
        case WindowType::BARTLETT:
            for (size_t i = 0; i < size; ++i) {
                double n = static_cast<double>(i);
                double N = static_cast<double>(size - 1);
                window[i] = 1.0 - std::abs((n - N / 2.0) / (N / 2.0));
            }
            break;
    }
    
    return window;
}

void FFTProcessor::applyWindow(const std::vector<double>& data, std::vector<double>& windowed_data) {
    size_t min_size = std::min(data.size(), window_func_.size());
    
    for (size_t i = 0; i < min_size; ++i) {
        windowed_data[i] = data[i] * window_func_[i];
    }
    
    // 剩余部分置零
    for (size_t i = min_size; i < windowed_data.size(); ++i) {
        windowed_data[i] = 0.0;
    }
}

bool FFTProcessor::isPowerOfTwo(size_t size) const {
    return size > 0 && (size & (size - 1)) == 0;
}

size_t FFTProcessor::autocorrelationPitchDetection(const std::vector<double>& data,
                                                   size_t min_period, size_t max_period) {
    size_t data_size = data.size();
    max_period = std::min(max_period, data_size / 2);
    
    double max_correlation = -1.0;
    size_t best_period = min_period;
    
    for (size_t period = min_period; period <= max_period; ++period) {
        double correlation = 0.0;
        double sum1 = 0.0, sum2 = 0.0;
        
        size_t samples = data_size - period;
        
        for (size_t i = 0; i < samples; ++i) {
            correlation += data[i] * data[i + period];
            sum1 += data[i] * data[i];
            sum2 += data[i + period] * data[i + period];
        }
        
        // 归一化相关系数
        if (sum1 > 0 && sum2 > 0) {
            correlation /= std::sqrt(sum1 * sum2);
            
            if (correlation > max_correlation) {
                max_correlation = correlation;
                best_period = period;
            }
        }
    }
    
    return best_period;
}

std::vector<size_t> FFTProcessor::findLocalMaxima(const std::vector<double>& data,
                                                  size_t min_peak_distance,
                                                  double min_peak_height) {
    std::vector<size_t> peaks;
    
    for (size_t i = min_peak_distance; i < data.size() - min_peak_distance; ++i) {
        if (data[i] < min_peak_height) continue;
        
        bool is_peak = true;
        
        // 检查是否为局部最大值
        for (size_t j = i - min_peak_distance; j <= i + min_peak_distance; ++j) {
            if (j != i && data[j] >= data[i]) {
                is_peak = false;
                break;
            }
        }
        
        if (is_peak) {
            peaks.push_back(i);
        }
    }
    
    return peaks;
}

double FFTProcessor::calculateSNR(double signal_power, double noise_power) {
    if (noise_power <= 0) {
        return std::numeric_limits<double>::infinity();
    }
    return 10.0 * std::log10(signal_power / noise_power);
}

// ==================== DecibelUtils 实现 ====================

double DecibelUtils::linearToDb(double linear_value, double reference) {
    if (linear_value <= 0 || reference <= 0) {
        return -std::numeric_limits<double>::infinity();
    }
    return 20.0 * std::log10(linear_value / reference);
}

double DecibelUtils::dbToLinear(double db_value, double reference) {
    return reference * std::pow(10.0, db_value / 20.0);
}

double DecibelUtils::powerToDb(double power, double reference_power) {
    if (power <= 0 || reference_power <= 0) {
        return -std::numeric_limits<double>::infinity();
    }
    return 10.0 * std::log10(power / reference_power);
}

// ==================== SignalUtils 实现 ====================

std::vector<double> SignalUtils::zeroPhaseBandpassFilter(const std::vector<double>& input_data,
                                                        double low_freq, double high_freq,
                                                        double sample_rate, int filter_order) {
    // 简化的零相位带通滤波器实现
    // 这里使用简单的FFT滤波方法
    
    size_t fft_size = 1;
    while (fft_size < input_data.size() * 2) {
        fft_size *= 2;
    }
    
    FFTProcessor fft_proc(fft_size, sample_rate);
    
    // 创建扩展输入数据
    std::vector<double> padded_data(fft_size, 0.0);
    for (size_t i = 0; i < input_data.size(); ++i) {
        padded_data[i] = input_data[i];
    }
    
    // FFT
    const auto& spectrum = fft_proc.computeFFT(padded_data);
    
    // 设计理想带通滤波器
    std::vector<Complex> filtered_spectrum = spectrum;
    double freq_resolution = sample_rate / static_cast<double>(fft_size);
    
    for (size_t i = 0; i < filtered_spectrum.size(); ++i) {
        double freq = static_cast<double>(i) * freq_resolution;
        if (freq < low_freq || freq > high_freq) {
            filtered_spectrum[i] = Complex(0.0, 0.0);
        }
    }
    
    // IFFT
    std::vector<double> filtered_data;
    fft_proc.computeIFFT(filtered_spectrum, filtered_data);
    
    // 提取原始长度的数据
    std::vector<double> result(input_data.size());
    for (size_t i = 0; i < input_data.size(); ++i) {
        result[i] = filtered_data[i];
    }
    
    return result;
}

std::vector<double> SignalUtils::detrend(const std::vector<double>& input_data, bool remove_linear) {
    std::vector<double> result = input_data;
    
    if (result.empty()) return result;
    
    // 去除直流分量
    double mean = 0.0;
    for (double val : result) {
        mean += val;
    }
    mean /= static_cast<double>(result.size());
    
    for (double& val : result) {
        val -= mean;
    }
    
    // 去除线性趋势
    if (remove_linear && result.size() > 2) {
        // 简单线性回归
        double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
        size_t n = result.size();
        
        for (size_t i = 0; i < n; ++i) {
            double x = static_cast<double>(i);
            double y = result[i];
            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_x2 += x * x;
        }
        
        double slope = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
        double intercept = (sum_y - slope * sum_x) / n;
        
        // 减去线性趋势
        for (size_t i = 0; i < n; ++i) {
            result[i] -= (slope * static_cast<double>(i) + intercept);
        }
    }
    
    return result;
}

std::vector<double> SignalUtils::normalize(const std::vector<double>& input_data, int method) {
    std::vector<double> result = input_data;
    
    if (result.empty()) return result;
    
    switch (method) {
        case 0: { // 最大值归一化
            double max_val = *std::max_element(result.begin(), result.end());
            double min_val = *std::min_element(result.begin(), result.end());
            double range = max_val - min_val;
            
            if (range > 0) {
                for (double& val : result) {
                    val = (val - min_val) / range;
                }
            }
            break;
        }
        
        case 1: { // 标准化（z-score）
            double mean = 0.0;
            for (double val : result) {
                mean += val;
            }
            mean /= static_cast<double>(result.size());
            
            double variance = 0.0;
            for (double val : result) {
                variance += (val - mean) * (val - mean);
            }
            variance /= static_cast<double>(result.size());
            double std_dev = std::sqrt(variance);
            
            if (std_dev > 0) {
                for (double& val : result) {
                    val = (val - mean) / std_dev;
                }
            }
            break;
        }
        
        case 2: { // 能量归一化
            double energy = 0.0;
            for (double val : result) {
                energy += val * val;
            }
            energy = std::sqrt(energy);
            
            if (energy > 0) {
                for (double& val : result) {
                    val /= energy;
                }
            }
            break;
        }
    }
    
    return result;
}

} // namespace DesignContest
