#include <stdlib.h>

extern unsigned char end;
static unsigned char *heap_end = &end;

void *malloc(size_t size)
{
    unsigned char *prev_heap_end = heap_end;
    heap_end += size;
    return prev_heap_end;
}