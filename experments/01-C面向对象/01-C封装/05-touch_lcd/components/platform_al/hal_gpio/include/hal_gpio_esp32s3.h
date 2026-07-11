/**
 * @file   hal_gpio.h
 * @author xLumina
 * @date   2026-04-28
 * @brief  GPIO 平台抽象
 * 
 * @par History:	修改历史
 *   - V1.0.0 | 2026-4-28 | xLumina | 初始版本创建
 *
 * @par Function List:	
 * 
 */
#pragma once

/********* 标准类型定义库 **********/
#include <stdbool.h>
#include <stdint.h>

#include "hal_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief esp32 GPIO 私有上下文
 */
typedef struct esp32s3_gpio_ctx_t {
    uint8_t pin;
} esp32s3_gpio_ctx_t;


extern const hal_gpio_ops_t g_gpio_ops;

#ifdef __cplusplus
}
#endif
