/**
 * @file   hal_gpio_esp32s3.c
 * @author xLumina
 * @date   2026-04-28
 * @brief  GPIO esp32s3 平台抽象
 * 
 * @par History:	修改历史
 *   - V1.0.0 | 2026-4-28 | xLumina | 初始版本创建
 *
 * @par Function List:	
 * 
 */


#include "hal_gpio_esp32s3.h"

/********* C库 **********/
#include <stdio.h>

/********* 平台相关库 **********/
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "hal_gpio_esp32s3";


uint8_t esp32s3_gpio_init(void *ctx, hal_gpio_config_t *cfg)
{
    esp_err_t ret;
    esp32s3_gpio_ctx_t *c = (esp32s3_gpio_ctx_t*)ctx;
    gpio_config_t io_cfg = {0};
    io_cfg.pin_bit_mask = 1ULL << c->pin;
    switch (cfg->dir) {
        case GPIO_DIR_OUTPUT:
            io_cfg.mode = GPIO_MODE_OUTPUT;
            break;
        case GPIO_DIR_INPUT:
            io_cfg.mode = GPIO_MODE_INPUT;
            break;
        default:
            ESP_LOGE(TAG, "hal [%s] gpio set dir fail\r\n", c->pin);
            return 0;
    }
    switch (cfg->pull) {
    case GPIO_PULL_UP_DISABLE:
        io_cfg.pull_up_en   = GPIO_PULLUP_DISABLE;
        break;
    case GPIO_PULL_UP_ENABLE:
        io_cfg.pull_up_en   = GPIO_PULLUP_ENABLE;
        break;
    case GPIO_PULL_DOWM_DISABLE:
        io_cfg.pull_up_en   = GPIO_PULLDOWN_DISABLE;
        break;
    case GPIO_PULL_DOWN_ENABLE:
        io_cfg.pull_up_en   = GPIO_PULLDOWN_ENABLE;
        break;
    default:
        ESP_LOGE(TAG, "hal [%s] gpio set pull fail\r\n", c->pin);
        return 0;
    }
    ret = gpio_config(&io_cfg);
    if (ESP_OK != ret) {
        ESP_LOGE(TAG, "hal [%s] gpio init fail\r\n", c->pin);
        return 0;
    }
    return 1;
}


uint8_t esp32s3_gpio_set_level(void *ctx, bool level)
{
    esp_err_t ret;
    esp32s3_gpio_ctx_t *c = (esp32s3_gpio_ctx_t*)ctx;
    ret = gpio_set_level(c->pin, level);
    if (ESP_OK != ret) {
        ESP_LOGE(TAG, "hal [%s] gpio set levle fail\r\n", c->pin);
        return 0;
    }
    return 1;

}

uint8_t esp32s3_gpio_get_level(void *ctx, bool *level)
{
    esp_err_t ret;
    esp32s3_gpio_ctx_t *c = (esp32s3_gpio_ctx_t*)ctx;
    *level = gpio_get_level(c->pin);
    return 1;
}


/* 初始化操作表 */
const hal_gpio_ops_t g_gpio_ops = {
    .init = esp32s3_gpio_init,
    .set_level = esp32s3_gpio_set_level,
    .get_level = esp32s3_gpio_get_level,
};
