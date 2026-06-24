CC := gcc
TARGET := TronZZ
SRC := src/main.c \
       src/agent.c \
       src/mj.c \
       src/graphisme.c
OBJ := $(SRC:.c=.o)
CFLAGS := -Wall -Wextra -g -std=c11 -Iinclude -fsanitize=undefined $(shell pkg-config --cflags sdl2 SDL2_image) -pthread
LDLIBS := $(shell pkg-config --libs sdl2 SDL2_image) -lm -pthread

.PHONY: all clean fclean re run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

src/%.o: src/%.c include/*.h
	$(CC) $(CFLAGS) -c $< -o $@

run play: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all
