#pragma once
#include "fpvcar-motor/motor.hpp"
#include "config.hpp"
#include <memory> // 用于 std::unique_ptr
#include <mutex>
#include <string>
#include <cstdint>  // 确保uint8_t可用

// 前向声明
class PCA9685;
class LinuxI2CBus;
class LinuxDelay;

/**
 * @brief 机器人控制器，管理所有电机（基于PCA9685）
 */
namespace fpvcar { namespace control {

class FpvCarController {
public:
    /**
     * @brief 构造函数（使用PCA9685）
     * @param i2c_device_path I2C设备路径，例如 "/dev/i2c-1"
     * @param config 包含所有通道号的配置结构体
     * @param pwm_frequency PWM频率（Hz），默认1000Hz适用于电机控制
     * @param pca9685_address PCA9685的I2C地址，默认0x40
     * @note 构造函数会自动初始化I2C总线和PCA9685芯片
     * @note 如果初始化失败，会抛出std::runtime_error异常
     */
    FpvCarController(const std::string& i2c_device_path,
                     const fpvcar::motorconfig::FpvCarChannelConfig& config,
                     float pwm_frequency = fpvcar::motorconfig::DEFAULT_PWM_FREQUENCY,
                     uint8_t pca9685_address = fpvcar::motorconfig::PCA9685_I2C_ADDRESS);

    /**
     * @brief 析构函数
     * 安全关闭：停止所有电机
     */
    ~FpvCarController();

    // 禁止拷贝和赋值
    FpvCarController(const FpvCarController&) = delete;
    FpvCarController& operator=(const FpvCarController&) = delete;

    // 高级运动指令
    void moveForward();
    void moveBackward();
    // 原地转向函数
    void turnLeft();
    void turnRight();
    // 边移动边转向函数
    void moveForwardAndTurnLeft();
    void moveForwardAndTurnRight();
    void moveBackwardAndTurnLeft();
    void moveBackwardAndTurnRight();
    // 停止所有电机
    void stopAll();

private:
    static constexpr float SPEED_NORMAL = 50.0F; // 基础速度 (外侧)
    static constexpr float SPEED_TURN_INNER = 25.0F; // 转向时内侧速度

    std::mutex m_hw_mutex; // 硬件互斥锁

    // PCA9685相关对象（生命周期管理）
    std::unique_ptr<LinuxI2CBus> _i2c_bus;
    std::unique_ptr<LinuxDelay> _delay;
    std::unique_ptr<PCA9685> _pwm;

    // 使用智能指针管理 Motor 对象
    std::unique_ptr<fpvcar::motor::Motor> motorFL; // 前左
    std::unique_ptr<fpvcar::motor::Motor> motorFR; // 前右
    std::unique_ptr<fpvcar::motor::Motor> motorBL; // 后左
    std::unique_ptr<fpvcar::motor::Motor> motorBR; // 后右
};

} } // namespace fpvcar::control
