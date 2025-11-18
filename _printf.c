#include "main.h"

/**
 * _printf - produces output according to a format
 * @format: format string
 *
 * Return: number of characters printed
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
    va_end(args);

    return (count);
}

/**
 * process_format - process format string with flags support
 * @format: format string
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int process_format(const char *format, va_list args, char buffer[], int *buff_ind)
{
    int i, count = 0;

    for (i = 0; format[i] != '\0'; i++)
    {
        if (format[i] == '%')
        {
            /* Use the version with flags support for question 8 */
            count += handle_specifier_with_flags(format, &i, args, buffer, buff_ind);
        }
        else
        {
            count += buffer_char(format[i], buffer, buff_ind);
        }
    }

    count += flush_buffer(buffer, buff_ind);
    return (count);
}
