#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "shell.h"

int main(void)
{
    char *line = NULL;
    size_t buffer_size = 0;
    ssize_t chars_read;

    while (1)
    {
        print_prompt();
        chars_read = getline(&line, &buffer_size, stdin);

        if (chars_read == -1)
        {
            printf("\n");
            break;
        }

        if (is_input_empty(line))
            continue;
    }

    free(line);
    return 0;
}
