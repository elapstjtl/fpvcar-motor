#include "fpvcar-motor/motor.hpp"
#include "libpca9685_agnostic/pca9685.hpp"
#include <algorithm>

namespace fpvcar { namespace motor {

Motor::Motor(PCA9685& pwm, uint8_t channelSpeed, uint8_t channelA, uint8_t channelB)
    : _pwm(pwm), _channelSpeed(channelSpeed), _channelA(channelA), _channelB(channelB) {
    
    // 初始化时停止电机
    stop();
}

void Motor::forward(float speed_percent) {
    // 前进：速度通道输出PWM，方向A全开，方向B全关
    _pwm.set_speed(_channelSpeed, speed_percent);  // 速度PWM
    _pwm.set_speed(_channelA, 100.0F);               // 方向A全开（正转使能）
    _pwm.set_speed(_channelB, 0.0F);                 // 方向B全关
}

void Motor::reverse(float speed_percent) {
    // 后退：速度通道输出PWM，方向A全关，方向B全开
    _pwm.set_speed(_channelSpeed, speed_percent);  // 速度PWM
    _pwm.set_speed(_channelA, 0.0F);                 // 方向A全关
    _pwm.set_speed(_channelB, 100.0F);               // 方向B全开（反转使能）
}

void Motor::stop() {
    // 停止：速度通道输出0，方向A和B都全关
    _pwm.set_speed(_channelSpeed, 0.0F);  // 速度通道为0
    _pwm.set_speed(_channelA, 0.0F);       // 方向A全关
    _pwm.set_speed(_channelB, 0.0F);       // 方向B全关
}

} } // namespace fpvcar::motor
