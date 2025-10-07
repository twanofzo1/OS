#include "GDT.h"

extern void setGdt(uint16_t limit, uint64_t base);
extern void reloadSegments(void);

uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor;
 
    // Create the high 32 bit segment
    descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    descriptor |=  base        & 0xFF000000;         // set base bits 31:24
 
    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;
 
    // Create the low 32 bit segment
    descriptor |= base  << 16;                       // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0

    return descriptor;
}
uint64_t gdt_table[5];

void gdt_initialize() {
    gdt_table[0] = create_descriptor(0, 0, 0);                  // Null
    gdt_table[1] = create_descriptor(0, 0xFFFFF, GDT_CODE_PL0); // Kernel code
    gdt_table[2] = create_descriptor(0, 0xFFFFF, GDT_DATA_PL0); // Kernel data
    gdt_table[3] = create_descriptor(0, 0xFFFFF, GDT_CODE_PL3); // User code
    gdt_table[4] = create_descriptor(0, 0xFFFFF, GDT_DATA_PL3); // User data

    extern void setGdt(uint16_t limit, uint64_t base);
    extern void reloadSegments(void);

    setGdt(sizeof(gdt_table) - 1, (uint64_t)&gdt_table);
    reloadSegments();
}

