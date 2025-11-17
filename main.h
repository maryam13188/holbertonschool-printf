#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

int _printf(const char *format, ...);
int process_format(const char *format, va_list args);
int handle_specifier(const char *format, int *i, va_list args);
int write_char(char c);
int print_char(va_list args);
int print_string(va_list args);
int print_percent(va_list args);
int print_int(va_list args);
int print_number(int n);
int print_binary(va_list args);
int print_binary_recursive(unsigned int n);
int print_unsigned(va_list args);
int print_octal(va_list args);
int print_hex_lower(va_list args);
int print_hex_upper(va_list args);
int print_unsigned_number(unsigned int n, unsigned int base, const char *digits);

#endif /* MAIN_H */
