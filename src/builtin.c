#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "error.h"

bool shell_should_exit = false;

/**
 * @brief Handle the `exit` builtin command.
 *
 * @param command Pointer to the parsed Command structure representing
 *                the `exit` command and its arguments.
 * @return int Always returns 0 after handling command.
 */
static int handle_exit(const Command *command)
{
    if (command->argc > 1)
    {
        shell_error("exit", "too many arguments");
        return 0;
    }

    shell_should_exit = true;
    return 0;
}

/**
 * @brief Handle the `cd` builtin command.
 *
 * @param command Pointer to the parsed Command structure containing the
 *                argument vector for the `cd` command.
 * @return int Returns 0 after handling the command, or -1 if the input
 *             command structure is invalid.
 */
static int handle_cd(const Command *command)
{
    if (command == NULL || command->argv == NULL)
        return -1;

    if (command->argc > 2)
    {
        shell_error("cd", "too many arguments");
        return 0;
    }

    const char *path = NULL;

    if (command->argc == 1)
    {
        path = getenv("HOME");
        if (path == NULL)
        {
            shell_error("cd", "HOME not set");
            return 0;
        }
    }
    else
    {
        path = command->argv[1];
    }

    if (chdir(path) == -1)
        print_error("cd");

    return 0;
}

/**
 * @brief Attempt to execute a builtin command.
 *
 * @param command Pointer to the parsed command structure containing the
 *                command and argument vector.
 * @return int Returns 0 if a builtin command was executed, or -1 if the
 *             command is not builtin or the input is invalid.
 */
int try_execute_builtin(const Command *command)
{
    if (command == NULL || command->name == NULL)
        return -1;

    if (strcmp(command->name, "exit") == 0)
        return handle_exit(command);

    if (strcmp(command->name, "cd") == 0)
        return handle_cd(command);

    return -1;
}
