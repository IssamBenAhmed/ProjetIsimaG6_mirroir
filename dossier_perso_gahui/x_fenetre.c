#include <SDL2/SDL.h>
#include <stdio.h>
#define ECRAN_HAUTEUR 1000
#define ECRAN_LARGEUR 1700

     /************************************/
     /*  exemple de création de fenêtres */
     /************************************/

int main(int argc, char **argv) {
   (void)argc;
   (void)argv;

   SDL_Window *window_1[100],*window_2[100];                     // Future fenêtre de droite

   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());                
        exit(EXIT_FAILURE);
      }


   int i =1;
   int j =1; 
   int nb_1 = 0;
   int nb_2 = 0;
   while (i <ECRAN_LARGEUR && j < ECRAN_HAUTEUR){
      window_1[nb_1 ++] = SDL_CreateWindow(
        "Fenêtre top to right",                    // codage en utf8, donc accents possibles
        i, j,                                  
        300, 200,                              // largeur = 400, hauteur = 300
        SDL_WINDOW_RESIZABLE); 
        
      if (window_1 == NULL) {
        SDL_Log("Error : SDL window 1 creation - %s\n", 
                 SDL_GetError());                 // échec de la création de la fenêtre
        SDL_Quit();                              // On referme la SDL       
        exit(EXIT_FAILURE);
      }  
      j = j+20; 
      i = i+40; 


   } 

   

   

   int k = 0 ; // x initial
   int l = 800; // y initial
   
   while(k <= 1700 && l>=0){
      //printf("x :%d, y=%d\n", k,l);
      window_2[nb_2++] = SDL_CreateWindow(
        "Fenêtre top to right",                    // codage en utf8, donc accents possibles
        k, l,                                  
        300, 200,                             
        SDL_WINDOW_RESIZABLE); 
        
      if (window_2== NULL) {
        SDL_Log("Error : SDL window 2 creation - %s\n", 
                 SDL_GetError());                 // échec de la création de la fenêtre
        SDL_Quit();                              // On referme la SDL       
        exit(EXIT_FAILURE);
      }  
      l -= 20; //hauteur
      k += 41; //largeur
      //hauteur
   }

   



       /* Normalement, on devrait ici remplir les fenêtres... */
   SDL_Delay(4000);                           // Pause exprimée  en ms

       /* et on referme tout ce qu'on a ouvert en ordre inverse de la création */
   for (int m = 0; m <= nb_1; m++) {
    if (window_1[m]!= NULL){
      SDL_DestroyWindow(window_1[m]);

    }
   }    
   for (int m = 0; m <= nb_2; m++) {
    if (window_2[m]!= NULL){
      SDL_DestroyWindow(window_2[m]);

    }
   }    
   

    
  SDL_Quit();                               
    return 0;
    }