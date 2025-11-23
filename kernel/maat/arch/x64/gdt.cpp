#include <arch/x64/gdt.hpp>
#include <cstdint>
#include <sys/logging.hpp>

namespace x64::gdt {
gdtEntry gdt[10];
gdtReg reg;

__attribute__((aligned(16))) uint8_t kernel_stack[16384];

__attribute__((aligned(16))) uint8_t user_stack[65536];

struct __attribute__((packed)) TSS {
  uint32_t reserved0;
  uint64_t rsp0;
  uint64_t rsp1, rsp2;
  uint64_t reserved1;
  uint64_t ist[7];
  uint64_t reserved2;
  uint16_t reserved3, io_map_base;
} tss;

void makeSegment(int num, uint8_t access, uint8_t granularity, uint32_t base,
                 uint32_t limit) {
  gdt[num].limit_low = limit & 0xFFFF;
  gdt[num].base_low = base & 0xFFFF;
  gdt[num].base_mid = (base >> 16) & 0xFF;
  gdt[num].access = access;
  gdt[num].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);
  gdt[num].base_high = (base >> 24) & 0xFF;
}

void flush() {
  asm volatile("mov $0x10, %%ax\n"
               "mov %%ax, %%ds\n"
               "mov %%ax, %%es\n"
               "mov %%ax, %%fs\n"
               "mov %%ax, %%gs\n"
               "mov %%ax, %%ss\n"
               "pushq $0x08\n"
               "lea 1f(%%rip), %%rax\n"
               "pushq %%rax\n"
               "lretq\n"
               "1:\n"
               :
               :
               : "rax");
}

void makeTSS(int num, uint64_t base, uint32_t limit) {
  makeSegment(num, 0x89, 0x00, base, limit);
  uint32_t *hi = (uint32_t *)&gdt[num + 1];
  hi[0] = base >> 32;
  hi[1] = 0;
}

void initTSS() { asm volatile("ltr %%ax" ::"a"(0x18)); }

void init() {
  // create segments, segmentation
  // on x64 is nonexistent but the cpu
  // still uses the gdt
  makeSegment(0, 0, 0, 0, 0);
  makeSegment(1, 0x9A, 0x20, 0, 0);
  makeSegment(2, 0x92, 0x00, 0, 0);
  makeTSS(3, (uint64_t)&tss, sizeof(tss) - 1);
  makeSegment(5, 0xFA, 0x20, 0, 0);
  makeSegment(6, 0xF2, 0x00, 0, 0);

  tss.rsp0 = (uint64_t)(kernel_stack + sizeof(kernel_stack));

  reg.limit = sizeof(gdt) - 1;
  reg.base = (uint64_t)&gdt;
  asm volatile("lgdt %0" ::"m"(reg));
  initTSS();
  Log::log("x64: Reloaded segments", DEBUG);
  flush();
}

} // namespace x64::gdt