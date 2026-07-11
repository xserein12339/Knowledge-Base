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
 * 
 */

#include "bsp_spi.h"
#include "board_pin.h"

/********** C库 **********/
#include <stdio.h>

/********** 平台相关库 **********/
#include "driver/spi_master.h"
#include "esp_log.h"

static const char *TAG = "bsp_spi";

/* SPI HOST ID */
#define BSP_SPI2_HOST   SPI2_HOST           ///< SPI HOST 2
/* SPI DMA 通道 */
#define SPI_USE_DMA     1   ///< 使用DMA
#if SPI_USE_DMA
    #define SPI_DMA_CH      SPI_DMA_CH_AUTO     ///< 自动选择通道
#else
    #define SPI_DMA_CH     SPI_DMA_DISABLED    ///< 不使用DMA
#endif


uint8_t bsp_spi_init(void)
{
    esp_err_t ret;
    const spi_bus_config_t cfg = {
        .mosi_io_num    = SPI2_MOSI_PIN,
        .miso_io_num    = SPI2_MISO_PIN,
        .sclk_io_num    = SPI2_SCLK_PIN,
        .quadhd_io_num  = -1,
        .quadwp_io_num  = -1,
        .max_transfer_sz    = 4096,
    };
    ret = spi_bus_initialize(BSP_SPI2_HOST, &cfg, SPI_DMA_CH);
    if (ESP_OK != ret) {
        ESP_LOGW(TAG, "BSP SPI bus init fail [%d]\r\n", ret);
        return 0;
    }

    ESP_LOGI(TAG, "BSP SPI bus init success\r\n");
    return 1;
}


uint8_t bsp_spi_deinit(void)
{
    esp_err_t ret;
    ret = spi_bus_free(BSP_SPI2_HOST);
    if (ESP_OK != ret) {
        ESP_LOGW(TAG, "BSP SPI Init fail [%d]\r\n", ret);
    }

    ESP_LOGI(TAG, "BSP SPI bus deinit success\r\n");
    return 1;
}