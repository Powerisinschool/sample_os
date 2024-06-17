#include <stddef.h>
/**
 * _strlen - computes the length of a string
 * @str: pointer to the string
 *
 * Return: length of the string
 */
size_t _strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

/**
 * _memcpy - copies memory area
 * @dest: memory area
 * @src: memory area
 * @n: bytes
 * Return: a pointer to dest
 */

char *_memcpy(char *dest, char *src, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		dest[i] = src[i];

	return (dest);
}

/**
 *  _atoi - a function that convert a string to an integer
 *  @s: string to be converted
 *
 *  Return: integer value of converted string
 */

int _atoi(char *s)
{
	int sign = 1;
	unsigned int num = 0;

	do {
		if (*s == '-')
			sign *= -1;

		else if (*s >= '0' && *s <= '9')
			num = (num * 10) + (*s - '0');
		else if (num > 0)
			break;

	} while (*s++);

	return (num * sign);
}

/**
 * rev_string -  a function that reverses a string
 * @s: string
 */

void rev_string(char *s)
{
	size_t i = _strlen((const char *) s);

	size_t j = 0;

	char temp[(const size_t) i+1];

	for (; i > 0; i--)
	{
		temp[j] = s[i - 1];
		j++;
	}

	while (i < j)
	{
		s[i] = temp[i];
		i++;
	}
}

/**
 *  _itoa - a function that convert an integer to a string
 *  @n: integer to be converted
 *  @str: buffer to store the converted string
 */

void _itoa(int n, char str[])
{
	int i, sign;
	if ((sign = n) < 0) n = -n;
	i = 0;
	do {
		str[i++] = n%10 + '0';
	} while ((n /= 10) > 0);

	if (sign < 0) str[i++] = '-';
	str[i] = '\0';
	rev_string(str);
}
