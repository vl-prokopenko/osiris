#pragma once

#include <arch/x64/idt.hpp>
#include <cstdint>

namespace Timer {
void timerInterrupt();
uint64_t getCount();
void sleep(uint64_t ms);
} // namespace Timer