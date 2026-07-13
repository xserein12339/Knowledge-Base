/**
 * @file    singleton_pattern1.c
 * @brief   实现UART外设懒汉式单例模式实现文件
 *
 * @details 
 *
 * @author  xserein
 * @version 1.0
 */

#include "uart_typedef.h"
#include "singleton_pattern2.h"

#define LOG_I(fmt, ...) \
    printf(fmt "\n", ##__VA_ARGS__)

/**
 * @brief UART句柄
 */
typedef struct uart_t {
    UART_TypeDef *regs;
    uart_config_t cfg;
    bool is_created;
    bool is_inited;
} uart_t;

/* 实例池 */
static uart_t g_instances[UART_INSTANCE_MAX];

/**
 * @brief 根据实例ID获取UART寄存器基地址
 * 
 * @param id UART实例编号 
 * 
 */
UART_TypeDef* uart_get_regs(uint8_t id)
{
    if (id >= UART_INSTANCE_MAX) {
        return NULL;
    }
    /* 返回对应mock regs */
    static UART_TypeDef g_mock_uarts[UART_INSTANCE_MAX] = {
        [UART_INSTANCE_1] = { .SR = UART_SR_TXE_Msk, .DR = 0 },
        [UART_INSTANCE_2] = { .SR = UART_SR_TXE_Msk, .DR = 0 },
        [UART_INSTANCE_3] = { .SR = UART_SR_TXE_Msk, .DR = 0 },
    };
    return &g_mock_uarts[id];
}

uint8_t uart_get_instance(uart_instance_t id, uart_instance_handle_t *handle)
{
    if (handle == NULL) {
        return 1U;
    }
    if ((uint32_t)id >= (uint32_t)UART_INSTANCE_MAX) {
        *handle = NULL;
        return 1U;
    }
    if (!g_instances[id].is_created) {
        g_instances[id].regs = uart_get_regs(id);   ///< 调用时创建实例
        if (NULL == g_instances[id].regs) {
            *handle = NULL;
            return 2U;
        }
        g_instances[id].is_inited = false;
    }
    g_instances[id].is_created = true;
    *handle = &g_instances[id];
    LOG_I("get instance %d, regs=%p", (int)id, (void *)(*handle)->regs);
    return 0;
}

uint8_t uart_init(const uart_config_t *cfg, uart_instance_handle_t handle)
{
    if (NULL == cfg) {
        return 1U;
    }
    if (true == handle->is_inited) {
        LOG_I("uart %p already initialized", (void *)handle->regs);
        return 1U;
    }
    /* 根据cfg内容对regs进行配置 */
    /* 拷贝配置与置位 */
    handle->cfg = *cfg;
    handle->is_inited = true;
    LOG_I("uart %p init success", (void *)handle->regs);
    return 0;
}

uint8_t uart_deinit(uart_instance_handle_t handle)
{
    if (NULL == handle) {
        return 1U;
    }
    /* 关闭寄存器配置 */
    /* 清除标志位 */
    handle->is_inited = false;
    LOG_I("uart %p deinit success", (void *)handle->regs);
    return 0;
}

uint8_t uart_tx_byte(uart_instance_handle_t handle, uint8_t data)
{
    if (NULL == handle|| !handle->is_inited) {
        return 1U;
    }
    /* 等待硬件指定标志位 */
    // while (!(handle->regs->SR & UART_SR_TXE_Msk)) {
    //     ;
    // }
    handle->regs->DR = data;
    LOG_I("uart %p tx byte 0x%02X", (void *)handle->regs, data);
    return 0;
}





