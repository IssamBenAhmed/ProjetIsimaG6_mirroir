// X_fenetre Issam BEN AHMED

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define NB_FENETRES_PAR_DIAG 10
#define TAILLE_FENETRE 150

int main(int argc, char *argv[]) {
  // anti-warning
  (void)argc;
  (void)argv;

  if (SDL_Init(SDL_INIT_VIDEO) !=
      0) { // si l'initialisation de la SDL est déffectueuse
    fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_DisplayMode ecran;
  if (SDL_GetCurrentDisplayMode(0, &ecran) !=
      0) { // si la récupération de la résolution de l'écran est déffectueuse
    fprintf(stderr, "Erreur DisplayMode : %s\n", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  int largeur_ecran = ecran.w;
  int hauteur_ecran = ecran.h;

  SDL_Window *mes_fenetres[NB_FENETRES_PAR_DIAG *
                           2]; // init du tableau de fenetre à générer
  int index = 0;

  // 1. Génération de la première diagonale
  for (int i = 0; i < NB_FENETRES_PAR_DIAG; i++) {
    int pos_x =
        i * (largeur_ecran - TAILLE_FENETRE) / (NB_FENETRES_PAR_DIAG - 1);
    int pos_y =
        i * (hauteur_ecran - TAILLE_FENETRE) / (NB_FENETRES_PAR_DIAG - 1);

    mes_fenetres[index] =
        SDL_CreateWindow("Diagonale 1", pos_x, pos_y, TAILLE_FENETRE,
                         TAILLE_FENETRE, SDL_WINDOW_SHOWN);
    SDL_Delay(50); // Raccourci un peu pour l'exemple
    index++;
  }

  // 2. Génération de la deuxième diagonale
  for (int i = 0; i < NB_FENETRES_PAR_DIAG; i++) {
    int pos_x =
        (largeur_ecran - TAILLE_FENETRE) -
        (i * (largeur_ecran - TAILLE_FENETRE) / (NB_FENETRES_PAR_DIAG - 1));
    int pos_y =
        i * (hauteur_ecran - TAILLE_FENETRE) / (NB_FENETRES_PAR_DIAG - 1);

    mes_fenetres[index] =
        SDL_CreateWindow("Diagonale 2", pos_x, pos_y, TAILLE_FENETRE,
                         TAILLE_FENETRE, SDL_WINDOW_SHOWN);
    SDL_Delay(50);
    index++;
  }

  bool isRunning = true;
  SDL_Event event;
  int x, y;

  // On cible la fenêtre en haut à gauche (indice 0)
  SDL_Window *fenetre_cible = mes_fenetres[0];

  while (isRunning) {
    while (SDL_PollEvent(&event)) {
      // Fermeture via la croix rouge d'une des fenêtres
      if (event.type == SDL_QUIT) {
        isRunning = false;
      }
      // Détection du clavier
      else if (event.type == SDL_KEYDOWN) {
        // Touche Echap pour tout quitter
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          isRunning = false;
        }
        // Flèches directionnelles pour déplacer la fenêtre 0
        else {
          SDL_GetWindowPosition(fenetre_cible, &x, &y);

          if (event.key.keysym.sym == SDLK_UP) {
            y -= 30; // On la déplace de 30 pixels d'un coup
          } else if (event.key.keysym.sym == SDLK_DOWN) {
            y += 30;
          } else if (event.key.keysym.sym == SDLK_LEFT) {
            x -= 30;
          } else if (event.key.keysym.sym == SDLK_RIGHT) {
            x += 30;
          }

          SDL_SetWindowPosition(fenetre_cible, x, y);
        }
      }
    }

    // Soulage le processeur
    SDL_Delay(16);
  }

  // 3. Nettoyage
  for (int i = 0; i < index; i++) {
    if (mes_fenetres[i] != NULL) {
      SDL_DestroyWindow(mes_fenetres[i]);
    }
  }

  SDL_Quit();
  return EXIT_SUCCESS;
}