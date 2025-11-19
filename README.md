# _printf Project
## Description
This project is a custom implementation of the printf function in C, developed as part of the Holberton School curriculum. The goal is to recreate the core functionality of the standard printf function from the C library, handling various format specifiers and producing output according to the given format string.
General
•	Allowed editors: vi, vim, emacs
•	Code style: Betty style (checked with betty-style.pl and betty-doc.pl)
•	Global variables: Not allowed
•	Functions per file: Maximum 5 functions
•	Header file: main.h with include guards
•	File endings: All files must end with a new line
Authorized Functions and Macros
•	write (man 2 write)
•	malloc (man 3 malloc)
•	free (man 3 free)
•	va_start (man 3 va_start)
•	va_end (man 3 va_end)
•	va_copy (man 3 va_copy)
•	va_arg (man 3 va_arg)
Task 0: I'm not going anywhere. You can print that wherever you want to. I'm here and I'm a Spur for life
Format specifiers handled:
•	c - characters
•	s - strings
•	% - percent symbol
Task 1: Education is when you read the fine print. Experience is what you get if you don't
Mandatory
Handle the following conversion specifiers:
•	d - signed decimal integers
•	i - signed integers
Expected Output
Let's try to printf a simple sentence.
Let's try to printf a simple sentence.
Character:[H]
Character:[H]
String:[I am a string !]
String:[I am a string !]
Percent:[%]
Percent:[%]
Length:[39, 39]
Length:[39, 39]
Negative:[-762534]
Negative:[-762534]
Edge Cases Handled
•	Unknown specifiers: Print the '%' and the following character as is
•	NULL strings: Handle NULL pointers gracefully
•	Empty format string: Return 0 characters printed
•	Format string ending with %: Handle appropriately

Documentation
A comprehensive manual page documenting the entire _printf project is available:
bash
man ./man_3_printf_project

The Real Challenge
This project pushes me beyond just writing code - it's about thinking through edge cases, understanding system calls, and building something that actually works reliably. It's one thing to use printf, but building it from scratch? That's where the real learning happens.






































