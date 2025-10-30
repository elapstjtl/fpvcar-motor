#include "fpvcar_controller.hpp"
#include "config.hpp"
#include <iostream>
#include <chrono>   // 用于 std::chrono
#include <thread>   // 用于 std::this_thread::sleep_for
#include <stdexcept>

int main() {
    // 1. 定义引脚配置
    FpvCarPinConfig myPins = {
        .fl_pin_a = 4,  .fl_pin_b = 17, // 示例引脚
        .fr_pin_a = 27, .fr_pin_b = 22, // 示例引脚
        .bl_pin_a = 10, .bl_pin_b = 9,  // 示例引脚
        .br_pin_a = 11, .br_pin_b = 5   // 示例引脚
    };

    try {
        // 2. 初始化 FpvCarController
        // 构造函数会负责打开芯片和请求所有8个引脚
        FpvCarController fpvcar(GPIO_CHIP_NAME, myPins, GPIO_CONSUMER_NAME);

        std::cout << "fpvcar 初始化成功！" << std::endl;

        // 3. 执行运动序列
        std::cout << "向前移动 2 秒..." << std::endl;
        fpvcar.moveForward();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "左转 1 秒..." << std::endl;
        fpvcar.turnLeft();
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "停止所有电机。" << std::endl;
        fpvcar.stopAll();

    } catch (const std::exception& e) {
        // 捕获初始化或运行时可能发生的错误
        std::cerr << "发生错误: " << e.what() << std::endl;
        return 1;
    }

    // 4. 程序结束
    // 当 'fpvcar' 对象离开作用域时，其析构函数被调用
    // 这将自动触发所有 unique_ptr<Motor> 和 gpiod::chip 的析构函数
    // 最终导致所有 GPIO 引脚被释放，GPIO 芯片被关闭 (RAII)
    std::cout << "程序退出，资源已自动释放。" << std::endl;
    return 0;
}