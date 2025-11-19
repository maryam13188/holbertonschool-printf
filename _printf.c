#include "main.h"
#include <stdarg.h>
#include <ctype.h>

static void init_opts(fmt_options *o)
{
    o->plus = o->space = o->hash = o->zero = o->dash = 0;
    o->width = 0;
    o->precision = 0;
    o->precision_specified = 0;
    o->length = 0;
}

int _printf(const char *format, ...)
{
    va_list args;
    int count = 0;
    fmt_options opts;

    if (!format)
        return (-1);

    va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            init_opts(&opts);

            /* parse flags */
            while (*format == '+' || *format == ' ' || *format == '#' ||
                   *format == '0' || *format == '-')
            {
                if (*format == '+')
                    opts.plus = 1;
                else if (*format == ' ')
                    opts.space = 1;
                else if (*format == '#')
                    opts.hash = 1;
                else if (*format == '0')
                    opts.zero = 1;
                else if (*format == '-')
                {
                    opts.dash = 1;
                    /* '-' overrides '0' */
                    opts.zero = 0;
                }
                format++;
            }

            /* parse width (number or *) */
            if (*format == '*')
            {
                int w = va_arg(args, int);
                if (w < 0)
                {
                    opts.dash = 1;
                    opts.width = -w;
                }
                else
                {
                    opts.width = w;
                }
                format++;
            }
            else if (isdigit((unsigned char)*format))
            {
                opts.width = 0;
                while (isdigit((unsigned char)*format))
                {
                    opts.width = opts.width * 10 + (*format - '0');
                    format++;
                }
            }

            /* parse precision ('.' followed by number or '*') */
            if (*format == '.')
            {
                format++;
                opts.precision_specified = 1;
                if (*format == '*')
                {
                    int p = va_arg(args, int);
                    if (p >= 0)
                    {
                        opts.precision = p;
                    }
                    else
                    {
                        /* negative precision means it's ignored */
                        opts.precision_specified = 0;
                        opts.precision = 0;
                    }
                    format++;
                }
                else
                {
                    opts.precision = 0;
                    while (isdigit((unsigned char)*format))
                    {
                        opts.precision = opts.precision * 10 + (*format - '0');
                        format++;
                    }
                }
            }

            /* parse length */
            if (*format == 'h' || *format == 'l')
            {
                opts.length = *format;
                format++;
            }

            if (*format == '\0')
            {
                va_end(args);
                return (-1);
            }

            switch (*format)
            {
            case 'c':
                count += print_char(args, &opts);
                break;

            case 's':
                count += print_string(args, &opts);
                break;

            case '%':
                count += print_percent(args, &opts);
                break;

            case 'r': /* custom: reversed string */
                count += print_reverse(args, &opts);
                break;

            case 'R': /* custom: ROT13 string */
                count += print_rot13(args, &opts);
                break;

            case 'S': /* custom: non-printable as \xHH */
                count += print_S(args, &opts);
                break;

            case 'd':
            case 'i':
                count += print_number(args, &opts);
                break;

            case 'u':
                count += print_unsigned(args, &opts, 10, 0);
                break;

            case 'o':
                count += print_unsigned(args, &opts, 8, 0);
                break;

            case 'x':
                count += print_unsigned(args, &opts, 16, 0);
                break;

            case 'X':
                count += print_unsigned(args, &opts, 16, 1);
                break;

            case 'p': /* pointer */
                count += print_pointer(args, &opts);
                break;

            case 'b': /* custom: binary */
                count += print_unsigned(args, &opts, 2, 0);
                break;

            default:
                count += _putchar('%');
                count += _putchar(*format);
                break;
            }
        }
        else
        {
            count += _putchar(*format);
        }

        format++;
    }

    va_end(args);
    return count;
}
