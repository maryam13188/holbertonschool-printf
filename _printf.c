#include "main.h"
#include <stdarg.h>

/**
 * print_unsigned - prints an unsigned integer using _putchar
 * @num: number to print
 *
 * Return: number of printed characters
 */
static int print_unsigned(unsigned int num)
{
	int count = 0;

	if (num / 10)
		count += print_unsigned(num / 10);

	_putchar((num % 10) + '0');
	count++;

	return (count);
}

/**
 * print_integer - prints an integer using _putchar
 * @n: integer to print
 *
 * Return: number of printed characters
 */
static int print_integer(int n)
{
	unsigned int num;
	int count = 0;

	if (n < 0)
	{
		_putchar('-');
		count++;
		num = (unsigned int)(-n);
	}
	else
	{
		num = (unsigned int)n;
	}

	count += print_unsigned(num);

	return (count);
}

/**
 * _printf - simplified printf that handles %d and %i
 * @format: format string
 *
 * Return: number of printed characters, or -1 on error
 */
int _printf(const char *format, ...)
{
	va_list ap;
	int i = 0;
	int count = 0;

	if (format == NULL)
		return (-1);

	va_start(ap, format);

	while (format[i] != '\0')
	{
		if (format[i] != '%')
		{
			_putchar(format[i]);
			count++;
		}
		else
		{
			i++;

			if (format[i] == '\0')
			{
				va_end(ap);
				return (-1);
			}

			if (format[i] == 'd' || format[i] == 'i')
			{
				int n = va_arg(ap, int);

				count += print_integer(n);
			}
			else if (format[i] == '%')
			{
				_putchar('%');
				count++;
			}
			else
			{
				_putchar('%');
				_putchar(format[i]);
				count += 2;
			}
		}
		i++;
	}

	va_end(ap);
	return (count);
}

