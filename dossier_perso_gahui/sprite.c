#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL_image.h>


void end_sdl(int ok,                                               // fin normale : ok = 0 ; anormale ok = 1
      char const* msg,                                       // message à afficher
      SDL_Window* window,                                    // fenêtre à fermer
      SDL_Renderer* renderer) {                              // renderer à fermer
       char msg_formated[255];                                      
       int l;                                         
                                
       if (!ok) {                                                        // Affichage de ce qui ne va pas
   strncpy(msg_formated, msg, 250);                                   
   l = strlen(msg_formated);                                      
   strcpy(msg_formated + l, " : %s\n");                                 
                                
   SDL_Log(msg_formated, SDL_GetError());                                 
       }                                            
                                
       if (renderer != NULL) {                                           // Destruction si nécessaire du renderer
   SDL_DestroyRenderer(renderer);                                  // Attention : on suppose que les NULL sont maintenus !!
   renderer = NULL;
       }
       if (window != NULL)   {                                           // Destruction si nécessaire de la fenêtre
   SDL_DestroyWindow(window);                                      // Attention : on suppose que les NULL sont maintenus !!
   window= NULL;
       }
                                
       SDL_Quit();                                          
                                
       if (!ok) {                             // On quitte si cela ne va pas            
   exit(EXIT_FAILURE);                                        
       }                                            
}                




int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    SDL_Window  *window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture * my_texture = NULL ; 

    SDL_DisplayMode screen;               // Future fenêtre de gauche  
       /* Initialisation de la SDL  + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n", 
                  SDL_GetError());                // l'initialisation de la SDL a échoué 
        exit(EXIT_FAILURE);
    }

       /* Création de la fenêtre de gauche */
    window= SDL_CreateWindow(
     "Fenêtre",                    // codage en utf8, donc accents possibles
     300, 500,                                  // coin haut gauche en haut gauche de l'écran
     1000, 800,                              // largeur = 400, hauteur = 300
     SDL_WINDOW_RESIZABLE);                 // redimensionnable

    if (window== NULL) {
        SDL_Log("Error : SDL window 1 creation - %s\n", 
                 SDL_GetError());                 // échec de la création de la fenêtre
        SDL_Quit();                              // On referme la SDL       
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

    //my_texture = IMG_LoadTexture(renderer,"Documents/Maze.png");
    //if (my_texture == NULL) end_sdl(0, "Echec du chargement de l'image dans la texture", window, renderer);

    


    SDL_RenderPresent(renderer);/* Normalement, on devrait ici remplir les fenêtres... */
    SDL_Delay(2000);                           // Pause exprimée  en ms

    SDL_DestroyTexture(my_texture);   
    SDL_DestroyRenderer(renderer);   
    SDL_DestroyWindow(window);
       
       
       
           

    SDL_Quit();                               
 
    return 0;
     }

