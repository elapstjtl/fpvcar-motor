cmake_minimum_required(VERSION 3.14)

project(fpvcar-motor VERSION 1.0.0 LANGUAGES CXX)

# =============================================================================
# FPV motor CMakeLists.txt
# =============================================================================
#
# 构建模式说明：
# 1. 开发环境：cmake ..
#    - 使用本地 libs/expected git submodule
#    - 使用本地 libs/libpca9685_agnostic git submodule
#    - 用于本地开发和测试
#
# 2. 生产环境（Yocto）：cmake ..
#    - 使用 Yocto sysroot 中的所有库
#    - 需要在 .bb 文件中添加:
#      DEPENDS += "tl-expected"
#    - 用于嵌入式系统构建
#
# 注意：本项目现在使用PCA9685模块控制电机，不再依赖libgpiod
# 注意：所有依赖库都作为 git submodule 位于 libs/ 目录
# =============================================================================

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置调试信息路径映射，避免 Yocto 构建时的 buildpaths 警告
if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=/usr/src/debug/fpvcar-motor/1.0")
endif()

# Yocto构建优化：设置适当的编译标志
if(CMAKE_CROSSCOMPILING)
    # 交叉编译时的优化设置
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
    message(STATUS "Cross-compiling detected, using optimized flags for Yocto")
endif()

# --- 1. 添加依赖库作为子目录（git submodules） ---
message(STATUS "Adding libs/expected as subdirectory (git submodule)")
add_subdirectory(libs/expected)

message(STATUS "Adding libs/libpca9685_agnostic as subdirectory (git submodule)")
add_subdirectory(libs/libpca9685_agnostic)


# --- 3. 定义项目目标 ---

# 库源文件（供外部程序链接）
set(LIB_SOURCES
    src/motor.cpp
    src/fpvcar_controller.cpp
)

# 创建库目标
add_library(fpvcar_motor ${LIB_SOURCES})
add_library(fpvcar::motor ALIAS fpvcar_motor)

# 对外头文件
target_include_directories(fpvcar_motor
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# 链接库依赖（公共）
target_link_libraries(fpvcar_motor
    PUBLIC
        tl::expected
        pca9685_agnostic
)

# --- 4. 默认构建测试可执行（使用库） ---
add_executable(fpvcar-motor-test
    test/test.cpp
)
target_link_libraries(fpvcar-motor-test PRIVATE fpvcar_motor)