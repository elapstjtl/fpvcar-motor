#pragma once
#include <cstdint>
#include <string>

// 前向声明（避免循环依赖）
// PCA9685类的完整定义在libpca9685_agnostic/pca9685.hpp中
class PCA9685;

/**
 * @brief 控制单个直流电机（基于PCA9685的3通道H桥控制）
 * 使用独立的PWM速度通道和两个方向通道控制电机
 */
namespace fpvcar { namespace motor {

class Motor {
public:
    /**
     * @brief 构造函数
     * @param pwm PCA9685对象引用（必须保持有效直到对象销毁）
     * @param channelSpeed PWM速度通道（0-100%占空比控制速度）
     * @param channelA 方向A通道（正转使能，全开/全关）
     * @param channelB 方向B通道（反转使能，全开/全关）
     * @note 使用引用而非指针，确保PCA9685对象必须存在
     * @note PCA9685对象的生命周期必须长于Motor对象
     */
    Motor(PCA9685& pwm, uint8_t channelSpeed, uint8_t channelA, uint8_t channelB);

    ~Motor() = default;

    // 禁止拷贝和赋值
    Motor(const Motor&) = delete;
    Motor& operator=(const Motor&) = delete;

    // 控制电机运动
    void forward(float speed_percent);  // 前进（速度通道输出PWM，方向A全开，方向B全关）
    void reverse(float speed_percent);  // 后退（速度通道输出PWM，方向A全关，方向B全开）
    void stop();     // 停止（速度通道输出0，方向A和B都全关）

private:
    PCA9685& _pwm;            // PCA9685对象引用
    uint8_t _channelSpeed;    // PWM速度通道
    uint8_t _channelA;        // 方向A通道（正转使能）
    uint8_t _channelB;        // 方向B通道（反转使能）
};

} } // namespace fpvcar::motor
