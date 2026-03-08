#ifndef SHELL_H
#define SHELL_H

void print_prompt(void);
int is_input_empty(const char *input);
char *read_input(void);
void trim_newline(char *input);

#endif
