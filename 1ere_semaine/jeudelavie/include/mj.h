#ifndef MJ_H
#define MJ_H

/* en-tete du maitre du jeu */
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief compte les voisins vivants autour d'une cellule.
 * @param grille matrice de la grille courante.
 * @param x coordonnee horizontale de la cellule.
 * @param y coordonnee verticale de la cellule.
 * @return nombre de voisins vivants.
 */
int compter_voisins(int grille[WIDTH][HEIGHT], int x, int y);

/**
 * @brief calcule la grille future a partir de la grille courante.
 * @param grille_courante matrice source.
 * @param grille_future matrice destination.
 */
void maj_synchrone(int grille_courante[WIDTH][HEIGHT],
                   int grille_future[WIDTH][HEIGHT]);

#endif
