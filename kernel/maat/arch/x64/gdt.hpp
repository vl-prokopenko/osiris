#pragma once

#include <cstdint>

typedef struct {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed)) gdtReg;

typedef struct {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_mid;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} __attribute__((packed)) gdtEntry;

namespace x64::gdt {
void makeSegment(int num, uint8_t access, uint8_t granularity, uint32_t base,
                 uint32_t limit);

void flush();

void init();

} // namespace x64::gdt