/**
 *  atoi - a function that convert a string to an integer
 *  @nptr: string to be converted
 *
 *  Return: integer value of converted string
 */
int atoi(const char *nptr)
{
	int sign = 1;
	unsigned int num = 0;

	do {
		if (*nptr == '-')
			sign *= -1;

		else if (*nptr >= '0' && *nptr <= '9')
			num = (num * 10) + (*nptr - '0');
		else if (num > 0)
			break;

	} while (*nptr++);

	return (num * sign);
}