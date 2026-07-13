/**
 * @file    singleton_pattern2.h
 * @brief   实现UART外设懒汉式单例模式声明文件
 *
 * @details 
 *
 * @author  xserein
 * @version 1.0
 */

#ifndef SINGLETON_PATTERN1_H
#define SINGLETON_PATTERN1_H

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =======================================================================  */
/*                                类型定义                                   */
/* ======================================================================== */
/* 前向声明UART实例句柄 */
typedef struct uart_t *uart_instance_handle_t;

/* UART外设实例枚举 */
typedef enum {
    UART_INSTANCE_1 = 0,
    UART_INSTANCE_2,
    UART_INSTANCE_3,
    UART_INSTANCE_MAX,
} uart_instance_t;

/**
 * @brief UART外设配置结构体
 */
typedef struct {
    uint8_t rx_pin;
    uint8_t tx_pin;
    uint32_t baud_rate;
} uart_config_t;


/* ======================================================================== */
/*                                API声明                                    */
/* ======================================================================== */

/**
 * @brief 获取实例句柄
 * 
 * @param[in] name UART外设实例名
 * @param[out] handle UART外设实例
 */
uint8_t uart_get_instance(uart_instance_t id, uart_instance_handle_t *handle);

/**
 * @brief UART初始化函数
 * 
 * @param[in] cfg 配置
 * @param[in] handle UART实例句柄
 */
uint8_t uart_init(const uart_config_t *cfg, uart_instance_handle_t handle);

/**
 * @brief UART释放函数
 * 
 * @param[in] handle UART实例句柄
 */
uint8_t uart_deinit(uart_instance_handle_t handle);

/**
 * @brief UART发送
 * 
 * @param[in] handle UART实例句柄
 * @param[in] data 发送字节数据
 */
uint8_t uart_tx_byte(uart_instance_handle_t handle, uint8_t data);



#ifdef __cplusplus
}
#endif
#endif /* SINGLETON_PATTERN1_H */
