#include <arch/halt.hpp>
#include <cstdarg>
#include <sys/console.hpp>
#include <sys/err.hpp>
#include <sys/kprintf.hpp>

namespace Err {
void panic(const char *fmt, ...) {
  kprintf("Panic!\n");
  va_list ap;
  va_start(ap, fmt);
  kvprintf(fmt, ap);
  va_end(ap);
  Arch::halt();
}
} // namespace Err