#include "main.h"

/**
 * print_char - prints a single character
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure
 *
 * Return: number of characters printed (always 1)
 */
int print_char(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    char c = va_arg(args, int);
    (void)info; /* Unused parameter */
    return (buffer_char(c, buffer, buff_ind));
}

/**
 * print_string - prints a string with precision support
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure
 *
 * Return: number of characters printed
 *
 * Handles string printing with precision limitation
 * Precision specifies maximum number of characters to print
 */
int print_string(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    char *str = va_arg(args, char *);
    int count = 0;
    int max_chars = info.has_precision ? info.precision : INT_MAX;

    if (str == NULL)
        str = "(null)";

    /* Print characters up to precision limit or string end */
    while (*str && max_chars > 0)
    {
        count += buffer_char(*str, buffer, buff_ind);
        str++;
        max_chars--;
    }
    return (count);
}

/**
 * print_custom_string - prints string with non-printable character handling
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure
 *
 * Return: number of characters printed
 *
 * Custom %S handler that prints non-printable chars as \xHH
 * where HH is the hexadecimal ASCII value
 */
int print_custom_string(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    char *str = va_arg(args, char *);
    int count = 0;
    unsigned char c;
    char hex_digits[] = "0123456789ABCDEF";
    int max_chars = info.has_precision ? info.precision : INT_MAX;

    if (str == NULL)
        str = "(null)";

    while (*str && max_chars > 0)
    {
        c = (unsigned char)*str;
        
        /* Check if character is non-printable */
        if (c < 32 || c >= 127)
        {
            /* Print as \xHH format */
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
        max_chars--;
    }
    return (count);
}

/**
 * print_pointer - prints pointer address in hexadecimal
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure
 *
 * Return: number of characters printed
 *
 * Prints memory addresses in 0xffffffffffff format
 * Handles NULL pointers by printing "(nil)"
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

    (void)info; /* Unused parameter */

    if (ptr == NULL)
        return (buffer_string("(nil)", buffer, buff_ind));
    
    /* Print "0x" prefix */
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
 * print_percent - prints a percent sign
 * @args: variable arguments list (unused)
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure (unused)
 *
 * Return: number of characters printed (always 1)
 */
int print_percent(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    (void)args;
    (void)info;
    return (buffer_char('%', buffer, buff_ind));
}

/**
 * print_int - prints integer with length modifiers and precision
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure
 *
 * Return: number of characters printed
 *
 * Handles both %d and %i specifiers with:
 * - Length modifiers (l, h)
 * - Precision specification
 * - Negative numbers
 */
int print_int(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    long n;
    
    /* Handle length modifiers */
    if (info.length == LENGTH_L)
        n = va_arg(args, long);
    else if (info.length == LENGTH_H)
        n = (short)va_arg(args, int);
    else
        n = va_arg(args, int);
        
    /* Use precision-aware printing if precision specified */
    if (info.has_precision)
        return (print_number_precision(n, buffer, buff_ind, info));
    else
        return (print_number(n, buffer, buff_ind, info));
}

/**
 * print_number - prints number recursively (without precision)
 * @n: number to print
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure
 *
 * Return: number of characters printed
 *
 * Recursive function to print numbers digit by digit
 */
int print_number(long n, char buffer[], int *buff_ind, format_info_t info)
{
    int count = 0;
    unsigned long num;

    (void)info;

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

    /* Recursively print higher digits first */
    if (num / 10)
        count += print_number(num / 10, buffer, buff_ind, info);

    count += buffer_char("0123456789"[num % 10], buffer, buff_ind);
    return (count);
}

/**
 * print_number_precision - prints number with precision handling
 * @n: number to print
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure
 *
 * Return: number of characters printed
 *
 * Implements precision for integers by adding leading zeros
 * Precision specifies minimum number of digits to display
 */
int print_number_precision(long n, char buffer[], int *buff_ind, format_info_t info)
{
    int count = 0;
    unsigned long num;
    char num_str[32];
    int num_len = 0;
    int i, zeros_needed;

    /* Special case: zero value with zero precision prints nothing */
    if (n == 0 && info.precision == 0)
        return (0);

    /* Handle negative numbers */
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

    /* Convert number to string in reverse order */
    if (num == 0)
    {
        num_str[num_len++] = '0';
    }
    else
    {
        while (num > 0)
        {
            num_str[num_len++] = '0' + (num % 10);
            num /= 10;
        }
    }

    /* Calculate how many leading zeros are needed */
    zeros_needed = info.precision - num_len;
    if (zeros_needed < 0) 
        zeros_needed = 0;

    /* Add leading zeros to meet precision requirement */
    for (i = 0; i < zeros_needed; i++)
        count += buffer_char('0', buffer, buff_ind);

    /* Print the actual digits in correct order */
    for (i = num_len - 1; i >= 0; i--)
        count += buffer_char(num_str[i], buffer, buff_ind);

    return (count);
}

/**
 * print_binary - prints unsigned integer in binary format
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure
 *
 * Return: number of characters printed
 *
 * Custom %b specifier that converts numbers to binary
 */
int print_binary(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned int n = va_arg(args, unsigned int);
    int count = 0;

    (void)info;

    /* Recursively print binary digits */
    if (n / 2)
        count += print_binary(n / 2, buffer, buff_ind, info);

    count += buffer_char("01"[n % 2], buffer, buff_ind);
    return (count);
}

/**
 * print_unsigned - prints unsigned integer with precision
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure
 *
 * Return: number of characters printed
 *
 * Handles %u specifier with length modifiers and precision
 */
int print_unsigned(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long n;
    int count = 0;
    char num_str[32];
    int num_len = 0;
    int i, zeros_needed;
    
    /* Handle length modifiers */
    if (info.length == LENGTH_L)
        n = va_arg(args, unsigned long);
    else if (info.length == LENGTH_H)
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);

    /* Handle zero value with zero precision */
    if (n == 0 && info.has_precision && info.precision == 0)
        return (0);

    /* Convert number to string in reverse order */
    if (n == 0)
    {
        num_str[num_len++] = '0';
    }
    else
    {
        while (n > 0)
        {
            num_str[num_len++] = '0' + (n % 10);
            n /= 10;
        }
    }

    /* Calculate leading zeros needed for precision */
    zeros_needed = 0;
    if (info.has_precision)
        zeros_needed = info.precision - num_len;
    if (zeros_needed < 0) 
        zeros_needed = 0;

    /* Add leading zeros */
    for (i = 0; i < zeros_needed; i++)
        count += buffer_char('0', buffer, buff_ind);

    /* Print digits in correct order */
    for (i = num_len - 1; i >= 0; i--)
        count += buffer_char(num_str[i], buffer, buff_ind);

    return (count);
}

/**
 * print_octal - prints unsigned integer in octal with precision
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure
 *
 * Return: number of characters printed
 *
 * Handles %o specifier with base-8 conversion
 */
int print_octal(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long n;
    int count = 0;
    char num_str[32];
    int num_len = 0;
    int i, zeros_needed;
    
    if (info.length == LENGTH_L)
        n = va_arg(args, unsigned long);
    else if (info.length == LENGTH_H)
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);

    if (n == 0 && info.has_precision && info.precision == 0)
        return (0);

    /* Convert to octal (base 8) */
    if (n == 0)
    {
        num_str[num_len++] = '0';
    }
    else
    {
        while (n > 0)
        {
            num_str[num_len++] = '0' + (n % 8);
            n /= 8;
        }
    }

    zeros_needed = 0;
    if (info.has_precision)
        zeros_needed = info.precision - num_len;
    if (zeros_needed < 0) 
        zeros_needed = 0;

    for (i = 0; i < zeros_needed; i++)
        count += buffer_char('0', buffer, buff_ind);

    for (i = num_len - 1; i >= 0; i--)
        count += buffer_char(num_str[i], buffer, buff_ind);

    return (count);
}

/**
 * print_hex_lower - prints unsigned integer in lowercase hexadecimal
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure
 *
 * Return: number of characters printed
 *
 * Handles %x specifier with base-16 conversion (lowercase)
 */
int print_hex_lower(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long n;
    int count = 0;
    char num_str[32];
    int num_len = 0;
    int i, zeros_needed;
    char hex_digits[] = "0123456789abcdef";
    
    if (info.length == LENGTH_L)
        n = va_arg(args, unsigned long);
    else if (info.length == LENGTH_H)
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);

    if (n == 0 && info.has_precision && info.precision == 0)
        return (0);

    /* Convert to hexadecimal (base 16) lowercase */
    if (n == 0)
    {
        num_str[num_len++] = '0';
    }
    else
    {
        while (n > 0)
        {
            num_str[num_len++] = hex_digits[n % 16];
            n /= 16;
        }
    }

    zeros_needed = 0;
    if (info.has_precision)
        zeros_needed = info.precision - num_len;
    if (zeros_needed < 0) 
        zeros_needed = 0;

    for (i = 0; i < zeros_needed; i++)
        count += buffer_char('0', buffer, buff_ind);

    for (i = num_len - 1; i >= 0; i--)
        count += buffer_char(num_str[i], buffer, buff_ind);

    return (count);
}

/**
 * print_hex_upper - prints unsigned integer in uppercase hexadecimal
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information structure
 *
 * Return: number of characters printed
 *
 * Handles %X specifier with base-16 conversion (uppercase)
 */
int print_hex_upper(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long n;
    int count = 0;
    char num_str[32];
    int num_len = 0;
    int i, zeros_needed;
    char hex_digits[] = "0123456789ABCDEF";
    
    if (info.length == LENGTH_L)
        n = va_arg(args, unsigned long);
    else if (info.length == LENGTH_H)
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);

    if (n == 0 && info.has_precision && info.precision == 0)
        return (0);

    /* Convert to hexadecimal (base 16) uppercase */
    if (n == 0)
    {
        num_str[num_len++] = '0';
    }
    else
    {
        while (n > 0)
        {
            num_str[num_len++] = hex_digits[n % 16];
            n /= 16;
        }
    }

    zeros_needed = 0;
    if (info.has_precision)
        zeros_needed = info.precision - num_len;
    if (zeros_needed < 0) 
        zeros_needed = 0;

    for (i = 0; i < zeros_needed; i++)
        count += buffer_char('0', buffer, buff_ind);

    for (i = num_len - 1; i >= 0; i--)
        count += buffer_char(num_str[i], buffer, buff_ind);

    return (count);
}
