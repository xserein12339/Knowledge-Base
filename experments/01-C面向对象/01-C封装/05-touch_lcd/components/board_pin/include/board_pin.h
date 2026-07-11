/**
 * @file   board_pin.h
 * @author xLumina
 * @date   2026-04-27
 * @brief  板级 引脚定义，管理引脚资源
 * 
 * @par History:	修改历史
 *   - V1.0.0 | 2026-4-27 | xLumina | 初始版本创建
 *
 * @par Function List:	功能函数
 * 
 */

#pragma once


#ifdef __cplusplus
extern "C" {
#endif


/* DISPLAY AND TOUCH SPI 引脚定义 */
#define SPI2_MOSI_PIN       12
#define SPI2_MISO_PIN       13
#define SPI2_SCLK_PIN       14

/* DISPLAY GPIO 引脚定义 */
#define DISPLAY_RESET_PIN   15
#define DISPLAY_DC_PIN      16
#define DISPLAY_LEDC_PIN    17
#define DISPLAY_CS_PIN      40

/* TOUCH GPIO 引脚定义 */
#define TOUCH_IRQ_PIN       18
#define TOUCH_CS_PIN        41












#ifdef __cplusplus
}
#endif


