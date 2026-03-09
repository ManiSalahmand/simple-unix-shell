#ifndef PARSER_H
#define PARSER_H

typedef struct Command
{
    char *name;
    char **argv;
    int argc;
} Command;

Command *parse_command_line(const char *input);
void free_command(Command *command);

#endif
