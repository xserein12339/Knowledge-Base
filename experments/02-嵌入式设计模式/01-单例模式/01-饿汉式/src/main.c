/**
 * @file    main.c
 * @brief   UART外设单例模式实现测试
 *
 * @details 
 *
 * @author  xserein
 * @version 1.0
 */
#include "singleton_pattern1.h"


int main(void)
{
    uart_instance_handle_t uart1_handle = NULL;
    uart_get_instance(UART_INSTANCE_1, &uart1_handle);
    const uart_config_t uart_cfg = {
        .rx_pin = 1,
        .tx_pin = 2,
        .baud_rate = 9600,
    };
    uart_init(&uart_cfg, uart1_handle);
    for (uint8_t i = 0; i <10; i ++) {
        uart_tx_byte(uart1_handle, i);
    }
    uart_deinit(uart1_handle);
    return 0;
}









