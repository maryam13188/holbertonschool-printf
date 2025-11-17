#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>

/**
 * struct format_s - format specifier structure
 * @spec: format specifier character
 * @func: pointer to corresponding function
 */
typedef struct format_s
{
    char spec;
    int (*func)(va_list);
} format_t;

/* Main printf function */
int _printf(const char *format, ...);

/* Helper functions */
int _putchar(char c);

/* Format specifier functions */
int print_char(va_list args);
int print_string(va_list args);
int print_percent(va_list args);

#endif /* MAIN_H */
