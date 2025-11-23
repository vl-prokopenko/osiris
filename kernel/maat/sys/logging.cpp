#include <sys/kprintf.hpp>
#include <sys/logging.hpp>

namespace Log {
void log(const char *msg, logLevel level) {
  static const char *loglevel_text = (level == DEBUG)  ? "DEBUG"
                                     : (level == WARN) ? "WARN"
                                                       : "ERROR";

  kprintf("[%s] %s\n", loglevel_text, msg);
}
} // namespace Log