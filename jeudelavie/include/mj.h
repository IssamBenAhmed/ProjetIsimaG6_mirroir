#ifdef MAITRE_JEU
#define MAITRE_JEU


#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define WIDTH 20 ;
#define HEIGHT 20;


int compter_voisins(int grille[WIDTH][HEIGHT], int x, int y);

void maj_synchrone(int grille_courante[WIDTH][HEIGHT], int grille_future[WIDTH][HEIGHT]);



#endif
