#ifndef GRAPHISME_H
#define GRAPHISME_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "config.h"

void dessiner_arene(SDL_Renderer *renderer, int grille[WIDTH][HEIGHT]);
void capturer_evenements(SDL_Event *event, int *direction_joueur, bool *running);

#endif
