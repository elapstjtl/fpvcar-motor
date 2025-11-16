#include "fpvcar-motor/fpvcar_controller.hpp"
#include "libpca9685_agnostic/pca9685.hpp"
#include "platform/linux/linux_i2c_bus.hpp"
#include "platform/linux/linux_delay.hpp"
#include <stdexcept>

namespace fpvcar { namespace control {

FpvCarController::FpvCarController(const std::string& i2c_device_path,
                                const fpvcar::motorconfig::FpvCarChannelConfig& config,
                                   float pwm_frequency,
                                   uint8_t pca9685_address) {

    // 步骤1：创建并初始化Linux I2C总线
    _i2c_bus = std::make_unique<LinuxI2CBus>(i2c_device_path);
    if (!_i2c_bus->is_valid()) {
        throw std::runtime_error("无法打开I2C设备: " + i2c_device_path + "。请检查接线和权限。");
    }

    // 步骤2：创建延迟接口对象
    _delay = std::make_unique<LinuxDelay>();

    // 步骤3：创建PCA9685驱动实例（构造函数自动初始化）
    _pwm = std::make_unique<PCA9685>(*_i2c_bus, *_delay, pca9685_address, true);

    // 步骤4：检查初始化状态
    if (!_pwm->is_initialized()) {
        throw std::runtime_error("PCA9685初始化失败。请检查I2C连接和设备地址。");
    }

    // 步骤5：设置PWM频率
    if (!_pwm->set_pwm_frequency(pwm_frequency)) {
        throw std::runtime_error("设置PWM频率失败: " + std::to_string(pwm_frequency) + " Hz");
    }

    // 步骤6：初始化四个电机（每个电机使用3个通道：速度、方向A、方向B）
    motorFL = std::make_unique<fpvcar::motor::Motor>(*_pwm, config.fl_channel_speed, config.fl_channel_1, config.fl_channel_2);
    motorFR = std::make_unique<fpvcar::motor::Motor>(*_pwm, config.fr_channel_speed, config.fr_channel_1, config.fr_channel_2);
    motorBL = std::make_unique<fpvcar::motor::Motor>(*_pwm, config.bl_channel_speed, config.bl_channel_1, config.bl_channel_2);
    motorBR = std::make_unique<fpvcar::motor::Motor>(*_pwm, config.br_channel_speed, config.br_channel_1, config.br_channel_2);
}

FpvCarController::~FpvCarController() {
    // 安全关闭：停止所有电机
    // 注意：析构函数中获取锁是安全的，因为此时对象正在被销毁
    // 但调用者应确保没有其他线程正在使用此对象
    std::lock_guard<std::mutex> lock(m_hw_mutex);

    // 停止所有电机
    if (motorFL) motorFL->stop();
    if (motorFR) motorFR->stop();
    if (motorBL) motorBL->stop();
    if (motorBR) motorBR->stop();
}

void FpvCarController::moveForward() {
    std::lock_guard<std::mutex> lock(m_hw_mutex);
    // 所有电机都前进
    motorFL->forward(SPEED_NORMAL);
    motorFR->forward(SPEED_NORMAL);
    motorBL->forward(SPEED_NORMAL);
    motorBR->forward(SPEED_NORMAL);
}

void FpvCarController::moveBackward() {
    std::lock_guard<std::mutex> lock(m_hw_mutex);
    motorFL->reverse(SPEED_NORMAL);
    motorFR->reverse(SPEED_NORMAL);
    motorBL->reverse(SPEED_NORMAL);
    motorBR->reverse(SPEED_NORMAL);
}

void FpvCarController::turnLeft() {
    std::lock_guard<std::mutex> lock(m_hw_mutex);
    // 差速转向：右侧轮子前进，左侧轮子后退
    motorFR->forward(SPEED_NORMAL);
    motorBR->forward(SPEED_NORMAL);
    motorFL->reverse(SPEED_NORMAL);
    motorBL->reverse(SPEED_NORMAL);
}

void FpvCarController::turnRight() {
    std::lock_guard<std::mutex> lock(m_hw_mutex);
    // 差速转向：左侧轮子前进，右侧轮子后退

    motorFL->forward(SPEED_NORMAL); 
    motorBL->forward(SPEED_NORMAL);
    motorFR->reverse(SPEED_NORMAL); 
    motorBR->reverse(SPEED_NORMAL); 
}
void FpvCarController::stopAll() {
    std::lock_guard<std::mutex> lock(m_hw_mutex);
    motorFL->stop();
    motorFR->stop();
    motorBL->stop();
    motorBR->stop();
}

void FpvCarController::moveForwardAndTurnLeft() {
    std::lock_guard<std::mutex> lock(m_hw_mutex);
    // 前进并左转：左侧（内侧）25%速度，右侧（外侧）50%速度
    motorFL->forward(SPEED_TURN_INNER);  // 前左：内侧
    motorBL->forward(SPEED_TURN_INNER);  // 后左：内侧
    motorFR->forward(SPEED_NORMAL);  // 前右：外侧
    motorBR->forward(SPEED_NORMAL);  // 后右：外侧
}

void FpvCarController::moveForwardAndTurnRight() {
    std::lock_guard<std::mutex> lock(m_hw_mutex);
    // 前进并右转：右侧（内侧）25%速度，左侧（外侧）50%速度
    motorFL->forward(SPEED_NORMAL);  // 前左：外侧
    motorBL->forward(SPEED_NORMAL);  // 后左：外侧
    motorFR->forward(SPEED_TURN_INNER);  // 前右：内侧
    motorBR->forward(SPEED_TURN_INNER);  // 后右：内侧
}

void FpvCarController::moveBackwardAndTurnLeft() {
    std::lock_guard<std::mutex> lock(m_hw_mutex);
    // 后退并左转：左侧（内侧）25%速度，右侧（外侧）50%速度
    motorFL->reverse(SPEED_TURN_INNER);  // 前左：内侧
    motorBL->reverse(SPEED_TURN_INNER);  // 后左：内侧
    motorFR->reverse(SPEED_NORMAL);  // 前右：外侧
    motorBR->reverse(SPEED_NORMAL);  // 后右：外侧
}

void FpvCarController::moveBackwardAndTurnRight() {
    std::lock_guard<std::mutex> lock(m_hw_mutex);
    // 后退并右转：右侧（内侧）25%速度，左侧（外侧）50%速度
    motorFL->reverse(SPEED_NORMAL);  // 前左：外侧
    motorBL->reverse(SPEED_NORMAL);  // 后左：外侧
    motorFR->reverse(SPEED_TURN_INNER);  // 前右：内侧
    motorBR->reverse(SPEED_TURN_INNER);  // 后右：内侧
}

} } // namespace fpvcar::control
