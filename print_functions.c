#include "main.h"

/**
 * print_char - prints a character
 * @args: arguments list
 * 
 * Return: number of characters printed (1)
 */
int print_char(va_list args)
{
    char c = va_arg(args, int);
    
    return (write(1, &c, 1));
}

/**
 * print_string - prints a string
 * @args: arguments list
 * 
 * Return: number of characters printed
 */
int print_string(va_list args)
{
    char *str = va_arg(args, char *);
    int count = 0;
    
    if (str == NULL)
        str = "(null)";
    
    while (str[count])
    {
        write(1, &str[count], 1);
        count++;
    }
    
    return (count);
}

/**
 * print_percent - prints a percent sign
 * @args: arguments list (unused)
 * 
 * Return: number of characters printed (1)
 */
int print_percent(va_list args)
{
    (void)args;
    return (write(1, "%", 1));
}
