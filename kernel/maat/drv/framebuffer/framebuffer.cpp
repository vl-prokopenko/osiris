// limine framebuffer driver
// !! WE ASSUME 8x16 FONT BY DEFAULT !!
#include <cstdint>
#include <drv/framebuffer/font.hpp>
#include <drv/framebuffer/framebuffer.hpp>
#include <limine.h>
#include <sys/logging.hpp>

Framebuffer fb(nullptr, 0, 0, 0, 0, 0, 0);

void Framebuffer::writePixel(int y, int x, std::uint32_t color) {
  _addr[y * (_pitch / 4) + x] = color;
}

void Framebuffer::putCharAt(char c, int x, int y, uint32_t color) {
  for (int row = 0; row < 16; row++) {
    uint8_t bits = basic_font_data[(uint8_t)c][row];
    for (int col = 0; col < 8; col++) {
      if (bits & (1 << (7 - col))) {
        writePixel(y + row, x + col, color);
      }
    }
  }
}

void Framebuffer::scroll() {
  const int CHAR_HEIGHT = 16;
  uint32_t *pixels = _addr;
  int row_pixels = _pitch / 4;

  for (int y = 0; y < _height - CHAR_HEIGHT; y++) {
    uint32_t *dst = pixels + y * row_pixels;
    uint32_t *src = pixels + (y + CHAR_HEIGHT) * row_pixels;
    for (int x = 0; x < row_pixels; x++) {
      dst[x] = src[x];
    }
  }

  for (int y = _height - CHAR_HEIGHT; y < _height; y++) {
    uint32_t *dst = pixels + y * row_pixels;
    for (int x = 0; x < row_pixels; x++) {
      dst[x] = 0;
    }
  }

  _cursor_y -= CHAR_HEIGHT;
  if (_cursor_y < 0)
    _cursor_y = 0;
}

void Framebuffer::newLine() {
  _cursor_x = 0;
  _cursor_y += 16;
}

void Framebuffer::putChar(char c, uint32_t color) {
  if (c == '\n') {
    newLine();
    return;
  }
  putCharAt(c, _cursor_x, _cursor_y, color);
  _cursor_x = _cursor_x + 8;

  if (_cursor_x + 8 >= _width) {
    newLine();
  }

  if (_cursor_y + 16 >= _height) {
    scroll();
  }
}

void Framebuffer::init(volatile struct limine_framebuffer_request *request) {
  // fetch framebuffer struct from arg
  const struct limine_framebuffer *framebuffer =
      request->response->framebuffers[0];

  _addr = reinterpret_cast<uint32_t *>(framebuffer->address);
  _pitch = framebuffer->pitch;
  _bpp = framebuffer->bpp;
  _width = framebuffer->width;
  _height = framebuffer->height;
}

namespace Fb {
void doInit(volatile struct limine_framebuffer_request *request) {
  fb.init(request);
}

void putCharAt(char c, int x, int y, uint32_t color) {
  fb.putCharAt(c, x, y, color);
}

void putChar(char c, uint32_t color) { fb.putChar(c, color); }

void puts(const char *str, uint32_t color) {
  while (*str) {
    putChar(*str, color);
    str++;
  }
}
} // namespace Fb