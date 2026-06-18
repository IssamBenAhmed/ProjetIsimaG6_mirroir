#include "../include/graphisme.h"

void dessiner_arene(SDL_Renderer *renderer, int grille[WIDTH][HEIGHT])
{
    SDL_Rect case_rect;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
                SDL_SetRenderDrawColor(renderer, 0, 180, 255, 255);
            } else if (grille[x][y] == CELL_AI_1) {
                SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
            } else if (grille[x][y] == CELL_AI_2) {
                SDL_SetRenderDrawColor(renderer, 0, 200, 100, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 220, 80, 200, 255);
            }

            SDL_RenderFillRect(renderer, &case_rect);
        }
    }

    SDL_RenderPresent(renderer);
}

void capturer_evenements(SDL_Event *event, int *direction_joueur, bool *running)
{
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
    } else if (event->key.keysym.sym == SDLK_RIGHT && *direction_joueur != DIR_LEFT) {
        *direction_joueur = DIR_RIGHT;
    } else if (event->key.keysym.sym == SDLK_DOWN && *direction_joueur != DIR_UP) {
        *direction_joueur = DIR_DOWN;
    } else if (event->key.keysym.sym == SDLK_LEFT && *direction_joueur != DIR_RIGHT) {
        *direction_joueur = DIR_LEFT;
    }
}

