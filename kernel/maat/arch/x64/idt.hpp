#pragma once

#include <cstdint>

namespace x64::Interrupts {
typedef struct {
  uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
  uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
  uint64_t int_no, err_code;
  uint64_t rip, cs, rflags, rsp, ss;
} registers_t;

struct idtEntry {
  uint16_t base_low;
  uint16_t sel;
  uint8_t always0;
  uint8_t flags;
  uint16_t base_mid;
  uint32_t base_high;
  uint32_t zero;
} __attribute__((packed));

struct idtReg {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1

void unmaskIrq(uint8_t irq);
void maskIrq(uint8_t irq);
void makeInterrupt(int num, uint64_t base, uint16_t sel, uint8_t flags);
void init();

} // namespace x64::Interrupts