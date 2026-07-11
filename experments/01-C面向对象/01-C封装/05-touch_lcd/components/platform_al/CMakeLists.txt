# --- 最终修正版：强制指定源码路径 ---

# 1. 获取目标芯片
set(CURRENT_TARGET ${IDF_TARGET})

# 2. 【关键修正】
# 不要只用 CMAKE_CURRENT_SOURCE_DIR，因为它在某些 ESP-IDF 内部调用中可能指向 build 目录。
# 我们使用 COMPONENT_DIR 变量，这是 ESP-IDF 专门为组件提供的变量，永远指向组件的源码目录。
# 如果 COMPONENT_DIR 不可用，则回退到 CMAKE_CURRENT_SOURCE_DIR 的父目录逻辑。
if(DEFINED COMPONENT_DIR)
    set(HAL_BASE_DIR ${COMPONENT_DIR})
else()
    # 如果 COMPONENT_DIR 未定义，通常是因为我们在主组件中，或者上下文不同
    # 此时 CMAKE_CURRENT_SOURCE_DIR 应该是正确的，但为了保险，我们打印出来确认
    set(HAL_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# 3. 构造查找模式
# 使用 HAL_BASE_DIR 拼接路径
set(GPIO_PATTERN "${HAL_BASE_DIR}/hal_gpio/hal_gpio_${CURRENT_TARGET}.c")
set(LEDC_PATTERN "${HAL_BASE_DIR}/hal_ledc/hal_ledc_${CURRENT_TARGET}.c")
set(SPI_PATTERN  "${HAL_BASE_DIR}/hal_spi/hal_spi_${CURRENT_TARGET}.c")

# 4. 调试打印 (这能帮你确认路径到底指向哪里)
message(STATUS ">>> HAL CHECK: Base Dir is '${HAL_BASE_DIR}'")
message(STATUS ">>> HAL CHECK: Looking for GPIO at: ${GPIO_PATTERN}")

# 5. 查找文件
file(GLOB GPIO_SRCS ${GPIO_PATTERN})
file(GLOB LEDC_SRCS ${LEDC_PATTERN})
file(GLOB SPI_SRCS  ${SPI_PATTERN})

# 6. 合并列表
set(HAL_SRCS ${GPIO_SRCS} ${LEDC_SRCS} ${SPI_SRCS})

# 7. 最终检查
if(NOT HAL_SRCS)
    message(FATAL_ERROR "HAL Error: No source files found for target '${CURRENT_TARGET}'.\n"
                        "Searched in: ${HAL_BASE_DIR}\n"
                        "Patterns:\n"
                        "  ${GPIO_PATTERN}\n"
                        "  ${LEDC_PATTERN}\n"
                        "  ${SPI_PATTERN}")
endif()

# 8. 注册组件
idf_component_register(
    SRCS 
        ${HAL_SRCS}
    INCLUDE_DIRS 
        hal_gpio/include
        hal_ledc/include
        hal_spi/include
    REQUIRES 
        driver
)