/**
 * strcmp -  a function that compares two strings
 * @s1: string 1
 * @s2: string 2
 * Return: s1 s2
 */
int strcmp(const char *s1, const char *s2)
{
	int i = 0;
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}