#include "memory.h"
#include <graphics/text/text.h>
#include "hardware/pit.h"
#include "terminal/terminal.h"
// Exit function.
static void exit(void) {
    for (;;) {
        asm ("hlt");
    }
}

void kmain(void) {
    if (!limine_ok()) {
        exit();
    }

    Terminal terminal;
    terminal_initialize(&terminal, 50, 50, TERMINAL_WIDTH, TERMINAL_HEIGHT);
    terminal_put_string(&terminal, "Hello, World! this is some text\n");
    
    while (1) {
        graphics_clear(0x00000F); // Fill screen with black
        terminal_put_string(&terminal, "test..");
        terminal_draw(&terminal);
        pit_sleep_ms(200);
    }


    exit();
}