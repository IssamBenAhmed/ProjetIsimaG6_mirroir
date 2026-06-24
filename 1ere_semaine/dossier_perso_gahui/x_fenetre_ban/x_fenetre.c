#include <SDL2/SDL.h>
#include <stdio.h>
#define ECRAN_HAUTEUR 700
#define ECRAN_LARGEUR 1850
#define TAILLE 100

/************************************/
/*  exemple de création de fenêtres */
/************************************/

void SDL_axe1(SDL_Window *window[TAILLE]) {
  int i = 1;
  int j = 1;
  int nb = 0;
  while (i < ECRAN_LARGEUR && j < ECRAN_HAUTEUR) {
    window[nb] = SDL_CreateWindow(
        "Fenêtre top to right", // codage en utf8, donc accents possibles
        i, j, 300, 200,         // largeur = 400, hauteur = 300
        0);

    if (window[nb] == NULL) {
      SDL_Log("Error : SDL window 1 creation - %s\n",
              SDL_GetError()); // échec de la création de la fenêtre
      SDL_Quit();              // On referme la SDL
      exit(EXIT_FAILURE);
    }
    j = j + 21; // modification de coordonnée y
    i = i + 49; // modification de coordonnée x
    nb++;
  }
}

void SDL_axe2(SDL_Window *window[TAILLE]) {
  int k = 0;             // x initial
  int l = ECRAN_HAUTEUR; // y initial
  int nb = 0;

  while (k <= ECRAN_LARGEUR && l >= 0) {
    // printf("x :%d, y=%d\n", k,l);
    window[nb] = SDL_CreateWindow(
        "Fenêtre top to right", // codage en utf8, donc accents possibles
        k, l, 300, 200, 0);

    if (window[nb] == NULL) {
      SDL_Log("Error : SDL window 2 creation - %s\n",
              SDL_GetError()); // échec de la création de la fenêtre
      SDL_Quit();              // On referme la SDL
      exit(EXIT_FAILURE);
    }
    l -= 21; // hauteur
    k += 49; // largeur
    nb++;
  }
}

void liberer_window(SDL_Window *window[TAILLE],
                    int nb) { // fonction pour libérer les fenetres
  for (int m = 0; m < nb; m++) {
    if (window[m] != NULL) {
      SDL_DestroyWindow(window[m]);
    }
  }
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  SDL_Window *window_1[TAILLE] = {NULL},
             *window_2[TAILLE] = {NULL}; // Future fenêtre de droite

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_axe1(window_1); // tracer un diagonale

  SDL_axe2(window_2); // tracer autre diagonale

  SDL_Delay(3000); // Pause exprimée  en ms

  liberer_window(window_1, TAILLE);
  liberer_window(window_2, TAILLE);

  SDL_Quit();
  return 0;
}