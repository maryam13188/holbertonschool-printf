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

    if (format[j] == '*')
    {
        width = va_arg(args, int);
        (*i)++;
        return (width);
    }

    while (format[j] >= '0' && format[j] <= '9')
    {
        width = width * 10 + (format[j] - '0');
        j++;
    }

    *i = j - 1;
    return (width);
}

/**
 * apply_field_width - apply field width padding
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @info: format information
 * @num_chars: number of characters in the output
 * @is_negative: whether the number is negative
 *
 * Return: number of padding characters added
 */
int apply_field_width(char buffer[], int *buff_ind, format_info_t info, int num_chars, int is_negative)
{
    int padding = 0;
    int i;

    (void)is_negative;

    if (info.width <= num_chars)
        return (0);

    padding = info.width - num_chars;

    for (i = 0; i < padding; i++)
    {
        buffer_char(' ', buffer, buff_ind);
    }

    return (padding);
}

/**
 * handle_specifier - handle format specifiers with field width
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
    int original_count = 0;
    format_info_t info = {0, LENGTH_NONE, 0};

    (*i)++; /* Skip % */
    
    if (format[*i] == '\0')
        return (-1);

    /* Parse field width */
    if (format[*i] == '*')
    {
        info.width = parse_width(format, i, args);
    }
    else if (format[*i] >= '0' && format[*i] <= '9')
    {
        info.width = parse_width(format, i, args);
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
    {
        original_count = print_char(args, buffer, buff_ind, info);
        count = original_count;
        if (info.width > 1)
        {
            count += apply_field_width(buffer, buff_ind, info, original_count, 0);
        }
    }
    else if (format[*i] == 's')
    {
        original_count = print_string(args, buffer, buff_ind, info);
        count = original_count;
        if (info.width > original_count)
        {
            count += apply_field_width(buffer, buff_ind, info, original_count, 0);
        }
    }
    else if (format[*i] == 'S')
    {
        count = print_custom_string(args, buffer, buff_ind, info);
    }
    else if (format[*i] == 'p')
    {
        count = print_pointer(args, buffer, buff_ind, info);
    }
    else if (format[*i] == '%')
    {
        original_count = print_percent(args, buffer, buff_ind, info);
        count = original_count;
        if (info.width > 1)
        {
            count += apply_field_width(buffer, buff_ind, info, original_count, 0);
        }
    }
    else if (format[*i] == 'd' || format[*i] == 'i')
    {
        original_count = print_int(args, buffer, buff_ind, info);
        count = original_count;
        if (info.width > original_count)
        {
            count += apply_field_width(buffer, buff_ind, info, original_count, 0);
        }
    }
    else if (format[*i] == 'b')
    {
        count = print_binary(args, buffer, buff_ind, info);
    }
    else if (format[*i] == 'u')
    {
        original_count = print_unsigned(args, buffer, buff_ind, info);
        count = original_count;
        if (info.width > original_count)
        {
            count += apply_field_width(buffer, buff_ind, info, original_count, 0);
        }
    }
    else if (format[*i] == 'o')
    {
        original_count = print_octal(args, buffer, buff_ind, info);
        count = original_count;
        if (info.width > original_count)
        {
            count += apply_field_width(buffer, buff_ind, info, original_count, 0);
        }
    }
    else if (format[*i] == 'x')
    {
        original_count = print_hex_lower(args, buffer, buff_ind, info);
        count = original_count;
        if (info.width > original_count)
        {
            count += apply_field_width(buffer, buff_ind, info, original_count, 0);
        }
    }
    else if (format[*i] == 'X')
    {
        original_count = print_hex_upper(args, buffer, buff_ind, info);
        count = original_count;
        if (info.width > original_count)
        {
            count += apply_field_width(buffer, buff_ind, info, original_count, 0);
        }
    }
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
