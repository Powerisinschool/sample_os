#include <stddef.h>

/**
 * memset - function that fills memory with constant byte
 * @s: character 1
 * @b: character 2
 * @n: integer
 * Return: a pointer to the memory
 */
char *memset(void *s, int b, size_t n)
{
	unsigned char *p = s;
	while (n--)
	{
		*p++ = (unsigned char) b;
	}
	return (s);
}