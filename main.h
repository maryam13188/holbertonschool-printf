#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

int _printf(const char *format, ...);
int print_char(va_list args);
int print_string(va_list args);
int print_percent(va_list args);

/* تعريف الـ structure هنا مباشرة */
typedef struct format_specifier
{
    char specifier;
    int (*func)(va_list);
} format_specifier_t;

#endif /* MAIN_H */
