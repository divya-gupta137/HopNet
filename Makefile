CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -g
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = hopnet

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean
