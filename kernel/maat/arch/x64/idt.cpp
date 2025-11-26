#include <arch/x64/idt.hpp>
#include <arch/x64/interrupts.hpp>
#include <cstdint>
#include <sys/console.hpp>
#include <sys/io.hpp>

namespace x64::Interrupts {

idtEntry idt[256];
idtReg reg;

extern "C" void do_isr0();
extern "C" void do_isr1();
extern "C" void do_isr2();
extern "C" void do_isr3();
extern "C" void do_isr4();
extern "C" void do_isr5();
extern "C" void do_isr6();
extern "C" void do_isr7();
extern "C" void do_isr8();
extern "C" void do_isr9();
extern "C" void do_isr10();
extern "C" void do_isr11();
extern "C" void do_isr12();
extern "C" void do_isr13();
extern "C" void do_isr14();
extern "C" void do_isr15();
extern "C" void do_isr16();
extern "C" void do_isr17();
extern "C" void do_isr18();
extern "C" void do_isr32();

void (*isrs[])() = {do_isr0,  do_isr1,  do_isr2,  do_isr3,  do_isr4,
                    do_isr5,  do_isr6,  do_isr7,  do_isr8,  do_isr9,
                    do_isr10, do_isr11, do_isr12, do_isr13, do_isr14,
                    do_isr15, do_isr16, do_isr17, do_isr18};

void maskIrq(uint8_t irq) {
  if (irq < 8) {
    uint8_t mask = io::inb(PIC1_DATA);
    mask |= (1 << irq);
    io::outb(PIC1_DATA, mask);
  } else {
    uint8_t mask = io::inb(PIC2_DATA);
    mask |= (1 << (irq - 8));
    io::outb(PIC2_DATA, mask);
  }
}

void unmaskIrq(uint8_t irq) {
  if (irq < 8) {
    uint8_t mask = io::inb(PIC1_DATA);
    mask &= ~(1 << irq);
    io::outb(PIC1_DATA, mask);
  } else {
    uint8_t mask = io::inb(PIC2_DATA);
    mask &= ~(1 << (irq - 8));
    io::outb(PIC2_DATA, mask);
  }
}

void remapPic(int offset1, int offset2) {
  io::outb(PIC1_CMD, 0x11);
  io::outb(PIC2_CMD, 0x11);

  io::outb(PIC1_DATA, offset1);
  io::outb(PIC2_DATA, offset2);

  io::outb(PIC1_DATA, 4);
  io::outb(PIC2_DATA, 2);

  io::outb(PIC1_DATA, 0x01);
  io::outb(PIC2_DATA, 0x01);

  io::outb(PIC1_DATA, 0xFF);
  io::outb(PIC2_DATA, 0xFF);
}

void makeInterrupt(int num, uint64_t base, uint16_t sel, uint8_t flags) {
  idt[num].base_low = (uint16_t)(base & 0xFFFF);
  idt[num].sel = sel;
  idt[num].always0 = 0;
  idt[num].flags = flags;
  idt[num].base_mid = (uint16_t)((base >> 16) & 0xFFFF);
  idt[num].base_high = (uint32_t)((base >> 32) & 0xFFFFFFFF);
  idt[num].zero = 0;
}

void init() {
  for (int i = 0; i < 29; ++i) {
    makeInterrupt(i, (uint64_t)isrs[i], 0x08, 0x8E);
  }
  makeInterrupt(32, (uint64_t)do_isr32, 0x08, 0x8E);
  reg.limit = (sizeof(struct idtEntry) * 256) - 1;
  reg.base = (uint64_t)&idt;
  asm volatile("lidt %0" : : "m"(reg));
  remapPic(32, 40);
  setupIrqs();
  x64::Interrupts::enable();
}

} // namespace x64::Interrupts