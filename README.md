# Simple Unix Shell

![Build](https://github.com/ManiSalahmand/simple-unix-shell/actions/workflows/build.yml/badge.svg)

A simple Unix shell implemented in C to understand how Unix shells work internally.
The project demonstrates core system programming concepts such as process
creation, command parsing, and program execution using `fork()` and `execvp()`.

## Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Build](#build)
- [Clean](#clean)
- [Run](#run)
- [Testing](#testing)

## Features

Current functionality:
- Interactive shell prompt
- Input reading using `getline()`
- Detection and ignoring of empty / whitespace-only input
- Command line parsing into `argv` format
- Execution of external commands using `fork()` + `execvp()`
- Parent process waits for child process using `waitpid()`
- Clean exit on EOF (`Ctrl+D`)
- Handle `SIGINT` (`Ctrl+C`) without existing the shell
- Built-in commands (`cd`, `exit`)
- Integration test suite
- Continuous integration build and test checks

Planned features:
- Pipes (`|`)
- Input redirection (`<`)
- Output redirection (`>`)
- Background processes (`&`)
- Command history
- Tab completion
- Environment variable support
- Job control

## Project Structure

```
simple-unix-shell/
├── .github/workflows/
├── build/
├── include/
│   ├── builtin.h
│   ├── error.h
│   ├── executor.h
│   ├── parser.h
│   ├── shell.h
│   └── signal_handler.h
├── src/
│   ├── builtin.c
│   ├── error.c
│   ├── executor.c
│   ├── input.c
│   ├── main.c
│   ├── parser.c
│   └── signals.c
├── tests/
│   ├── test_shell.sh
│   └── test_sigint.py
├── .gitignore
├── LICENSE
├── Makefile
└── README.md
```

## Requirements

- GCC
- GNU Make
- POSIX-compatible system (Linux / macOS / WSL)
- Python 3
- pytest

## Build

Compile the project using the provided Makefile:

```bash
make
```

This produces the executable:

```bash
./build/shell
```

To clean the compiled binary:

```bash
make clean
```

## Run

Start the shell with:

```bash
./build/shell
```

Example session:

```bash
shell> echo hello
hello
shell>
```

Exit with:

```bash
Ctrl+D
```

## Testing
Run the integration test suite:

```bash
make test
```

The tests verify:
- prompt behavior
- whitespace handling
- command parsing and argument handling
- execution of valid commands
- handling of invalid commands
- execution of multiple commands in sequence
- correct handling of `SIGNINT` (`Ctrl+C`)
- interruption of child processes without terminating the shell
- `cd` and `exit` builtins
