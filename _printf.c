#include "main.h"

/**
 * parse_flags - parse flag characters
 * @format: format string
 * @i: pointer to current index
 * @info: pointer to format info
 *
 * Return: number of flags parsed
 */
int parse_flags(const char *format, int *i, format_info_t *info)
{
    int count = 0;
    int j = *i;

    while (format[j] == '+' || format[j] == ' ' || format[j] == '#')
    {
        if (format[j] == '+')
            info->flags |= FLAG_PLUS;
        else if (format[j] == ' ')
            info->flags |= FLAG_SPACE;
        else if (format[j] == '#')
            info->flags |= FLAG_HASH;
        j++;
        count++;
    }

    *i = j - 1;
    return (count);
}

/**
 * parse_width - parse field width from format string
 * @format: format string
 * @i: pointer to current index
 * @args: variable arguments list
 *
 * Return: field width value
 */
int parse_width(const char *format, int *i, va_list args)
{
    int width = 0;
    int j = *i;

    if (format[j] == '*')
    {
        width = va_arg(args, int);
        (*i)++;
        return (width);
    }

    while (format[j] >= '0' && format[j] <= '9')
    {
        width = width * 10 + (format[j] - '0');
        j++;
    }

    *i = j - 1;
    return (width);
}

/**
 * apply_field_width - apply field width padding
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 * @width: field width
 * @num_chars: number of characters in the output
 *
 * Return: number of padding characters added
 */
int apply_field_width(char buffer[], int *buff_ind, int width, int num_chars)
{
    int padding = 0;
    int i;

    if (width <= num_chars)
        return (0);

    padding = width - num_chars;

    for (i = 0; i < padding; i++)
    {
        buffer_char(' ', buffer, buff_ind);
    }

    return (padding);
}

/**
 * calculate_number_length - calculate the length of a number for field width
 * @n: the number
 * @base: base of the number
 *
 * Return: number of digits
 */
int calculate_number_length(long n, int base)
{
    int length = 0;
    unsigned long num;

    if (n < 0)
    {
        length++;
        num = -n;
    }
    else
    {
        num = n;
    }

    if (num == 0)
        return 1;

    while (num > 0)
    {
        length++;
        num /= base;
    }

    return (length);
}

/**
 * handle_specifier - handle format specifiers
 * @format: format string
 * @i: pointer to current index
 * @args: variable arguments list
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed for specifier
 */
int handle_specifier(const char *format, int *i, va_list args, char buffer[], int *buff_ind)
{
    int count = 0;
    format_info_t info = {0, LENGTH_NONE, 0};
    int start_index = *i;
    int has_specifier = 0;
    char temp_buffer[BUFFER_SIZE];
    int temp_buff_ind = 0;
    int j, temp_count, num_length;
    long n;
    unsigned long un;
    char *str;
    int str_len;
    char c;

    (*i)++; /* Skip % */
    
    if (format[*i] == '\0')
        return (-1);

    /* Parse flags */
    parse_flags(format, i, &info);

    /* Parse field width */
    if (format[*i] == '*')
    {
        info.width = parse_width(format, i, args);
    }
    else if (format[*i] >= '0' && format[*i] <= '9')
    {
        info.width = parse_width(format, i, args);
    }

    /* Parse length modifiers */
    if (format[*i] == 'l')
    {
        info.length = LENGTH_L;
        (*i)++;
    }
    else if (format[*i] == 'h')
    {
        info.length = LENGTH_H;
        (*i)++;
    }

    if (format[*i] == '\0')
        return (-1);

    /* Handle specifiers */
    switch (format[*i])
    {
        case 'c':
            c = va_arg(args, int);
            if (info.width > 1)
            {
                count += apply_field_width(buffer, buff_ind, info.width, 1);
            }
            count += buffer_char(c, buffer, buff_ind);
            has_specifier = 1;
            break;

        case 's':
            str = va_arg(args, char *);
            if (str == NULL)
                str = "(null)";
            
            str_len = 0;
            while (str[str_len])
                str_len++;
                
            if (info.width > str_len)
            {
                count += apply_field_width(buffer, buff_ind, info.width, str_len);
            }
            count += buffer_string(str, buffer, buff_ind);
            has_specifier = 1;
            break;

        case 'S':
            count = print_custom_string(args, buffer, buff_ind, info);
            has_specifier = 1;
            break;

        case 'p':
            count = print_pointer(args, buffer, buff_ind, info);
            has_specifier = 1;
            break;

        case '%':
            if (info.width > 1)
            {
                count += apply_field_width(buffer, buff_ind, info.width, 1);
            }
            count += buffer_char('%', buffer, buff_ind);
            has_specifier = 1;
            break;

        case 'd':
        case 'i':
            /* Get the number first to calculate its length */
            if (info.length == LENGTH_L)
                n = va_arg(args, long);
            else if (info.length == LENGTH_H)
                n = (short)va_arg(args, int);
            else
                n = va_arg(args, int);

            /* Calculate the number length for field width */
            num_length = calculate_number_length(n, 10);
            
            if (info.width > num_length)
            {
                count += apply_field_width(buffer, buff_ind, info.width, num_length);
            }
            
            /* Print the actual number */
            temp_buff_ind = 0;
            temp_count = print_int(args, temp_buffer, &temp_buff_ind, info);
            for (j = 0; j < temp_buff_ind; j++)
            {
                buffer_char(temp_buffer[j], buffer, buff_ind);
            }
            count += temp_count;
            has_specifier = 1;
            break;

        case 'b':
            count = print_binary(args, buffer, buff_ind, info);
            has_specifier = 1;
            break;

        case 'u':
            /* Get the number first to calculate its length */
            if (info.length == LENGTH_L)
                un = va_arg(args, unsigned long);
            else if (info.length == LENGTH_H)
                un = (unsigned short)va_arg(args, unsigned int);
            else
                un = va_arg(args, unsigned int);

            /* Calculate the number length for field width */
            num_length = calculate_number_length(un, 10);
            
            if (info.width > num_length)
            {
                count += apply_field_width(buffer, buff_ind, info.width, num_length);
            }
            
            /* Print the actual number */
            temp_buff_ind = 0;
            temp_count = print_unsigned(args, temp_buffer, &temp_buff_ind, info);
            for (j = 0; j < temp_buff_ind; j++)
            {
                buffer_char(temp_buffer[j], buffer, buff_ind);
            }
            count += temp_count;
            has_specifier = 1;
            break;

        case 'o':
            temp_buff_ind = 0;
            temp_count = print_octal(args, temp_buffer, &temp_buff_ind, info);
            
            if (info.width > temp_count)
            {
                count += apply_field_width(buffer, buff_ind, info.width, temp_count);
            }
            
            for (j = 0; j < temp_buff_ind; j++)
            {
                buffer_char(temp_buffer[j], buffer, buff_ind);
            }
            count += temp_count;
            has_specifier = 1;
            break;

        case 'x':
            temp_buff_ind = 0;
            temp_count = print_hex_lower(args, temp_buffer, &temp_buff_ind, info);
            
            if (info.width > temp_count)
            {
                count += apply_field_width(buffer, buff_ind, info.width, temp_count);
            }
            
            for (j = 0; j < temp_buff_ind; j++)
            {
                buffer_char(temp_buffer[j], buffer, buff_ind);
            }
            count += temp_count;
            has_specifier = 1;
            break;

        case 'X':
            temp_buff_ind = 0;
            temp_count = print_hex_upper(args, temp_buffer, &temp_buff_ind, info);
            
            if (info.width > temp_count)
            {
                count += apply_field_width(buffer, buff_ind, info.width, temp_count);
            }
            
            for (j = 0; j < temp_buff_ind; j++)
            {
                buffer_char(temp_buffer[j], buffer, buff_ind);
            }
            count += temp_count;
            has_specifier = 1;
            break;
    }

    if (!has_specifier)
    {
        /* Unknown specifier - print as is */
        int k;
        for (k = start_index; k <= *i; k++)
        {
            buffer_char(format[k], buffer, buff_ind);
            count++;
        }
    }

    return (count);
}

/**
 * _printf - produces output according to a format
 * @format: character string containing directives
 *
 * Return: number of characters printed (excluding null byte)
 */
int _printf(const char *format, ...)
{
    va_list args;
    char buffer[BUFFER_SIZE];
    int buff_ind = 0;
    int count = 0;
    int i = 0;
    int specifier_count;

    if (format == NULL)
        return (-1);

    va_start(args, format);

    while (format[i])
    {
        if (format[i] == '%')
        {
            specifier_count = handle_specifier(format, &i, args, buffer, &buff_ind);
            if (specifier_count == -1)
            {
                flush_buffer(buffer, &buff_ind);
                va_end(args);
                return (-1);
            }
            count += specifier_count;
        }
        else
        {
            count += buffer_char(format[i], buffer, &buff_ind);
        }
        i++;
    }

    flush_buffer(buffer, &buff_ind);
    va_end(args);

    return (count);
}
