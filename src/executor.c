#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "executor.h"
#include "error.h"

/**
 * @brief Execute a parsed command in a child process.
 *
 * @param command Pointer to a parsed command structure.
 * @return int The child process status on success, or -1 on failure.
 */
int execute_command(const Command *command)
{
    if (command == NULL || command->argv == NULL || command->name == NULL)
        return -1;

    pid_t pid = fork();
    if (pid == -1)
    {
        print_error("fork failed");
        return -1;
    }

    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        execvp(command->name, command->argv);
        print_error(command->name);
        exit(EXIT_FAILURE);
    }

    int status;
    while (waitpid(pid, &status, 0) == -1)
    {
        if (errno == EINTR)
            continue;

        print_error("waitpid failed");
        return -1;
    }

    return status;
}
