#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

/**
 * @brief Counts the number of tokens (arguments) in the input string.
 *
 * @param input The input command line string to analyze.
 * @return int The number of detected tokens in the input string.
 */
static int count_tokens(const char *input)
{
    int count = 0;
    bool in_token = false;

    while (*input != '\0')
    {
        if (*input == ' ' || *input == '\t' || *input == '\n')
        {
            in_token = false;
        }
        else if (!in_token)
        {
            in_token = true;
            count++;
        }

        input++;
    }

    return count;
}

/**
 * @brief Frees partially allocated memory for a Command structure.
 *
 * @param command Pointer to the Command structure being cleaned up.
 * @param filled Number of argument entries in argv that were successfully allocated.
 */
static void free_partial_command(Command *command, int filled)
{
    if (command == NULL)
        return;

    if (command->argv != NULL)
    {
        for (int i = 0; i < filled; i++)
            free(command->argv[i]);

        free(command->argv);
    }

    free(command);
}

/**
 * @brief Parses a command line string into a Command structure.
 *
 * @param input The raw command line input string entered by the user.
 * @return Command* Pointer to a newly allocated Command structure on
 *                  success, or NULL if parsing fails or the input contains no
 *                  tokens.
 */
Command *parse_command_line(const char* input)
{
    if (input == NULL)
        return NULL;

    int token_count = count_tokens(input);
    if (token_count == 0)
        return NULL;

    Command *command = malloc(sizeof(Command));
    if (command == NULL)
        return NULL;

    command->argc = token_count;
    command->name = NULL;
    command->argv = malloc(sizeof(char *) * (token_count + 1));
    if (command->argv == NULL)
    {
        free(command);
        return NULL;
    }

    char *copy = strdup(input);
    if (copy == NULL)
    {
        free(command->argv);
        free(command);
        return NULL;
    }

    char *saveptr;
    char *token = strtok_r(copy, " \t\n", &saveptr);
    int i = 0;

    while (token != NULL)
    {
        command->argv[i] = strdup(token);
        if (command->argv[i] == NULL)
        {
            free(copy);
            free_partial_command(command, i);
            return NULL;
        }

        i++;
        token = strtok_r(NULL, " \t\n", &saveptr);
    }

    command->argv[i] = NULL;
    command->name = command->argv[0];

    free(copy);
    return command;
}

/**
 * @brief Frees all memory associated with a Command structure.
 *
 * @param command Pointer to the Command structure to be freed.
 */
void free_command(Command *command)
{
    if (command == NULL)
        return;

    if (command->argv != NULL)
    {
        for (int i = 0; i < command->argc; i++)
            free(command->argv[i]);

        free(command->argv);
    }

    free(command);
}
