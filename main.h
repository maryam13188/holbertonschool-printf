#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

#define BUFFER_SIZE 1024

/* Length modifiers */
#define LENGTH_NONE 0
#define LENGTH_L    1
#define LENGTH_H    2

typedef struct format_info
{
    int length;
    int precision;
    int has_precision;
} format_info_t;

/* Main printf function */
int _printf(const char *format, ...);

/* Conversion specifier functions */
int print_char(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_string(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_custom_string(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_pointer(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_percent(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_int(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_number(long n, char buffer[], int *buff_ind, format_info_t info);
int print_number_precision(long n, char buffer[], int *buff_ind, format_info_t info);
int print_binary(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_unsigned(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_octal(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_hex_lower(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_hex_upper(va_list args, char buffer[], int *buff_ind, format_info_t info);

#endif /* MAIN_H */
