# CC compiler
CC := gcc

# Flags
CFLAGS := -Wall -Wextra -g -std=c11 -I"../SMA_REINFORCE/SMA_REINFORCE_SANS PARALLELISATION/include" -fsanitize=address -fsanitize=undefined
LDFLAGS := -fsanitize=address -fsanitize=undefined -lm

# Executable name
TARGET := run_tests

# Targets
all: $(TARGET)

$(TARGET): tests.c ../SMA_REINFORCE/SMA_REINFORCE_SANS\ PARALLELISATION/src/agent.c ../SMA_REINFORCE/SMA_REINFORCE_SANS\ PARALLELISATION/src/mj.c
	$(CC) $(CFLAGS) tests.c "../SMA_REINFORCE/SMA_REINFORCE_SANS PARALLELISATION/src/agent.c" "../SMA_REINFORCE/SMA_REINFORCE_SANS PARALLELISATION/src/mj.c" -o $(TARGET) $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o
