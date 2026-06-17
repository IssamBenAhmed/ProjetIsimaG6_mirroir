#ifndef MJ_H
#define MJ_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"




int compter_voisins(int grille[WIDTH][HEIGHT], int x, int y);

void maj_synchrone(int grille_courante[WIDTH][HEIGHT], int grille_future[WIDTH][HEIGHT]);



#endif
