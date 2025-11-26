#include <arch/x64/idt.hpp>
#include <arch/x64/interrupts.hpp>
#include <sys/err.hpp>
#include <sys/io.hpp>
#include <sys/time/timer.hpp>

namespace x64::Interrupts {

// lookup table for irq handlers
// not used for isrs, we have 256 entries to
// make it easier to lookup entries.
irq_handler_t handlers[256] = {nullptr};

const char *isr_msgs[] = {"division by zero",
                          "debug",
                          "non maskable interrupt",
                          "breakpoint",
                          "into detected overflow",
                          "out of bounds",
                          "invalid opcode",
                          "no coprocessor",
                          "double fault",
                          "coprocessor segment overrun",
                          "bad tss",
                          "segment not present",
                          "stack fault",
                          "general protection fault",
                          "page fault",
                          "unknown interrupt",
                          "coprocessor fault",
                          "alignment check",
                          "machine check"};

// put irq into table and unmask
void createIrq(int num, irq_handler_t handler) {
  handlers[32] = handler;
  unmaskIrq(num);
}

void setupIrqs() { createIrq(0, Timer::timerInterrupt); }

// send EOI, call after
// interrupt is finished handling.
void sendEoi() { io::outb(0x20, 0x20); }

extern "C" void handle(registers_t *regs) {
  if (regs->int_no < 19) {
    Err::panic("unhandled exception %s code %d", isr_msgs[regs->int_no],
               regs->err_code);
  } else if (regs->int_no > 31) {
    irq_handler_t handler = handlers[regs->int_no];
    if (handler) {
      handler();
    } else {
      Err::panic("cannot handle irq %d", regs->int_no);
    }
  } else {
    Err::panic("unhandled exception %d RESERVED!?", regs->int_no);
  }
}

void disable() { asm volatile("cli"); }
void enable() { asm volatile("sti"); }

} // namespace x64::Interrupts