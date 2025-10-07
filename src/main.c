#include "memory.h"
#include <graphics/text/text.h>
#include "hardware/pit.h"
#include "terminal/terminal.h"
#include "GDT/GDT.h"
// Exit function.
static void exit(void) {
    for (;;) {
        asm ("hlt");
    }
}


void itos(uint64_t value, char* str) {
    int i = 0;
    // Special case for zero
    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    // Convert digits to string
    while (value > 0) {
        str[i++] = (value % 10) + '0';
        value /= 10;
    }
    str[i] = '\0';

    // Reverse the string
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

void itohexstr(uint64_t value, char* str) {
    int i = 0;
    // Special case for zero
    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    // Convert digits to string
    while (value > 0) {
        uint8_t digit = value & 0xF;
        if (digit < 10) {
            str[i++] = digit + '0';
        } else {
            str[i++] = digit - 10 + 'A';
        }
        value >>= 4;
    }
    str[i] = '\0';

    // Reverse the string
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}


void kmain(void) {
    if (!limine_ok()) {
        exit();
    }

    Terminal terminal;
    terminal_initialize(&terminal, 50, 50, TERMINAL_WIDTH, TERMINAL_HEIGHT);
    terminal_put_string(&terminal, "Hello, World! this is some text\n");

    uint64_t null_descriptor = create_descriptor(0, 0, 0);
    uint64_t kernel_code_segment = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
    uint64_t kernel_data_segment = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
    uint64_t user_code_segment = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
    uint64_t user_data_segment = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));

    char buffer[50];
    itohexstr(null_descriptor, buffer);
    terminal_put_string(&terminal, "Null Descriptor: 0x");
    terminal_put_string(&terminal, buffer);
    terminal_put_string(&terminal, "\n");

    itohexstr(kernel_code_segment, buffer);
    terminal_put_string(&terminal, "Kernel Code Segment: 0x");
    terminal_put_string(&terminal, buffer);
    terminal_put_string(&terminal, "\n");

    itohexstr(kernel_data_segment, buffer);
    terminal_put_string(&terminal, "Kernel Data Segment: 0x");
    terminal_put_string(&terminal, buffer);
    itohexstr(user_code_segment, buffer);
    terminal_put_string(&terminal, "\n");

    terminal_put_string(&terminal, "User Code Segment: 0x");
    terminal_put_string(&terminal, buffer);
    itohexstr(user_data_segment, buffer);
    terminal_put_string(&terminal, "\n");

    terminal_put_string(&terminal, "User Data Segment: 0x");
    terminal_put_string(&terminal, buffer);
    terminal_put_string(&terminal, "\n");


    while (1) {
        graphics_clear(0x00000F); // Fill screen with black
        terminal_put_string(&terminal, "test..");
        terminal_draw(&terminal);
        pit_sleep_ms(200);
    }


    exit();
}