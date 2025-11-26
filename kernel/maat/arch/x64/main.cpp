#include <arch/halt.hpp>
#include <arch/x64/gdt.hpp>
#include <arch/x64/idt.hpp>
#include <cstddef>
#include <cstdint>
#include <drv/framebuffer/framebuffer.hpp>
#include <limine.h>
#include <sys/console.hpp>
#include <sys/kprintf.hpp>
#include <sys/logging.hpp>
#include <sys/uart.hpp>

namespace {

__attribute__((used, section(".limine_requests"))) volatile std::uint64_t
    limine_base_revision[] = LIMINE_BASE_REVISION(4);

}

namespace {

__attribute__((used,
               section(".limine_requests"))) volatile limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
                           .revision = 0,
                           .response = nullptr};

}

namespace {

__attribute__((used, section(".limine_requests_start"))) volatile std::uint64_t
    limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) volatile std::uint64_t
    limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

} // namespace

extern "C" {
int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }
void __cxa_pure_virtual() {
  for (;;)
    asm("hlt");
}
void *__dso_handle;
}

extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern "C" void kmain() {
  // call global constructors
  for (std::size_t i = 0; &__init_array[i] != __init_array_end; i++) {
    __init_array[i]();
  }

  Console::init();
  Fb::doInit(&framebuffer_request);
  Log::log(DEBUG, "Starting maat");
  x64::gdt::init();
  x64::Interrupts::init();

  Arch::halt();
}
