#include <stdio.h>
#include <stdlib.h>

#include "builtin.h"
#include "error.h"
#include "executor.h"
#include "parser.h"
#include "shell.h"
#include "signal_handler.h"

int main(void)
{
    // Set up signal handling before entering the shell loop.
    if (setup_signal_handlers() == -1)
    {
        print_error("sigaction failed");
        return EXIT_FAILURE;
    }

    // Keep running until the exit builtin sets the shell exit flag.
    while (!shell_should_exit)
    {
        print_prompt();

        // Read a full line of user input.
        char *line = read_input();
        if (line == NULL)
            continue;

        // Ignore empty or whitespace-only input.
        if (is_input_empty(line))
        {
            free(line);
            continue;
        }

        // Parse the raw input into a Command structure.
        Command *command = parse_command_line(line);
        free(line);

        // Skip invalid input if parsing failed.
        if (command == NULL)
            continue;

        // Execute the parsed command, then release its memory.
        execute_command(command);
        free_command(command);
    }

    return 0;
}
