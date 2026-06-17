#include "graphisme.h"

#include <stdio.h>
#include <stdlib.h>

SDL_Window *creer_fenetre(int largeur, int hauteur)
{
    SDL_Window *window = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Erreur SDL_Init : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow(
        "Jeu de la Vie - SMA",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        largeur,
        hauteur,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL)
    {
        SDL_Log("Erreur SDL_CreateWindow : %s", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    return window;
}

SDL_Renderer *creer_renderer(SDL_Window *window)
{
    SDL_Renderer *renderer = NULL;

    if (window == NULL)
    {
        SDL_Log("Erreur : la fenetre est NULL");
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL)
    {
        SDL_Log("Erreur SDL_CreateRenderer : %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    return renderer;
}

void dessiner_grille(SDL_Renderer *renderer,
                     int grille[HEIGHT][WIDTH],
                     float zoom,
                     int cam_x,
                     int cam_y)
{
    SDL_Rect cellule;
    int taille_affichee;
    int x;
    int y;

    taille_affichee = (int)(CELL_SIZE * zoom);

    if (taille_affichee < 1)
    {
        taille_affichee = 1;
    }

    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            cellule.x = x * taille_affichee + cam_x;
            cellule.y = y * taille_affichee + cam_y;
            cellule.w = taille_affichee;
            cellule.h = taille_affichee;

            if (grille[y][x] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &cellule);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &cellule);
            }

            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderDrawRect(renderer, &cellule);
        }
    }
}

void gerer_interaction(SDL_Event *event,
                       int grille[HEIGHT][WIDTH],
                       float *zoom,
                       int *cam_x,
                       int *cam_y)
{
    static int clic_droit_appuye = 0;
    static int ancien_x = 0;
    static int ancien_y = 0;

    int souris_x;
    int souris_y;
    int taille_affichee;

    int local_x;
    int local_y;

    int case_x;
    int case_y;

    taille_affichee = (int)(CELL_SIZE * (*zoom));

    if (taille_affichee < 1)
    {
        taille_affichee = 1;
    }

    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            souris_x = event->button.x;
            souris_y = event->button.y;

            local_x = souris_x - *cam_x;
            local_y = souris_y - *cam_y;

            if (local_x >= 0 && local_y >= 0)
            {
                case_x = local_x / taille_affichee;
                case_y = local_y / taille_affichee;

                if (case_x >= 0 && case_x < WIDTH &&
                    case_y >= 0 && case_y < HEIGHT)
                {
                    grille[case_y][case_x] = 1 - grille[case_y][case_x];
                }
            }
        }

        if (event->button.button == SDL_BUTTON_RIGHT)
        {
            clic_droit_appuye = 1;
            ancien_x = event->button.x;
            ancien_y = event->button.y;
        }
    }

    if (event->type == SDL_MOUSEBUTTONUP)
    {
        if (event->button.button == SDL_BUTTON_RIGHT)
        {
            clic_droit_appuye = 0;
        }
    }

    if (event->type == SDL_MOUSEMOTION)
    {
        if (clic_droit_appuye)
        {
            *cam_x = *cam_x + event->motion.x - ancien_x;
            *cam_y = *cam_y + event->motion.y - ancien_y;

            ancien_x = event->motion.x;
            ancien_y = event->motion.y;
        }
    }

    if (event->type == SDL_MOUSEWHEEL)
    {
        if (event->wheel.y > 0)
        {
            *zoom = *zoom + 0.1f;
        }

        if (event->wheel.y < 0 && *zoom > 0.2f)
        {
            *zoom = *zoom - 0.1f;
        }
    }
}
        

