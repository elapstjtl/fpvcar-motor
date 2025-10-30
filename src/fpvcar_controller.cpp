#include "fpvcar_controller.hpp"
#include <stdexcept> // 用于抛出异常

FpvCarController::FpvCarController(const std::string& chipName, const FpvCarPinConfig& config, const std::string& consumer)
    : chip(chipName) {

    // 初始化四个电机（请求对应的两路输出线）
    motorFL = std::make_unique<Motor>(chip, config.fl_pin_a, config.fl_pin_b, consumer);
    motorFR = std::make_unique<Motor>(chip, config.fr_pin_a, config.fr_pin_b, consumer);
    motorBL = std::make_unique<Motor>(chip, config.bl_pin_a, config.bl_pin_b, consumer);
    motorBR = std::make_unique<Motor>(chip, config.br_pin_a, config.br_pin_b, consumer);
}

void FpvCarController::moveForward() {
    // 假设所有电机都前进
    motorFL->forward();
    motorFR->forward();
    motorBL->forward();
    motorBR->forward();
}

void FpvCarController::moveBackward() {
    motorFL->reverse();
    motorFR->reverse();
    motorBL->reverse();
    motorBR->reverse();
}

void FpvCarController::turnLeft() {
    // 差速转向：左侧轮子后退，右侧轮子前进
    motorFL->reverse();
    motorBL->reverse();
    motorFR->forward();
    motorBR->forward();
}

void FpvCarController::turnRight() {
    // 差速转向：左侧轮子前进，右侧轮子后退
    motorFL->forward();
    motorBL->forward();
    motorFR->reverse();
    motorBR->reverse();
}

void FpvCarController::stopAll() {
    motorFL->stop();
    motorFR->stop();
    motorBL->stop();
    motorBR->stop();
}