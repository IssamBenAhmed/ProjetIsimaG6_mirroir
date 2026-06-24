#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/agent.h"
#include "../include/config.h"
#include "../include/graphique.h"
#include "../include/mj.h"

/* point d'entree principal du programme */
int main(int argc, char *argv[]) {
  /* evite les warnings pour les arguments non utilises */
  (void)argc;
  (void)argv;

  /* initialisation de sdl */
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Erreur d'initialisation de SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  /* creation de la fenetre principale */
  SDL_Window *window = SDL_CreateWindow(
      "Jeu de la Vie - Architecture SMA", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

  /* arrete le programme si la fenetre ne peut pas etre creee */
  if (!window) {
    fprintf(stderr, "Erreur de creation de la fenetre: %s\n", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  /* creation du renderer */
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  /* arrete le programme si le renderer ne peut pas etre cree */
  if (!renderer) {
    fprintf(stderr, "Erreur de creation du renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  /* grille double tampon pour basculer entre courant et futur */
  int mondes[2][WIDTH][HEIGHT] = {0};
  int courant = 0;
  int futur = 1;

  /* variables d'etat de la simulation et de la camera */
  float zoom = 1.0f;
  int cam_x = 0;
  int cam_y = 0;
  bool running = true;
  bool paused =
      true; /* demarre en pause pour pouvoir dessiner au clic initial */

  SDL_Event event;
  Uint32 last_update_time = SDL_GetTicks();

  /* boucle principale du jeu */
  while (running) {

    /* gestion des evenements et interactions */
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_SPACE) {
          paused = !paused; /* barre espace pause ou lecture */
        } else if (event.key.keysym.sym == SDLK_ESCAPE) {
          running = false; /* echap quitter */
        }
      }

      /* appel au module graphisme pour gerer la souris */
      gerer_interaction(&event, mondes[courant], &zoom, &cam_x, &cam_y);
    }

    /* mise a jour de la simulation par le mj */
    Uint32 current_time = SDL_GetTicks();
    if (!paused && (current_time - last_update_time >= DELAY_MS)) {

      /* le mj calcule le futur a partir du courant */
      maj_synchrone(mondes[courant], mondes[futur]);

      /* permutation des indices en o(1) */
      courant = 1 - courant;
      futur = 1 - futur;

      last_update_time = current_time;
    }

    /* phase de rendu graphique par le graphiste */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); /* fond noir */
    SDL_RenderClear(renderer);

    /* le graphiste dessine la grille courante actualisee */
    dessiner_grille(renderer, mondes[courant], zoom, cam_x, cam_y);

    SDL_RenderPresent(renderer);

    /* limiteur pour ne pas saturer le thread cpu inutilement */
    SDL_Delay(1);
  }

  /* nettoyage et fermeture de la fenetre */
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}