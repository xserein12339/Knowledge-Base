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


#ifdef __cplusplus
extern "C" {
#endif

/* GPIO 模式 */
typedef enum hal_gpio_dir_t {
    GPIO_DIR_OUTPUT = 0,    ///< 输出模式
    GPIO_DIR_INPUT,         ///< 输入模式
} hal_gpio_dir_t;

/* GPIO 上下拉 */
typedef enum hal_gpio_pull_t {
    GPIO_PULL_UP_DISABLE = 0,   ///< 禁止上拉
    GPIO_PULL_UP_ENABLE,        ///< 上拉使能
    GPIO_PULL_DOWM_DISABLE,     ///< 禁止下拉
    GPIO_PULL_DOWN_ENABLE,      ///< 下拉使能   
} hal_gpio_pull_t;

/**
 * @brief GPIO 配置
 */
typedef struct hal_gpio_config_t {
    hal_gpio_dir_t  dir;
    hal_gpio_pull_t pull;
} hal_gpio_config_t;

/**
 * @brief GPIO 操作表
 */
typedef struct hal_gpio_ops_t {
    uint8_t (*init)(void *ctx, hal_gpio_config_t *cfg);
    uint8_t (*set_level)(void *ctx, bool level);
    uint8_t (*get_level)(void *ctx, bool *level);
    /* 可扩展 */
} hal_gpio_ops_t;

/**
 * @brief GPIO 句柄
 */
typedef struct hal_gpio_handle_t {
    const hal_gpio_ops_t *ops;
    void *ctx;
} hal_gpio_handle_t;


static inline uint8_t hal_gpio_init(hal_gpio_handle_t *handle, hal_gpio_config_t *cfg);

static inline uint8_t hal_gpio_set_level(hal_gpio_handle_t *handle, bool level);

static inline uint8_t hal_gpio_get_level(hal_gpio_handle_t *handle, bool *level);



#ifdef __cplusplus
}
#endif
