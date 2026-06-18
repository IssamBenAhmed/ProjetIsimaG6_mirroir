#include "../include/graphisme.h"
#include<SDL2/SLD.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>

void dessiner_arene(SDL_Renderer *renderer, int grille[WIDTH][HEIGHT])
{
    SDL_Rect case_rect;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            case_rect.x = x * TAILLE_CASE;
            case_rect.y = y * TAILLE_CASE;
            case_rect.w = TAILLE_CASE;
            case_rect.h = TAILLE_CASE;

            if (grille[x][y] == 1)
            {
                SDL_RenderCopy(renderer, moto_joueur, NULL, &case_rect);
            }
            else if (grille[x][y] == 2)
            {
                SDL_RenderCopy(renderer, moto_1, NULL, &case_rect);
            }
            else if (grille[x][y] == 3)
            {
                SDL_RenderCopy(renderer, moto_2, NULL, &case_rect);
            }
            else if (grille[x][y] == 4)
            {
                SDL_RenderCopy(renderer, moto_3, NULL, &case_rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}


void capturer_evenements(SDL_Event *event, int *direction_joueur, bool *running)
{
    if (event->type == SDL_QUIT)
    {
        *running = false;
    }

    if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.sym == SDLK_ESCAPE)
        {
            *running = false;
        }

        if (event->key.keysym.sym == SDLK_UP)
        {
            if (*direction_joueur != BAS)
            {
                *direction_joueur = HAUT;
            }
        }

        if (event->key.keysym.sym == SDLK_RIGHT)
        {
            if (*direction_joueur != GAUCHE)
            {
                *direction_joueur = DROITE;
            }
        }

        if (event->key.keysym.sym == SDLK_DOWN)
        {
            if (*direction_joueur != HAUT)
            {
                *direction_joueur = BAS;
            }
        }

        if (event->key.keysym.sym == SDLK_LEFT)
        {
            if (*direction_joueur != DROITE)
            {
                *direction_joueur = GAUCHE;
            }
        }
    }
}

