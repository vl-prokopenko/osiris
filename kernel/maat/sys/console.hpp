#pragma once

#include <cstdint>

namespace Console {
void init();
void write(const char *str, uint32_t color);
void putchar(char c, uint32_t color);
} // namespace Console

#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLACK 0x000000
#define COLOR_RED 0xFF0000
#define COLOR_YELLOW 0xFFFF00
#define COLOR_GREEN 0xa0fd6c