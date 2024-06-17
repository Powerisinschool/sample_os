#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "utils.h"

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

size_t get_offset(size_t y, size_t x);

/* Hardware text mode color constants. */
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

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

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

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

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

void terminal_putchar(char c)
{
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            // Scroll up
            for (size_t y = 1; y < VGA_HEIGHT; y++) {
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    size_t current_offset = get_offset(x, y);
                    size_t prev_offset = get_offset(x, y - 1);
                    terminal_buffer[prev_offset] = terminal_buffer[current_offset];
                }
            }

            // Clear the last line
            size_t last_line_offset = get_offset(0, VGA_HEIGHT - 1);
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                terminal_buffer[last_line_offset + x] = vga_entry(' ', terminal_color);
            }

            terminal_row = VGA_HEIGHT - 1;
        }
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {
                // Scroll up
                for (size_t y = 1; y < VGA_HEIGHT; y++) {
                    for (size_t x = 0; x < VGA_WIDTH; x++) {
                        size_t current_offset = get_offset(x, y);
                        size_t prev_offset = get_offset(x, y - 1);
                        terminal_buffer[prev_offset] = terminal_buffer[current_offset];
                    }
                }

                // Clear the last line
                size_t last_line_offset = get_offset(0, VGA_HEIGHT - 1);
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    terminal_buffer[last_line_offset + x] = vga_entry(' ', terminal_color);
                }

                terminal_row = VGA_HEIGHT - 1;
            }
        }
    }
}

void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char *data)
{
    terminal_write(data, _strlen(data));
}

size_t get_offset(size_t x, size_t y) { return (y * VGA_WIDTH + x); }

void kernel_main(void)
{
    terminal_initialize();
    int i = 0;
    for (i = 0; i < 26; i++) {
        char str[255];
        _itoa(i, str);
        terminal_writestring(str);
        terminal_putchar('\n');
    }
    // Lines 1-3 should be scrolled off
    terminal_writestring("Hello, kernel World\nThis is Tolu speaking!\n");
}
