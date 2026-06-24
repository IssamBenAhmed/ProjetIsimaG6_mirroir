// nid de serpent sdl2 - ben ahmed issam

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SNAKE_LENGTH 40 // plus long pour accentuer la fluidite
#define SPACING                                                                \
  5.0f // distance exacte maintenue entre chaque articulation, l'ajout de 'f'
       // force le type float pour des calculs plus precis et rapides
#define SPEED 4.0f
#define RECT_SIZE 16
#define PI 3.14159265358979323846f

// utilisation de flottants pour un mouvement continu et des calculs precis
typedef struct {
  float x, y;
} Segment;

int main(int argc, char *argv[]) {
  // neutralisation des warnings
  (void)argc;
  (void)argv;

  // initialisation du generateur de nombres aleatoires
  srand(time(NULL));

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    return EXIT_FAILURE;

  SDL_Window *window = SDL_CreateWindow(
      "Serpent Fluide", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "Erreur creation fenetre : %s\n", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  // activation de la vsync pour une animation fluide callee sur le taux de
  // rafraichissement
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    fprintf(stderr, "Erreur creation renderer : %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  Segment snake[SNAKE_LENGTH];
  for (int i = 0; i < SNAKE_LENGTH; i++) {
    snake[i].x = WINDOW_WIDTH / 2.0f;
    snake[i].y = WINDOW_HEIGHT / 2.0f;
  }

  float base_angle = 0.0f;
  bool isRunning = true;
  SDL_Event event;

  while (isRunning) {
    // 1. evenements
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ||
          (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
        isRunning = false;
      }
    }

    // 2. mathematiques du deplacement de la tete

    // modification lente de l'angle principal pour que le serpent tourne au
    // hasard (rand() % 100) - 50 donne un nombre entre -50 et 49 on multiplie
    // par un tres petit float pour lisser la rotation d'une frame a l'autre
    // "gère la découverte de la map par le serpent"
    base_angle += ((rand() % 100) - 50) * 0.002f;

    // ajout de la fonction sinus basee sur le temps ecoule (sdl_getticks)
    // le sinus cree une onde reguliere entre -1 et 1
    // cela rajoute une oscillation (dandinement) continue autour de l'angle de
    // base
    float head_angle = base_angle + sin(SDL_GetTicks() * 0.005f) * 0.8f;

    // conversion des coordonnees polaires (angle, vitesse) vers cartesiennes
    // (x, y) la trigonometrie classique: l'axe x depend du cosinus, l'axe y
    // depend du sinus
    snake[0].x += cos(head_angle) * SPEED;
    snake[0].y += sin(head_angle) * SPEED;

    // gestion des collisions avec les bords par reflexion d'angle
    // pi (m_pi) represente 180 degres. faire m_pi - angle inverse la direction
    // horizontale faire -angle inverse la direction verticale
    if (snake[0].x < 0) {
      snake[0].x = 0;
      base_angle = PI - base_angle;
    } // cos(pi-angle) = -cos(angle)
    if (snake[0].x > WINDOW_WIDTH) {
      snake[0].x = WINDOW_WIDTH;
      base_angle = PI - base_angle;
    }
    if (snake[0].y < 0) {
      snake[0].y = 0;
      base_angle = -base_angle;
    } // sin(-angle) = -sin(angle)
    if (snake[0].y > WINDOW_HEIGHT) {
      snake[0].y = WINDOW_HEIGHT;
      base_angle = -base_angle;
    }

    // 3. cinematique inverse (le corps suit la tete)

    for (int i = 1; i < SNAKE_LENGTH; i++) {
      // calcul du vecteur de direction entre le segment precedent et l'actuel
      float dx = snake[i - 1].x - snake[i].x;
      float dy = snake[i - 1].y - snake[i].y;

      // calcul de la distance eulerienne
      float dist = sqrt(dx * dx + dy * dy);

      // si l'articulation s'eloigne au-dela de l'espacement autorise, on la
      // tire
      if (dist > SPACING) {
        // (dx / dist) et (dy / dist) creent un vecteur normalise (de taille 1)
        // on multiplie ce vecteur par l'espacement voulu pour placer le segment
        // a la bonne distance on soustrait ce resultat a la position du segment
        // precedent
        snake[i].x = snake[i - 1].x - (dx / dist) * SPACING;
        snake[i].y = snake[i - 1].y - (dy / dist) * SPACING;
      }
    }

    // 4. rendu graphique

    SDL_SetRenderDrawColor(renderer, 20, 30, 40, 255);
    SDL_RenderClear(renderer);

    // on dessine a l'envers (de la queue vers la tete)
    // de cette facon la tete est affichee en dernier et recouvre le reste du
    // corps
    for (int i = SNAKE_LENGTH - 1; i >= 0; i--) {

      // on centre le carre sur les coordonnees du segment
      SDL_Rect rect = {(int)(snake[i].x - RECT_SIZE / 2),
                       (int)(snake[i].y - RECT_SIZE / 2), RECT_SIZE, RECT_SIZE};

      // la couleur varie en fonction de la position dans le corps (i)
      SDL_SetRenderDrawColor(renderer, 255 - (i * 5), 100 + (i * 3), 50, 255);
      SDL_RenderFillRect(renderer, &rect);

      // trace du bord noir
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderDrawRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
  }

  // 5. nettoyage
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}