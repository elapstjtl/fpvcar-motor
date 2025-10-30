#pragma once
#include "motor.hpp"
#include "config.hpp"
#include <gpiod.hpp>
#include <memory> // 用于 std::unique_ptr

/**
 * @brief 机器人控制器，管理所有电机
 */
class FpvCarController {
public:
    /**
     * @brief 构造函数
     * @param chipName GPIO 芯片名称 (e.g., "gpiochip0")
     * @param config 包含所有 8 个引脚号的配置结构体
     * @param consumer 消费者名称
     */
    FpvCarController(const std::string& chipName, const FpvCarPinConfig& config, const std::string& consumer);

    // 默认析构函数，RAII 将自动处理 chip 和所有 motor
    ~FpvCarController() = default;

    // 禁止拷贝和赋值
    FpvCarController(const FpvCarController&) = delete;
    FpvCarController& operator=(const FpvCarController&) = delete;

    // 高级运动指令
    void moveForward();
    void moveBackward();
    void turnLeft();
    void turnRight();
    void stopAll();

private:
    gpiod::chip chip; // GPIO 芯片对象 (RAII)
    
    // 使用智能指针管理 Motor 对象
    std::unique_ptr<Motor> motorFL; // 前左
    std::unique_ptr<Motor> motorFR; // 前右
    std::unique_ptr<Motor> motorBL; // 后左
    std::unique_ptr<Motor> motorBR; // 后右
};