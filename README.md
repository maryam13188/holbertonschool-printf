
# _printf Project - Custom printf Implementation
## Description
This project is a custom implementation of the printf function in C, developed as part of the Holberton School curriculum. The goal is to recreate the core functionality of the standard printf function from the C library, handling various format specifiers and producing output according to the given format string.

## Project Structure
```
printf/
├── main.h
├── _printf.c
├── functions.c
├── man_3_printf
└── README.md
```

| File | Description |
|------|-------------|
| _printf.c | Contains the main logic for the _printf function, loop for parsing the format string, and specifier selection |
| functions.c | Contains helper functions: _putchar (writes char to stdout), print_string (writes strings), print_int (handles %d/%i), and print_number (recursive integer printing) |
| main.h | Header file containing function prototypes and standard library includes (<stdarg.h>, <unistd.h>, <string.h>) |
| man_3_printf | Custom manual page documenting the function's usage, synopsis, and examples |

## General
- **Allowed editors:** vi, vim, emacs
- **Code style:** Betty style (checked with betty-style.pl and betty-doc.pl)
- **Global variables:** Not allowed
- **Functions per file:** Maximum 5 functions
- **Header file:** main.h with include guards
- **File endings:** All files must end with a new line
## Authorized Functions and Macros

- `write` (man 2 write)
- `malloc` (man 3 malloc) 
- `free` (man 3 free)
- `va_start` (man 3 va_start)
- `va_end` (man 3 va_end)
- `va_copy` (man 3 va_copy)
- `va_arg` (man 3 va_arg)
### Task 0: I'm not going anywhere. You can print that wherever you want to. I'm here and I'm a Spur for life
**Format specifiers handled:**
- `c` - characters
- `s` - strings  
- `%` - percent symbol
### Task 1: Education is when you read the fine print. Experience is what you get if you don't

**Mandatory** - Handle the following conversion specifiers:
- `d` - signed decimal integers
- `i` - signed integers
## Edge Cases Handled

- **Unknown specifiers:** Print the '%' and the following character as is
- **NULL strings:** Handle NULL pointers gracefully  
- **Empty format string:** Return 0 characters printed
- **Format string ending with %:** Handle appropriately
## Usage Examples
```c
#include "main.h"

int main(void)
{
    _printf("Hello, %s!\n", "World");
    _printf("Character: %c\n", 'A');
    _printf("Percent: %%\n");
    _printf("Integer: %d\n", 42);
    return (0);
}


Manual Page
A comprehensive manual page is available for the entire _printf project:

man ./man_3_printf_project

































