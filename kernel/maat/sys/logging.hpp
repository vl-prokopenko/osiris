#pragma once

#include <sys/kprintf.hpp>

enum logLevel { DEBUG, WARN, ERROR };

namespace Log {

void log(logLevel level, const char *fmt, ...);

} // namespace Log
