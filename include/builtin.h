#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdbool.h>

#include "parser.h"

extern bool shell_should_exit;

int try_execute_builtin(const Command *command);

#endif
