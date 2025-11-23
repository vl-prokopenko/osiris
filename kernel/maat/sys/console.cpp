#include <drv/framebuffer/framebuffer.hpp>
#include <sys/kprintf.hpp>
#include <sys/logging.hpp>
#include <sys/uart.hpp>

namespace Console {
// call uart, fb init
// procedures...
void init() { Uart::init(); }

void write(const char *str, uint32_t color) {
  Uart::puts(str);
  Fb::puts(str, color);
}

void putchar(char c, uint32_t color) {
  Uart::putChar(c);
  Fb::putChar(c, color);
}
} // namespace Console
