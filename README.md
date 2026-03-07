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

## Requirements

- GCC
- GNU Make

## Build

Compile the project using the provided Makefile:

```bash
make
```

To clean the compiled binary:

```bash
make clean
```

## Run

```bash
./shell
```
