#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

/* Flag characters */
#define FLAG_PLUS   1
#define FLAG_SPACE  2
#define FLAG_HASH   4

/* Length modifiers */
#define LENGTH_NONE 0
#define LENGTH_L    1
#define LENGTH_H    2

typedef struct format_info
{
    int flags;
    int length;
    int width;
} format_info_t;

/* Main printf function */
int _printf(const char *format, ...);

/* Buffer management functions */
int flush_buffer(char buffer[], int *buff_ind);
int buffer_char(char c, char buffer[], int *buff_ind);
int buffer_string(char *str, char buffer[], int *buff_ind);

/* Conversion specifier functions */
int print_char(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_string(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_custom_string(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_pointer(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_percent(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_int(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_number(long n, char buffer[], int *buff_ind, format_info_t info);
int print_binary(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_binary_recursive(unsigned int n, char buffer[], int *buff_ind, format_info_t info);
int print_unsigned(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_octal(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_hex_lower(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_hex_upper(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_unsigned_number(unsigned long n, unsigned int base, const char *digits, char buffer[], int *buff_ind, format_info_t info);

/* Field width functions */
int apply_field_width(char buffer[], int *buff_ind, format_info_t info, int num_chars, int is_negative);
int parse_width(const char *format, int *i, va_list args);

/* Helper functions */
int handle_specifier(const char *format, int *i, va_list args, char buffer[], int *buff_ind);

#endif /* MAIN_H */
