# CC compiler
CC := gcc

# Flags
CFLAGS := -Wall -Wextra -g -O2 -std=c11 -I"../include" -fsanitize=address -fsanitize=undefined
LDFLAGS := -fsanitize=address -fsanitize=undefined -lm

# Executable name
TARGET := run_tests

.PHONY: all run clean clear

all: $(TARGET)

$(TARGET): tests.c ../src/agent.c ../src/mj.c
	$(CC) $(CFLAGS) tests.c "../src/agent.c" "../src/mj.c" -o $(TARGET) $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

clear: clean
