#include "main.h"

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
 * @info: format information
 * @num_chars: number of characters in the output
 * @is_negative: whether the number is negative
 *
 * Return: number of padding characters added
 */
int apply_field_width(char buffer[], int *buff_ind, format_info_t info, int num_chars, int is_negative)
{
    int padding = 0;
    int i;

    (void)is_negative;

    if (info.width <= num_chars)
        return (0);

    padding = info.width - num_chars;

    for (i = 0; i < padding; i++)
    {
        buffer_char(' ', buffer, buff_ind);
    }

    return (padding);
}

/**
 * handle_specifier - handle format specifiers with field width
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
    char temp_buffer[BUFFER_SIZE];
    int temp_buff_ind = 0;
    int j, temp_count;

    (*i)++; /* Skip % */
    
    if (format[*i] == '\0')
        return (-1);

    /* Parse field width - FIXED: Parse before checking for specifiers */
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

    /* Handle specifiers with proper field width application */
    if (format[*i] == 'c')
    {
        char c = va_arg(args, int);
        if (info.width > 1)
        {
            count += apply_field_width(buffer, buff_ind, info, 1, 0);
        }
        count += buffer_char(c, buffer, buff_ind);
    }
    else if (format[*i] == 's')
    {
        char *str = va_arg(args, char *);
        if (str == NULL)
            str = "(null)";
        
        int str_len = 0;
        while (str[str_len])
            str_len++;
            
        if (info.width > str_len)
        {
            count += apply_field_width(buffer, buff_ind, info, str_len, 0);
        }
        count += buffer_string(str, buffer, buff_ind);
    }
    else if (format[*i] == 'S')
    {
        count = print_custom_string(args, buffer, buff_ind, info);
    }
    else if (format[*i] == 'p')
    {
        count = print_pointer(args, buffer, buff_ind, info);
    }
    else if (format[*i] == '%')
    {
        if (info.width > 1)
        {
            count += apply_field_width(buffer, buff_ind, info, 1, 0);
        }
        count += buffer_char('%', buffer, buff_ind);
    }
    else if (format[*i] == 'd' || format[*i] == 'i')
    {
        /* Use temporary buffer to calculate the number length first */
        temp_buff_ind = 0;
        temp_count = print_int(args, temp_buffer, &temp_buff_ind, info);
        
        if (info.width > temp_count)
        {
            count += apply_field_width(buffer, buff_ind, info, temp_count, 0);
        }
        
        /* Copy from temp buffer to main buffer */
        for (j = 0; j < temp_buff_ind; j++)
        {
            buffer_char(temp_buffer[j], buffer, buff_ind);
        }
        count += temp_count;
    }
    else if (format[*i] == 'b')
    {
        count = print_binary(args, buffer, buff_ind, info);
    }
    else if (format[*i] == 'u')
    {
        /* Use temporary buffer for unsigned numbers */
        temp_buff_ind = 0;
        temp_count = print_unsigned(args, temp_buffer, &temp_buff_ind, info);
        
        if (info.width > temp_count)
        {
            count += apply_field_width(buffer, buff_ind, info, temp_count, 0);
        }
        
        for (j = 0; j < temp_buff_ind; j++)
        {
            buffer_char(temp_buffer[j], buffer, buff_ind);
        }
        count += temp_count;
    }
    else if (format[*i] == 'o')
    {
        /* Use temporary buffer for octal numbers */
        temp_buff_ind = 0;
        temp_count = print_octal(args, temp_buffer, &temp_buff_ind, info);
        
        if (info.width > temp_count)
        {
            count += apply_field_width(buffer, buff_ind, info, temp_count, 0);
        }
        
        for (j = 0; j < temp_buff_ind; j++)
        {
            buffer_char(temp_buffer[j], buffer, buff_ind);
        }
        count += temp_count;
    }
    else if (format[*i] == 'x')
    {
        /* Use temporary buffer for hex numbers */
        temp_buff_ind = 0;
        temp_count = print_hex_lower(args, temp_buffer, &temp_buff_ind, info);
        
        if (info.width > temp_count)
        {
            count += apply_field_width(buffer, buff_ind, info, temp_count, 0);
        }
        
        for (j = 0; j < temp_buff_ind; j++)
        {
            buffer_char(temp_buffer[j], buffer, buff_ind);
        }
        count += temp_count;
    }
    else if (format[*i] == 'X')
    {
        /* Use temporary buffer for hex numbers */
        temp_buff_ind = 0;
        temp_count = print_hex_upper(args, temp_buffer, &temp_buff_ind, info);
        
        if (info.width > temp_count)
        {
            count += apply_field_width(buffer, buff_ind, info, temp_count, 0);
        }
        
        for (j = 0; j < temp_buff_ind; j++)
        {
            buffer_char(temp_buffer[j], buffer, buff_ind);
        }
        count += temp_count;
    }
    else
    {
        /* Unknown specifier - print as is */
        buffer_char('%', buffer, buff_ind);
        buffer_char(format[*i], buffer, buff_ind);
        count = 2;
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
