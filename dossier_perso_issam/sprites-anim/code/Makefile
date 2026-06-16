CC := gcc
TARGET := sprites_anim
SRC := main.c
SDL_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null)
SDL_LIBS := $(shell sdl2-config --libs 2>/dev/null)
SDL_IMAGE_LIBS := -lSDL2_image
CFLAGS := -Wall -Wextra -std=c11 $(SDL_CFLAGS)
LDLIBS := $(SDL_LIBS) $(SDL_IMAGE_LIBS) -lm

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

re: clean all

.PHONY: all run clean re