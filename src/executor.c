#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtin.h"
#include "executor.h"
#include "error.h"

/**
 * @brief Execute a parsed command.
 *
 * Runs shell built0ins directly in the parent process. For external
 * commands, forks a child, applies any requested redirections, and
 * executes the program with execvp()
 *
 * @param command Pointer to a parsed command structure.
 * @return int The child process status on success, or -1 on failure.
 */
int execute_command(const Command *command)
{
    if (command == NULL || command->argv == NULL || command->name == NULL)
        return -1;

    // Built-ins must run in the shell process itself.
    if (try_execute_builtin(command) == 0)
        return 0;

    pid_t pid = fork();
    if (pid == -1)
    {
        print_error("fork failed");
        return -1;
    }

    if (pid == 0)
    {
        // Redirect standard input from a file if requested.
        if (command->input_file != NULL)
        {
            int fd = open(command->input_file, O_RDONLY);

            if (fd < 0)
            {
                print_error("open");
                exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDIN_FILENO) < 0)
            {
                print_error("dup2");
                close(fd);
                exit(EXIT_FAILURE);
            }

            close(fd);
        }

        // Redirect standard output to a file if requested.
        if (command->output_file != NULL)
        {
            int fd = open(command->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                print_error("open");
                exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                close(fd);
                print_error("dup2");
                exit(EXIT_FAILURE);
            }

            close(fd);
        }

        // Child processes should use the default Ctrl+C behavior.
        signal(SIGINT, SIG_DFL);
        execvp(command->name, command->argv);
        // execvp() only returns if execution failed.
        print_error(command->name);
        exit(EXIT_FAILURE);
    }

    int status;
    // Retry waitpid() if it was interrupted by a signal.
    while (waitpid(pid, &status, 0) == -1)
    {
        if (errno == EINTR)
            continue;

        print_error("waitpid failed");
        return -1;
    }

    return status;
}
