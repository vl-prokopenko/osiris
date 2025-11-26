// Timer interface, not bound
// to any specific timer
#include <arch/x64/interrupts.hpp>
#include <sys/logging.hpp>
#include <sys/time/timer.hpp>

namespace Timer {
uint64_t count;

uint64_t getCount() { return count; }

void sleep(uint64_t ms) {
  uint64_t start = Timer::getCount();
  while (Timer::getCount() < start + ms) {
    asm volatile("hlt");
  }
}

void timerInterrupt() {
  count++;
  x64::Interrupts::sendEoi();
}
} // namespace Timer