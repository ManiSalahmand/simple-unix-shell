#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>

void print_prompt(void);
bool is_input_empty(const char *input);
char *read_input(void);
void trim_newline(char *input);

#endif
