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
                write(1, "%", 1);
                write(1, &format[i], 1);
                count += 2;
            }
        }
        else
        {
            write(1, &format[i], 1);
            count++;
        }
        i++;
    }
    
    va_end(args);
    return (count);
}
