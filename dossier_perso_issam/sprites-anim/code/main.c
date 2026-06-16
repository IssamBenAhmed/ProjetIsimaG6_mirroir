// sprite main - ben ahmed issam
// Auteur / titre du fichier

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

static SDL_Texture *load_texture_from_image(const char *file_image_name,
                      SDL_Window *window,
                      SDL_Renderer *renderer)
{
  // Charge une image depuis `file_image_name` et la convertit en SDL_Texture.
  SDL_Surface *my_image = IMG_Load(file_image_name); // Charge l'image en surface (format logiciel)
  if (my_image == NULL) {
    end_sdl(false, "Chargement de l'image impossible", window, renderer); // Erreur si échec
  }

  SDL_Texture *my_texture = SDL_CreateTextureFromSurface(renderer, my_image); // Convertit la surface en texture GPU
  SDL_FreeSurface(my_image); // Libère la surface car elle n'est plus nécessaire

  if (my_texture == NULL) {
    end_sdl(false, "Echec de la transformation de la surface en texture", window, renderer); // Erreur si échec
  }

  return my_texture; // Retourne la texture prête à être rendue
}

int main(int argc, char *argv[])
{
  const char *image_path = NULL;      // Pointeur vers le chemin de l'image (si fourni en argument)
  SDL_Window *window = NULL;          // Pointeur vers la fenêtre SDL
  SDL_Renderer *renderer = NULL;      // Pointeur vers le renderer SDL
  SDL_Texture *texture = NULL;        // Pointeur vers la texture chargée (optionnel)

  (void)argc;                         // Evite l'avertissement sur argc non utilisé

  if (argv[1] != NULL) {              // Si un argument est passé au programme
    image_path = argv[1];             // on le considère comme chemin d'image à charger
  }

  if (SDL_Init(SDL_INIT_VIDEO) != 0) { // Initialise le sous-système vidéo de SDL
    end_sdl(false, SDL_GetError(), window, renderer); // Quitte si l'initialisation échoue
  }

  if ((IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)) == 0) {
    // Initialise SDL_image pour PNG et JPG; vérifie que l'initialisation a réussi
    end_sdl(false, IMG_GetError(), window, renderer); // Quitte en cas d'erreur
  }

  window = SDL_CreateWindow("sprites-anim",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                SDL_WINDOW_SHOWN);
  // Crée une fenêtre nommée "sprites-anim", centrée, de taille définie, visible
  if (window == NULL) {
    end_sdl(false, SDL_GetError(), window, renderer); // Quitte si création échoue
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  // Crée un renderer matériel avec synchronisation verticale si possible
  if (renderer == NULL) {
    end_sdl(false, SDL_GetError(), window, renderer); // Quitte si création échoue
  }

  if (image_path != NULL) {           // Si un chemin d'image a été fourni
    texture = load_texture_from_image(image_path, window, renderer); // Charge la texture
  }

  bool running = true;                // Drapeau de la boucle principale
  SDL_Event event;                    // Structure pour récupérer les événements SDL

  while (running) {                   // Boucle principale du programme
    while (SDL_PollEvent(&event)) {   // Traite tous les événements en file
      if (event.type == SDL_QUIT) {   // Si la fenêtre est fermée
        running = false;              // on quitte la boucle
      } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        running = false;              // Quitte si l'utilisateur presse Échap
      }
    }

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // Définit la couleur de nettoyage (gris foncé)
    SDL_RenderClear(renderer);          // Efface le buffer de rendu avec la couleur définie

    if (texture != NULL) {              // Si une texture est chargée
      SDL_Rect destination = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}; // Rectangle cible couvrant la fenêtre
      SDL_RenderCopy(renderer, texture, NULL, &destination); // Dessine la texture étirée sur la fenêtre
    }

    SDL_RenderPresent(renderer);        // Présente le rendu à l'écran (swap buffers)
    SDL_Delay(16);                      // Petite pause (~60 FPS) pour limiter l'utilisation CPU
  }

  if (texture != NULL) {
    SDL_DestroyTexture(texture);        // Libère la texture si elle existait
  }

  end_sdl(true, NULL, window, renderer); // Nettoie et quitte proprement (ok = true)
  return EXIT_SUCCESS;                   // Valeur de retour (ne sera pas atteinte à cause de end_sdl qui fait exit)
}