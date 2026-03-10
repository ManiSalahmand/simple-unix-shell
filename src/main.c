#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "executor.h"
#include "parser.h"
#include "shell.h"

int main(void)
{
    while (1)
    {
        print_prompt();

        char *line = read_input();

        if (line == NULL)
            continue;

        if (is_input_empty(line))
        {
            free(line);
            continue;
        }

        Command *command = parse_command_line(line);
        free(line);

        if (command == NULL)
        {
            print_error("command parsing failed");
            continue;
        }

        execute_command(command);
        free_command(command);
    }

    return 0;
}
