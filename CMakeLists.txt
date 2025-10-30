cmake_minimum_required(VERSION 3.14)

project(fpvcar-motor VERSION 1.0.0 LANGUAGES CXX)

# =============================================================================
# FPV motor CMakeLists.txt
# =============================================================================
#
# 构建模式说明：
# 1. 开发环境：cmake -DUSE_FETCHCONTENT=ON ..
#    - 自动下载 tl::expected 和 CLI11 依赖库
#    - 此时要求系统已安装 libgpiod (sudo apt install libgpiod-dev)
#    - 用于本地开发和测试
#
# 2. 生产环境（Yocto）：cmake ..
#    - 使用 Yocto sysroot 中的所有库
#    - 需要在 .bb 文件中添加:
#      DEPENDS += "tl-expected cli11 libgpiod-cxx"
#    - 用于嵌入式系统构建
#
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

# 查找 CMake 依赖发现工具
find_package(PkgConfig REQUIRED)

# --- 1. 条件化处理 C++/CMake 依赖 ---
# 检查是否启用 FetchContent（用于开发环境）
option(USE_FETCHCONTENT "Use FetchContent to download dependencies" OFF)

if(USE_FETCHCONTENT)
    # 开发环境：使用 FetchContent 下载依赖
    message(STATUS "Using FetchContent for tl::expected and CLI11")
    include(FetchContent)
    
    # tl::expected
    FetchContent_Declare(
        expected
        GIT_REPOSITORY https://github.com/TartanLlama/expected.git
        GIT_TAG v1.1.0
    )
    FetchContent_MakeAvailable(expected)
    add_library(TartanLlama::expected ALIAS expected)
else()
    # 生产/Yocto 环境：使用 Yocto sysroot 中的库
    message(STATUS "Using find_package for tl::expected and CLI11 (Yocto build)")
    find_package(expected REQUIRED)
endif()

# --- 2. 统一处理系统库 libgpiod (始终使用 PkgConfig) ---
message(STATUS "Locating system dependency: libgpiod (via pkg-config)")

# 查找 C++ 绑定 (libgpiodcxx.pc 会自动拉入 C 库 libgpiod)
# 无论是在 Yocto 还是本地开发，都依赖系统安装的 .pc 文件
pkg_check_modules(GPIODCXX REQUIRED libgpiodcxx)
pkg_check_modules(GPIOD REQUIRED libgpiod)

# 创建一个统一的 INTERFACE 目标，以保持链接目标名称一致
add_library(libgpiod::gpiodcxx INTERFACE IMPORTED)
set_property(TARGET libgpiod::gpiodcxx PROPERTY 
    INTERFACE_INCLUDE_DIRECTORIES ${GPIODCXX_INCLUDE_DIRS})
set_property(TARGET libgpiod::gpiodcxx PROPERTY 
    INTERFACE_LINK_LIBRARIES ${GPIODCXX_LIBRARIES})


# --- 3. 定义项目目标 ---

# 源文件（C++）
set(SOURCES
    src/main.cpp

    src/motor.cpp
    src/fpvcar_controller.cpp
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# --- 包含目录 ---
target_include_directories(${PROJECT_NAME} PRIVATE 
    include # 项目本地的头文件目录
)

# --- 统一的链接库 ---
target_link_libraries(${PROJECT_NAME} PRIVATE
    # 无论在哪种模式下，目标名称都保持一致
    TartanLlama::expected
    libgpiod::gpiodcxx
    ${GPIODCXX_LIBRARIES}
    ${GPIOD_LIBRARIES}
)

# --- 4. 安装规则 (Yocto必须) ---
# 这会将编译好的可执行文件安装到 Yocto 镜像的 /usr/bin 目录
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)