cmake_minimum_required(VERSION 3.10)
project(shape_oop C)

# 设置C标准（推荐C11以支持匿名结构体等特性）
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 添加可执行目标，包含所有源文件
add_executable(shape_demo
    main.c
    base_class.c
    derived_class.c
)

# 将当前目录加入头文件搜索路径
target_include_directories(shape_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 开启常用警告，帮助发现OOP模式中的潜在问题
target_compile_options(shape_demo PRIVATE -Wall -Wextra -pedantic)