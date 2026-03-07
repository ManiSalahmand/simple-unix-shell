# Simple Unix Shell
A simple Unix shell implemented in C that supports command execution, pipes, redirections, and built-in commands. Built as a system programming project to understand how shells work.

## Features

- Execute external commands
- Built-in commands (`cd`, `exit`)
- Pipes (`|`)
- Input and output redirection (`<`, `>`)
- Background processes (`&`)
- Signal handling

## Project Structure

```
simple-unix-shell/
├── src/        # Source files
├── include/    # Header files
├── build/      # Compiled output
├── Makefile
├── README.md
└── .gitignore
```

## Build

```bash
make
```

## Run

```bash
./shell
```
