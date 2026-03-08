# Simple Unix Shell

![Build](https://github.com/ManiSalahmand/simple-unix-shell/actions/workflows/build.yml/badge.svg)

A simple Unix shell implemented in C that supports command execution, pipes, redirections, and built-in commands. Built as a system programming project to understand how shells work.

## Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Build](#build)
- [Clean](#clean)
- [Run](#run)

## Features

Current functionality:
- Interactive shell prompt
- Input reading using `getline`
- Detection and ignoring of empty / whitespace-only input
- Clean exit on EOF (`Ctrl+D`)
- Integration test suite
- Continuous integration build and test checks

Planned features:
- Execute external commands
- Built-in commands (`cd`, `exit`)
- Pipes (`|`)
- Input and output redirection (`<`, `>`)
- Background processes (`&`)
- Signal handling

## Project Structure

```
simple-unix-shell/
├── .github/workflows/  # CI pipeline
├── build/              # Compiled output
├── include/
│   ├── error.h         # Error handling interface
│   └── shell.h         # Shell core declarations
├── src/
│   ├── error.c         # Error handling implementation
│   ├── input.c         # Input utilities
│   └── main.c          # Main shell loop
├── tests/
│   └── test_shell.sh   # Integration tests
├── .gitignore
├── LICENSE
├── Makefile
└── README.md
```

## Requirements

- GCC
- GNU Make

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
shell> ls
shell> pwd
shell>
```

Exit with:

```bash
Ctrl+D
```
