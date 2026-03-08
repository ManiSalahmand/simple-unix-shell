#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "error.h"

/**
 * @brief Print a system error message.
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
 * @param context The command or shell component where the error occurred.
 * @param message A descriptive message explaining the error.
 */
void shell_error(const char *context, const char *message)
{
    fprintf(stderr, "shell: %s: %s\n", context, message);
}
