#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

/* Flag characters for question 8 */
#define FLAG_PLUS   1
#define FLAG_SPACE  2
#define FLAG_HASH   4

/* All original functions from questions 0-7 */
int _printf(const char *format, ...);
int process_format(const char *format, va_list args, char buffer[], int *buff_ind);
int handle_specifier(const char *format, int *i, va_list args, char buffer[], int *buff_ind);
int flush_buffer(char buffer[], int *buff_ind);
int buffer_char(char c, char buffer[], int *buff_ind);
int buffer_string(char *str, char buffer[], int *buff_ind);
int print_char(va_list args, char buffer[], int *buff_ind);
int print_string(va_list args, char buffer[], int *buff_ind);
int print_custom_string(va_list args, char buffer[], int *buff_ind);
int print_pointer(va_list args, char buffer[], int *buff_ind);
int print_percent(va_list args, char buffer[], int *buff_ind);
int print_int(va_list args, char buffer[], int *buff_ind);
int print_number(int n, char buffer[], int *buff_ind);
int print_binary(va_list args, char buffer[], int *buff_ind);
int print_binary_recursive(unsigned int n, char buffer[], int *buff_ind);
int print_unsigned(va_list args, char buffer[], int *buff_ind);
int print_octal(va_list args, char buffer[], int *buff_ind);
int print_hex_lower(va_list args, char buffer[], int *buff_ind);
int print_hex_upper(va_list args, char buffer[], int *buff_ind);
int print_unsigned_number(unsigned int n, unsigned int base, const char *digits, char buffer[], int *buff_ind);
int print_hex_byte(unsigned char c, char buffer[], int *buff_ind);
int print_pointer_address(void *ptr, char buffer[], int *buff_ind);

/* New functions for question 8 - flag support */
int handle_specifier_with_flags(const char *format, int *i, va_list args, char buffer[], int *buff_ind);
int parse_flags(const char *format, int *i);
int print_int_with_flags(va_list args, char buffer[], int *buff_ind, int flags);
int print_octal_with_flags(va_list args, char buffer[], int *buff_ind, int flags);
int print_hex_lower_with_flags(va_list args, char buffer[], int *buff_ind, int flags);
int print_hex_upper_with_flags(va_list args, char buffer[], int *buff_ind, int flags);

#endif /* MAIN_H */
