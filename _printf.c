#include "main.h"

/**
 * flush_buffer - flush buffer to stdout
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int flush_buffer(char buffer[], int *buff_ind)
{
    int count = 0;

    if (*buff_ind > 0)
    {
        count = write(1, buffer, *buff_ind);
        *buff_ind = 0;
    }

    return (count);
}

/**
 * buffer_char - buffer a character
 * @c: character to buffer
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: 1 (number of characters buffered)
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
 * buffer_string - buffer a string
 * @str: string to buffer
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters buffered
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
 * parse_flags - parse flag characters from format string
 * @format: format string
 * @i: pointer to current index
 * @info: pointer to format info structure
 *
 * Return: number of flag characters parsed
 */
int parse_flags(const char *format, int *i, format_info_t *info)
{
    int j = *i + 1;
    int count = 0;

    info->flags = 0;
    
    while (format[j] == '+' || format[j] == ' ' || format[j] == '#')
    {
        if (format[j] == '+')
            info->flags |= FLAG_PLUS;
        else if (format[j] == ' ')
            info->flags |= FLAG_SPACE;
        else if (format[j] == '#')
            info->flags |= FLAG_HASH;
        
        j++;
        count++;
    }
    
    *i = j - 1;
    return (count);
}

/**
 * handle_specifier - handle format specifiers with flags
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
    format_info_t info = {0};

    (*i)++;
    if (format[*i] == '\0')
        return (-1);

    /* Parse flags */
    parse_flags(format, i, &info);

    if (format[*i] == '\0')
        return (-1);

    /* Handle specifiers with flags */
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
