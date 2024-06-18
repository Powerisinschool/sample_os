#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>

void *malloc(size_t size);
void free(void *ptr);
int atoi(const char *nptr);
char *_itoa(int value, char str[]);

__attribute__((__noreturn__))
void abort(void);

#endif // STDLIB_H