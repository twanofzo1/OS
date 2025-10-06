#pragma once

#include <stdint.h>

static inline void physical_io_write_8(uint16_t port, uint8_t val) {
    asm volatile (
        "outb %0, %1" : : "a"(val), "Nd"(port)
    );
}

static inline uint8_t physical_io_read_8(uint16_t port) {
    uint8_t ret;
    asm volatile (
        "inb %1, %0" : "=a"(ret) : "Nd"(port)
    );
    return ret;
}

static inline uint16_t physical_io_read_16(uint16_t port) {
    uint8_t low = physical_io_read_8(port);
    uint8_t high = physical_io_read_8(port);
    return (uint16_t)(high << 8 | low);
}

static inline void physical_io_write_16(uint16_t port, uint16_t val) {
    physical_io_write_8(port, (uint8_t)(val & 0xFF));       // Low byte
    physical_io_write_8(port, (uint8_t)((val >> 8) & 0xFF)); // High byte
}

static inline void physical_io_write_32(uint16_t port, uint32_t val) {
    physical_io_write_8(port, (uint8_t)(val & 0xFF));         // Byte 0
    physical_io_write_8(port, (uint8_t)((val >> 8) & 0xFF));  // Byte 1
    physical_io_write_8(port, (uint8_t)((val >> 16) & 0xFF)); // Byte 2
    physical_io_write_8(port, (uint8_t)((val >> 24) & 0xFF)); // Byte 3
}

static inline uint32_t physical_io_read_32(uint16_t port) {
    uint32_t b0 = physical_io_read_8(port);
    uint32_t b1 = physical_io_read_8(port);
    uint32_t b2 = physical_io_read_8(port);
    uint32_t b3 = physical_io_read_8(port);
    return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
}