This project is a custom implementation of the printf function in C, created as part of the Holberton School curriculum. The function produces output according to a format string and handles various conversion specifiers.

main.h: Header file containing function prototypes and necessary includes
_printf.c: Main implementation of the _printf function
print_functions.c: Contains helper functions for different format specifiers
man_3_printf: Manual page documenting the _printf function

The _printf function currently handles the following conversion specifiers:

%c: Prints a single character

%s: Prints a string of characters

%%: Prints a percent sign

%d: Prints a signed decimal integer

%i: Prints a signed integer

Authors: (Maryam13188,Munirahenad)

Helper Functions:
print_char: Handles %c specifier

print_string: Handles %s specifier

print_percent: Handles %% specifier

print_int: Handles %d and %i specifiers

Files
main.h: Header file with all prototypes and includes

_printf.c: Main printf implementation

print_functions.c: Helper functions for format specifiers

man_3_printf: Manual page documentation
