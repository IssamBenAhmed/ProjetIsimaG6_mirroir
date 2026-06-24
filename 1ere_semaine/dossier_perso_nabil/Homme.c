// HOmme dessiné fait par LAAMARCHI NABIL

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Programme : Homme dessiné avec des formes géométriques

    Idée du programme :
    - On crée une fenêtre SDL.
    - On crée un renderer pour dessiner dans la fenêtre.
    - On dessine un homme avec des formes simples :
        tête   : cercle plein
        corps  : rectangle
        bras   : lignes
        jambes : lignes
        pieds  : lignes
    - Les flèches du clavier déplacent l'homme.
    - q ou Échap permet de quitter le programme.
*/

#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 600

#define VITESSE 5

int main(void) {
  SDL_Window *window = NULL; // Fenêtre principale.
  SDL_Renderer *renderer =
      NULL; // Outil qui permet de dessiner dans la fenêtre.

  SDL_Event event;               // Variable qui reçoit les événements.
  SDL_bool continuer = SDL_TRUE; // Variable qui contrôle la boucle principale.

  SDL_Rect corps; // Rectangle qui représente le corps de l'homme.

  int homme_x = 400; // Position horizontale de l'homme.
  int homme_y = 300; // Position verticale de l'homme.

  int x;          // Variable utilisée pour dessiner la tête.
  int y;          // Variable utilisée pour dessiner la tête.
  int rayon = 25; // Rayon de la tête.

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Erreur : SDL ne demarre pas : %s\n", SDL_GetError());
    return 1;
  }

  window = SDL_CreateWindow("Un homme", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, LARGEUR_FENETRE,
                            HAUTEUR_FENETRE, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("Erreur creation fenetre : %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
    printf("Erreur creation renderer : %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  /*
      Boucle principale.
      Tant que continuer vaut SDL_TRUE, le programme reste ouvert.
  */
  while (continuer) {
    while (SDL_PollEvent(&event)) {
      /*
          Si on clique sur la croix de la fenêtre,
          on quitte le programme.
      */
      if (event.type == SDL_QUIT) {
        continuer = SDL_FALSE;
      }

      /*
          Si une touche du clavier est appuyée.
      */
      if (event.type == SDL_KEYDOWN) {
        /*
            q ou Échap permettent de quitter.
        */
        if (event.key.keysym.sym == SDLK_ESCAPE ||
            event.key.keysym.sym == SDLK_q) {
          continuer = SDL_FALSE;
        }
      }
    }

    /*
        SDL_GetKeyboardState permet de savoir si une touche
        est maintenue appuyée.

        Cela permet d'obtenir un déplacement plus fluide.
    */
    const Uint8 *touches = SDL_GetKeyboardState(NULL);

    /*
        Déplacement vers le haut.quand y diminue, on monte.
    */
    if (touches[SDL_SCANCODE_UP]) {
      homme_y = homme_y - VITESSE;
    }

    /*
        Déplacement vers le bas.quand y augmente, on descend.
    */
    if (touches[SDL_SCANCODE_DOWN]) {
      homme_y = homme_y + VITESSE;
    }

    /*
        Déplacement vers la gauche.
        Quand x diminue, on va à gauche.
    */
    if (touches[SDL_SCANCODE_LEFT]) {
      homme_x = homme_x - VITESSE;
    }

    /*
        Déplacement vers la droite.
        Quand x augmente, on va à droite.
    */
    if (touches[SDL_SCANCODE_RIGHT]) {
      homme_x = homme_x + VITESSE;
    }

    /*
        Limites de la fenêtre.
        On empêche l'homme de sortir de l'écran.
    */
    if (homme_x < 50) {
      homme_x = 50;
    }

    if (homme_x > LARGEUR_FENETRE - 50) {
      homme_x = LARGEUR_FENETRE - 50;
    }

    if (homme_y < 120) {
      homme_y = 120;
    }

    if (homme_y > HAUTEUR_FENETRE - 80) {
      homme_y = HAUTEUR_FENETRE - 80;
    }

    /*
        On choisit la couleur noire.
        Ensuite, on efface l'écran avec cette couleur.
    */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    /*
        Dessin du sol.
        Le sol est une ligne grise horizontale.
    */
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawLine(renderer, 0, 430, LARGEUR_FENETRE, 430);

    /*
        Dessin de la tête.
        On parcourt un carré autour du centre de la tête.
        Si le point est à l'intérieur du cercle, on le dessine.
    */
    SDL_SetRenderDrawColor(renderer, 230, 190, 140, 255);

    for (x = -rayon; x <= rayon; x++) {
      for (y = -rayon; y <= rayon; y++) {
        if (x * x + y * y <= rayon * rayon) {
          SDL_RenderDrawPoint(renderer, homme_x + x, homme_y - 100 + y);
        }
      }
    }

    /*
        Dessin du corps.
        Le corps est un rectangle bleu.
    */
    corps.x = homme_x - 20;
    corps.y = homme_y - 75;
    corps.w = 40;
    corps.h = 70;

    SDL_SetRenderDrawColor(renderer, 0, 120, 255, 255);
    SDL_RenderFillRect(renderer, &corps);

    /*
        Dessin des bras.
        Les bras sont deux lignes de couleur peau.
    */
    SDL_SetRenderDrawColor(renderer, 230, 190, 140, 255);

    SDL_RenderDrawLine(renderer, homme_x - 20, homme_y - 60, homme_x - 55,
                       homme_y - 25);

    SDL_RenderDrawLine(renderer, homme_x + 20, homme_y - 60, homme_x + 55,
                       homme_y - 25);

    /*
        Dessin des jambes.
        Les jambes sont deux lignes bleues.
    */
    SDL_SetRenderDrawColor(renderer, 40, 40, 200, 255);

    SDL_RenderDrawLine(renderer, homme_x - 10, homme_y - 5, homme_x - 35,
                       homme_y + 70);

    SDL_RenderDrawLine(renderer, homme_x + 10, homme_y - 5, homme_x + 35,
                       homme_y + 70);

    /*
        Dessin des pieds.
        Les pieds sont deux petites lignes horizontales.
    */
    SDL_RenderDrawLine(renderer, homme_x - 35, homme_y + 70, homme_x - 60,
                       homme_y + 70);

    SDL_RenderDrawLine(renderer, homme_x + 35, homme_y + 70, homme_x + 60,
                       homme_y + 70);

    /*
        Affichage de tout ce qui a été dessiné.
    */
    SDL_RenderPresent(renderer);

    SDL_Delay(16);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
