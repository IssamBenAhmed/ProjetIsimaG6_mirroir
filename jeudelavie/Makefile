CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -Iinclude $(shell pkg-config --cflags sdl2)
LDLIBS = $(shell pkg-config --libs sdl2)

NAME = jeudelavie

SRC = src/main.c \
      src/agent.c \
      src/mj.c \
      src/graphique.c

OBJ = $(SRC:.c=.o)

.PHONY: all clean fclean re run

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDLIBS)

src/%.o: src/%.c include/*.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME)