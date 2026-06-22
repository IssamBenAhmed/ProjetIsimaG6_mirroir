#ifndef AGENT_H
#define AGENT_H

#include "config.h"

float random_float(void);
int choisir_action(Perception p);

typedef struct {
    // Module 1 : Vision linéaire de précision (Les rayons) Lidar
    float distances_murs[3]; // [0]=Devant, [1]=Gauche, [2]=Droite
    
    // Module 2 : Vision globale "Floue" (Les zones) Radar
    float densite_obstacles[4]; // [0]=Avant-G, [1]=Avant-D, [2]=Arriere-G, [3]=Arriere-D
    int zone_adversaire_1;      // ID de la zone (0 à 3) où se trouve l'adversaire
    int zone_adversaire_2;
    int zone_adversaire_3;
} Perception;

#endif
