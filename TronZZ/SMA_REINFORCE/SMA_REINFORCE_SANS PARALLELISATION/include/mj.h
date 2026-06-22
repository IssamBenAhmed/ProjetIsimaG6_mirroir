#ifndef MJ_H
#define MJ_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "config.h"






void initialiser_grille( int grille[WIDTH][HEIGHT]) ;

void initialiser_partie (int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1]) ;

int compter_moto_vivants(bool etats_vie[MAX_MOTOS + 1]); 

int trouver_gagnant(bool etats_vie[MAX_MOTOS + 1]) ;

int cause_mort( int grille[WIDTH][HEIGHT],  int x , int y); 

static void assigner_zone_adversaire(int id,int zone,int *z1,int *z2,int *z3) ;

static void obtenir_vecteur(int direction, int *dx, int *dy); 

static float calculer_densite_zone( int grille[WIDTH][HEIGHT],int x,int y,int dx1, int dy1, int dx2,int dy2) ;

int existence_adversaire_dans_un_zone ( int grille[WIDTH][HEIGHT],int x,int y,int dx1, int dy1, int dx2,int dy2 , int id_adversaire) ;

void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction, Perception * perception ) ;

void mettre_a_jour_monde(int grille[WIDTH][HEIGHT], int pos_motos[5][2], int dir_motos[5], bool etats_vie[5]);

void nettoyer_trainee(int grille[WIDTH][HEIGHT], int id_moto);


#endif
