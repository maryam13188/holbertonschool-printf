#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include <ctype.h>  /* for isprint */
#include <stdint.h> /* for uintptr_t */

/* Basic _putchar (1 on success, -1 on error) */
int _putchar(char c)
{
    int r = putchar((unsigned char)c);
    return (r == EOF) ? -1 : 1;
}

/* helper: write string with width/precision/prefix/sign handling */
int print_number_base_str(const char *str, fmt_options *opts, int negative, const char *prefix)
{
    int len = (int)strlen(str);
    int prefix_len = prefix ? (int)strlen(prefix) : 0;
    int total = 0;
    int pad = 0;
    int sign_char = 0; /* 0 none, '+' or ' ' or '-' */
    int i;
    int left_pad = 0;
    int right_pad = 0;
    int content_len = 0;
    char pad_ch = ' ';

    if (negative)
        sign_char = '-';
    else if (opts->plus)
        sign_char = '+';
    else if (opts->space)
        sign_char = ' ';

    if (opts->precision_specified)
    {
        if (opts->precision > len)
            pad = opts->precision - len;
        /* when precision is specified, '0' flag is ignored for padding the field */
    }

    /* Special case: when precision is specified as 0 and the value is 0, nothing is printed for integers */
    if (opts->precision_specified && opts->precision == 0 && len == 1 && str[0] == '0')
    {
        /* treat as empty number (len 0) */
        len = 0;
        pad = 0; /* no precision zeros needed */
    }

    /* compute content length and field padding */
    content_len = len + pad + prefix_len + (sign_char ? 1 : 0);
    if (opts->width > content_len)
        left_pad = opts->width - content_len;
    else
        left_pad = 0;

    /* left padding (if not dash) */
    pad_ch = (opts->zero && !opts->precision_specified) ? '0' : ' ';
    if (!opts->dash)
    {
        if (pad_ch == '0')
        {
            if (sign_char)
                total += _putchar((char)sign_char);
            if (prefix_len)
            {
                for (i = 0; i < prefix_len; i++)
                    total += _putchar(prefix[i]);
            }
            for (i = 0; i < left_pad; i++)
                total += _putchar('0');
        }
        else
        {
            for (i = 0; i < left_pad; i++)
                total += _putchar(' ');
            if (sign_char)
                total += _putchar((char)sign_char);
            if (prefix_len)
            {
                for (i = 0; i < prefix_len; i++)
                    total += _putchar(prefix[i]);
            }
        }
    }
    else
    {
        /* if dash flag, print sign and prefix first */
        if (sign_char)
            total += _putchar((char)sign_char);
        if (prefix_len)
        {
            for (i = 0; i < prefix_len; i++)
                total += _putchar(prefix[i]);
        }
    }

    /* precision zeros */
    for (i = 0; i < pad; i++)
        total += _putchar('0');

    /* print number digits */
    for (i = 0; i < len; i++)
        total += _putchar(str[i]);

    /* right padding if dash */
    if (opts->dash)
    {
        content_len = len + pad + prefix_len + (sign_char ? 1 : 0);
        right_pad = (opts->width > content_len) ? (opts->width - content_len) : 0;
        for (i = 0; i < right_pad; i++)
            total += _putchar(' ');
    }

    return total;
}

/* Print a single char */
int print_char(va_list args, fmt_options *opts)
{
    char c;
    int count = 0;
    int i;
    int width = 0;

    if (opts)
        width = opts->width;

    c = (char)va_arg(args, int);

    if (width <= 1)
    {
        return _putchar(c);
    }

    if (!opts->dash)
    {
        for (i = 0; i < width - 1; i++)
            count += _putchar(' ');
        count += _putchar(c);
    }
    else
    {
        count += _putchar(c);
        for (i = 0; i < width - 1; i++)
            count += _putchar(' ');
    }

    return count;
}

/* Print a string */
int print_string(va_list args, fmt_options *opts)
{
    char *s = va_arg(args, char *);
    int count = 0;
    int len;
    int i;

    if (!s)
        s = "(null)";

    /* apply precision: maximum characters printed */
    len = (int)strlen(s);
    if (opts->precision_specified && opts->precision < len)
        len = opts->precision;

    /* width handling */
    if (!opts->dash && opts->width > len)
    {
        for (i = 0; i < opts->width - len; i++)
            count += _putchar(' ');
    }

    for (i = 0; i < len; i++)
        count += _putchar(s[i]);

    if (opts->dash && opts->width > len)
    {
        for (i = 0; i < opts->width - len; i++)
            count += _putchar(' ');
    }

    return count;
}

/* Print percent sign */
int print_percent(va_list args, fmt_options *opts)
{
    int count = 0;
    int width = 0;
    int i;

    (void)args;

    if (opts)
        width = opts->width;

    if (width <= 1)
        return _putchar('%');

    if (!opts || !opts->dash)
    {
        for (i = 0; i < width - 1; i++)
            count += _putchar(' ');
        count += _putchar('%');
    }
    else
    {
        count += _putchar('%');
        for (i = 0; i < width - 1; i++)
            count += _putchar(' ');
    }

    return count;
}

/* helper: integer to string for unsigned long using buffer (returns pointer) */
static char *ultoa_base(unsigned long value, int base, int uppercase, char *buf, int bufsize)
{
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    int i = 0;
    int j;

    if (value == 0)
    {
        buf[i++] = '0';
        buf[i] = '\0';
        return buf;
    }

    while (value && i < bufsize - 1)
    {
        buf[i++] = digits[value % base];
        value /= base;
    }
    /* reverse */
    for (j = 0; j < i / 2; j++)
    {
        char t = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = t;
    }
    buf[i] = '\0';
    return buf;
}

/* Print signed number with flags/width/precision/length */
int print_number(va_list args, fmt_options *opts)
{
    long val = 0;
    int negative = 0;
    char buf[70];

    if (opts->length == 'l')
        val = va_arg(args, long);
    else if (opts->length == 'h')
        val = (short)va_arg(args, int);
    else
        val = va_arg(args, int);

    if (val < 0)
    {
        negative = 1;
        /* careful with LONG_MIN */
        if (opts->length == 'l')
            val = (unsigned long)(-(long)val);
        else
            val = -val;
    }

    /* convert absolute value to string */
    ultoa_base((unsigned long)val, 10, 0, buf, sizeof(buf));

    /* If precision is specified as 0 and value is 0, print no digits */
    if (opts->precision_specified && opts->precision == 0 && buf[0] == '0')
    {
        buf[0] = '\0';
    }

    /* prefix is handled by print_number_base_str (sign handled separately) */
    return print_number_base_str(buf, opts, negative, NULL);
}

/* Print unsigned number with base, using flags/length */
int print_unsigned(va_list args, fmt_options *opts, int base, int uppercase)
{
    unsigned long val = 0;
    char buf[70];
    const char *prefix = NULL;
    char prefix_buf[3] = {0};

    if (opts->length == 'l')
        val = va_arg(args, unsigned long);
    else if (opts->length == 'h')
        val = (unsigned short)va_arg(args, unsigned int);
    else
        val = va_arg(args, unsigned int);

    /* alternate form '#' */
    if (opts->hash && val != 0)
    {
        if (base == 8)
        {
            prefix_buf[0] = '0';
            prefix_buf[1] = '\0';
            prefix = prefix_buf;
        }
        else if (base == 16)
        {
            prefix_buf[0] = '0';
            prefix_buf[1] = uppercase ? 'X' : 'x';
            prefix_buf[2] = '\0';
            prefix = prefix_buf;
        }
    }

    ultoa_base(val, base, uppercase, buf, sizeof(buf));
    /* If precision is specified as 0 and value is 0, print no digits */
    if (opts->precision_specified && opts->precision == 0 && buf[0] == '0')
    {
        buf[0] = '\0';
    }
    return print_number_base_str(buf, opts, 0, prefix);
}

/* Print a reversed string (custom %r) */
int print_reverse(va_list args, fmt_options *opts)
{
    char *s;
    int len;
    int i;
    int count = 0;
    int width = 0;
    int to_print;

    s = va_arg(args, char *);
    if (!s)
        s = "(null)";

    /* length of string */
    len = (int)strlen(s);

    /* apply precision: maximum characters printed */
    if (opts && opts->precision_specified && opts->precision < len)
        to_print = opts->precision;
    else
        to_print = len;

    if (opts)
        width = opts->width;

    /* width padding on the left if not dash */
    if (!opts || !opts->dash)
    {
        for (i = 0; i < width - to_print; i++)
            count += _putchar(' ');
    }

    /* print in reverse order the first to_print characters */
    for (i = to_print - 1; i >= 0; i--)
        count += _putchar(s[i]);

    /* right padding if dash */
    if (opts && opts->dash)
    {
        for (i = 0; i < width - to_print; i++)
            count += _putchar(' ');
    }

    return count;
}

/* Print non-printable characters as \xHH (custom %S) */
int print_S(va_list args, fmt_options *opts)
{
    const char *s = va_arg(args, char *);
    int count = 0;
    int i, j;
    int slen;
    int to_print;
    int width = 0;
    int expanded_len = 0;
    unsigned char ch;
    const char *hex_digits = "0123456789ABCDEF";

    if (!s)
        s = "(null)";

    slen = (int)strlen(s);
    if (opts && opts->precision_specified && opts->precision < slen)
        to_print = opts->precision;
    else
        to_print = slen;

    if (opts)
        width = opts->width;

    /* compute expanded length (printable -> 1, non-printable -> 4 (\xHH)) */
    for (i = 0; i < to_print; i++)
    {
        ch = (unsigned char)s[i];
        if (isprint(ch))
            expanded_len += 1;
        else
            expanded_len += 4;
    }

    /* left padding */
    if (!opts || !opts->dash)
    {
        for (i = 0; i < width - expanded_len; i++)
            count += _putchar(' ');
    }

    /* print characters (convert non-printable to \xHH) */
    for (i = 0; i < to_print; i++)
    {
        ch = (unsigned char)s[i];
        if (isprint(ch))
        {
            count += _putchar((char)ch);
        }
        else
        {
            count += _putchar('\\');
            count += _putchar('x');
            /* high nibble */
            count += _putchar(hex_digits[(ch >> 4) & 0xF]);
            /* low nibble */
            count += _putchar(hex_digits[ch & 0xF]);
        }
    }

    /* right padding */
    if (opts && opts->dash)
    {
        for (j = 0; j < width - expanded_len; j++)
            count += _putchar(' ');
    }

    return count;
}

/* Print ROT13'ed string (custom %R) */
int print_rot13(va_list args, fmt_options *opts)
{
    char *s;
    int count = 0;
    int len;
    int i;
    int to_print;
    int width = 0;
    char c, out;

    s = va_arg(args, char *);
    if (!s)
        s = "(null)";

    len = (int)strlen(s);

    if (opts && opts->precision_specified && opts->precision < len)
        to_print = opts->precision;
    else
        to_print = len;

    if (opts)
        width = opts->width;

    /* left padding */
    if (!opts || !opts->dash)
    {
        for (i = 0; i < width - to_print; i++)
            count += _putchar(' ');
    }

    for (i = 0; i < to_print; i++)
    {
        c = s[i];
        out = c;
        if (c >= 'a' && c <= 'z')
            out = (char)((((c - 'a') + 13) % 26) + 'a');
        else if (c >= 'A' && c <= 'Z')
            out = (char)((((c - 'A') + 13) % 26) + 'A');
        count += _putchar(out);
    }

    /* right padding */
    if (opts && opts->dash)
    {
        for (i = 0; i < width - to_print; i++)
            count += _putchar(' ');
    }

    return count;
}

/* Print pointer (%p) */
int print_pointer(va_list args, fmt_options *opts)
{
    void *p = va_arg(args, void *);
    int count = 0;
    char buf[70];

    /* print (nil) for NULL pointer similar to many printf implementations */
    if (p == NULL)
    {
        const char *nil = "(nil)";
        int len = (int)strlen(nil);
        int i;
        int width = opts ? opts->width : 0;

        if (!opts || !opts->dash)
        {
            for (i = 0; i < width - len; i++)
                count += _putchar(' ');
        }
        for (i = 0; i < len; i++)
            count += _putchar(nil[i]);
        if (opts && opts->dash)
        {
            for (i = 0; i < width - len; i++)
                count += _putchar(' ');
        }
        return count;
    }

    /* convert pointer value to unsigned long (use uintptr_t for correctness) */
    ultoa_base((unsigned long)(uintptr_t)p, 16, 0, buf, sizeof(buf));

    /* always use "0x" prefix for %p */
    return print_number_base_str(buf, opts, 0, "0x");
}
