#include "main.h"

/**
 * print_char - prints a single character
 */
int print_char(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    char c = va_arg(args, int);
    (void)info;
    return (buffer_char(c, buffer, buff_ind)); /* NOW THIS WILL WORK */
}

/**
 * print_string - prints a string with precision support
 */
int print_string(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    char *str = va_arg(args, char *);
    int count = 0;
    int max_chars = info.has_precision ? info.precision : INT_MAX;

    if (str == NULL)
        str = "(null)";

    while (*str && max_chars > 0)
    {
        count += buffer_char(*str, buffer, buff_ind); /* NOW THIS WILL WORK */
        str++;
        max_chars--;
    }
    return (count);
}

/**
 * print_pointer - prints pointer address
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
        return (buffer_string("(nil)", buffer, buff_ind)); /* NOW THIS WILL WORK */
    
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
 * print_binary - prints unsigned int in binary
 */
int print_binary(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned int n = va_arg(args, unsigned int);
    (void)info;
    return (print_binary_recursive(n, buffer, buff_ind, info)); /* FIXED: Use recursive function */
}

/**
 * print_binary_recursive - helper function for binary printing
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

/* باقي الدوال تبقى كما هي بدون تغيير */
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
        max_chars--;
    }
    return (count);
}

int print_percent(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    (void)args;
    (void)info;
    return (buffer_char('%', buffer, buff_ind));
}

int print_int(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    long n;
    
    if (info.length == LENGTH_L)
        n = va_arg(args, long);
    else if (info.length == LENGTH_H)
        n = (short)va_arg(args, int);
    else
        n = va_arg(args, int);
        
    if (info.has_precision)
        return (print_number_precision(n, buffer, buff_ind, info));
    else
        return (print_number(n, buffer, buff_ind, info));
}

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

    if (num / 10)
        count += print_number(num / 10, buffer, buff_ind, info);

    count += buffer_char("0123456789"[num % 10], buffer, buff_ind);
    return (count);
}

int print_number_precision(long n, char buffer[], int *buff_ind, format_info_t info)
{
    int count = 0;
    unsigned long num;
    char num_str[32];
    int num_len = 0;
    int i, zeros_needed;

    if (n == 0 && info.precision == 0)
        return (0);

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

    zeros_needed = info.precision - num_len;
    if (zeros_needed < 0) 
        zeros_needed = 0;

    for (i = 0; i < zeros_needed; i++)
        count += buffer_char('0', buffer, buff_ind);

    for (i = num_len - 1; i >= 0; i--)
        count += buffer_char(num_str[i], buffer, buff_ind);

    return (count);
}

int print_unsigned(va_list args, char buffer[], int *buff_ind, format_info_t info)
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
