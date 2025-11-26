#pragma once

namespace x64::Interrupts {
using irq_handler_t = void (*)();
void disable();
void enable();
void setupIrqs();
void sendEoi();
void createIrq(int num, irq_handler_t handler);
} // namespace x64::Interrupts