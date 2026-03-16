#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdbool.h>

#include "parser.h"

/**
 * @brief Global flag used to terminate the shell main loop.
 *
 * Set by the `exit` builtin command. The main loop checks this
 * flag to determine when the shell should terminate.
 */
extern bool shell_should_exit;

/**
 * @brief Attempt to execute a builtin command.
 *
 * Checks whether the parsed command corresponds to a supported
 * shell builtin (such as `cd` or `exit`) and executes it if so.
 *
 * @param command Pointer to the parsed Command structure.
 * @return int Returns 0 if a builtin was executed, or -1 if the
 *             command is not a builtin or the input is invalid.
 */
int try_execute_builtin(const Command *command);

#endif // BUILTIN_H
