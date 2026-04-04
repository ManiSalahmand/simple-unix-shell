#ifndef PARSER_H
#define PARSER_H

/**
 * @brief Represents one parsed command in a pipeline.
 *
 * This structure stores the command name, argument vector,
 * argument count, and optional input/output redirection files,
 * and a pointer ot the next command when pipes are used.
 */
typedef struct Command
{
    char *name;             // Command name (same as argv[0])
    char **argv;            // NULL-terminated argument list
    int argc;               // Number of arguments
    char *input_file;       // Input redirection file (<)
    char * output_file;     // Output redirection file (>)
    struct Command *next;   // Next command in apipeline, or NULL
} Command;

/**
 * @brief Parse a command line string into one or more linked commands.
 *
 * Splits the input into pipeline segments, extracts arguments for each
 * command, and input/output redirection operators.
 *
 * @param input Raw command line string.
 * @return Pointer to a dynamically allocated Command structure,
 *         or NULL if parsing fails.
 */
Command *parse_command_line(const char *input);

/**
 * @brief Free all memory associated with a command or command pipeline.
 *
 * @param command Pointer to the Command to free.
 */
void free_command(Command *command);

#endif // PARSER_H
