CC := gcc

# Nom de l'exécutable final
TARGET := tronzz

# Liste des fichiers sources
SRC := src/main.c \
       src/agent.c \
       src/mj.c \
       src/graphisme.c \
       src/parallel.c

OBJ := $(SRC:.c=.o)

# Flags génériques
CFLAGS := -Wall -Wextra -g -O2 -std=c11 -Iinclude $(shell pkg-config --cflags sdl2)
LDLIBS := $(shell pkg-config --libs sdl2) -lSDL2_image -lm

.PHONY: all parallel noreinforce clean fclean re run clear

# Règle par défaut : REINFORCE standard (séquentiel)
all: $(TARGET)

# Cible pour la compilation avec Parallélisation (C11 threads)
parallel: CFLAGS += -DPARALLEL -pthread
parallel: LDLIBS += -pthread
parallel: re

# Cible pour la compilation sans REINFORCE (Heuristique simple)
noreinforce: CFLAGS += -DNO_REINFORCE
noreinforce: re

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

src/%.o: src/%.c include/*.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)


fclean: clean
	rm -f $(TARGET)

re: fclean $(TARGET)

run: all
	./$(TARGET) play
