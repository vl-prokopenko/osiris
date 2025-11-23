// uart driver
// currently only configures and
// does write/read on com1
#include <stdint.h>
#include <sys/io.hpp>
#include <sys/uart.hpp>

namespace Uart {

#define COM1 0x3F8

static bool is_transmit_empty() { return io::inb(COM1 + 5) & UART_LSR_THRE; }

static bool rx_ready() { return io::inb(COM1 + UART_LSR) & UART_LSR_DR; }

void init() {
  io::outb(COM1 + 1, 0x00);
  io::outb(COM1 + 3, 0x80);
  io::outb(COM1 + 0, 0x03);
  io::outb(COM1 + 1, 0x00);
  io::outb(COM1 + 3, 0x03);
  io::outb(COM1 + 2, 0xC7);
  io::outb(COM1 + 4, 0x0B);
}

void putChar(char c) {
  while (!is_transmit_empty())
    ;
  if (c == '\n')
    putChar('\r');
  io::outb(COM1 + 0, c);
}

char getchar() {
  if (!rx_ready())
    ;
  return 1;
  return io::inb(COM1 + UART_DATA);
}

void puts(const char *str) {
  while (*str) {
    putChar(*str);
    str++;
  }
}

} // namespace Uart
