// printsimple main - ben ahmed issam

#include <SDL2/SDL.h>           // inclut l'api principale de sdl2 (fenetre, rendu, evenements, etc.)
#include <SDL2/SDL_image.h>     
#include <stdbool.h>            
#include <stdio.h>              
#include <stdlib.h>             

#define WINDOW_WIDTH 800       // constante largeur de fenetre
#define WINDOW_HEIGHT 600      // constante hauteur de fenetre

static void end_sdl(bool ok, const char *message, SDL_Window *window, SDL_Renderer *renderer)
{
  // fonction utilitaire: nettoie les ressources sdl et termine le programme.
  // ok : true => sortie reussie, false => sortie en erreur.
  if (message != NULL) {
    fprintf(stderr, "%s\n", message); // affiche le message d'erreur si fourni
  }

  if (renderer != NULL) {
    SDL_DestroyRenderer(renderer);    // libere le renderer si cree
  }

  if (window != NULL) {
    SDL_DestroyWindow(window);        // libere la fenetre si creee
  }

  IMG_Quit();                         // desactive sdl_image
  SDL_Quit();                         // desactive sdl2
  exit(ok ? EXIT_SUCCESS : EXIT_FAILURE); // termine le processus avec le code approprie
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