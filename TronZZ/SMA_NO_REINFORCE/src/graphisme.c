#include "../include/graphisme.h"

void dessiner_arene(SDL_Renderer *renderer, int grille[WIDTH][HEIGHT]) {
  SDL_Rect case_rect;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0,
                         255); // 255=opaque totalement visible # transparent
  SDL_RenderClear(renderer);

  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      if (grille[x][y] == CELL_EMPTY) {
        continue;
      }

      case_rect.x = x * CELL_SIZE;
      case_rect.y = y * CELL_SIZE;
      case_rect.w = CELL_SIZE;
      case_rect.h = CELL_SIZE;

      if (grille[x][y] == CELL_PLAYER) {
        SDL_SetRenderDrawColor(renderer, 0, 180, 255, 255); // bleu clair
      } else if (grille[x][y] == CELL_AI_1) {
        SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255); // orange
      } else if (grille[x][y] == CELL_AI_2) {
        SDL_SetRenderDrawColor(renderer, 0, 200, 100, 255); // vert clair
      } else {
        SDL_SetRenderDrawColor(renderer, 220, 80, 200, 255); // rose violet
      }

      SDL_RenderFillRect(renderer, &case_rect);
    }
  }

  SDL_RenderPresent(renderer);
}

void capturer_evenements(SDL_Event *event, int *direction_joueur,
                         bool *running) {
  if (event->type == SDL_QUIT) {
    *running = false;
    return;
  }

  if (event->type != SDL_KEYDOWN) {
    return;
  }

  if (event->key.keysym.sym == SDLK_ESCAPE) {
    *running = false;
    return;
  }

  if (event->key.keysym.sym == SDLK_UP && *direction_joueur != DIR_DOWN) {
    *direction_joueur = DIR_UP;
  } else if (event->key.keysym.sym == SDLK_RIGHT &&
             *direction_joueur != DIR_LEFT) {
    *direction_joueur = DIR_RIGHT;
  } else if (event->key.keysym.sym == SDLK_DOWN &&
             *direction_joueur != DIR_UP) {
    *direction_joueur = DIR_DOWN;
  } else if (event->key.keysym.sym == SDLK_LEFT &&
             *direction_joueur != DIR_RIGHT) {
    *direction_joueur = DIR_LEFT;
  }
}
void dessiner_panneau_fin(SDL_Renderer *renderer) {
  // Panneau central rouge
  SDL_Rect panneau = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50, 300,
                      100}; // x,y,largeur,hauteur
  SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
  SDL_RenderFillRect(renderer, &panneau);

  // Bordure blanche
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // conteur blan
  SDL_RenderDrawRect(renderer, &panneau);

  // Dessin manuel des lettres E N D en gros pixels
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  // Lettre E
  SDL_Rect e1 = {panneau.x + 50, panneau.y + 20, 10, 60};
  SDL_Rect e2 = {panneau.x + 50, panneau.y + 20, 40, 10};
  SDL_Rect e3 = {panneau.x + 50, panneau.y + 45, 30, 10};
  SDL_Rect e4 = {panneau.x + 50, panneau.y + 70, 40, 10};
  SDL_RenderFillRect(renderer, &e1);
  SDL_RenderFillRect(renderer, &e2);
  SDL_RenderFillRect(renderer, &e3);
  SDL_RenderFillRect(renderer, &e4);

  // Lettre N
  SDL_Rect n1 = {panneau.x + 120, panneau.y + 20, 10, 60};
  SDL_Rect n2 = {panneau.x + 160, panneau.y + 20, 10, 60};
  SDL_Rect n3 = {panneau.x + 130, panneau.y + 30, 15, 15};
  SDL_Rect n4 = {panneau.x + 145, panneau.y + 45, 15, 15};
  SDL_RenderFillRect(renderer, &n1);
  SDL_RenderFillRect(renderer, &n2);
  SDL_RenderFillRect(renderer, &n3);
  SDL_RenderFillRect(renderer, &n4);

  // Lettre D
  SDL_Rect d1 = {panneau.x + 200, panneau.y + 20, 10, 60};
  SDL_Rect d2 = {panneau.x + 200, panneau.y + 20, 30, 10};
  SDL_Rect d3 = {panneau.x + 200, panneau.y + 70, 30, 10};
  SDL_Rect d4 = {panneau.x + 230, panneau.y + 30, 10, 40};
  SDL_RenderFillRect(renderer, &d1);
  SDL_RenderFillRect(renderer, &d2);
  SDL_RenderFillRect(renderer, &d3);
  SDL_RenderFillRect(renderer, &d4);
}
