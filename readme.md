## fpvcar-motor 库使用指南

本项目提供电机控制库目标 `fpvcar_motor`（别名 `fpvcar::motor`），可被其他 CMake 工程直接链接使用。

### 依赖
- 必需依赖：`tl::expected`、`libgpiodcxx`（会间接链接 `libgpiod`）
- 开发模式可启用 `-DUSE_FETCHCONTENT=ON` 自动拉取 `tl::expected`；`libgpiodcxx` 需系统安装对应 pkg-config 包。
- 生产/Yocto 环境：由 Yocto 配方提供依赖与安装，不在本工程内执行安装。

### 构建本项目
```bash
cd ~/jtl/fpvcar/fpvcar-motor/
rm -rf build/
mkdir build && cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=../toolchains/rpi4_64.toolchain.cmake -DUSE_FETCHCONTENT=ON ..

```
构建后会生成库 `fpvcar_motor` 以及默认测试可执行 `fpvcar-motor-test`。

### 在其他程序中使用

#### 同一源码树中使用（推荐用于本地开发）
在你的上层 CMake 工程中将本项目作为子目录引入，并链接库：
```cmake
add_subdirectory(external/fpvcar-motor)

add_executable(my_app src/main.cpp)
target_link_libraries(my_app PRIVATE fpvcar::motor)
```
注意：确保包含头文件目录来自库的 PUBLIC 包含，正常无需额外添加 `include` 路径。

### 测试程序
编译后可运行测试程序（会尝试操作 GPIO）：
```bash
./build/fpvcar-motor-test
```


