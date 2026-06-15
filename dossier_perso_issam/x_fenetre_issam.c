//X_fenetre Issam BEN AHMED

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Initialisation
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {// si erreur init
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Fenetre X - Projet ISIMA G6",
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

    bool isRunning = true;
    int currentWidth = WINDOW_WIDTH;
    int currentHeight = WINDOW_HEIGHT;
    SDL_Event event;

    // Boucle d'événements
    while (isRunning) {
        while (SDL_PollEvent(&event)) { // le moteur surveille les événements
            if (event.type == SDL_QUIT) { // Si l'utilisateur ferme la fenêtre
                isRunning = false; // On quitte la boucle
            } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                currentWidth = event.window.data1;
                currentHeight = event.window.data2;
            } else if (event.type == SDL_KEYDOWN) { // Si une touche est pressée
                if (event.key.keysym.sym == SDLK_ESCAPE) {// Si c'est la touche Echap
                    isRunning = false;// On quitte la boucle
                }
            }
        }

        // 1. Fond noir
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // 2. Couleur du trait (Rouge)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        // 3. Tracé du X
        SDL_RenderDrawLine(renderer, 0, 0, currentWidth, currentHeight);
        SDL_RenderDrawLine(renderer, 0, currentHeight, currentWidth, 0);

        // 4. Affichage
        SDL_RenderPresent(renderer);
        
        // Limite à ~60 FPS pour soulager le CPU
        SDL_Delay(16); 
    }

    // Nettoyage
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}