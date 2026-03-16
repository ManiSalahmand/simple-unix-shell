#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "error.h"
#include "shell.h"

/**
 * @brief Displays the shell prompt.
 */
void print_prompt(void)
{
    printf("shell> ");
    fflush(stdout); // ensure the prompt appears immediately
}

/**
 * @brief Checks whether the input string contains only whitespace characters.
 *
 * @param input The input string to check.
 * @return bool Returns true if the input is NULL or contains only
 *              whitespace characters, otherwise false.
 */
bool is_input_empty(const char* input)
{
    if (input == NULL)
        return true;

    for (size_t i = 0; input[i] != '\0'; i++)
    {
        if (!isspace((unsigned char)input[i]))
            return false;
    }

    return true;
}

/**
 * @brief Reads a line of input from stdin.
 *
 * Uses getline() to dynamically allocate a buffer large enough
 * to hold the full input line.
 *
 * @return char* Pointer to input string (must be freed by caller)
 *               or NULL if the read was interrupted or failed.
 */
char *read_input(void)
{
    char *line = NULL;
    size_t buffer_size = 0;
    ssize_t chars_read;

    chars_read = getline(&line, &buffer_size, stdin);

    if (chars_read == -1)
    {
        free(line);

        // Exit the shell cleanly on EOF (Ctrl+D)
        if (feof(stdin))
        {
            printf("\n");
            exit(0);
        }

        // If interrupted by a signal (e.g., Ctrl+C), retry the loop
        if (errno == EINTR)
        {
            clearerr(stdin);
            return NULL;
        }

        print_error("getline failed");
        return NULL;
    }

    trim_newline(line);

    return line;
}

/**
 * @brief Removes the trailing newline from an input string.
 *
 * getline() stores the newline character if present, so this
 * function replaces it with a null terminator.
 *
 * @param input Input string returned by getline().
 */
void trim_newline(char *input)
{
    if (input == NULL)
        return;

    size_t len = strlen(input);

    if (len > 0 && input[len - 1] == '\n')
        input[len - 1] = '\0';
}
