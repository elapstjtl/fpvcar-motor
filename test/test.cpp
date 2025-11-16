#include "fpvcar-motor/fpvcar_controller.hpp"
#include "fpvcar-motor/config.hpp"
#include <iostream>
#include <chrono>   // 用于 std::chrono
#include <thread>   // 用于 std::this_thread::sleep_for
#include <stdexcept>

int main() {
    // 1. 定义通道配置
    // 使用默认通道配置（每个电机使用3个通道：速度、方向A、方向B，共12个通道）
    try {
        // 2. 初始化 FpvCarController（使用PCA9685）
        // 构造函数会负责初始化I2C总线和PCA9685芯片
        fpvcar::control::FpvCarController fpvcar(
            fpvcar::motorconfig::I2C_DEVICE_PATH,           // I2C设备路径
            fpvcar::motorconfig::DEFAULT_CHANNELS,          // 通道配置
            fpvcar::motorconfig::DEFAULT_PWM_FREQUENCY,     // PWM频率（1000Hz）
            fpvcar::motorconfig::PCA9685_I2C_ADDRESS        // PCA9685 I2C地址（0x40）
        );

        std::cout << "fpvcar 初始化成功！" << std::endl;
        std::cout << "使用PCA9685控制电机，I2C设备: " << fpvcar::motorconfig::I2C_DEVICE_PATH << std::endl;
        std::cout << "PWM频率: " << fpvcar::motorconfig::DEFAULT_PWM_FREQUENCY << " Hz" << std::endl;

        // 3. 执行运动序列
        std::cout << "\n向前移动 1 秒..." << std::endl;
        fpvcar.moveForward();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fpvcar.stopAll();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "向后移动 1 秒..." << std::endl;
        fpvcar.moveBackward();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fpvcar.stopAll();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "左转 1 秒..." << std::endl;
        fpvcar.turnLeft();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fpvcar.stopAll();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "右转 1 秒..." << std::endl;
        fpvcar.turnRight();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fpvcar.stopAll();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "前进并左转 3 秒..." << std::endl;
        fpvcar.moveForwardAndTurnLeft();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        fpvcar.stopAll();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "前进并右转 3 秒..." << std::endl;
        fpvcar.moveForwardAndTurnRight();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        fpvcar.stopAll();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "后退并左转 3 秒..." << std::endl;
        fpvcar.moveBackwardAndTurnLeft();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        fpvcar.stopAll();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "后退并右转 3 秒..." << std::endl;
        fpvcar.moveBackwardAndTurnRight();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        fpvcar.stopAll();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "\n停止所有电机。" << std::endl;
        fpvcar.stopAll();

    } catch (const std::exception& e) {
        // 捕获初始化或运行时可能发生的错误
        std::cerr << "发生错误: " << e.what() << std::endl;
        std::cerr << "请检查：" << std::endl;
        std::cerr << "  1. I2C设备是否存在: " << fpvcar::motorconfig::I2C_DEVICE_PATH << std::endl;
        std::cerr << "  2. 是否有I2C访问权限（需要root或i2c组权限）" << std::endl;
        std::cerr << "  3. PCA9685模块是否正确连接" << std::endl;
        std::cerr << "  4. PCA9685 I2C地址是否正确: 0x" << std::hex 
                  << static_cast<int>(fpvcar::motorconfig::PCA9685_I2C_ADDRESS) << std::dec << std::endl;
        return 1;
    }

    // 4. 程序结束
    // 当 'fpvcar' 对象离开作用域时，其析构函数被调用
    // 这将自动触发所有资源的释放（RAII）
    std::cout << "\n程序退出，资源已自动释放。" << std::endl;
    return 0;
}
