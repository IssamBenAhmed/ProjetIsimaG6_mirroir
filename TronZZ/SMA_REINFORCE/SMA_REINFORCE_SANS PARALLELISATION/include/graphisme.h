/**
 * @file graphisme.h
 * @brief Gestion de l'affichage graphique 2D (SDL2) pour le jeu TronZZ.
 */

#ifndef GRAPHISME_H
#define GRAPHISME_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "config.h"

/**
 * @brief Charge en memoire GPU les images et textures des motos (PNG).
 * @param renderer Le renderer SDL associe a la fenetre active.
 * @return True si le chargement a reussi, False sinon.
 */
bool initialiser_textures_motos(SDL_Renderer *renderer);

/**
 * @brief Libere proprement de la memoire GPU toutes les textures chargees.
 */
void liberer_textures_motos(void);

/**
 * @brief Affiche l'ecran titre avec animation et attend le choix de l'utilisateur.
 * @param window Pointeur vers la fenetre SDL.
 * @param renderer Le renderer SDL utilise pour le dessin.
 * @return True si l'utilisateur valide (Entree/Espace), False s'il quitte (Echap/Fermeture).
 */
bool afficher_ecran_titre(SDL_Window *window, SDL_Renderer *renderer);

/**
 * @brief Dessine l'arene du jeu (le fond, la grille, les trainees et les motos animées).
 * @param renderer Le renderer SDL de destination.
 * @param grille La grille 2D globale.
 * @param pos_motos Tableau des positions courantes des motos.
 * @param dir_motos Tableau des directions courantes des motos.
 * @param etats_vie Tableau de l'etat de vie de chaque moto.
 */
void dessiner_arene(SDL_Renderer *renderer,
                    int grille[WIDTH][HEIGHT],
                    int pos_motos[MAX_MOTOS + 1][2],
                    int dir_motos[MAX_MOTOS + 1],
                    bool etats_vie[MAX_MOTOS + 1]);

/**
 * @brief Capture et traite les evenements clavier du joueur humain.
 * @param event Pointeur vers l'evenement SDL capture.
 * @param direction_joueur Pointeur vers la direction de deplacement du joueur humain a mettre a jour.
 * @param running Pointeur vers le booleen controlant la boucle principale du jeu.
 */
void capturer_evenements(SDL_Event *event, int *direction_joueur, bool *running);

/**
 * @brief Dessine un ecran de fin de partie personnalise (WIN / LOSE) en incrustation de type néon.
 * @param renderer Le renderer SDL de destination.
 * @param joueur_gagne True si le joueur humain a gagne, False s'il a perdu.
 */
void dessiner_panneau_fin(SDL_Renderer *renderer, bool joueur_gagne);

#endif