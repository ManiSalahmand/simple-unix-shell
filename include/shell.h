#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>

/**
 * @brief Display the interactive shell prompt.
 */
void print_prompt(void);

/**
 * @brief Check whether an input string contains only whitespace.
 *
 * @param input The string to check.
 * @return true if the string is NULL or contains only whitespace.
 * @return false if the string contains non-whitespace characters.
 */
bool is_input_empty(const char *input);

/**
 * @brief Read a line of input from stdin.
 *
 * Uses getline() to read a full line from the user.
 *
 * @return Pointer to a dynamically allocated string containing the input.
 *         The caller must free the returned buffer.
 *         Returns NULL if the read was interrupted or failed.
 */
char *read_input(void);

/**
 * @brief Remove the trailing newline from a string if present.
 *
 * @param input String returned by getline().
 */
void trim_newline(char *input);

#endif // SHELL_H
