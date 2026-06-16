CC := gcc
TARGET := x_fenetre
SRC := x_fenetre.c
SDL_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null)
SDL_LIBS := $(shell sdl2-config --libs 2>/dev/null)
CFLAGS := -Wall -Wextra -std=c11 -include stdlib.h $(SDL_CFLAGS)
LDLIBS := $(SDL_LIBS) -lm

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(TARGET)

re: clean all
