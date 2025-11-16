#pragma once
#include <string>
#include <cstdint>

namespace fpvcar {
namespace motorconfig {

// 存放四个电机PCA9685通道号的结构体
// 每个电机使用3个通道：PWM速度通道、方向A通道、方向B通道
struct FpvCarChannelConfig {
    // 前左 (Front-Left) motorA
    uint8_t fl_channel_speed;  // PWM速度通道
    uint8_t fl_channel_1;       // 方向A通道（正转使能）
    uint8_t fl_channel_2;       // 方向B通道（反转使能）
    // 前右 (Front-Right) motorB
    uint8_t fr_channel_speed;
    uint8_t fr_channel_1;
    uint8_t fr_channel_2;   
    // 后左 (Back-Left) motorC
    uint8_t bl_channel_speed;
    uint8_t bl_channel_1;
    uint8_t bl_channel_2;
    // 后右 (Back-Right) motorD
    uint8_t br_channel_speed;
    uint8_t br_channel_1;
    uint8_t br_channel_2;
};

// 默认 I2C 设备路径
inline constexpr const char* I2C_DEVICE_PATH = "/dev/i2c-1";

// 默认 PCA9685 I2C 地址
inline constexpr uint8_t PCA9685_I2C_ADDRESS = 0x40;

// 默认 PWM 频率（10kHz），适用于电机控制
inline constexpr float DEFAULT_PWM_FREQUENCY = 10000.0F;

// 默认通道配置
inline constexpr FpvCarChannelConfig DEFAULT_CHANNELS = {
    .fl_channel_speed = 12,  .fl_channel_1 = 0,  .fl_channel_2 = 1,
    .fr_channel_speed = 13,  .fr_channel_1 = 2,  .fr_channel_2 = 3,
    .bl_channel_speed = 14,  .bl_channel_1 = 4,  .bl_channel_2 = 5,
    .br_channel_speed = 15,  .br_channel_1 = 6,  .br_channel_2 = 7,
};

} // namespace config
} // namespace fpvcar