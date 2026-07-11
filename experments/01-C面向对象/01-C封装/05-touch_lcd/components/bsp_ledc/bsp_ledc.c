/**
 * @file   bsp_ledc.h
 * @author xLumina
 * @date   2026-04-27
 * @brief  板级 LEDC 驱动
 * 
 * @par History:	修改历史
 *   - V1.0.0 | 2026-4-27 | xLumina | 初始版本创建
 *
 * @par Function List:	
 *      - bsp_ledc_init() LEDC 初始化
 *      - bsp_ledc_deinit() LEDC 反初始化
 *      - bsp_ledc_set_duty() LEDC 设置占空比
 *      - bsp_ledc_start() LEDC 开始输出
 *      - bsp_ledc_stop() LEDC 停止输出
 * 
 */

#include "bsp_ledc.h"
#include "board_pin.h"

/********* 标准类型定义库 **********/
#include <stdio.h>

/********* 平台相关库 **********/
#include "driver/ledc.h"
#include "esp_log.h"

static const char *TAG = "bsp_ledc";

/* LEDC 频率 */
#define LEDC_FREQ_HZ    10000000
#define LEDC_MODE       LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL    LEDC_CHANNEL_0
#define LEDC_DUTY_RES   LEDC_TIMER_12_BIT

uint8_t bsp_ledc_init(void)
{
    esp_err_t ret;
    const ledc_timer_config_t tim_cfg = {
        .timer_num      = LEDC_TIMER_0,
        .speed_mode     = LEDC_MODE,
        .freq_hz        = LEDC_FREQ_HZ,
        .clk_cfg        = LEDC_AUTO_CLK,
        .duty_resolution = LEDC_TIMER_12_BIT,
    };
    ret = ledc_timer_config(&tim_cfg);
    if (ESP_OK != ret) {
        ESP_LOGW(TAG, "LEDC init fail [%d]\r\n", ret);
        return 0;
    }
    const ledc_channel_config_t chan_cfg = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = DISPLAY_LEDC_PIN,
        .duty           = 0, 
    };
    ret = ledc_channel_config(&chan_cfg);
    if (ESP_OK != ret) {
        ESP_LOGW(TAG, "LEDC init fail [%d]\r\n", ret);
        return 0;
    }
    ESP_LOGI(TAG, "LEDC init success\r\n");
    return 1;
}


uint8_t bsp_ledc_deinit(void)
{
    esp_err_t ret;


    ESP_LOGI(TAG, "LEDC deinit success\r\n");
    return 1;
}


uint8_t bsp_ledc_set_duty(uint32_t duty)
{
    esp_err_t ret;
    if (100 < duty) {
        duty = 100;
    }
    uint32_t du = (4096 * duty) / 100;
    ret = ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, du);
    if (ESP_OK != ret) {
        ESP_LOGW(TAG, "LEDC set duty fail [%d]\r\n", ret);
        return 0;
    }
    ESP_LOGW(TAG, "LEDC set duty success\r\n");
    return 1;
}


uint8_t bsp_ledc_start(void)
{
    esp_err_t ret;
    ret = ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    if (ESP_OK != ret) {
        ESP_LOGW(TAG, "LEDC start fail [%d]\r\n", ret);
        return 0;
    }
    ESP_LOGI(TAG, "LEDC start success\r\n");
    return 1;
}


uint8_t bsp_ledc_stop(void)
{
    esp_err_t ret;
    ret = ledc_stop(LEDC_MODE, LEDC_CHANNEL, 0);
    if (ESP_OK != ret) {
        ESP_LOGW(TAG, "LEDC stop fail [%d]\r\n", ret);
        return 0;
    }
    ESP_LOGW(TAG, "LEDC stop success\r\n");
    return 1;
}

