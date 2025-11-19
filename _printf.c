#include "main.h"

/**
 * flush_buffer - writes buffer contents to stdout
 * @buffer: character buffer array
 * @buff_ind: pointer to buffer index
 *
 * Return: 0 on success
 *
 * Flushes the buffer content to standard output when called
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
 * buffer_char - adds a character to buffer, flushes if full
 * @c: character to add to buffer
 * @buffer: character buffer array
 * @buff_ind: pointer to buffer index
 *
 * Return: 1 (number of characters added)
 *
 * Manages buffer writing with automatic flush when buffer is full
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
 * parse_number - parses a number from format string
 * @format: format string being processed
 * @i: pointer to current index in format string
 *
 * Return: parsed integer value
 *
 * Extracts numerical values from format string for width/precision
 */
int parse_number(const char *format, int *i)
{
    int num = 0;

    while (format[*i] >= '0' && format[*i] <= '9')
    {
        num = num * 10 + (format[*i] - '0');
        (*i)++;
    }
    (*i)--; /* Adjust for main loop increment */

    return (num);
}

/**
 * handle_specifier - processes format specifiers with modifiers
 * @format: format string
 * @i: pointer to current index in format string
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed for this specifier, -1 on error
 *
 * Handles the complete processing of format specifiers including:
 * - Precision (.number)
 * - Length modifiers (l, h)
 * - Conversion specifiers (c, s, d, i, etc.)
 */
int handle_specifier(const char *format, int *i, va_list args, char buffer[], int *buff_ind)
{
    int count = 0;
    format_info_t info = {LENGTH_NONE, -1, 0};

    (*i)++; /* Skip the '%' character */
    
    if (format[*i] == '\0') /* Handle trailing % */
        return (-1);

    /* Parse precision specification */
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
            (*i)--; /* Adjust for cases like "%.s" */
        }
    }

    /* Parse length modifiers (l for long, h for short) */
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

    if (format[*i] == '\0') /* Handle incomplete specifier */
        return (-1);

    /* Route to appropriate handler based on conversion specifier */
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
    else /* Handle unknown specifier by printing % and the character */
    {
        buffer_char('%', buffer, buff_ind);
        buffer_char(format[*i], buffer, buff_ind);
        count = 2;
    }

    return (count);
}

/**
 * _printf - produces formatted output according to format string
 * @format: character string containing zero or more directives
 *
 * Return: number of characters printed (excluding null byte)
 *
 * Custom printf implementation that supports:
 * - Basic conversion specifiers: c, s, %, d, i, u, o, x, X, p, b, S
 * - Precision handling (.number)
 * - Length modifiers: l, h
 * - Buffer management for efficient output
 */
int _printf(const char *format, ...)
{
    va_list args;
    char buffer[BUFFER_SIZE];
    int buff_ind = 0;
    int count = 0;
    int i = 0;
    int specifier_count;

    if (format == NULL) /* Handle NULL format string */
        return (-1);

    va_start(args, format);

    /* Process each character in format string */
    while (format[i])
    {
        if (format[i] == '%') /* Format specifier found */
        {
            specifier_count = handle_specifier(format, &i, args, buffer, &buff_ind);
            if (specifier_count == -1) /* Error in specifier */
            {
                flush_buffer(buffer, &buff_ind);
                va_end(args);
                return (-1);
            }
            count += specifier_count;
        }
        else /* Regular character */
        {
            count += buffer_char(format[i], buffer, &buff_ind);
        }
        i++;
    }

    /* Flush any remaining characters in buffer */
    flush_buffer(buffer, &buff_ind);
    va_end(args);

    return (count);
}
