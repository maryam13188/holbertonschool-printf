#include "main.h"

/**
 * print_char - prints a character
 * @args: arguments list
 *
 * Return: number of characters printed (1)
 */
int print_char(va_list args)
{
char c = va_arg(args, int);

return (write(1, &c, 1));
}

/**
 * print_string - prints a string
 * @args: arguments list
 *
 * Return: number of characters printed
 */
int print_string(va_list args)
{
char *str = va_arg(args, char *);
int count = 0;

if (str == NULL)
str = "(null)";

while (str[count])
{
write(1, &str[count], 1);
count++;
}

return (count);
}

/**
 * print_percent - prints a percent sign
 * @args: arguments list (unused)
 *
 * Return: number of characters printed (1)
 */
int print_percent(va_list args)
{
(void)args;
return (write(1, "%", 1));
}

/**
 * print_int - prints an integer
 * @args: arguments list
 *
 * Return: number of characters printed
 */
int print_int(va_list args)
{
int n = va_arg(args, int);
return (print_number(n));
}

/**
 * print_number - prints a number recursively
 * @n: number to print
 *
 * Return: number of characters printed
 */
int print_number(int n)
{
int count = 0;
unsigned int num;

if (n < 0)
{
count += write(1, "-", 1);
num = -n;
}
else
{
num = n;
}

if (num / 10)
count += print_number(num / 10);

count += write(1, &"0123456789"[num % 10], 1);
return (count);
}

/**
 * print_binary - prints an unsigned int in binary
 * @args: arguments list
 *
 * Return: number of characters printed
 */
int print_binary(va_list args)
{
unsigned int n = va_arg(args, unsigned int);
return (print_binary_recursive(n));
}

/**
 * print_binary_recursive - prints binary recursively
 * @n: number to print in binary
 *
 * Return: number of characters printed
 */
int print_binary_recursive(unsigned int n)
{
int count = 0;

if (n / 2)
count += print_binary_recursive(n / 2);

count += write(1, &"01"[n % 2], 1);
return (count);
}

/**
 * print_unsigned - prints an unsigned integer
 * @args: arguments list
 *
 * Return: number of characters printed
 */
int print_unsigned(va_list args)
{
unsigned int n = va_arg(args, unsigned int);
return (print_unsigned_number(n, 10, "0123456789"));
}

/**
 * print_octal - prints an unsigned integer in octal
 * @args: arguments list
 *
 * Return: number of characters printed
 */
int print_octal(va_list args)
{
unsigned int n = va_arg(args, unsigned int);
return (print_unsigned_number(n, 8, "01234567"));
}

/**
 * print_hex_lower - prints an unsigned integer in hexadecimal (lowercase)
 * @args: arguments list
 *
 * Return: number of characters printed
 */
int print_hex_lower(va_list args)
{
unsigned int n = va_arg(args, unsigned int);
return (print_unsigned_number(n, 16, "0123456789abcdef"));
}

/**
 * print_hex_upper - prints an unsigned integer in hexadecimal (uppercase)
 * @args: arguments list
 *
 * Return: number of characters printed
 */
int print_hex_upper(va_list args)
{
unsigned int n = va_arg(args, unsigned int);
return (print_unsigned_number(n, 16, "0123456789ABCDEF"));
}

/**
 * print_unsigned_number - prints unsigned number in given base
 * @n: number to print
 * @base: base for conversion (8, 10, 16)
 * @digits: digits to use for the base
 *
 * Return: number of characters printed
 */
int print_unsigned_number(unsigned int n, unsigned int base, const char *digits)
{
int count = 0;

if (n / base)
count += print_unsigned_number(n / base, base, digits);

count += write(1, &digits[n % base], 1);
return (count);

