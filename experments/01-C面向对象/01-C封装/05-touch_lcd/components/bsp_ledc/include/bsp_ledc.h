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
#pragma once

/********* 标准类型定义库 **********/
#include <stdbool.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief 板级 LEDC 初始化函数
 * 
 * @return 
 *      - 0 失败
 *      - 1 成功
 */
uint8_t bsp_ledc_init(void);

/**
 * @brief 板级 LEDC 反初始化函数
 * 
 * @return 
 *      - 0 失败
 *      - 1 成功
 */
uint8_t bsp_ledc_deinit(void);

/**
 * @brief LEDC 设置占空比，
 * 
 * @param[in] duty 占空比， 0-100
 * 
 * @return 
 *      - 0 失败
 *      - 1 成功
 * 
 */
uint8_t bsp_ledc_set_duty(uint32_t duty);

/**
 * @brief LEDC 开始输出
 * 
 * @return 
 *      - 0 失败
 *      - 1 成功
 * 
 */
uint8_t bsp_ledc_start(void);

/**
 * @brief LEDC 停止输出
 * 
 * @return 
 *      - 0 失败
 *      - 1 成功
 * 
 */
uint8_t bsp_ledc_stop(void);

#ifdef __cplusplus
}
#endif
