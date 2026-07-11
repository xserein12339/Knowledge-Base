/**
 * @file   bsp_spi.h
 * @author xLumina
 * @date   2026-04-27
 * @brief  板级 SPI 驱动
 * 
 * @par History:	修改历史
 *   - V1.0.0 | 2026-4-27 | xLumina | 初始版本创建
 *
 * @par Function List:
 *      - bsp_spi_init() SPI 总线初始化
 *      - bsp_spi_deinit() SPI 总线反初始化
 */

#pragma once

/********* 标准类型定义库 **********/
#include <stdbool.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief 板级 SPI 总线初始化
 * 
 * @return 
 *      - 0 失败
 *      - 1 成功
 * 
 */
uint8_t bsp_spi_init(void);

/**
 * @brief 板级 SPI 总线反初始化
 * 
 * @return 
 *      - 0 失败
 *      - 1 成功
 * 
 */
uint8_t bsp_spi_deinit(void);

#ifdef __cplusplus
}
#endif

