CC := gcc

TARGET := sma_reinforce_avec_paral

SRC := src/main.c \
       src/agent.c \
       src/mj.c \
       src/graphisme.c \
       src/parallel.c

OBJ := $(SRC:.c=.o)

CFLAGS := -Wall -Wextra -g -pg -std=c11 -Iinclude $(shell pkg-config --cflags sdl2) -pthread
LDLIBS := -pg $(shell pkg-config --libs sdl2) -lm -pthread

.PHONY: all clean fclean re run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

src/%.o: src/%.c include/*.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all
