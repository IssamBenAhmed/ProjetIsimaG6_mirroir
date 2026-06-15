//nid serpent sdl2 - Issam Ben Ahmed

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

/*1. Init la SDL ,
2. Création de la fenêtre,
3. Création du rendu matériel ,
4.create loop +dessin,
5. Nettoyage */

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Initialisation

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {// si erreur init
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Création de la fenêtre

    SDL_Window* window = SDL_CreateWindow("Nid de serpent - Projet ISIMA G6",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {// si erreur window
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Création du rendu matériel

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {// si erreur renderer
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    