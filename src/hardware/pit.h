#pragma once
#include <stdint.h>

#define PIT_IO_ADDRESS_0 0x40 // read/write
#define PIT_IO_ADDRESS_1 0x41 // read/write
#define PIT_IO_ADDRESS_2 0x42 // read/write

void pit_initialize(uint32_t frequency, uint8_t channel);
void pit_sleep_ms(uint32_t ms);