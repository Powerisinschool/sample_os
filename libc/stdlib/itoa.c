/**
 *  _itoa - a function that convert an integer to a string
 *  @n: integer to be converted
 *  @str: buffer to store the converted string
 */
char *_itoa(int value, char str[])
{
	int k = value;
	int i = 0;

	if (k < 0)
	{
		i++;
		str[0] = '-';
		k *= -1;
		value *= -1;
	}

	while (k > 0) {
		k /= 10;
		i++;
	}

	k = value;

	while (k > 0)
	{
		str[--i] = (k % 10) + '0';
		k /= 10;
	}
	return (str);
}