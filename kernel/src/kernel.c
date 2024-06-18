// #include "utils.h"    // Custom header for additional utility functions

#include <kernel/tty.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __linux__
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#ifndef __i386__
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

/**
 * kernel_main - Main entry point of the kernel.
 *
 * Initializes the terminal, prints a sequence of numbers, and writes
 * introductory messages to the VGA text buffer.
 */
void kernel_main(void)
{
    terminal_initialize();

	// Print numbers from 0 to 25, each on a new line
    // for (int i = 0; i < 26; i++) {
    //     char str[255];
    //     _itoa(i, str);
    //     terminal_write_string(str);
    //     terminal_putchar('\n');
    // }
    // Lines 1-3 should be off screen
    // terminal_write_string("Hello, kernel World\nThis is Tolu speaking!\n");

    print("Tolu's bash\n\nbash >> ");
}
