#ifndef GRAPHISME_H
#define GRAPHISME_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "config.h"

/*
 * Affiche l'écran titre avant le début du jeu.
 * Retourne true si le joueur appuie sur Entrée ou Espace.
 * Retourne false si le joueur ferme la fenêtre ou appuie sur Échap.
 */
bool afficher_ecran_titre(SDL_Window *window, SDL_Renderer *renderer);

/*
 * Dessine l'arène du jeu.
 */
void dessiner_arene(SDL_Renderer *renderer,
                    int grille[WIDTH][HEIGHT],
                    int pos_motos[MAX_MOTOS + 1][2],
                    int dir_motos[MAX_MOTOS + 1],
                    bool etats_vie[MAX_MOTOS + 1]);

/*
 * Gère les événements clavier du joueur.
 */
void capturer_evenements(SDL_Event *event, int *direction_joueur, bool *running);

/*
 * Dessine le panneau final.
 * joueur_gagne = true  -> affiche WIN
 * joueur_gagne = false -> affiche LOSE
 */
void dessiner_panneau_fin(SDL_Renderer *renderer, bool joueur_gagne);

#endif
