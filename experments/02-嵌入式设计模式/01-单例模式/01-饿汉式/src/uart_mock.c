/**
 * @file    uart_mock.c
 * @brief   UART外设单例模式实现MOCK测试
 *
 * @details 
 *
 * @author  xserein
 * @version 1.0
 */
#include "uart_typedef.h"

UART_TypeDef g_mock_uart1 = { .SR = UART_SR_TXE_Msk, .DR = 0 };
UART_TypeDef g_mock_uart2 = { .SR = UART_SR_TXE_Msk, .DR = 0 };
UART_TypeDef g_mock_uart3 = { .SR = UART_SR_TXE_Msk, .DR = 0 };
