#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M_PI 3.14

void end_sdl(int ok,                   // fin normale : ok = 0 ; anormale ok = 1
             char const *msg,          // message à afficher
             SDL_Window *window,       // fenêtre à fermer
             SDL_Renderer *renderer) { // renderer à fermer
  char msg_formated[255];
  int l;

  if (!ok) { // Affichage de ce qui ne va pas
    strncpy(msg_formated, msg, 250);
    l = strlen(msg_formated);
    strcpy(msg_formated + l, " : %s\n");

    SDL_Log(msg_formated, SDL_GetError());
  }

  if (renderer != NULL) { // Destruction si nécessaire du renderer
    SDL_DestroyRenderer(
        renderer); // Attention : on suppose que les NULL sont maintenus !!
    renderer = NULL;
  }
  if (window != NULL) { // Destruction si nécessaire de la fenêtre
    SDL_DestroyWindow(
        window); // Attention : on suppose que les NULL sont maintenus !!
    window = NULL;
  }

  SDL_Quit();

  if (!ok) { // On quitte si cela ne va pas
    exit(EXIT_FAILURE);
  }
}

void play_with_texture_4(SDL_Texture *texture, SDL_Window *window,
                         SDL_Renderer *renderer) {
  SDL_Rect window_dim = {0};
  SDL_GetWindowSize(window, &window_dim.w,
                    &window_dim.h); // Rectangle définissant la fenêtre

  int tex_w, tex_h;
  SDL_QueryTexture(
      texture, NULL, NULL, &tex_w,
      &tex_h); // Rectangle définissant la zone de la texture à récupérer

  int frame_count = 8; // frame de animation à utiliser, en fonction de l'image
  int source_w = tex_w / frame_count;
  int source_h = tex_h / 4;

  SDL_Rect source = {
      0, 0, source_w,
      source_h}; // Rectangle définissant la zone de la texture à récupérer

  SDL_Rect destination = {// Rectangle définissant où la zone_source doit être
                          // déposée dans le renderer
                          0, (window_dim.h - source_h * 2) / 2, source_w * 2,
                          source_h * 2};

  int nb_it = 200; // nombre itération

  for (int i = 0; i < nb_it; i++) {

    source.x = (i / 5) % frame_count *
               source_w; // on déplace la zone de source à utiliser

    destination.x += 5; // on déplace image

    // préparation et affichage de l'image
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, &source, &destination);
    SDL_RenderPresent(renderer);

    SDL_Delay(30);
  }
}

void play_with_texture_3_alter(SDL_Texture *my_texture, SDL_Window *window,
                               SDL_Renderer *renderer) {
  SDL_Rect source =
               {0}, // Rectangle définissant la zone de la texture à récupérer
      window_dimensions = {0}, // Rectangle définissant la fenêtre, on
                               // n'utilisera que largeur et hauteur
      destination = {0}; // Rectangle définissant où la zone_source doit être
                         // déposée dans le renderer

  SDL_GetWindowSize(
      window, &window_dimensions.w,
      &window_dimensions.h); // Récupération des dimensions de la fenêtre
  SDL_QueryTexture(my_texture, NULL, NULL, &source.w,
                   &source.h); // Récupération des dimensions de l'image

  // On décide de déplacer dans la fenêtre cette image
  float zoom = 0.25; // Facteur de zoom entre l'image source et l'image affichée

  int nb_it = 200;                 // Nombre d'images de l'animation
  destination.w = source.w * zoom; // On applique le zoom sur la largeur
  destination.h = source.h * zoom; // On applique le zoom sur la hauteur
  destination.x =
      (window_dimensions.w - destination.w) / 2; // On centre en largeur
  float h = (window_dimensions.h - destination.h) / 2;
  float angle;
  float radius = h; // hauteur du déplacement à effectuer

  for (int i = 0; i < nb_it; ++i) { // mouvement cercle

    angle = (2 * M_PI * i) / nb_it;

    destination.x =
        (window_dimensions.w / 2) + radius * cos(angle) - destination.w / 2;

    destination.y =
        (window_dimensions.h / 2) + radius * sin(angle) - destination.h / 2;

    SDL_RenderClear(renderer); // Effacer l'image précédente

    SDL_SetTextureAlphaMod(
        my_texture,
        ((float)i / nb_it) *
            255); // L'opacité va passer de 0 à 255 au fil de l'animation
    SDL_RenderCopy(renderer, my_texture, &source,
                   &destination); // Préparation de l'affichage
    SDL_RenderPresent(renderer);  // Affichage de la nouvelle image
    SDL_Delay(30);                // Pause en ms
  }
  SDL_RenderClear(renderer); // Effacer la fenêtre une fois le travail terminé
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Texture *my_texture = NULL;

  // SDL_DisplayMode screen;               // Future fenêtre de gauche
  /* Initialisation de la SDL  + gestion de l'échec possible */
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n",
            SDL_GetError()); // l'initialisation de la SDL a échoué
    exit(EXIT_FAILURE);
  }

  // Création de la fenêtre de gauche
  window =
      SDL_CreateWindow("Fenêtre", // codage en utf8, donc accents possibles
                       300, 500,  // coin haut gauche en haut gauche de l'écran
                       1000, 800, // largeur = 400, hauteur = 300
                       SDL_WINDOW_RESIZABLE); // redimensionnable

  if (window == NULL) {
    SDL_Log("Error : SDL window 1 creation - %s\n",
            SDL_GetError()); // échec de la création de la fenêtre
    SDL_Quit();              // On referme la SDL
    exit(EXIT_FAILURE);
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL)
    end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

  my_texture = IMG_LoadTexture(renderer, "./image/Maze.png");
  if (my_texture == NULL)
    end_sdl(0, "Echec du chargement de l'image dans la texture", window,
            renderer);

  SDL_Rect destination; // sone d'affichage d'image
  // ici, dans l'animation, il y a 3 petits animation
  // phase1
  destination.x = 0;
  destination.y = 0;
  destination.w = 1000;
  destination.h = 800;

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, my_texture, NULL, &destination);
  SDL_RenderPresent(
      renderer); // Normalement, on devrait ici remplir les fenêtres...
  SDL_Delay(2000);

  destination.x = 400;
  destination.y = 300;
  destination.w = 200;
  destination.h = 300;
  my_texture = IMG_LoadTexture(renderer, "./image/emoji_uh.png");
  SDL_RenderCopy(renderer, my_texture, NULL, &destination);
  SDL_RenderPresent(
      renderer); // Normalement, on devrait ici remplir les fenêtres...
  SDL_Delay(1000);

  SDL_RenderClear(renderer); // fin de phase 1 de animation
  // phase 2
  destination.x = 000;
  destination.y = 000;
  destination.w = 700;
  destination.h = 900;

  my_texture = IMG_LoadTexture(renderer, "./image/Nebula Aqua-Pink.png");
  SDL_RenderCopy(renderer, my_texture, NULL, &destination);
  SDL_RenderPresent(
      renderer); // Normalement, on devrait ici remplir les fenêtres...
  SDL_Delay(1000);

  my_texture = IMG_LoadTexture(renderer, "./image/emoji.png");

  play_with_texture_3_alter(my_texture, window, renderer);

  SDL_RenderClear(renderer); // fin de phase 2

  // phase 3
  my_texture = IMG_LoadTexture(renderer, "./image/player-spritemap-v9.png");
  play_with_texture_4(my_texture, window, renderer);

  SDL_Delay(2000);

  // ici, animation finit donc libérer l'image et fenetre

  SDL_DestroyTexture(my_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
