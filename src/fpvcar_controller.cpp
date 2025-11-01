#include "fpvcar-motor/fpvcar_controller.hpp"
#include <stdexcept> // 用于抛出异常

namespace fpvcar { namespace control {

FpvCarController::FpvCarController(const std::string& chipName, const fpvcar::config::FpvCarPinConfig& config, const std::string& consumer)
    : chip(chipName) {

    // 初始化STBY引脚为高电平  
    stby_pin_offset = config.stby_pin;
    if(stby_pin_offset < 0) {
        throw std::invalid_argument("STBY引脚号不能为负数");
    }
    auto builder = chip.prepare_request();
    builder.set_consumer(consumer);
    gpiod::line_settings settings;
    settings.set_direction(gpiod::line::direction::OUTPUT);
    builder.add_line_settings(static_cast<unsigned int>(stby_pin_offset), settings);
    stby_request.emplace(builder.do_request());
    stby_request->set_value(stby_pin_offset, gpiod::line::value::ACTIVE);



    // 初始化四个电机（请求对应的两路输出线）
    motorFL = std::make_unique<fpvcar::motor::Motor>(chip, config.fl_pin_a, config.fl_pin_b, consumer);
    motorFR = std::make_unique<fpvcar::motor::Motor>(chip, config.fr_pin_a, config.fr_pin_b, consumer);
    motorBL = std::make_unique<fpvcar::motor::Motor>(chip, config.bl_pin_a, config.bl_pin_b, consumer);
    motorBR = std::make_unique<fpvcar::motor::Motor>(chip, config.br_pin_a, config.br_pin_b, consumer);
}

FpvCarController::~FpvCarController() {
    // 安全关闭：停止所有电机
    stopAll();
    
    // 将STBY引脚设置为低电平以禁用驱动器
    if (stby_request.has_value() && stby_pin_offset >= 0) {
        stby_request->set_value(stby_pin_offset, gpiod::line::value::INACTIVE);
    }
    // stby_request 会通过 RAII 自动释放资源
}

void FpvCarController::ensureStbyActive() {
    // 确保STBY引脚为高电平，使能驱动器
    if (stby_request.has_value() && stby_pin_offset >= 0) {
        stby_request->set_value(stby_pin_offset, gpiod::line::value::ACTIVE);
    }
}

void FpvCarController::moveForward() {
    // 确保STBY引脚为高电平
    ensureStbyActive();
    // 假设所有电机都前进
    motorFL->forward();
    motorFR->forward();
    motorBL->forward();
    motorBR->forward();
}

void FpvCarController::moveBackward() {
    // 确保STBY引脚为高电平
    ensureStbyActive();
    motorFL->reverse();
    motorFR->reverse();
    motorBL->reverse();
    motorBR->reverse();
}

void FpvCarController::turnLeft() {
    // 确保STBY引脚为高电平
    ensureStbyActive();
    // 差速转向：左侧轮子前进，右侧轮子后退
    motorFL->forward();
    motorBL->forward();
    motorFR->reverse();
    motorBR->reverse();
}

void FpvCarController::turnRight() {
    // 确保STBY引脚为高电平
    ensureStbyActive();
    // 差速转向：左侧轮子后退，右侧轮子前进
    motorFL->reverse();
    motorBL->reverse();
    motorFR->forward();
    motorBR->forward();
}

void FpvCarController::stopAll() {
    motorFL->stop();
    motorFR->stop();
    motorBL->stop();
    motorBR->stop();
}

} } // namespace fpvcar::control