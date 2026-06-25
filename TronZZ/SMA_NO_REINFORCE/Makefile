CC := gcc
TARGET := sma_no_reinforce
SRC := src/main.c \
       src/agent.c \
       src/mj.c \
       src/graphisme.c
OBJ := $(SRC:.c=.o)
CFLAGS := -Wall -Wextra -Werror -g -std=c11 -Iinclude -fsanitize=address -fsanitize=undefined $(shell pkg-config --cflags sdl2)
LDLIBS := -fsanitize=address -fsanitize=undefined $(shell pkg-config --libs sdl2) -lm

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
