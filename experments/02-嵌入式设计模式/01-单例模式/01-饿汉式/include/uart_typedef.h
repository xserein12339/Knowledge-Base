#ifndef UART_TYPEDEF_H
#define UART_TYPEDEF_H

#include <stdint.h>
#include <stdbool.h>
/* ========== 主机 Mock 定义 ========== */
typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
} UART_TypeDef;

/* 为每个实例分配独立的模拟寄存器块 */
extern UART_TypeDef g_mock_uart1;
extern UART_TypeDef g_mock_uart2;
extern UART_TypeDef g_mock_uart3;

#define UART1 (&g_mock_uart1)
#define UART2 (&g_mock_uart2)
#define UART3 (&g_mock_uart3)

#define UART_SR_TXE_Msk (1U << 7)

#endif /* UART_TYPEDEF_H */