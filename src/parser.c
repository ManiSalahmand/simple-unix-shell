#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "parser.h"

/**
 * @brief Count whitespace-separated tokens in the input string.
 *
 * Tokens are separated by spaces, tabs or newlines.
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
 * @brief Trim leading and trailing whitespace from a mutable string.
 *
 * @param text String to trim in place.
 * @return char* Pointer to the trimmed start of the string.
 */
static char *trim_whitespace(char *text)
{
    while (*text == ' ' || *text == '\t' || *text == '\n')
        text++;

    size_t len = strlen(text);
    while (len > 0)
    {
        char c = text[len - 1];
        if (c != ' ' && c != '\t' && c != '\n')
            break;

        text[len - 1] = '\0';
        len--;
    }

    return text;
}

/**
 * @brief Check whether the input contains an empty pipeline segment
 *
 * Rejects leading pipes, trailing pipes, and consecutive pipes with only
 * whitespace between them.
 *
 * @param input input Raw command line string.
 * @return bool true if an empty pipeline segment is present, otherwise false.
 */
static bool has_empty_pipe_segment(const char *input)
{
    bool need_command = true;

    while (*input != '\0')
    {
        if (*input == ' ' || *input == '\t' || *input == '\n')
        {
            input++;
            continue;
        }

        if (*input == '|')
        {
            if (need_command)
                return true;

            need_command = true;
            input++;
            continue;
        }

        need_command = false;
        input++;
    }

    return need_command;
}

/**
 * @brief Free a partially initialized Command structure.
 *
 * Used during parse failures to release only the memory that was
 * successfully allocated.
 *
 * @param command Pointer to the Command structure being cleaned up.
 * @param filled Number of argument entries in argv already allocated.
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

    free(command->input_file);
    free(command->output_file);
    free(command);
}

/**
 * @brief Parse a single command segment with optional redirections.
 *
 * @param input One pipeline segment without any pipe characters.
 * @return Command* Newly allocated Command success, or NULL on failure
 */
static Command *parse_single_command(const char *input)
{
    int token_count = count_tokens(input);
    if (token_count == 0)
        return NULL;

    Command *command = calloc(1, sizeof(Command));
    if (command == NULL)
        return NULL;

    command->argv = calloc(token_count + 1, sizeof(char *));
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

    char *saveptr = NULL;
    char *token = strtok_r(copy, " \t\n", &saveptr);
    int i = 0;

    while (token != NULL)
    {
        if (strcmp(token, "<") == 0)
        {
            token = strtok_r(NULL, " \t\n", &saveptr);
            if (token == NULL)
            {
                shell_error("parser", "missing filename after '<'");
                free(copy);
                free_partial_command(command, i);
                return NULL;
            }

            free(command->input_file);
            command->input_file = strdup(token);
            if (command->input_file == NULL)
            {
                free(copy);
                free_partial_command(command, i);
                return NULL;
            }
        }
        else if (strcmp(token, ">") == 0)
        {
            token = strtok_r(NULL, " \t\n", &saveptr);
            if (token == NULL)
            {
                shell_error("parser", "missing filename after '>'");
                free(copy);
                free_partial_command(command, i);
                return NULL;
            }

            free(command->output_file);
            command->output_file = strdup(token);
            if (command->output_file == NULL)
            {
                free(copy);
                free_partial_command(command, i);
                return NULL;
            }
        }
        else
        {
            command->argv[i] = strdup(token);
            if (command->argv[i] == NULL)
            {
                free(copy);
                free_partial_command(command, i);
                return NULL;
            }

            i++;
        }

        token = strtok_r(NULL, " \t\n", &saveptr);
    }

    command->argv[i] = NULL;
    command->argc = i;

    if (i > 0)
        command->name = command->argv[0];

    free(copy);
    return command;
}

/**
 * @brief Parse a command line string into a linked pipeline.
 *
 * Splits the input on pipe operators and parses each command segment.
 *
 * @param input The raw command line input string entered by the user.
 * @return Command* Pointer to a newly allocated Command structure on
 *                  success, or NULL if parsing fails or the input
 *                  no commands.
 */
Command *parse_command_line(const char* input)
{
    if (input == NULL)
        return NULL;

    if (has_empty_pipe_segment(input))
    {
        shell_error("parser", "missing command near '|'");
        return NULL;
    }

    char *copy = strdup(input);
    if (copy == NULL)
        return NULL;

    Command *head = NULL;
    Command *tail = NULL;
    char *saveptr = NULL;
    char *segment = strtok_r(copy, "|", &saveptr);

    while (segment != NULL)
    {
        char *trimmed = trim_whitespace(segment);
        if (*trimmed == '\0')
        {
            shell_error("parser", "missing command near '|'");
            free(copy);
            free_command(head);
            return NULL;
        }

        Command *command = parse_single_command(trimmed);
        if (command == NULL)
        {
            free(copy);
            free_command(head);
            return NULL;
        }

        if (head == NULL)
            head = command;
        else
            tail->next = command;

        tail = command;
        segment = strtok_r(NULL, "|", &saveptr);
    }

    free(copy);
    return head;
}

/**
 * @brief Frees all memory associated with a Command structure.
 *
 * @param command Pointer to the Command structure to be freed.
 */
void free_command(Command *command)
{
    while (command != NULL)
    {
        Command *next = command->next;

        if (command->argv != NULL)
        {
            for (int i = 0; i < command->argc; i++)
                free(command->argv[i]);

            free(command->argv);
        }

        free(command->input_file);
        free(command->output_file);
        free(command);

        command = next;
    }
}
