#include "main.h"
#include <stdarg.h>

/* Helper functions for padding */
static int handle_padding(int total_len, format_info_t info, char buffer[], int *buff_ind, int is_zero_pad)
{
    int count = 0;
    char pad_char = (is_zero_pad && (info.flags & FLAG_ZERO) && !(info.flags & FLAG_MINUS)) ? '0' : ' ';
   
    if (info.width > total_len && !(info.flags & FLAG_MINUS))
        count += buffer_chars(pad_char, info.width - total_len, buffer, buff_ind);
   
    return count;
}

static int handle_left_padding(int total_len, format_info_t info, char buffer[], int *buff_ind)
{
    int count = 0;
    if (info.width > total_len && (info.flags & FLAG_MINUS))
        count += buffer_chars(' ', info.width - total_len, buffer, buff_ind);
    return count;
}

/* Calculate number length */
static int calculate_number_length(long n, int base)
{
    int length = 0;
    unsigned long num;

    if (n == 0)
        return 1;

    if (n < 0)
        num = -n;
    else
        num = n;

    while (num > 0)
    {
        length++;
        num /= base;
    }

    return length;
}

static int calculate_unsigned_length(unsigned long n, int base)
{
    int length = 0;
    unsigned long num = n;

    if (n == 0)
        return 1;

    while (num > 0)
    {
        length++;
        num /= base;
    }

    return length;
}

/* Basic print functions */
int print_char(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    char c = va_arg(args, int);
    int count = handle_padding(1, info, buffer, buff_ind, 0);
    count += buffer_char(c, buffer, buff_ind);
    count += handle_left_padding(1, info, buffer, buff_ind);
    return count;
}

int print_string(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    char *str = va_arg(args, char *);
    int str_len = 0, count = 0;
   
    if (!str) str = "(null)";
    while (str[str_len]) str_len++;
   
    count += handle_padding(str_len, info, buffer, buff_ind, 0);
    count += buffer_string(str, buffer, buff_ind);
    count += handle_left_padding(str_len, info, buffer, buff_ind);
    return count;
}

int print_percent(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    (void)args;
    int count = handle_padding(1, info, buffer, buff_ind, 0);
    count += buffer_char('%', buffer, buff_ind);
    count += handle_left_padding(1, info, buffer, buff_ind);
    return count;
}

/* Helper function to print unsigned numbers */
static int print_unsigned_recursive(unsigned long num, int base, const char *digits,
                                   char buffer[], int *buff_ind)
{
    int count = 0;
   
    if (num / base)
        count += print_unsigned_recursive(num / base, base, digits, buffer, buff_ind);
   
    count += buffer_char(digits[num % base], buffer, buff_ind);
    return count;
}

/* Generic number printing function */
static int print_generic_number(long n, unsigned long un, int is_signed, int base,
                               const char *digits, format_info_t info,
                               char buffer[], int *buff_ind)
{
    int count = 0, length = 0, prefix_len = 0;
    unsigned long num;
    char sign = 0;
   
    /* Determine number and sign */
    if (is_signed) {
        num = (n < 0) ? -n : n;
        if (n < 0) sign = '-';
        else if (info.flags & FLAG_PLUS) sign = '+';
        else if (info.flags & FLAG_SPACE) sign = ' ';
    } else {
        num = un;
    }
   
    /* Calculate length */
    length = (num == 0) ? 1 : calculate_unsigned_length(num, base);
   
    /* Calculate prefix */
    if (sign) prefix_len++;
    if ((info.flags & FLAG_HASH) && num != 0) {
        if (base == 8) prefix_len++;
        else if (base == 16) prefix_len += 2;
    }
   
    int total_len = length + prefix_len;
    int is_zero_pad = (info.flags & FLAG_ZERO) && !(info.flags & FLAG_MINUS);
   
    /* Right padding */
    if (!is_zero_pad || prefix_len > 0) {
        count += handle_padding(total_len, info, buffer, buff_ind, 0);
    }
   
    /* Print sign */
    if (sign) {
        buffer_char(sign, buffer, buff_ind);
        count++;
    }
   
    /* Print hex/octal prefix */
    if ((info.flags & FLAG_HASH) && num != 0) {
        if (base == 8) {
            buffer_char('0', buffer, buff_ind);
            count++;
        }
        else if (base == 16) {
            buffer_char('0', buffer, buff_ind);
            buffer_char((digits[10] == 'a') ? 'x' : 'X', buffer, buff_ind);
            count += 2;
        }
    }
   
    /* Zero padding (if no prefix) */
    if (is_zero_pad && prefix_len == 0) {
        count += handle_padding(total_len, info, buffer, buff_ind, 1);
    }
   
    /* Print number */
    if (num == 0) {
        buffer_char('0', buffer, buff_ind);
        count++;
    } else {
        count += print_unsigned_recursive(num, base, digits, buffer, buff_ind);
    }
   
    /* Left padding */
    count += handle_left_padding(total_len, info, buffer, buff_ind);
   
    return count;
}

/* Wrapper functions */
int print_int(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    long n;
   
    if (info.length == LENGTH_L)
        n = va_arg(args, long);
    else if (info.length == LENGTH_H)
        n = (short)va_arg(args, int);
    else
        n = va_arg(args, int);
       
    return print_generic_number(n, 0, 1, 10, "0123456789", info, buffer, buff_ind);
}

int print_unsigned(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long n;
   
    if (info.length == LENGTH_L)
        n = va_arg(args, unsigned long);
    else if (info.length == LENGTH_H)
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);
       
    return print_generic_number(0, n, 0, 10, "0123456789", info, buffer, buff_ind);
}

int print_octal(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long n;
   
    if (info.length == LENGTH_L)
        n = va_arg(args, unsigned long);
    else if (info.length == LENGTH_H)
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);
       
    return print_generic_number(0, n, 0, 8, "01234567", info, buffer, buff_ind);
}

int print_hex_lower(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long n;
   
    if (info.length == LENGTH_L)
        n = va_arg(args, unsigned long);
    else if (info.length == LENGTH_H)
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);
       
    return print_generic_number(0, n, 0, 16, "0123456789abcdef", info, buffer, buff_ind);
}

int print_hex_upper(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned long n;
   
    if (info.length == LENGTH_L)
        n = va_arg(args, unsigned long);
    else if (info.length == LENGTH_H)
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);
       
    return print_generic_number(0, n, 0, 16, "0123456789ABCDEF", info, buffer, buff_ind);
}

int print_binary(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    unsigned int n = va_arg(args, unsigned int);
    return print_generic_number(0, n, 0, 2, "01", info, buffer, buff_ind);
}

/* Special functions */
int print_custom_string(va_list args, char buffer[], int *buff_ind, format_info_t info)
{
    char *str = va_arg(args, char *);
    int count = 0;
    char pad_char = ' ';
    int width = info.width;
    int str_len = 0;
    char *temp_str;
    unsigned char c;

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
           
            /* Print hex digits manually */
            char hex_digits[] = "0123456789ABCDEF";
            buffer_char(hex_digits[(c >> 4) & 0x0F], buffer, buff_ind);
            buffer_char(hex_digits[c & 0x0F], buffer, buff_ind);
            count += 2;
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

    return count;
}

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
   
    /* Print pointer address */
    unsigned long address = (unsigned long)ptr;
    if (address == 0)
    {
        buffer_char('0', buffer, buff_ind);
        count++;
    }
    else
    {
        count += print_unsigned_recursive(address, 16, "0123456789abcdef", buffer, buff_ind);
    }
   
    if (width > ptr_len && (info.flags & FLAG_MINUS))
    {
        count += buffer_chars(pad_char, width - ptr_len, buffer, buff_ind);
    }

    return (count);
}
