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
 * @brief Return the number of commands in a pipeline.
 *
 * @param command Pointer to the first command.
 * @return int Number of linked commands.
 */
static int count_pipeline_commands(const Command *command)
{
    int count = 0;

    while (command != NULL)
    {
        count++;
        command = command->next;
    }

    return count;
}

/**
 * @brief Apply input and output redirections for one command.
 *
 * @param command Pointer to the command whose redirections should be applied.
 * @return int Returns 0 on success, or -1 on failure.
 */
static int apply_redirections(const Command *command)
{
    if (command->input_file != NULL)
    {
        int fd = open(command->input_file, O_RDONLY);
        if (fd < 0)
        {
            print_error("open");
            return -1;
        }

        if (dup2(fd, STDIN_FILENO) < 0)
        {
            close(fd);
            print_error("dup2");
            return -1;
        }

        close(fd);
    }

    if (command->output_file != NULL)
    {
        int fd = open(command->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            print_error("open");
            return -1;
        }

        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            close(fd);
            print_error("dup2");
            return -1;
        }

        close(fd);
    }

    return 0;
}

/**
 * @brief Execute one or more parsed commands.
 *
 * Runs shell built-ins directly in the parent process when no pipe is used/
 * For external commands and pipelines, forks child processes, wires pipes,
 * applies redirections, and executes programs with execvp().
 *
 * @param command Pointer to the first parsed command structure.
 * @return int The final child process status on success, or -1 on failure.
 */
int execute_command(const Command *command)
{
    if (command == NULL || command->argv == NULL || command->name == NULL)
        return -1;

    if (command->next == NULL && try_execute_builtin(command) == 0)
        return 0;

    int command_count = count_pipeline_commands(command);
    pid_t *pids = calloc((size_t)command_count, sizeof(pid_t));
    if (pids == NULL)
    {
        print_error("calloc failed");
        return -1;
    }

    int prev_read_fd = -1;
    int index = 0;
    const Command *current = command;

    while (current != NULL)
    {
        int pipefd[2] = {-1, -1};
        if (current->next != NULL)
        {
            if (pipe(pipefd) == -1)
            {
                print_error("pipe failed");
                if (prev_read_fd != -1)
                    close(prev_read_fd);
                free(pids);
                return -1;
            }
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            print_error("fork failed");
            if (prev_read_fd != -1)
                close(prev_read_fd);
            if (pipefd[0] != -1)
                close(pipefd[0]);
            if (pipefd[1] != -1)
                close(pipefd[1]);
            free(pids);
            return -1;
        }

        if (pid == 0)
        {
            signal (SIGINT, SIG_DFL);

            if (prev_read_fd != -1)
            {
                if (dup2(prev_read_fd, STDIN_FILENO) < 0)
                {
                    print_error("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            if (pipefd[1] != -1)
            {
                if (dup2(pipefd[1], STDOUT_FILENO) < 0)
                {
                    print_error("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            if (prev_read_fd != -1)
                close(prev_read_fd);
            if (pipefd[0] != -1)
                close(pipefd[0]);
            if (pipefd[1] != -1)
                close(pipefd[1]);

            if (apply_redirections(current) == -1)
                exit(EXIT_FAILURE);

            execvp(current->name, current->argv);
            print_error(current->name);
            exit(EXIT_FAILURE);
        }

        pids[index++] = pid;

        if (prev_read_fd != -1)
            close(prev_read_fd);
        if (pipefd[1] != -1)
            close(pipefd[1]);

        prev_read_fd = pipefd[0];
        current = current->next;
    }

    if (prev_read_fd != -1)
        close(prev_read_fd);

    int final_status = 0;
    for (int i = 0; i < index; i++)
    {
        int status;
        while (waitpid(pids[i], &status, 0) == -1)
        {
            if (errno == EINTR)
                continue;

            print_error("waitpid failed");
            free(pids);
            return -1;
        }

        final_status = status;
    }

    free(pids);
    return final_status;
}
