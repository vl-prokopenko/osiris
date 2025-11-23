#pragma once

#include <sys/kprintf.hpp>

enum logLevel { DEBUG, WARN, ERROR };

namespace Log {

void log(const char *msg, logLevel level);

} // namespace Log
