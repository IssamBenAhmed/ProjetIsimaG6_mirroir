#ifndef MJ_H
#define MJ_H

#include <stdbool.h>

#include "config.h"

void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction,
                         float perception[PERCEPTION_SIZE]);
void mettre_a_jour_monde(int grille[WIDTH][HEIGHT],
                         int pos_motos[MAX_MOTOS + 1][2],
                         int dir_motos[MAX_MOTOS + 1],
                         bool etats_vie[MAX_MOTOS + 1]);
void nettoyer_trainee(int grille[WIDTH][HEIGHT], int id_moto);

#endif
