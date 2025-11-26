namespace Arch {
void halt() {
  for (;;) {
    // asm volatile("cli");
    asm volatile("hlt");
  }
}
} // namespace Arch