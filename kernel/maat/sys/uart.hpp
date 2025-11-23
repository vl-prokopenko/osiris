#pragma once

#define COM1 0x3F8

#define UART_DATA 0
#define UART_INT_EN 1
#define UART_FIFO_CTL 2
#define UART_LINE_CTL 3
#define UART_MCR 4
#define UART_LSR 5
#define UART_MSR 6
#define UART_SCR 7

#define UART_LSR_DR 0x01
#define UART_LSR_THRE 0x20

namespace Uart {

void init();
void putChar(char c);
char getchar();
void puts(const char *str);

} // namespace Uart
