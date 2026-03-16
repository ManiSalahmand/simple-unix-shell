#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

/**
 * @brief Execute a parsed command.
 *
 * Runs shell built-ins directly in the parent process. For external
 * commands, forks a child process and executes the program using execvp().
 *
 * @param command Pointer to a parsed Command structure.
 * @return int Child process status on success, or -1 on failure.
 */
int execute_command(const Command *command);

#endif // EXECUTOR_H
