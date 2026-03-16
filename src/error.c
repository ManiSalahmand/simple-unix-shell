#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "error.h"

/**
 * @brief Print a system error message.
 *
 * Uses perror() to print the provided message followed by the
 * description of the current errno value.
 *
 * @param message A short description of the operation that failed.
 */
void print_error(const char *message)
{
    perror(message);
}

/**
 * @brief Print a custom shell error message.
 *
 * Prints a formatted error message to stderr for shell-specific
 * errors that are not related to errno.
 *
 * Example:
 * shell: cd: missing argument
 *
 * @param context The command or shell component where the error occurred.
 * @param message A descriptive message explaining the error.
 */
void shell_error(const char *context, const char *message)
{
    fprintf(stderr, "shell: %s: %s\n", context, message);
}
