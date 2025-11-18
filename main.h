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

typedef struct {
    int flags;
} format_info_t;

int _printf(const char *format, ...);
int process_format(const char *format, va_list args, char buffer[], int *buff_ind);
int handle_specifier(const char *format, int *i, va_list args, char buffer[], int *buff_ind, format_info_t info);
int parse_flags(const char *format, int *i);
int flush_buffer(char buffer[], int *buff_ind);
int buffer_char(char c, char buffer[], int *buff_ind);
int buffer_string(char *str, char buffer[], int *buff_ind);
int buffer_chars(char c, int count, char buffer[], int *buff_ind);

/* Print functions with format_info */
int print_char(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_string(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_custom_string(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_pointer(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_percent(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_int(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_number(long n, char buffer[], int *buff_ind, format_info_t info);
int print_binary(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_binary_recursive(unsigned long n, char buffer[], int *buff_ind, format_info_t info);
int print_unsigned(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_octal(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_hex_lower(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_hex_upper(va_list args, char buffer[], int *buff_ind, format_info_t info);
int print_unsigned_number(unsigned long n, unsigned int base, const char *digits, char buffer[], int *buff_ind, format_info_t info);
int print_hex_byte(unsigned char c, char buffer[], int *buff_ind, format_info_t info);
int print_pointer_address(void *ptr, char buffer[], int *buff_ind, format_info_t info);

#endif /* MAIN_H */
