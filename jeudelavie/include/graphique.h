#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include <SDL2/SDL.h>
#include "config.h"

SDL_Window *creer_fenetre(int largeur, int hauteur);

SDL_Renderer *creer_renderer(SDL_Window *window);

void dessiner_grille(SDL_Renderer *renderer,int grille[WIDTH][HEIGHT],float zoom,int cam_x,int cam_y);

void gerer_interaction(SDL_Event *event,int grille[WIDTH][HEIGHT],float *zoom, int *cam_x, int *cam_y);

#endif




