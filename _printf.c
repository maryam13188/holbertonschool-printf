#include "main.h"

/**
 * _printf - produces output according to a format
 * @format: character string containing directives
 * 
 * Return: number of characters printed (excluding null byte)
 */
int _printf(const char *format, ...)
{
    va_list args;
    int count = 0, i = 0, j;
    
    format_specifier_t specifiers[] = {
        {'c', print_char},
        {'s', print_string},
        {'%', print_percent},
        {'\0', NULL}
    };
    
    if (format == NULL)
        return (-1);
    
    va_start(args, format);
    
    while (format[i])
    {
        if (format[i] == '%')
        {
            i++;
            if (format[i] == '\0')
                return (-1);
            
            j = 0;
            while (specifiers[j].specifier != '\0')
            {
                if (format[i] == specifiers[j].specifier)
                {
                    count += specifiers[j].func(args);
                    break;
                }
                j++;
            }
            
            if (specifiers[j].specifier == '\0')
            {
                _putchar('%');
                _putchar(format[i]);
                count += 2;
            }
        }
        else
        {
            _putchar(format[i]);
            count++;
        }
        i++;
    }
    
    va_end(args);
    return (count);
}

/**
 * _putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putchar(char c)
{
    return (write(1, &c, 1));
}
