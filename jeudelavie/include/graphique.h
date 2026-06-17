#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include <SDL2/SDL.h>
#include "config.h"

/**
 * @brief cree la fenetre principale sdl.
 * @param largeur largeur de la fenetre.
 * @param hauteur hauteur de la fenetre.
 * @return pointeur vers la fenetre creee.
 */
SDL_Window *creer_fenetre(int largeur, int hauteur);

/**
 * @brief cree le renderer sdl associe a la fenetre.
 * @param window fenetre sdl cible.
 * @return pointeur vers le renderer cree.
 */
SDL_Renderer *creer_renderer(SDL_Window *window);

/**
 * @brief dessine la grille courante avec zoom et camera.
 * @param renderer renderer sdl utilise pour l'affichage.
 * @param grille matrice de l'etat courant.
 * @param zoom facteur de zoom courant.
 * @param cam_x decalage horizontal de la camera.
 * @param cam_y decalage vertical de la camera.
 */
void dessiner_grille(SDL_Renderer *renderer,int grille[WIDTH][HEIGHT],float zoom,int cam_x,int cam_y);

/**
 * @brief traite les evenements souris pour l'edition et la navigation.
 * @param event evenement sdl courant.
 * @param grille matrice modifiable.
 * @param zoom pointeur vers le zoom courant.
 * @param cam_x pointeur vers le decalage horizontal.
 * @param cam_y pointeur vers le decalage vertical.
 */
void gerer_interaction(SDL_Event *event,int grille[WIDTH][HEIGHT],float *zoom, int *cam_x, int *cam_y);

#endif




