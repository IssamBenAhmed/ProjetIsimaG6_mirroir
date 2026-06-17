#ifdef MAITRE_JEU
#define MAITRE_JEU


#include <stdlib.h>
#include <stdio.h>
#include <string.h>




int compter_voisins(int grille[WIDTH][HEIGHT], int x, int y);

void maj_synchrone(int grille_courante[WIDTH][HEIGHT], int grille_future[WIDTH][HEIGHT]);



#endif
