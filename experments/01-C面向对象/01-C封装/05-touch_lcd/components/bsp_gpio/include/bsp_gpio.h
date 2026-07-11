/**
 * @file   bsp_gpio.h
 * @author xLumina
 * @date   2026-04-27
 * @brief  板级gpio驱动
 * 
 * @par History:	修改历史
 *   - V1.0.0 | 2026-4-27 | xLumina | 初始版本创建
 *
 * @par Function List:	
 *      - bsp_gpio_init() GPIO 初始化
 *      - bsp_gpio_deinit() GPIO 反初始化
 * 
 */
#pragma once

/********* 标准类型定义库 **********/
#include <stdbool.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief 板级 GPIO 初始化函数
 * 
 * @return
 *      - 0 失败
 *      - 1成功
 * 
 */
uint8_t bsp_gpio_init(void);

/**
 * @brief 板级 GPIO 反初始化函数
 * 
 * @return
 *      - 0 失败
 *      - 1成功
 * 
 */
uint8_t bsp_gpio_deinit(void);



#ifdef __cplusplus
}
#endif
