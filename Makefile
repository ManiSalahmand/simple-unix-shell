CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude

SRC = src/main.c
TARGET = shell

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
