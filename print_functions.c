#include "main.h"

/**
 * print_char - prints a character
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed (1)
 */
int print_char(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    char c = va_arg(args, int);
    (void)info;
    return (buffer_char(c, buffer, buff_ind));
}

/**
 * print_string - prints a string
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_string(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    char *str = va_arg(args, char *);
    (void)info;
    return (buffer_string(str, buffer, buff_ind));
}

/**
 * print_custom_string - prints a string with special handling for non-printable chars
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_custom_string(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    char *str = va_arg(args, char *);
    int count = 0;
    unsigned char c;
    char hex_digits[] = "0123456789ABCDEF";

    (void)info;

    if (str == NULL)
        str = "(null)";

    while (*str)
    {
        c = (unsigned char)*str;
        
        if (c < 32 || c >= 127)
        {
            buffer_char('\\', buffer, buff_ind);
            buffer_char('x', buffer, buff_ind);
            buffer_char(hex_digits[(c >> 4) & 0x0F], buffer, buff_ind);
            buffer_char(hex_digits[c & 0x0F], buffer, buff_ind);
            count += 4;
        }
        else
        {
            buffer_char(c, buffer, buff_ind);
            count++;
        }
        str++;
    }
    return (count);
}

/**
 * print_pointer - prints a pointer address
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_pointer(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    void *ptr = va_arg(args, void *);
    unsigned long address;
    char hex_digits[] = "0123456789abcdef";
    int count = 0;
    int started = 0;
    int digit;
    unsigned long mask = 0xF;
    int shift;

    (void)info;

    if (ptr == NULL)
        return (buffer_string("(nil)", buffer, buff_ind));
    
    buffer_char('0', buffer, buff_ind);
    buffer_char('x', buffer, buff_ind);
    count += 2;

    address = (unsigned long)ptr;
    shift = (sizeof(void *) * 8) - 4;

    if (address == 0)
    {
        buffer_char('0', buffer, buff_ind);
        return (count + 1);
    }

    for (; shift >= 0; shift -= 4)
    {
        digit = (address >> shift) & mask;
        
        if (digit != 0 || started)
        {
            buffer_char(hex_digits[digit], buffer, buff_ind);
            count++;
            started = 1;
        }
    }

    return (count);
}

/**
 * print_percent - prints a percent sign
 * @args: arguments list (unused)
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed (1)
 */
int print_percent(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    (void)args;
    (void)info;
    return (buffer_char('%', buffer, buff_ind));
}

/**
 * print_int - prints an integer with flags
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_int(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    int n = va_arg(args, int);
    return (print_number(n, buffer, buff_ind, info));
}

/**
 * print_number - prints a number recursively with flags
 * @n: number to print
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_number(int n, char buffer[], int *buff_ind, format_info_t info)
{
    int count = 0;
    unsigned int num;

    /* Handle flags for positive numbers */
    if (n >= 0)
    {
        if (info.flags & FLAG_PLUS)
        {
            buffer_char('+', buffer, buff_ind);
            count++;
        }
        else if (info.flags & FLAG_SPACE)
        {
            buffer_char(' ', buffer, buff_ind);
            count++;
        }
    }

    if (n < 0)
    {
        buffer_char('-', buffer, buff_ind);
        count++;
        num = -n;
    }
    else
    {
        num = n;
    }

    if (num / 10)
        count += print_number(num / 10, buffer, buff_ind, info);

    count += buffer_char("0123456789"[num % 10], buffer, buff_ind);
    return (count);
}

/**
 * print_binary - prints an unsigned int in binary
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_binary(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned int n = va_arg(args, unsigned int);
    (void)info;
    return (print_binary_recursive(n, buffer, buff_ind, info));
}

/**
 * print_binary_recursive - prints binary recursively
 * @n: number to print in binary
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_binary_recursive(unsigned int n, char buffer[], int *buff_ind, format_info_t info)
{
    int count = 0;

    (void)info;

    if (n / 2)
        count += print_binary_recursive(n / 2, buffer, buff_ind, info);

    count += buffer_char("01"[n % 2], buffer, buff_ind);
    return (count);
}

/**
 * print_unsigned - prints an unsigned integer
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_unsigned(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned int n = va_arg(args, unsigned int);
    (void)info;
    return (print_unsigned_number(n, 10, "0123456789", buffer, buff_ind, info));
}

/**
 * print_octal - prints an unsigned integer in octal with # flag
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_octal(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned int n = va_arg(args, unsigned int);
    int count = 0;

    /* Handle # flag for octal */
    if ((info.flags & FLAG_HASH) && n != 0)
    {
        buffer_char('0', buffer, buff_ind);
        count++;
    }

    count += print_unsigned_number(n, 8, "01234567", buffer, buff_ind, info);
    return (count);
}

/**
 * print_hex_lower - prints an unsigned integer in hexadecimal (lowercase) with # flag
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_hex_lower(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned int n = va_arg(args, unsigned int);
    int count = 0;

    /* Handle # flag for hexadecimal */
    if ((info.flags & FLAG_HASH) && n != 0)
    {
        buffer_char('0', buffer, buff_ind);
        buffer_char('x', buffer, buff_ind);
        count += 2;
    }

    count += print_unsigned_number(n, 16, "0123456789abcdef", buffer, buff_ind, info);
    return (count);
}

/**
 * print_hex_upper - prints an unsigned integer in hexadecimal (uppercase) with # flag
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_hex_upper(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned int n = va_arg(args, unsigned int);
    int count = 0;

    /* Handle # flag for hexadecimal */
    if ((info.flags & FLAG_HASH) && n != 0)
    {
        buffer_char('0', buffer, buff_ind);
        buffer_char('X', buffer, buff_ind);
        count += 2;
    }

    count += print_unsigned_number(n, 16, "0123456789ABCDEF", buffer, buff_ind, info);
    return (count);
}

/**
 * print_unsigned_number - prints unsigned number in given base
 * @n: number to print
 * @base: base for conversion (8, 10, 16)
 * @digits: digits to use for the base
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_unsigned_number(unsigned int n, unsigned int base, const char *digits, char buffer[], int *buff_ind, format_info_t info)
{
    int count = 0;

    (void)info;

    if (n / base)
        count += print_unsigned_number(n / base, base, digits, buffer, buff_ind, info);

    count += buffer_char(digits[n % base], buffer, buff_ind);
    return (count);
}
