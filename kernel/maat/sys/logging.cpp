#include <sys/console.hpp>
#include <sys/kprintf.hpp>
#include <sys/logging.hpp>

namespace Log {
void log(const char *msg, logLevel level) {
  static const char *loglevel_text = (level == DEBUG)  ? "DEBUG"
                                     : (level == WARN) ? "WARN"
                                                       : "ERROR";

  uint32_t color = (level == DEBUG)  ? COLOR_WHITE
                   : (level == WARN) ? COLOR_YELLOW
                                     : COLOR_RED;

  Console::write("[", color);
  Console::write(loglevel_text, color);
  Console::write("] ", color);
  Console::write(msg, color);
  Console::write("\n", color);
}
} // namespace Log