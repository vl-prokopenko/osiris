#include <cstdarg>
#include <sys/console.hpp>
#include <sys/kprintf.hpp>
#include <sys/logging.hpp>

namespace Log {
void log(logLevel level, const char *fmt, ...) {
  static const char *loglevel_text = (level == DEBUG)  ? "DEBUG"
                                     : (level == WARN) ? "WARN"
                                                       : "ERROR";

  uint32_t color = (level == DEBUG)  ? COLOR_WHITE
                   : (level == WARN) ? COLOR_YELLOW
                                     : COLOR_RED;

  Console::write("[", color);
  Console::write(loglevel_text, color);
  Console::write("] ", color);

  va_list ap;
  va_start(ap, fmt);
  kvprintf(fmt, ap);
  va_end(ap);

  Console::putchar('\n', color);
}
} // namespace Log