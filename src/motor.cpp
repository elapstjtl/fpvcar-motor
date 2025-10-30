#include "motor.hpp"

Motor::Motor(gpiod::chip& chip, int pinA, int pinB, const std::string& consumer) {
    // 记录 offset，便于后续按 offset 设值
    pinAOffset = pinA;
    pinBOffset = pinB;

    if (pinAOffset < 0 || pinBOffset < 0) {
        throw std::invalid_argument("引脚号不能为负数");
    }
    // v2：使用请求构造器统一请求多条线为输出
    auto builder = chip.prepare_request();
    builder.set_consumer(consumer);
    gpiod::line_settings settings; // 创建一个设置对象
    settings.set_direction(gpiod::line::direction::OUTPUT); // 设置为输出
    // 请求引脚控制权，并设置为输出
    builder.add_line_settings(static_cast<unsigned int>(pinAOffset), settings);
    builder.add_line_settings(static_cast<unsigned int>(pinBOffset), settings);

    // 获取可操作的请求句柄（RAII）
    request.emplace(builder.do_request());

    // 默认输出低电平（停止）
    request->set_value(pinAOffset, gpiod::line::value::INACTIVE);
    request->set_value(pinBOffset, gpiod::line::value::INACTIVE);
}

void Motor::forward() {
    // 假设 A=1, B=0 为前进
    request->set_value(pinAOffset, gpiod::line::value::ACTIVE);
    request->set_value(pinBOffset, gpiod::line::value::INACTIVE);
}

void Motor::reverse() {
    // 假设 A=0, B=1 为后退
    request->set_value(pinAOffset, gpiod::line::value::INACTIVE);
    request->set_value(pinBOffset, gpiod::line::value::ACTIVE);
}

void Motor::stop() {
    // 假设 A=0, B=0 为停止 (低电平刹车)
    // 根据您的 H 桥模块，A=1, B=1 可能也是刹车
    request->set_value(pinAOffset, gpiod::line::value::INACTIVE);
    request->set_value(pinBOffset, gpiod::line::value::INACTIVE);
}