#pragma once
#include <string>

// GPIO 芯片名称，通常是 "gpiochip0"
constexpr const char* GPIO_CHIP_NAME = "gpiochip0";

// 标识谁在使用这些引脚
constexpr const char* GPIO_CONSUMER_NAME = "fpvcar-motor";

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
};