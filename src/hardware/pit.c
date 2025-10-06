#include "pit.h"
#include "physical_io.h"

/*
readme:
    the PIT (Programmable Interval Timer) is a hardware timer used in x86 computers
    it is used to generate interrupts at regular intervals
    it is used by the operating system to keep track of time and to schedule tasks
    it is also used to implement functions like sleep() and delay()
    the PIT operates at a base frequency of 1.193182 MHz (1193182 Hz)
    by setting a divisor value, the PIT can be configured to generate interrupts at a lower frequency
*/


// https://wiki.osdev.org/Programmable_Interval_Timer


//The PIT chip uses the following I/O ports:
#define PIT_FREQUENCY 1193182 // Hz
#define PIT_TICKS_PER_MS 1193 // For approximately 1ms sleep

#define PIT_COMMAND_PORT 0x43 // write-only

/*
Bits         Usage
7 and 6      Select channel :
                0 0 = Channel 0
                0 1 = Channel 1
                1 0 = Channel 2
                1 1 = Read-back command (8254 only)
*/
#define PIT_CHANNEL_0        0x00 //0b00000000 
#define PIT_CHANNEL_1        0x40 //0b01000000 
#define PIT_CHANNEL_2        0x80 //0b10000000 
#define PIT_READBACK_COMMAND 0xC0 //0b11000000
/*
Bits         Usage
5 and 4      Access mode :
                0 0 = Latch count value command
                0 1 = Access mode: lobyte only
                1 0 = Access mode: hibyte only
                1 1 = Access mode: lobyte/hibyte
*/
#define PIT_ACCESS_MODE_LATCH_COUNT     0x00 //0b00000000
#define PIT_ACCESS_MODE_LOBYTE_ONLY     0x10 //0b00010000
#define PIT_ACCESS_MODE_HIBYTE_ONLY     0x20 //0b00100000
#define PIT_ACCESS_MODE_LOBYTE_HIBYTE   0x30 //0b00110000
/*
Bits         Usage
3 to 1       Operating mode :
                0 0 0 = Mode 0 (interrupt on terminal count)
                0 0 1 = Mode 1 (hardware re-triggerable one-shot)
                0 1 0 = Mode 2 (rate generator)
                0 1 1 = Mode 3 (square wave generator)
                1 0 0 = Mode 4 (software triggered strobe)
                1 0 1 = Mode 5 (hardware triggered strobe)
                1 1 0 = Mode 2 (rate generator, same as 010b)
                1 1 1 = Mode 3 (square wave generator, same as 011b)
*/
#define PIT_OPERATING_MODE_INTERRUPT                 0x00 //0b00000000
#define PIT_OPERATING_MODE_ONE_SHOT                  0x02 //0b00000010
#define PIT_OPERATING_MODE_RATE_GENERATOR            0x04 //0b00000100
#define PIT_OPERATING_MODE_SQUARE_WAVE               0x06 //0b00000110
#define PIT_OPERATING_MODE_SOFTWARE_TRIGGERED_STROBE 0x08 //0b00001000
#define PIT_OPERATING_MODE_HARDWARE_TRIGGERED_STROBE 0x0A //0b00001010

/*
Bits         Usage
0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
*/
#define PIT_BCD_MODE    0x01 //0b1
#define PIT_BINARY_MODE 0x00 //0b0


#define PIT_COMMAND(channel, access_mode, operating_mode, bcd_mode) \
    (channel | access_mode | operating_mode | bcd_mode)


/*
PIT outputs
Channel 0 is connected directly to IRQ0, so it is best to use it only for purposes that should generate interrupts. 
Channel 1 is unusable, and may not even exist. 
Channel 2 is connected to the PC speaker, but can be used for other purposes without producing audible speaker tones.
*/


void pit_initialize(uint32_t frequency,uint8_t channel) {
    if (frequency == 0 || frequency > PIT_FREQUENCY) {
        return;
    }
    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);

    uint8_t io_address;
    switch (channel)
    {
    case PIT_CHANNEL_0:
        io_address = PIT_IO_ADDRESS_0;
        break;
    case PIT_CHANNEL_1:
        io_address = PIT_IO_ADDRESS_1;
        break;
    case PIT_CHANNEL_2:
        io_address = PIT_IO_ADDRESS_2;
        break;
    default:
        return;
    }

    // Send command byte
    physical_io_write_8(PIT_COMMAND_PORT, PIT_COMMAND(channel, PIT_ACCESS_MODE_LOBYTE_HIBYTE, PIT_OPERATING_MODE_SQUARE_WAVE, PIT_BINARY_MODE));

    // Send frequency divisor
    physical_io_write_16(io_address, divisor);
}


void pit_sleep_ms(uint32_t ms) {
    if (ms == 0) return;
    
    // Break down large delays into smaller chunks
    while (ms > 0) {
        // Calculate delay for this iteration (max ~54ms per iteration due to 16-bit limit)
        // (65536(uint16 max) ticks / 1193 ticks/ms) ≈ 54.9 ms
        uint16_t current_ms = (ms > 54) ? 54 : ms;
        
        // Calculate ticks needed for this delay
        uint16_t ticks = (uint16_t)(current_ms * PIT_TICKS_PER_MS);
        
        // Configure PIT channel 2 for interrupt mode
        physical_io_write_8(PIT_COMMAND_PORT, PIT_COMMAND(PIT_CHANNEL_2, PIT_ACCESS_MODE_LOBYTE_HIBYTE, PIT_OPERATING_MODE_INTERRUPT, PIT_BINARY_MODE));
        physical_io_write_16(PIT_IO_ADDRESS_2, ticks);
        
        // Wait for the counter to reach zero
        uint16_t current_count;
        do {
            physical_io_write_8(PIT_COMMAND_PORT, PIT_COMMAND(PIT_CHANNEL_2, PIT_ACCESS_MODE_LATCH_COUNT, 0, 0));
            current_count = physical_io_read_16(PIT_IO_ADDRESS_2);

        } while (current_count > 0);
        
        ms -= current_ms;
    }
}




