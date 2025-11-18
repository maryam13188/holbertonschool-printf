#include "main.h"

/**
 * flush_buffer - flush buffer to stdout
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters printed
 */
int flush_buffer(char buffer[], int *buff_ind)
{
    int count = 0;

    if (*buff_ind > 0)
    {
        count = write(1, buffer, *buff_ind);
        *buff_ind = 0;
    }

    return (count);
}

/**
 * buffer_char - buffer a character
 * @c: character to buffer
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: 1 (number of characters buffered)
 */
int buffer_char(char c, char buffer[], int *buff_ind)
{
    if (*buff_ind >= BUFFER_SIZE)
        flush_buffer(buffer, buff_ind);

    buffer[*buff_ind] = c;
    (*buff_ind)++;

    return (1);
}

/**
 * buffer_string - buffer a string
 * @str: string to buffer
 * @buffer: character buffer
 * @buff_ind: pointer to buffer index
 *
 * Return: number of characters buffered
 */
int buffer_string(char *str, char buffer[], int *buff_ind)
{
    int count = 0;

    if (str == NULL)
        str = "(null)";

    while (*str)
    {
        count += buffer_char(*str, buffer, buff_ind);
        str++;
    }

    return (count);
}
