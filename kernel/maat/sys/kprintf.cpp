#include <cstdarg>
#include <cstdint>
#include <sys/console.hpp>

static void print_integer(int value, int base = 10, bool is_signed = true) {
  char buf[32];
  int i = 0;
  uint32_t n;

  if (is_signed && value < 0) {
    Console::putchar('-', COLOR_WHITE);
    n = (uint32_t)(-value);
  } else {
    n = (uint32_t)value;
  }

  if (n == 0) {
    Console::putchar('0', COLOR_WHITE);
    return;
  }

  while (n > 0) {
    int digit = n % base;
    buf[i++] = (digit < 10 ? '0' + digit : 'a' + (digit - 10));
    n /= base;
  }

  while (i--)
    Console::putchar(buf[i], COLOR_WHITE);
}

void kprintf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  for (const char *p = fmt; *p; p++) {
    if (*p != '%') {
      Console::putchar(*p, COLOR_WHITE);
      continue;
    }

    p++;
    if (!*p)
      break;

    switch (*p) {
    case 'd': {
      int val = va_arg(ap, int);
      print_integer(val, 10, true);
      break;
    }
    case 'x':
    case 'p': {
      unsigned int val = va_arg(ap, unsigned int);
      print_integer(val, 16, false);
      break;
    }
    case 's': {
      const char *s = va_arg(ap, const char *);
      if (!s)
        s = "**NULL**";
      while (*s)
        Console::putchar(*s++, COLOR_WHITE);
      break;
    }
    default:
      Console::write("kprintf(): unknown format specifier", COLOR_RED);
      Console::putchar(*p, COLOR_WHITE);
      break;
    }
  }

  va_end(ap);
}
