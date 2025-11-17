#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

int _printf(const char *format, ...);
int process_format(const char *format, va_list args);
int handle_specifier(const char *format, int *i, va_list args);
int write_char(char c);
int write_specifier(char c);
int print_char(va_list args);
int print_string(va_list args);
int print_percent(va_list args);


int _putchar(char c);

#endif /* MAIN_H */

