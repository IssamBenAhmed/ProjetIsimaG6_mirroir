#ifndef GRAPHISME_H
#define GRAPHISME_H

#include <SDL2/SDL.h>

/* Dimensions de la grille */
#define WIDTH 80
#define HEIGHT 50
#define TAILLE_CASE 10

/* Création de la fenêtre SDL2 */
SDL_Window *creer_fenetre(int largeur, int hauteur);

/* Création du renderer SDL2 */
SDL_Renderer *creer_renderer(SDL_Window *window);

/* Dessin de la grille */
void dessiner_grille(SDL_Renderer *renderer,
                     int grille[HEIGHT][WIDTH],
                     float zoom,
                     int cam_x,
                     int cam_y);

/* Gestion des interactions souris */
void gerer_interaction(SDL_Event *event,
                       int grille[HEIGHT][WIDTH],
                       float *zoom,
                       int *cam_x,
                       int *cam_y);

#endif