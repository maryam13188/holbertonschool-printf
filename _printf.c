#include "main.h"

/**
 * parse_width - parse field width from format string
 * @format: format string
 * @i: pointer to current index
 * @args: variable arguments list
 *
 * Return: field width value
 */
int parse_width(const char *format, int *i, va_list args)
{
    int width = 0;
    int j = *i;

    while (format[j] >= '0' && format[j] <= '9')
    {
        width = width * 10 + (format[j] - '0');
        j++;
    }

    *i = j - 1;
    return (width);
}

/**
 * handle_specifier - handle format specifiers (BASIC + FIELD WIDTH)
 * @format: format string
 * @i: pointer to current index
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed for specifier
 */
int handle_specifier(const char *format, int *i, va_list args, char buffer[], int *buff_ind)
{
    int count = 0;
    int width = 0;

    (*i)++; /* Skip % */
    
    if (format[*i] == '\0')
        return (-1);

    /* Parse field width - SIMPLE */
    if (format[*i] >= '0' && format[*i] <= '9')
    {
        width = parse_width(format, i, args);
    }

    if (format[*i] == '\0')
        return (-1);

    /* Handle specifiers - BASIC */
    if (format[*i] == 'c')
    {
        char c = va_arg(args, int);
        count += buffer_char(c, buffer, buff_ind);
    }
    else if (format[*i] == 's')
    {
        char *str = va_arg(args, char *);
        count += buffer_string(str, buffer, buff_ind);
    }
    else if (format[*i] == 'S')
    {
        count = print_custom_string(args, buffer, buff_ind, (format_info_t){0, LENGTH_NONE, 0});
    }
    else if (format[*i] == 'p')
    {
        count = print_pointer(args, buffer, buff_ind, (format_info_t){0, LENGTH_NONE, 0});
    }
    else if (format[*i] == '%')
    {
        count += buffer_char('%', buffer, buff_ind);
    }
    else if (format[*i] == 'd' || format[*i] == 'i')
    {
        count = print_int(args, buffer, buff_ind, (format_info_t){0, LENGTH_NONE, 0});
    }
    else if (format[*i] == 'b')
    {
        count = print_binary(args, buffer, buff_ind, (format_info_t){0, LENGTH_NONE, 0});
    }
    else if (format[*i] == 'u')
    {
        count = print_unsigned(args, buffer, buff_ind, (format_info_t){0, LENGTH_NONE, 0});
    }
    else if (format[*i] == 'o')
    {
        count = print_octal(args, buffer, buff_ind, (format_info_t){0, LENGTH_NONE, 0});
    }
    else if (format[*i] == 'x')
    {
        count = print_hex_lower(args, buffer, buff_ind, (format_info_t){0, LENGTH_NONE, 0});
    }
    else if (format[*i] == 'X')
    {
        count = print_hex_upper(args, buffer, buff_ind, (format_info_t){0, LENGTH_NONE, 0});
    }
    else
    {
        /* Unknown specifier - print as is */
        buffer_char('%', buffer, buff_ind);
        buffer_char(format[*i], buffer, buff_ind);
        count = 2;
    }

    return (count);
}

/**
 * _printf - produces output according to a format
 * @format: character string containing directives
 *
 * Return: number of characters printed (excluding null byte)
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
