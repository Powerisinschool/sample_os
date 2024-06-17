#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "utils.h"    // Custom header for additional utility functions

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

/**
 * vga_color - Enumerates hardware text mode color constants.
 */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15
};

/**
 * vga_entry_color - Combines foreground and background colors into a VGA color byte.
 * @fg: Foreground color.
 * @bg: Background color.
 *
 * Return: VGA color byte composed of fg and bg colors.
 */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

/**
 * vga_entry - Creates a VGA character entry with specific color attributes.
 * @uc: Character to display.
 * @color: Color attributes (foreground and background).
 *
 * Return: VGA character entry with specified attributes.
 */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;		// Width of the VGA text buffer
static const size_t VGA_HEIGHT = 25;	// Height of the VGA text buffer

size_t terminal_row;					// Current row position in the terminal.
size_t terminal_column;					// Current column position in the terminal.
uint8_t terminal_color;					// Current color attributes for terminal text.
uint16_t *terminal_buffer;				// Pointer to VGA text buffer memory.


/**
 * get_offset - Computes the memory offset for a given position in VGA text buffer.
 * @x: Column position.
 * @y: Row position.
 *
 * Return: Offset in memory corresponding to the position (x, y) in the VGA buffer.
 */
size_t get_offset(size_t y, size_t x) { return (y * VGA_WIDTH + x); }

/**
 * terminal_initialize - Initializes the VGA text mode terminal.
 *
 * This function sets up the terminal to start at the top-left corner,
 * clears the screen with default color attributes, and initializes
 * the VGA text buffer.
 */
void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t *) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

/**
 * terminal_setcolor - Sets the color attributes for the terminal text.
 * @color: Color byte to set (combination of foreground and background colors).
 *
 * This function updates the terminal text color to the specified color.
 */
void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

/**
 * terminal_putentryat - Puts a character with specified color at a specific position in the terminal.
 * @c: Character to display.
 * @color: Color attributes for the character.
 * @x: Column position in the terminal.
 * @y: Row position in the terminal.
 *
 * This function places a character with the specified color at the given
 * position (x, y) in the VGA text buffer.
 */
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

// void terminal_putchar(char c)
// {
//     size_t offset = get_offset(terminal_column, terminal_row);
//     if (c == '\n') {
//         terminal_column = 0;
//         terminal_row++;
//         // if (++terminal_row == VGA_HEIGHT)
//         //     terminal_row = 0;
//     } else {
//         terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
//         if (++terminal_column == VGA_WIDTH) {
//             terminal_column = 0;
//             terminal_row++;
//             // if (++terminal_row == VGA_HEIGHT)
//             //     terminal_row = 0;
//         }
//     }

//     // We'll implement our scroll here
//     if (terminal_row == VGA_HEIGHT) {
//         int i;
//         for (i = 1; i < VGA_HEIGHT; i++)
//             _memcpy(get_offset(0, i+1) + terminal_buffer,
//                     get_offset(0, i) + terminal_buffer,
//                     VGA_WIDTH);
        
//         char *last_line = get_offset(0, VGA_HEIGHT-1) + terminal_buffer;
//         for (i = 0; i < VGA_WIDTH; i++) last_line[i] = 0;
//         offset -= VGA_WIDTH;
//     }
// }

/**
 * terminal_scroll - Scrolls the terminal up by one line when reaching the bottom.
 *
 * When the terminal reaches the bottom (VGA_HEIGHT), this function scrolls
 * the entire screen up by one line. It clears the last line and adjusts
 * the terminal_row pointer to maintain the visible screen within bounds.
 */
void terminal_scroll()
{
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            size_t current_offset = get_offset(x, y);
            size_t prev_offset = get_offset(x, y - 1);
            terminal_buffer[prev_offset] = terminal_buffer[current_offset];
        }
    }

    size_t last_line_offset = get_offset(0, VGA_HEIGHT - 1);
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[last_line_offset + x] = vga_entry(' ', terminal_color);
    }

    terminal_row = VGA_HEIGHT - 1;
}

/**
 * terminal_putchar - Displays a character at the current cursor position in the terminal.
 * @c: Character to display.
 *
 * This function displays the character @c at the current cursor position in the
 * VGA text buffer. It handles newline characters by moving to the beginning of
 * the next line and implements scrolling when reaching the bottom of the screen.
 */
void terminal_putchar(char c)
{
    // size_t offset = get_offset(terminal_column, terminal_row);

    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_scroll();
        }
    } else if (c == '\r') {
		terminal_column = 0;
	} else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {
                terminal_scroll();
            }
        }
    }
}

/**
 * terminal_write - Displays a string of characters in the terminal.
 * @data: Pointer to the string to display.
 * @size: Number of characters in the string.
 *
 * This function displays the string of characters pointed to by @data
 * with length @size in the VGA text buffer by calling terminal_putchar
 * for each character.
 */
void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

/**
 * terminal_writestring - Displays a null-terminated string in the terminal.
 * @data: Pointer to the null-terminated string to display.
 *
 * This function displays the null-terminated string pointed to by @data
 * in the VGA text buffer by calling terminal_write with the string length.
 */
void terminal_writestring(const char *data)
{
    terminal_write(data, _strlen(data));
}

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
    for (int i = 0; i < 26; i++) {
        char str[255];
        _itoa(i, str);
        terminal_writestring(str);
        terminal_putchar('\n');
    }
    // Lines 1-3 should be scrolled off
    terminal_writestring("Hello, kernel World\nThis is Tolu speaking!\n");
}
