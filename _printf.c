#include "main.h"

/**
 * _printf - produces output according to a format
 * @format: character string containing directives
 *
 * Return: number of characters printed (excluding null byte)
 */
int _printf(const char *format, ...)
{
	va_list args;
	int count = 0;

	if (format == NULL)
		return (-1);

	va_start(args, format);
	count = process_format(format, args);
	va_end(args);

	return (count);
}

/**
 * process_format - process the format string
 * @format: format string to process
 * @args: variable arguments list
 *
 * Return: number of characters printed
 */
int process_format(const char *format, va_list args)
{
	int count = 0, i = 0;

	while (format[i])
	{
		if (format[i] == '%')
			count += handle_specifier(format, &i, args);
		else
			count += write_char(format[i]);
		i++;
	}
	return (count);
}

/**
 * handle_specifier - handle format specifiers
 * @format: format string
 * @i: pointer to current index
 * @args: variable arguments list
 *
 * Return: number of characters printed for specifier
 */
int handle_specifier(const char *format, int *i, va_list args)
{
	int count = 0;

	(*i)++;
	if (format[*i] == '\0')
		return (-1);

	if (format[*i] == 'c')
		count = print_char(args);
	else if (format[*i] == 's')
		count = print_string(args);
	else if (format[*i] == '%')
		count = print_percent(args);
	else
		count = write_specifier(format[*i]);

	return (count);
}

/**
 * write_char - write a single character
 * @c: character to write
 *
 * Return: 1 (number of characters written)
 */
int write_char(char c)
{
	return (write(1, &c, 1));
}

/**
 * write_specifier - write unknown specifier
 * @c: specifier character
 *
 * Return: number of characters written (always 2)
 */
int write_specifier(char c)
{
	write(1, "%", 1);
	write(1, &c, 1);
	return (2);
}
