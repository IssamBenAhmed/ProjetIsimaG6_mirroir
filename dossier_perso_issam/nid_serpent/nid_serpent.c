//nid serpent sdl2 - Issam Ben Ahmed

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SNAKE_LENGTH 15
#define SEGMENT_SIZE 20

int main(int argc, char* argv[]) {
    // no warnings
    (void)argc;
    (void)argv;
    // seed aleatoire
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) return EXIT_FAILURE; // si erreur moteur

    SDL_Window* window = SDL_CreateWindow("Serpent Aleatoire", 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          WINDOW_WIDTH, WINDOW_HEIGHT, 
                                          SDL_WINDOW_SHOWN);
    if (window == NULL) { // si erreur fenetre
        fprintf(stderr, "Erreur creation fenetre : %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) { // si erreur renderer
        fprintf(stderr, "Erreur creation renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // initialisation des articulations du serpent
    SDL_Rect snake[SNAKE_LENGTH]; // tableau des rectangles composite du serpent
    for(int i = 0; i < SNAKE_LENGTH; i++) {
        snake[i].x = WINDOW_WIDTH / 2 - (i * SEGMENT_SIZE); // on place cote a cote tout les rectangles
        snake[i].y = WINDOW_HEIGHT / 2;
        snake[i].w = SEGMENT_SIZE;
        snake[i].h = SEGMENT_SIZE;
    }

    // vecteur de direction de la tete (demarre vers la droite)
    int dx = SEGMENT_SIZE;
    int dy = 0;
    
    bool isRunning = true;
    SDL_Event event;

    while(isRunning) {
        // evenements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                isRunning = false; // si echap ou croix rouge est presse
            }
        }

        //10% de chance de tourner a chaque frame
        if (rand() % 10 == 0) {
            int r = rand() % 4;
            // on s'assure qu'il ne fait pas demi-tour sur lui-même
            if      (r == 0 && dx == 0) { dx = SEGMENT_SIZE;  dy = 0; }             // droite
            else if (r == 1 && dx == 0) { dx = -SEGMENT_SIZE; dy = 0; }             // gauche
            else if (r == 2 && dy == 0) { dx = 0;             dy = SEGMENT_SIZE; }  // bas
            else if (r == 3 && dy == 0) { dx = 0;             dy = -SEGMENT_SIZE; } // haut
        }

        // mise a jour des positions (le corps suit la tete)
        for(int i = SNAKE_LENGTH - 1; i > 0; i--) {
            snake[i].x = snake[i-1].x;
            snake[i].y = snake[i-1].y;
        }

        // fait avancer la tête
        snake[0].x += dx;
        snake[0].y += dy;

        // gestion des collisions avec les murs (rebonds)
        if (snake[0].x < 0) { 
            snake[0].x = 0; dx = SEGMENT_SIZE; dy = 0; 
        }
        if (snake[0].x >= WINDOW_WIDTH) { 
            snake[0].x = WINDOW_WIDTH - SEGMENT_SIZE; dx = -SEGMENT_SIZE; dy = 0; 
        }
        if (snake[0].y < 0) { 
            snake[0].y = 0; dx = 0; dy = SEGMENT_SIZE; 
        }
        if (snake[0].y >= WINDOW_HEIGHT) { 
            snake[0].y = WINDOW_HEIGHT - SEGMENT_SIZE; dx = 0; dy = -SEGMENT_SIZE; 
        }

        //rendu graphique
        SDL_SetRenderDrawColor(renderer, 20, 30, 40, 255); // Fond sombre
        SDL_RenderClear(renderer);

        for(int i = 0; i < SNAKE_LENGTH; i++) {
            // couleur fixe pour tout le serpent
            SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);
            SDL_RenderFillRect(renderer, &snake[i]);
            
            // jointures (contours noirs)
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &snake[i]);
        }

        SDL_RenderPresent(renderer);
        
        // vitesse de déplacement du serpent
        SDL_Delay(80); 
    }

    // nettoyage
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}