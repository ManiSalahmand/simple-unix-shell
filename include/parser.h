#ifndef PARSER_H
#define PARSER_H

/**
 * @brief Represents a parsed command line.
 *
 * This structure stores the command name, argument vector,
 * argument count, and optional input/output redirection files.
 */
typedef struct Command
{
    char *name;         // Command name (same as argv[0])
    char **argv;        // NULL-terminated argument list
    int argc;           // Number of arguments
    char *input_file;   // Input redirection file (<)
    char * output_file; // Output redirection file (>)
} Command;

/**
 * @brief Parse a command line string into a Command structure.
 *
 * Splits the input into tokens, extracts arguments, and detects
 * input/output redirection operators.
 *
 * @param input Raw command line string.
 * @return Pointer to a dynamically allocated Command structure,
 *         or NULL if parsing fails.
 */
Command *parse_command_line(const char *input);

/**
 * @brief Free all memory associated with a Command structure.
 *
 * @param command Pointer to the Command to free.
 */
void free_command(Command *command);

#endif // PARSER_H
