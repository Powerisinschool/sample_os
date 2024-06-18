#include <kernel/tty.h>
#include <stdlib.h>
#include <string.h>

static const size_t VGA_WIDTH = 80;		// Width of the VGA text buffer
static const size_t VGA_HEIGHT = 25;	// Height of the VGA text buffer

size_t terminal_row;					// Current row position in the terminal.
size_t terminal_column;					// Current column position in the terminal.
uint8_t terminal_color;					// Current color attributes for terminal text.
uint16_t *terminal_buffer;				// Pointer to VGA text buffer memory.


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
 * terminal_scroll - Scrolls the terminal up by one line when reaching the bottom.
 *
 * When the terminal reaches the bottom (VGA_HEIGHT), this function scrolls
 * the entire screen up by one line. It clears the last line and adjusts
 * the terminal_row pointer to maintain the visible screen within bounds.
 */
void terminal_scroll()
{
    if (terminal_row < VGA_HEIGHT)
        return;
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
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }
    }
    terminal_scroll();
}

/**
 * terminal_write_string - Displays a null-terminated string in the terminal.
 * @data: Pointer to the null-terminated string to display.
 *
 * This function displays the null-terminated string pointed to by @data
 * in the VGA text buffer by calling terminal_write with the string length.
 */
void terminal_write_string(const char *data)
{
    terminal_write(data, strlen(data));
}

/**
 * get_offset - Computes the memory offset for a given position in VGA text buffer.
 * @x: Column position.
 * @y: Row position.
 *
 * Return: Offset in memory corresponding to the position (x, y) in the VGA buffer.
 */
size_t get_offset(size_t x, size_t y) { return (y * VGA_WIDTH + x); }
