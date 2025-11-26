#pragma once

#include <cstddef>
#include <cstdint>
#include <limine.h>

class Framebuffer {
public:
  Framebuffer(uint32_t *addr, int width, int height, int cursor_x, int cursor_y,
              int pitch, int bpp)
      : _addr(addr), _width(width), _height(height), _cursor_x(cursor_x),
        _cursor_y(cursor_y), _pitch(pitch), _bpp(bpp) {}

  void writePixel(int y, int x, std::uint32_t color);
  void deleteCursor();
  void drawCursor();
  void putCharAt(char c, int x, int y, uint32_t color);
  void putChar(char c, uint32_t color);
  void scroll();
  void newLine();
  void init(volatile struct limine_framebuffer_request *request);

private:
  uint32_t *_addr;
  int _width;
  int _height;
  int _cursor_x, _cursor_y, _pitch, _bpp;
}; // class Framebuffer

namespace Fb {
void doInit(volatile struct limine_framebuffer_request *request);
void putCharAt(char c, int x, int y, uint32_t color);
void putChar(char c, uint32_t color);
void puts(const char *str, uint32_t color);
} // namespace Fb
