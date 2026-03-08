#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "error.h"

int main(void)
{
    char *line;

    while (1)
    {
        print_prompt();

        line = read_input();

        if (line == NULL)
            continue;

        if (is_input_empty(line))
        {
            free(line);
            continue;
        }

        free(line);
    }

    return 0;
}
