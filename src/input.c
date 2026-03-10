#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "error.h"
#include "shell.h"

/**
 * @brief Displays the shell prompt to the user.
 *
 */
void print_prompt(void)
{
    printf("shell> ");
    fflush(stdout);
}

/**
 * @brief Checks whether the input string contains only whitespace characters.
 *
 * @param input The input string to check.
 * @return bool Returns true if the input is empty or contains only
 *              whitespace characters, otherwise returns false.
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
 * @brief Reads a line of input from the user.
 *
 * @return char* Pointer to input string (must be freed by caller).
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

        if (feof(stdin))
        {
            printf("\n");
            exit(0);
        }

        print_error("getline failed");
        return NULL;
    }

    trim_newline(line);

    return line;
}

/**
 * @brief Removes the trailing newline from input if present.
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
