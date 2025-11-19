#include "main.h"

/**
 * flush_buffer - writes buffer contents to stdout
 */
int flush_buffer(char buffer[], int *buff_ind)
{
    if (*buff_ind > 0)
    {
        write(1, buffer, *buff_ind);
        *buff_ind = 0;
    }
    return (0);
}

/**
 * buffer_char - adds a character to buffer
 */
int buffer_char(char c, char buffer[], int *buff_ind)
{
    if (*buff_ind >= BUFFER_SIZE)
        flush_buffer(buffer, buff_ind);

    buffer[*buff_ind] = c;
    (*buff_ind)++;
    return (1);
}

/**
 * buffer_string - adds a string to buffer
 */
int buffer_string(char *str, char buffer[], int *buff_ind)
{
    int count = 0;

    if (str == NULL)
        str = "(null)";

    while (*str)
    {
        count += buffer_char(*str, buffer, buff_ind);
        str++;
    }
    return (count);
}

/**
 * parse_number - parses a number from format string
 */
int parse_number(const char *format, int *i)
{
    int num = 0;

    while (format[*i] >= '0' && format[*i] <= '9')
    {
        num = num * 10 + (format[*i] - '0');
        (*i)++;
    }
    (*i)--;

    return (num);
}

/**
 * handle_specifier - handle format specifiers
 */
int handle_specifier(const char *format, int *i, va_list args, char buffer[], int *buff_ind)
{
    int count = 0;
    format_info_t info = {LENGTH_NONE, -1, 0};

    (*i)++;
    
    if (format[*i] == '\0')
        return (-1);

    /* Parse precision */
    if (format[*i] == '.')
    {
        (*i)++;
        info.has_precision = 1;
        info.precision = 0;
        
        if (format[*i] >= '0' && format[*i] <= '9')
        {
            info.precision = parse_number(format, i);
        }
        else
        {
            (*i)--;
        }
    }

    /* Parse length modifiers */
    if (format[*i] == 'l')
    {
        info.length = LENGTH_L;
        (*i)++;
    }
    else if (format[*i] == 'h')
    {
        info.length = LENGTH_H;
        (*i)++;
    }

    if (format[*i] == '\0')
        return (-1);

    /* Handle specifiers */
    if (format[*i] == 'c')
        count = print_char(args, buffer, buff_ind, info);
    else if (format[*i] == 's')
        count = print_string(args, buffer, buff_ind, info);
    else if (format[*i] == 'S')
        count = print_custom_string(args, buffer, buff_ind, info);
    else if (format[*i] == 'p')
        count = print_pointer(args, buffer, buff_ind, info);
    else if (format[*i] == '%')
        count = print_percent(args, buffer, buff_ind, info);
    else if (format[*i] == 'd' || format[*i] == 'i')
        count = print_int(args, buffer, buff_ind, info);
    else if (format[*i] == 'b')
        count = print_binary(args, buffer, buff_ind, info);
    else if (format[*i] == 'u')
        count = print_unsigned(args, buffer, buff_ind, info);
    else if (format[*i] == 'o')
        count = print_octal(args, buffer, buff_ind, info);
    else if (format[*i] == 'x')
        count = print_hex_lower(args, buffer, buff_ind, info);
    else if (format[*i] == 'X')
        count = print_hex_upper(args, buffer, buff_ind, info);
    else
    {
        buffer_char('%', buffer, buff_ind);
        buffer_char(format[*i], buffer, buff_ind);
        count = 2;
    }

    return (count);
}

/**
 * _printf - produces output according to a format
 */
int _printf(const char *format, ...)
{
    va_list args;
    char buffer[BUFFER_SIZE];
    int buff_ind = 0;
    int count = 0;
    int i = 0;
    int specifier_count;

    if (format == NULL)
        return (-1);

    va_start(args, format);

    while (format[i])
    {
        if (format[i] == '%')
        {
            specifier_count = handle_specifier(format, &i, args, buffer, &buff_ind);
            if (specifier_count == -1)
            {
                flush_buffer(buffer, &buff_ind);
                va_end(args);
                return (-1);
            }
            count += specifier_count;
        }
        else
        {
            count += buffer_char(format[i], buffer, &buff_ind);
        }
        i++;
    }

    flush_buffer(buffer, &buff_ind);
    va_end(args);

    return (count);
}
