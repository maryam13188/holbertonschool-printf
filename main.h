#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <limits.h>

typedef struct fmt_options
{
    int plus;
    int space;
    int hash;
    int zero;
    int dash;
    int width;
    int precision;
    int precision_specified;
    char length; /* 'l' or 'h' or 0 */
} fmt_options;

int _printf(const char *format, ...);
int _putchar(char c);

/* Print functions (now accept options) */
int print_char(va_list args, fmt_options *opts);
int print_string(va_list args, fmt_options *opts);
int print_percent(va_list args, fmt_options *opts);
int print_number(va_list args, fmt_options *opts);
int print_unsigned(va_list args, fmt_options *opts, int base, int uppercase);

/* custom specifiers */
int print_reverse(va_list args, fmt_options *opts);
int print_rot13(va_list args, fmt_options *opts);
int print_S(va_list args, fmt_options *opts);       /* non-printable -> \\xHH */
int print_pointer(va_list args, fmt_options *opts); /* %p */
/* Number base printer (internal helper) */
int print_number_base_str(const char *str, fmt_options *opts, int negative, const char *prefix);

#endif
