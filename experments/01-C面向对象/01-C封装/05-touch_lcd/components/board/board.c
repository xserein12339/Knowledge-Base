#include <stdio.h>
#include "board.h"
#include "hal_gpio.h"
#include "hal_gpio_esp32s3.h"

#define DISPLAY_RESET_PIN 11

/* 定义 display 的 reset 引脚 */
static esp32s3_gpio_ctx_t display_reset_ctx = {
    .pin = DISPLAY_RESET_PIN,
};

static hal_gpio_handle_t display_reset_gpio = {
    .ops = &g_gpio_ops,
    .ctx = &display_reset_gpio,
};


void init(void)
{
    
}
