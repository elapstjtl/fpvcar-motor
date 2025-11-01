#pragma once
#include <string>


namespace fpvcar {
namespace config {

// 存放四个电机引脚号的结构体
struct FpvCarPinConfig {
    // 前左 (Front-Left)
    int fl_pin_a = 17;
    int fl_pin_b = 27;
    // 前右 (Front-Right)
    int fr_pin_a = 22;
    int fr_pin_b = 23;   
    // 后左 (Back-Left)
    int bl_pin_a = 24;
    int bl_pin_b = 25;
    // 后右 (Back-Right)
    int br_pin_a = 5;
    int br_pin_b = 6;
    // STBY引脚，高电平有效
    int stby_pin = 26;
};

// 默认 GPIO 芯片名称，通常是 "gpiochip0"
inline constexpr const char* GPIO_CHIP_NAME = "/dev/gpiochip0";

// 默认消费者名称
inline constexpr const char* GPIO_CONSUMER_NAME = "fpvcar-motor";

// 默认引脚配置（编译期常量）
inline constexpr FpvCarPinConfig DEFAULT_PINS = {
    .fl_pin_a = 17,  .fl_pin_b = 27,
    .fr_pin_a = 22,  .fr_pin_b = 23,
    .bl_pin_a = 24,  .bl_pin_b = 25,
    .br_pin_a = 5,   .br_pin_b = 6,
    .stby_pin = 26
};

} // namespace config
} // namespace fpvcar