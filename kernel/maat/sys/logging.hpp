#pragma once

#include <sys/kprintf.hpp>

namespace Log {

// Log::logout << "Hi!"
class logout {
public:
  template <typename T> logout &operator<<(T value) {
    kprintf(value);
    return *this;
  }
};

inline logout logout;

} // namespace Log
