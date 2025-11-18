#include "main.h"
#include <limits.h>

/**
 * calculate_number_length - calculates the length of a number
 * @n: number to calculate length for
 * @base: base of the number system
 *
 * Return: length of the number
 */
int calculate_number_length(long n, int base)
{
    int length = 0;
    unsigned long num;

    if (n < 0)
    {
        length++; /* for the minus sign */
        num = -n;
    }
    else
    {
        num = n;
    }

    if (n == 0)
        return (1);

    while (num > 0)
    {
        length++;
        num /= base;
    }

    return (length);
}

/**
 * calculate_unsigned_length - calculates the length of an unsigned number
 * @n: number to calculate length for
 * @base: base of the number system
 *
 * Return: length of the number
 */
int calculate_unsigned_length(unsigned long n, int base)
{
    int length = 0;
    unsigned long num = n;

    if (n == 0)
        return (1);

    while (num > 0)
    {
        length++;
        num /= base;
    }

    return (length);
}

/**
 * print_char - prints a character with field width
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_char(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    char c = va_arg(args, int);
    int count = 0;
    char pad_char = ' ';
    int width = info.width;

    if (width > 1 && !(info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - 1, buffer, buff_ind);
    }

    count += buffer_char(c, buffer, buff_ind);

    if (width > 1 && (info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - 1, buffer, buff_ind);
    }

    return (count);
}

/**
 * print_string - prints a string with field width
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
    int count = 0;
    char pad_char = ' ';
    int width = info.width;
    int str_len;

    if (str == NULL)
        str = "(null)";

    str_len = 0;
    while (str[str_len])
        str_len++;

    if (width > str_len && !(info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - str_len, buffer, buff_ind);
    }

    count += buffer_string(str, buffer, buff_ind);

    if (width > str_len && (info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - str_len, buffer, buff_ind);
    }

    return (count);
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
    char pad_char = ' ';
    int width = info.width;
    int str_len = 0;
    char *temp_str;

    if (str == NULL)
        str = "(null)";

    /* Calculate actual length with escape sequences */
    temp_str = str;
    while (*temp_str)
    {
        c = (unsigned char)*temp_str;
        if (c < 32 || c >= 127)
            str_len += 4; /* \xXX */
        else
            str_len++;
        temp_str++;
    }

    if (width > str_len && !(info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - str_len, buffer, buff_ind);
    }

    /* Print the actual string */
    temp_str = str;
    while (*temp_str)
    {
        c = (unsigned char)*temp_str;
       
        if (c < 32 || c >= 127)
        {
            buffer_char('\\', buffer, buff_ind);
            buffer_char('x', buffer, buff_ind);
            count += 2;
            count += print_hex_byte(c, buffer, buff_ind, info);
        }
        else
        {
            buffer_char(c, buffer, buff_ind);
            count++;
        }
        temp_str++;
    }

    if (width > str_len && (info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - str_len, buffer, buff_ind);
    }

    return (count);
}

/**
 * print_hex_byte - prints a byte as 2-digit hexadecimal (uppercase)
 * @c: byte to print
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed (always 2)
 */
int print_hex_byte(unsigned char c, char buffer[], int *buff_ind, format_info_t info)
{
    int count = 0;
    char hex_digits[] = "0123456789ABCDEF";

    (void)info; /* Width not used for hex bytes */

    /* Print first hex digit */
    buffer_char(hex_digits[(c >> 4) & 0x0F], buffer, buff_ind);
    count++;
   
    /* Print second hex digit */
    buffer_char(hex_digits[c & 0x0F], buffer, buff_ind);
    count++;

    return (count);
}

/**
 * print_pointer - prints a pointer address with field width
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
    int count = 0;
    char pad_char = ' ';
    int width = info.width;
    int ptr_len;

    if (ptr == NULL)
    {
        ptr_len = 5; /* "(nil)" */
        if (width > ptr_len && !(info.flags & FLAG_MINUS))
        {
            count += buffer_chars(pad_char, width - ptr_len, buffer, buff_ind);
        }
       
        count += buffer_string("(nil)", buffer, buff_ind);
       
        if (width > ptr_len && (info.flags & FLAG_MINUS))
        {
            count += buffer_chars(pad_char, width - ptr_len, buffer, buff_ind);
        }
       
        return (count);
    }

    /* Calculate pointer length: 2 for "0x" + hex digits */
    ptr_len = 2 + calculate_unsigned_length((unsigned long)ptr, 16);
   
    if (width > ptr_len && !(info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - ptr_len, buffer, buff_ind);
    }
   
    buffer_char('0', buffer, buff_ind);
    buffer_char('x', buffer, buff_ind);
    count += 2;
    count += print_pointer_address(ptr, buffer, buff_ind, info);
   
    if (width > ptr_len && (info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - ptr_len, buffer, buff_ind);
    }

    return (count);
}

/**
 * print_pointer_address - prints pointer address in hexadecimal
 * @ptr: pointer to print
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_pointer_address(void *ptr, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long address = (unsigned long)ptr;
    int count = 0;
    char hex_digits[] = "0123456789abcdef";
    int started = 0;
    int digit;
    unsigned long mask = 0xF;
    int shift = (sizeof(void *) * 8) - 4;

    (void)info; /* Width handled in calling function */

    /* Handle the case when address is 0 */
    if (address == 0)
    {
        buffer_char('0', buffer, buff_ind);
        return (1);
    }

    /* Print hexadecimal digits, skipping leading zeros */
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
 * print_percent - prints a percent sign with field width
 * @args: arguments list (unused)
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_percent(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    int count = 0;
    char pad_char = ' ';
    int width = info.width;

    (void)args;

    if (width > 1 && !(info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - 1, buffer, buff_ind);
    }

    count += buffer_char('%', buffer, buff_ind);

    if (width > 1 && (info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - 1, buffer, buff_ind);
    }

    return (count);
}

/**
 * print_int - prints an integer with flags, length modifiers, and field width
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_int(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    long n;
    int count = 0;
    char pad_char = ' ';
    int width = info.width;
    int num_length, total_length;
    int has_sign = 0;

    /* Handle length modifiers */
    if (info.length == LENGTH_L)
        n = va_arg(args, long);
    else if (info.length == LENGTH_H)
        n = (short)va_arg(args, int);
    else
        n = va_arg(args, int);

    /* Calculate number length */
    num_length = calculate_number_length(n, 10);
   
    /* Adjust for flags - only for positive numbers */
    if (n >= 0 && (info.flags & (FLAG_PLUS | FLAG_SPACE)))
    {
        num_length++;
        has_sign = 1;
    }

    total_length = num_length;

    /* Handle zero padding - sign should be before zeros */
    if ((info.flags & FLAG_ZERO) && !(info.flags & FLAG_MINUS) && width > total_length)
    {
        pad_char = '0';
        /* Print sign first if zero padding */
        if (has_sign)
        {
            if (info.flags & FLAG_PLUS)
                buffer_char('+', buffer, buff_ind);
            else if (info.flags & FLAG_SPACE)
                buffer_char(' ', buffer, buff_ind);
            count++;
            has_sign = 0; /* Don't print sign again in print_number */
        }
    }

    /* Handle right padding (left alignment) */
    if (width > total_length && !(info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - total_length, buffer, buff_ind);
    }

    /* If sign wasn't printed with zero padding, print it now */
    if (has_sign)
    {
        if (info.flags & FLAG_PLUS)
            buffer_char('+', buffer, buff_ind);
        else if (info.flags & FLAG_SPACE)
            buffer_char(' ', buffer, buff_ind);
        count++;
    }

    count += print_number(n, buffer, buff_ind, info);

    /* Handle left padding (right alignment) */
    if (width > total_length && (info.flags & FLAG_MINUS))
    {
        count += buffer_chars(' ', width - total_length, buffer, buff_ind);
    }

    return (count);
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
int print_number(long n, char buffer[], int *buff_ind, format_info_t info)
{
    int count = 0;
    unsigned long num;

    /* Handle sign in print_int, not here */
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
 * print_binary - prints an unsigned int in binary with field width
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
    int count = 0;
    char pad_char = ' ';
    int width = info.width;
    int bin_length;

    bin_length = calculate_unsigned_length(n, 2);
    if (n == 0)
        bin_length = 1;

    if (width > bin_length && !(info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - bin_length, buffer, buff_ind);
    }

    count += print_binary_recursive(n, buffer, buff_ind, info);

    if (width > bin_length && (info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - bin_length, buffer, buff_ind);
    }

    return (count);
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
int print_binary_recursive(unsigned long n, char buffer[], int *buff_ind, format_info_t info)
{
    int count = 0;

    (void)info; /* Flags not used for binary */

    if (n / 2)
        count += print_binary_recursive(n / 2, buffer, buff_ind, info);

    count += buffer_char("01"[n % 2], buffer, buff_ind);
    return (count);
}

/**
 * print_unsigned - prints an unsigned integer with length modifiers and field width
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_unsigned(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long n;
    int count = 0;
    char pad_char = ' ';
    int width = info.width;
    int num_length;

    /* Handle length modifiers */
    if (info.length == LENGTH_L)
        n = va_arg(args, unsigned long);
    else if (info.length == LENGTH_H)
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);

    num_length = calculate_unsigned_length(n, 10);

    /* Handle zero padding */
    if ((info.flags & FLAG_ZERO) && !(info.flags & FLAG_MINUS) && width > num_length)
        pad_char = '0';

    if (width > num_length && !(info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - num_length, buffer, buff_ind);
    }

    count += print_unsigned_number(n, 10, "0123456789", buffer, buff_ind, info);

    if (width > num_length && (info.flags & FLAG_MINUS))
    {
        count += buffer_chars(' ', width - num_length, buffer, buff_ind);
    }

    return (count);
}

/**
 * print_octal - prints an unsigned integer in octal with flags, length modifiers, and field width
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_octal(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long n;
    int count = 0;
    char pad_char = ' ';
    int width = info.width;
    int num_length, prefix_length = 0, total_length;

    /* Handle length modifiers */
    if (info.length == LENGTH_L)
        n = va_arg(args, unsigned long);
    else if (info.length == LENGTH_H)
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);

    num_length = calculate_unsigned_length(n, 8);

    /* Handle # flag for octal */
    if ((info.flags & FLAG_HASH) && n != 0)
        prefix_length = 1;

    total_length = num_length + prefix_length;

    /* Handle zero padding */
    if ((info.flags & FLAG_ZERO) && !(info.flags & FLAG_MINUS) && width > total_length)
        pad_char = '0';

    if (width > total_length && !(info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - total_length, buffer, buff_ind);
    }

    /* Handle # flag for octal */
    if ((info.flags & FLAG_HASH) && n != 0)
    {
        buffer_char('0', buffer, buff_ind);
        count++;
    }

    count += print_unsigned_number(n, 8, "01234567", buffer, buff_ind, info);

    if (width > total_length && (info.flags & FLAG_MINUS))
    {
        count += buffer_chars(' ', width - total_length, buffer, buff_ind);
    }

    return (count);
}

/**
 * print_hex_lower - prints an unsigned integer in hexadecimal (lowercase) with flags, length modifiers, and field width
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_hex_lower(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long n;
    int count = 0;
    char pad_char = ' ';
    int width = info.width;
    int num_length, prefix_length = 0, total_length;

    /* Handle length modifiers */
    if (info.length == LENGTH_L)
        n = va_arg(args, unsigned long);
    else if (info.length == LENGTH_H)
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);

    num_length = calculate_unsigned_length(n, 16);

    /* Handle # flag for hexadecimal */
    if ((info.flags & FLAG_HASH) && n != 0)
        prefix_length = 2;

    total_length = num_length + prefix_length;

    /* Handle zero padding */
    if ((info.flags & FLAG_ZERO) && !(info.flags & FLAG_MINUS) && width > total_length && !prefix_length)
        pad_char = '0';

    if (width > total_length && !(info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - total_length, buffer, buff_ind);
    }

    /* Handle # flag for hexadecimal */
    if ((info.flags & FLAG_HASH) && n != 0)
    {
        buffer_char('0', buffer, buff_ind);
        buffer_char('x', buffer, buff_ind);
        count += 2;
    }

    count += print_unsigned_number(n, 16, "0123456789abcdef", buffer, buff_ind, info);

    if (width > total_length && (info.flags & FLAG_MINUS))
    {
        count += buffer_chars(' ', width - total_length, buffer, buff_ind);
    }

    return (count);
}

/**
 * print_hex_upper - prints an unsigned integer in hexadecimal (uppercase) with flags, length modifiers, and field width
 * @args: arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 *
 * Return: number of characters printed
 */
int print_hex_upper(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long n;
    int count = 0;
    char pad_char = ' ';
    int width = info.width;
    int num_length, prefix_length = 0, total_length;

    /* Handle length modifiers */
    if (info.length == LENGTH_L)
        n = va_arg(args, unsigned long);
    else if (info.length == LENGTH_H)
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);

    num_length = calculate_unsigned_length(n, 16);

    /* Handle # flag for hexadecimal */
    if ((info.flags & FLAG_HASH) && n != 0)
        prefix_length = 2;

    total_length = num_length + prefix_length;

    /* Handle zero padding */
    if ((info.flags & FLAG_ZERO) && !(info.flags & FLAG_MINUS) && width > total_length && !prefix_length)
        pad_char = '0';

    if (width > total_length && !(info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - total_length, buffer, buff_ind);
    }

    /* Handle # flag for hexadecimal */
    if ((info.flags & FLAG_HASH) && n != 0)
    {
        buffer_char('0', buffer, buff_ind);
        buffer_char('X', buffer, buff_ind);
        count += 2;
    }

    count += print_unsigned_number(n, 16, "0123456789ABCDEF", buffer, buff_ind, info);

    if (width > total_length && (info.flags & FLAG_MINUS))
    {
        count += buffer_chars(' ', width - total_length, buffer, buff_ind);
    }

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
int print_unsigned_number(unsigned long n, unsigned int base, const char *digits, char buffer[], int *buff_ind, format_info_t info)
{
    int count = 0;

    (void)info; /* Flags and width handled in calling functions */

    if (n / base)
        count += print_unsigned_number(n / base, base, digits, buffer, buff_ind, info);

    count += buffer_char(digits[n % base], buffer, buff_ind);
    return (count);
}
