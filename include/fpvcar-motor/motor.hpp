#pragma once
#include <gpiod.hpp> // 引入 libgpiod v2 C++ API
#include <string>
#include <vector>
#include <optional>

/**
 * @brief 控制单个直流电机（基于2-pin）
 * 使用 libgpiod v2：通过一次 lines 请求获得句柄，然后按 offset 设值
 */
namespace fpvcar { namespace motor {

class Motor {
public:
    /**
     * @brief 构造函数（v2 API）
     * @param chip 已打开的 gpiod::chip 对象引用
     * @param pinA H桥的 IN1 引脚号（offset）
     * @param pinB H桥的 IN2 引脚号（offset）
     * @param consumer 消费者名称
     */
    Motor(gpiod::chip& chip, int pinA, int pinB, const std::string& consumer);

    ~Motor() = default;

    // 禁止拷贝和赋值，因为 GPIO 资源是唯一的
    Motor(const Motor&) = delete;
    Motor& operator=(const Motor&) = delete;

    // 控制电机运动
    void forward();  // 前进 
    void reverse();  // 后退 
    void stop();     // 停止/刹车

private:
    // v2 中通过 line_request 统一管理多个引脚
    std::optional<gpiod::line_request> request; // 已请求的 lines 句柄（RAII 自动释放）
    int pinAOffset{};
    int pinBOffset{};
};

} } // namespace fpvcar::motor