#ifndef ERROR_H
#define ERROR_H

/**
 * @brief Print a system error message.
 *
 * Prints the given message followed by the description of the current
 * errno value using perror().
 *
 * @param message A short description of the failed operation.
 */
void print_error(const char *message);

/**
 * @brief Print a shell-specific error message.
 *
 * Prints a formatted error message to stderr for errors generated
 * by the shell itself rather than the operating system.
 *
 * Format:
 * shell: <context>: <message>
 *
 * @param context The command or shell component where the error occurred.
 * @param message A descriptive message explaining the error.
 */
void shell_error(const char *context, const char *message);

#endif // ERROR_H
