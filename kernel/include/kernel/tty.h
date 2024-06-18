#ifndef KERNEL_TTY_H
#define KERNEL_TTY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_write_string(const char* data);
size_t get_offset(size_t x, size_t y);

#endif
