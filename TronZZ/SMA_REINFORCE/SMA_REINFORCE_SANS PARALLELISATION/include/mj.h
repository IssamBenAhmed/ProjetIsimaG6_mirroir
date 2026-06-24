#ifndef MJ_H
#define MJ_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "config.h"
#include "agent.h"

void modifier_recompense (float recompense, FrameMemoire * frame) ;

void initialiser_grille( int grille[WIDTH][HEIGHT]) ;

void initialiser_partie (int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1]) ;

int compter_moto_vivants(bool etats_vie[MAX_MOTOS + 1]); 

int trouver_gagnant(bool etats_vie[MAX_MOTOS + 1]) ;

bool gagnant_ou_pas(bool etats_vie[MAX_MOTOS + 1], EpisodeMemoire memoires[MAX_MOTOS + 1]);

int cause_mort( int grille[WIDTH][HEIGHT],  int x , int y); 

int existence_adversaire_dans_un_zone(int grille[WIDTH][HEIGHT], int x, int y, int dx1, int dy1, int dx2, int dy2, int mon_id);
void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction, int mon_id, Perception * perception);

void mettre_a_jour_monde(int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1], EpisodeMemoire memoires[MAX_MOTOS + 1]); 

void mettre_a_jour_monde_entrainement(int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1], EpisodeMemoire memoires[MAX_MOTOS + 1]);

void nettoyer_trainee(int grille[WIDTH][HEIGHT], int id_moto);

#endif