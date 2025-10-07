#pragma once
#include <stdint.h>



#define IST7_HIGHER_OFFSET  0x58;
#define IST7_LOWER_OFFSET   0x54;
#define IST6_HIGHER_OFFSET  0x50;
#define IST6_LOWER_OFFSET   0x4C;
#define IST5_HIGHER_OFFSET  0x48;
#define IST5_LOWER_OFFSET   0x44;
#define IST4_HIGHER_OFFSET  0x40;
#define IST4_LOWER_OFFSET   0x3C;
#define IST3_HIGHER_OFFSET  0x38;
#define IST3_LOWER_OFFSET   0x34;
#define IST2_HIGHER_OFFSET  0x30;
#define IST2_LOWER_OFFSET   0x2C;
#define IST1_HIGHER_OFFSET  0x28;
#define IST1_LOWER_OFFSET   0x24;
#define RSP2_HIGHER_OFFSET  0x18;
#define RSP2_LOWER_OFFSET   0x14;
#define RSP1_HIGHER_OFFSET  0x10;
#define RSP1_LOWER_OFFSET   0x0C;
#define RSP0_HIGHER_OFFSET  0x08;
#define RSP0_LOWER_OFFSET   0x04;



typedef struct {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist[7];
    uint64_t reserved2;
    uint16_t iopb_offset;
} TSS_Long_Mode __attribute__((packed));

TSS_Long_Mode tss;


typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;       // type = 0x9 (available TSS)
    uint8_t  granularity;
    uint8_t  base_high;
    uint32_t base_upper;   // full 64-bit base
    uint32_t reserved;
} TSSDescriptor64 __attribute__((packed));

void set_tss_descriptor(TSSDescriptor64 *desc, uint64_t base, uint32_t limit);

void set_tss_descriptor(TSSDescriptor64 *desc, uint64_t base, uint32_t limit) {
    desc->limit_low    = limit & 0xFFFF;
    desc->base_low     = base & 0xFFFF;
    desc->base_middle  = (base >> 16) & 0xFF;
    desc->access       = 0x89;  // present, type 0x9 (available TSS)
    desc->granularity  = ((limit >> 16) & 0x0F);
    desc->base_high    = (base >> 24) & 0xFF;
    desc->base_upper   = (base >> 32) & 0xFFFFFFFF;
    desc->reserved     = 0;
}


uint8_t kernel_stack[4096];

TSS_Long_Mode tss;

void setup_tss() {
    tss.rsp0 = (uint64_t)&kernel_stack[4095]; // top of kernel stack
    tss.iopb_offset = sizeof(tss);            // no I/O bitmap
}