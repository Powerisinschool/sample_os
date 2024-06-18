#include <string.h>

/**
 * memcpy - copies memory area
 * @dest: memory area
 * @src: memory area
 * @n: bytes
 * Return: a pointer to dest
 */
char *memcpy(void *dest, const void *src, size_t n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;

	while(n--)
		*d++ = *s++;

	return (dest);
}