#include "main.h"

/**
 * print_char - prints a character
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed (1)
 */
int print_char(va_list args, char buffer[], int *buff_ind)
{
    char c = va_arg(args, int);
    return (buffer_char(c, buffer, buff_ind));
}

/**
 * print_string - prints a string
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_string(va_list args, char buffer[], int *buff_ind)
{
    char *str = va_arg(args, char *);
    return (buffer_string(str, buffer, buff_ind));
}

/**
 * print_percent - prints a percent sign
 * @args: arguments list (unused)
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed (1)
 */
int print_percent(va_list args, char buffer[], int *buff_ind)
{
    (void)args;
    return (buffer_char('%', buffer, buff_ind));
}

/**
 * print_int - prints an integer
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_int(va_list args, char buffer[], int *buff_ind)
{
    int n = va_arg(args, int);
    return (print_number(n, buffer, buff_ind));
}

/**
 * print_number - prints a number recursively
 * @n: number to print
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_number(int n, char buffer[], int *buff_ind)
{
    int count = 0;
    unsigned int num;

    if (n < 0)
    {
        count += buffer_char('-', buffer, buff_ind);
        num = -n;
    }
    else
    {
        num = n;
    }

    if (num / 10)
        count += print_number(num / 10, buffer, buff_ind);

    count += buffer_char("0123456789"[num % 10], buffer, buff_ind);
    return (count);
}

/**
 * print_binary - prints an unsigned int in binary
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_binary(va_list args, char buffer[], int *buff_ind)
{
    unsigned int n = va_arg(args, unsigned int);
    return (print_binary_recursive(n, buffer, buff_ind));
}

/**
 * print_binary_recursive - prints binary recursively
 * @n: number to print in binary
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_binary_recursive(unsigned int n, char buffer[], int *buff_ind)
{
    int count = 0;

    if (n / 2)
        count += print_binary_recursive(n / 2, buffer, buff_ind);

    count += buffer_char("01"[n % 2], buffer, buff_ind);
    return (count);
}

/**
 * print_unsigned - prints an unsigned integer
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_unsigned(va_list args, char buffer[], int *buff_ind)
{
    unsigned int n = va_arg(args, unsigned int);
    return (print_unsigned_number(n, 10, "0123456789", buffer, buff_ind));
}

/**
 * print_octal - prints an unsigned integer in octal
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_octal(va_list args, char buffer[], int *buff_ind)
{
    unsigned int n = va_arg(args, unsigned int);
    return (print_unsigned_number(n, 8, "01234567", buffer, buff_ind));
}

/**
 * print_hex_lower - prints an unsigned integer in hexadecimal (lowercase)
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_hex_lower(va_list args, char buffer[], int *buff_ind)
{
    unsigned int n = va_arg(args, unsigned int);
    return (print_unsigned_number(n, 16, "0123456789abcdef", buffer, buff_ind));
}

/**
 * print_hex_upper - prints an unsigned integer in hexadecimal (uppercase)
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_hex_upper(va_list args, char buffer[], int *buff_ind)
{
    unsigned int n = va_arg(args, unsigned int);
    return (print_unsigned_number(n, 16, "0123456789ABCDEF", buffer, buff_ind));
}

/**
 * print_unsigned_number - prints unsigned number in given base
 * @n: number to print
 * @base: base for conversion (8, 10, 16)
 * @digits: digits to use for the base
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_unsigned_number(unsigned int n, unsigned int base, const char *digits, char buffer[], int *buff_ind)
{
    int count = 0;

    if (n / base)
        count += print_unsigned_number(n / base, base, digits, buffer, buff_ind);

    count += buffer_char(digits[n % base], buffer, buff_ind);
    return (count);
}
