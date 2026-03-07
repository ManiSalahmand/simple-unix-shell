#include <stdio.h>
#include <ctype.h>
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
 * @return int Returns 1 if the input is empty or contains only whitespace,
 *             otherwise returns 0.
 */
int is_input_empty(const char* input)
{
    if (input == NULL)
        return 1;

    for (size_t i = 0; input[i] != '\0'; i++)
    {
        if (!isspace((unsigned char)input[i]))
            return 0;
    }

    return 1;
}
