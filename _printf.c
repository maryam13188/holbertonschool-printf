#include "main.h"

/**
 * flush_buffer - writes the buffer to stdout
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters written
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
 * @c: character to add
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: 1 (character was buffered)
 */
int buffer_char(char c, char buffer[], int *buff_ind)
{
    if (*buff_ind == BUFFER_SIZE)
        flush_buffer(buffer, buff_ind);
    
    buffer[*buff_ind] = c;
    (*buff_ind)++;
    return (1);
}

/**
 * buffer_string - adds a string to buffer
 * @str: string to add
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

    while (str[count])
    {
        buffer_char(str[count], buffer, buff_ind);
        count++;
    }
    return (count);
}

/**
 * handle_specifier - handle format specifiers
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

    (*i)++;
    if (format[*i] == '\0')
        return (-1);

    if (format[*i] == 'c')
        count = print_char(args, buffer, buff_ind);
    else if (format[*i] == 's')
        count = print_string(args, buffer, buff_ind);
    else if (format[*i] == '%')
        count = print_percent(args, buffer, buff_ind);
    else if (format[*i] == 'd' || format[*i] == 'i')
        count = print_int(args, buffer, buff_ind);
    else if (format[*i] == 'b')
        count = print_binary(args, buffer, buff_ind);
    else if (format[*i] == 'u')
        count = print_unsigned(args, buffer, buff_ind);
    else if (format[*i] == 'o')
        count = print_octal(args, buffer, buff_ind);
    else if (format[*i] == 'x')
        count = print_hex_lower(args, buffer, buff_ind);
    else if (format[*i] == 'X')
        count = print_hex_upper(args, buffer, buff_ind);
    else
    {
        buffer_char('%', buffer, buff_ind);
        buffer_char(format[*i], buffer, buff_ind);
        count = 2;
    }

    return (count);
}

/**
 * process_format - process the format string
 * @format: format string to process
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int process_format(const char *format, va_list args, char buffer[], int *buff_ind)
{
    int count = 0, i = 0;
    int specifier_count;

    while (format[i])
    {
        if (format[i] == '%')
        {
            specifier_count = handle_specifier(format, &i, args, buffer, buff_ind);
            if (specifier_count == -1)
                return (-1);
            count += specifier_count;
        }
        else
        {
            buffer_char(format[i], buffer, buff_ind);
            count++;
        }
        i++;
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

    if (format == NULL)
        return (-1);

    va_start(args, format);
    count = process_format(format, args, buffer, &buff_ind);
    flush_buffer(buffer, &buff_ind); /* Flush any remaining characters */
    va_end(args);

    return (count);
}
