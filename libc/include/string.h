#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char *str);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
char *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int b, size_t n);

#endif // STRING_H