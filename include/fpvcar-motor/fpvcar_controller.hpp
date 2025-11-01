#pragma once
#include "fpvcar-motor/motor.hpp"
#include "config.hpp"
#include <gpiod.hpp>
#include <memory> // 用于 std::unique_ptr

/**
 * @brief 机器人控制器，管理所有电机
 */
namespace fpvcar { namespace control {

class FpvCarController {
public:
    /**
     * @brief 构造函数
     * @param chipName GPIO 芯片名称 (e.g., "gpiochip0")
     * @param config 包含所有 8 个引脚号的配置结构体
     * @param consumer 消费者名称
     */
    FpvCarController(const std::string& chipName, const fpvcar::config::FpvCarPinConfig& config, const std::string& consumer);

    /**
     * @brief 析构函数
     * 安全关闭：停止所有电机并将STBY引脚设置为低电平以禁用驱动器
     */
    ~FpvCarController();

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
    std::optional<gpiod::line_request> stby_request;  // STBY的line句柄
    int stby_pin_offset{};  // STBY引脚号，用于析构时安全关闭
    // 使用智能指针管理 Motor 对象
    std::unique_ptr<fpvcar::motor::Motor> motorFL; // 前左
    std::unique_ptr<fpvcar::motor::Motor> motorFR; // 前右
    std::unique_ptr<fpvcar::motor::Motor> motorBL; // 后左
    std::unique_ptr<fpvcar::motor::Motor> motorBR; // 后右

    // 确保STBY引脚为高电平（使能驱动器）
    void ensureStbyActive();
};

} } // namespace fpvcar::control