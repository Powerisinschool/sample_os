#include <string.h>

/**
 * strlen - computes the length of a string
 * @str: pointer to the string
 *
 * Return: length of the string
 */
size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}