CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11 -D_POSIX_C_SOURCE=200809L -Iinclude

SRC = $(wildcard src/*.c)
BUILD_DIR = build
TARGET = $(BUILD_DIR)/shell

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test: $(TARGET)
	./tests/test_shell.sh

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test clean
