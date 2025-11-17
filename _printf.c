#include "main.h"
/**
*_printf - produces output accourding to the format
*
*@format: format string containing format specifiers
*
*Return: number of character printed
*/
int _printf(const char *format, ...)
{
va_list args;
int c = 0;
if (!format || (format[0] == '%' && format[1] == '\0'))
return (-1);
va_start(args, format);

while (*format)
{
if (*format == '%')
format++;
if (*format == 'c')
c += _putchar(va_arg(args, int));
else if (*format == 's')
c += print_string(va_arg(args, char *));
else if (*format == '%')
c += _putchar('%');
else if (*format == 'd' || *format == 'i')
c += print_number(va_arg(args, int));
else
{
c += _putchar('%');
c += _putchar(*format);
}
}
va_end(args);
return (c);

}
