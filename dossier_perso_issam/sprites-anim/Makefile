CC := gcc

# Nom de l'exécutable généré par la compilation.
TARGET := sprites_anim

# Fichier source principal du projet.
SRC := Sprite.c

# Options de compilation récupérées via sdl2-config.
SDL_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null)
SDL_LIBS := $(shell sdl2-config --libs 2>/dev/null)
SDL_IMAGE_LIBS := -lSDL2_image

# Drapeaux de compilation pour avoir des warnings utiles.
CFLAGS := -Wall -Wextra -std=c11 $(SDL_CFLAGS)

# Bibliothèques liées à l'édition de liens.
LDLIBS := $(SDL_LIBS) $(SDL_IMAGE_LIBS) -lm

all: $(TARGET)

# Construit l'exécutable à partir du source unique.
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

# Lance le programme compilé.
run: $(TARGET)
	./$(TARGET)

# Supprime l'exécutable généré.
clean:
	rm -f $(TARGET)

# Raccourci pour reconstruire complètement.
re: clean all

.PHONY: all run clean re