# --- 修正版：针对 DAL 驱动层的构建脚本 ---

# 1. 获取目标芯片 (虽然这里没用到，但保留以防后续逻辑需要)
set(CURRENT_TARGET ${IDF_TARGET})

# 2. 确定基础路径
# 使用 COMPONENT_DIR 确保指向源码目录，而不是 build 目录
if(DEFINED COMPONENT_DIR)
    set(BASE_DIR ${COMPONENT_DIR})
else()
    set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()


set(DRIVER_SRCS
    "${BASE_DIR}/dal_display/ili9488.c"
    "${BASE_DIR}/dal_touch/xpt2046.c"
)

# 4. 检查文件是否存在（防御性编程）
foreach(SRC_FILE ${DRIVER_SRCS})
    if(NOT EXISTS ${SRC_FILE})
        message(FATAL_ERROR "Driver Error: Source file not found: ${SRC_FILE}")
    endif()
endforeach()

# 5. 打印调试信息
message(STATUS ">>> DAL CHECK: Building drivers from: ${BASE_DIR}")
message(STATUS ">>> DAL CHECK: Sources: ${DRIVER_SRCS}")

# 6. 注册组件
idf_component_register(
    SRCS 
        ${DRIVER_SRCS}
    INCLUDE_DIRS 
        dal_display/include  # 确保这个路径包含 ili9488.h
        dal_touch/include    # 确保这个路径包含 xpt2046.h

    REQUIRES 
        platform_al 
        osal 

)