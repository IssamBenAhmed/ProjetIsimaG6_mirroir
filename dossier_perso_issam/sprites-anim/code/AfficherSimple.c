// printsimple main - ben ahmed issam

#include <SDL2/SDL.h>           // Inclut l'API principale de SDL2 (fenêtre, rendu, événements, etc.)
#include <SDL2/SDL_image.h>     
#include <stdbool.h>            
#include <stdio.h>              
#include <stdlib.h>             

#define WINDOW_WIDTH 800       // Constante largeur de fenêtre
#define WINDOW_HEIGHT 600      // Constante hauteur de fenêtre

static void end_sdl(bool ok, const char *message, SDL_Window *window, SDL_Renderer *renderer)
{
  // Fonction utilitaire: nettoie les ressources SDL et termine le programme.
  // `ok` : true => sortie réussie, false => sortie en erreur.
  if (message != NULL) {
    fprintf(stderr, "%s\n", message); // Affiche le message d'erreur si fourni
  }

  if (renderer != NULL) {
    SDL_DestroyRenderer(renderer);    // Libère le renderer si créé
  }

  if (window != NULL) {
    SDL_DestroyWindow(window);        // Libère la fenêtre si créée
  }

  IMG_Quit();                         // Désactive SDL_image
  SDL_Quit();                         // Désactive SDL2
  exit(ok ? EXIT_SUCCESS : EXIT_FAILURE); // Termine le processus avec le code approprié
}

void PrintImage(char *image){
    
    SDL_Window *window = SDL_CreateWindow(image, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          WINDOW_WIDTH, WINDOW_HEIGHT, 
                                          SDL_WINDOW_SHOWN);


    SDL_Renderer* renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );

 
    if(!window){
        end_sdl(false,"erreur ouverture window",window,renderer);
    }
    if(!renderer){
        end_sdl(false,"erreur init renderer",window,renderer);
    }




    SDL_Texture *mytexture = IMG_LoadTexture(renderer,image);
    SDL_RenderCopy(renderer,mytexture,NULL,NULL);
    SDL_RenderPresent(renderer);


    bool isrunning = true;
    SDL_Event event;

    while(isrunning){

        while(SDL_PollEvent(&event)){

            if(event.type == SDL_QUIT ||
                 (event.type == SDL_KEYDOWN &&
                     event.key.keysym.sym == SDLK_ESCAPE)) {

                isrunning = false;

            }

        }
        
    }
    
    
    SDL_DestroyTexture(mytexture);
    IMG_Quit();
    SDL_Quit();
    

}

int main() {
    PrintImage("/home/issam/Images/BODY_skeleton.png");
    return 1;
}