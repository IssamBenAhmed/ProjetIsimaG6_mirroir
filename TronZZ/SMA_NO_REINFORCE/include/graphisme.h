#ifndef GRAPHISME_H
#define GRAPHISME_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "config.h"

void dessiner_arene(SDL_Renderer *renderer, int grille[WIDTH][HEIGHT]);
void capturer_evenements(SDL_Event *event, int *direction_joueur,
                         bool *running);
void dessiner_panneau_fin(SDL_Renderer *renderer);
#endif
