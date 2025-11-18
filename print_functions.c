#include "main.h"
#include <limits.h>

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
 * print_custom_string - prints a string with special handling for non-printable chars
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_custom_string(va_list args, char buffer[], int *buff_ind)
{
    char *str = va_arg(args, char *);
    int count = 0;
    unsigned char c;

    if (str == NULL)
        str = "(null)";

    while (*str)
    {
        c = (unsigned char)*str;
        
        if (c < 32 || c >= 127)
        {
            buffer_char('\\', buffer, buff_ind);
            buffer_char('x', buffer, buff_ind);
            count += 2;
            count += print_hex_byte(c, buffer, buff_ind);
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
 * print_hex_byte - prints a byte as 2-digit hexadecimal (uppercase)
 * @c: byte to print
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed (always 2)
 */
int print_hex_byte(unsigned char c, char buffer[], int *buff_ind)
{
    int count = 0;
    char hex_digits[] = "0123456789ABCDEF";

    buffer_char(hex_digits[(c >> 4) & 0x0F], buffer, buff_ind);
    count++;
    
    buffer_char(hex_digits[c & 0x0F], buffer, buff_ind);
    count++;

    return (count);
}

/**
 * print_pointer - prints a pointer address
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_pointer(va_list args, char buffer[], int *buff_ind)
{
    void *ptr = va_arg(args, void *);
    
    if (ptr == NULL)
        return (buffer_string("(nil)", buffer, buff_ind));
    
    buffer_char('0', buffer, buff_ind);
    buffer_char('x', buffer, buff_ind);
    
    return (2 + print_pointer_address(ptr, buffer, buff_ind));
}

/**
 * print_pointer_address - prints pointer address in hexadecimal
 * @ptr: pointer to print
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int print_pointer_address(void *ptr, char buffer[], int *buff_ind)
{
    unsigned long address = (unsigned long)ptr;
    int count = 0;
    char hex_digits[] = "0123456789abcdef";
    int started = 0;
    int digit;
    unsigned long mask = 0xF;
    int shift = (sizeof(void *) * 8) - 4;

    if (address == 0)
    {
        buffer_char('0', buffer, buff_ind);
        return (1);
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

/* ========== QUESTION 8 - FLAG SUPPORT ========== */

/**
 * parse_flags - parse flag characters
 * @format: format string
 * @i: pointer to current index
 *
 * Return: flags bitmask
 */
int parse_flags(const char *format, int *i)
{
    int flags = 0;
    int j = *i + 1;

    while (format[j] == '+' || format[j] == ' ' || format[j] == '#')
    {
        if (format[j] == '+')
            flags |= FLAG_PLUS;
        else if (format[j] == ' ')
            flags |= FLAG_SPACE;
        else if (format[j] == '#')
            flags |= FLAG_HASH;
        j++;
    }

    *i = j - 1; /* Move index to last flag character */
    return (flags);
}

/**
 * handle_specifier_with_flags - handle format specifiers with flags support
 * @format: format string
 * @i: pointer to current index
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed for specifier
 */
int handle_specifier_with_flags(const char *format, int *i, va_list args, char buffer[], int *buff_ind)
{
    int count = 0;
    int flags = 0;
    int start_index = *i;
    char specifier;

    (*i)++;
    if (format[*i] == '\0')
        return (-1);

    /* Parse flags */
    flags = parse_flags(format, i);

    /* Get the final specifier after flags */
    specifier = format[*i];

    /* Handle specifier with flags */
    if (specifier == 'c')
        count = print_char(args, buffer, buff_ind);
    else if (specifier == 's')
        count = print_string(args, buffer, buff_ind);
    else if (specifier == 'S')
        count = print_custom_string(args, buffer, buff_ind);
    else if (specifier == 'p')
        count = print_pointer(args, buffer, buff_ind);
    else if (specifier == '%')
        count = print_percent(args, buffer, buff_ind);
    else if (specifier == 'd' || specifier == 'i')
        count = print_int_with_flags(args, buffer, buff_ind, flags);
    else if (specifier == 'b')
        count = print_binary(args, buffer, buff_ind);
    else if (specifier == 'u')
        count = print_unsigned(args, buffer, buff_ind);
    else if (specifier == 'o')
        count = print_octal_with_flags(args, buffer, buff_ind, flags);
    else if (specifier == 'x')
        count = print_hex_lower_with_flags(args, buffer, buff_ind, flags);
    else if (specifier == 'X')
        count = print_hex_upper_with_flags(args, buffer, buff_ind, flags);
    else
    {
        /* Print the % and all flags and the specifier */
        int k;
        for (k = start_index; k <= *i; k++)
        {
            buffer_char(format[k], buffer, buff_ind);
            count++;
        }
    }

    return (count);
}

/**
 * print_int_with_flags - prints an integer with flags
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @flags: flags bitmask
 *
 * Return: number of characters printed
 */
int print_int_with_flags(va_list args, char buffer[], int *buff_ind, int flags)
{
    int n = va_arg(args, int);
    int count = 0;

    /* Handle + flag for positive numbers */
    if (n >= 0 && (flags & FLAG_PLUS))
    {
        buffer_char('+', buffer, buff_ind);
        count++;
    }
    /* Handle space flag for positive numbers (if no + flag) */
    else if (n >= 0 && (flags & FLAG_SPACE) && !(flags & FLAG_PLUS))
    {
        buffer_char(' ', buffer, buff_ind);
        count++;
    }

    count += print_number(n, buffer, buff_ind);
    return (count);
}

/**
 * print_octal_with_flags - prints an unsigned integer in octal with flags
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @flags: flags bitmask
 *
 * Return: number of characters printed
 */
int print_octal_with_flags(va_list args, char buffer[], int *buff_ind, int flags)
{
    unsigned int n = va_arg(args, unsigned int);
    int count = 0;

    /* Handle # flag for octal - prefix with 0 if number is not zero */
    if ((flags & FLAG_HASH) && n != 0)
    {
        buffer_char('0', buffer, buff_ind);
        count++;
    }

    count += print_unsigned_number(n, 8, "01234567", buffer, buff_ind);
    return (count);
}

/**
 * print_hex_lower_with_flags - prints an unsigned integer in hexadecimal (lowercase) with flags
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @flags: flags bitmask
 *
 * Return: number of characters printed
 */
int print_hex_lower_with_flags(va_list args, char buffer[], int *buff_ind, int flags)
{
    unsigned int n = va_arg(args, unsigned int);
    int count = 0;

    /* Handle # flag for hexadecimal - prefix with 0x if number is not zero */
    if ((flags & FLAG_HASH) && n != 0)
    {
        buffer_char('0', buffer, buff_ind);
        buffer_char('x', buffer, buff_ind);
        count += 2;
    }

    count += print_unsigned_number(n, 16, "0123456789abcdef", buffer, buff_ind);
    return (count);
}

/**
 * print_hex_upper_with_flags - prints an unsigned integer in hexadecimal (uppercase) with flags
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @flags: flags bitmask
 *
 * Return: number of characters printed
 */
int print_hex_upper_with_flags(va_list args, char buffer[], int *buff_ind, int flags)
{
    unsigned int n = va_arg(args, unsigned int);
    int count = 0;

    /* Handle # flag for hexadecimal - prefix with 0X if number is not zero */
    if ((flags & FLAG_HASH) && n != 0)
    {
        buffer_char('0', buffer, buff_ind);
        buffer_char('X', buffer, buff_ind);
        count += 2;
    }

    count += print_unsigned_number(n, 16, "0123456789ABCDEF", buffer, buff_ind);
    return (count);
}
