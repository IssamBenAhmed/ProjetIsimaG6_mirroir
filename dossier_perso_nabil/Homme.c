#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 600
#define VITESSE 5

void dessiner_cercle_plein(SDL_Renderer *renderer, int cx, int cy, int rayon)
{
    int x;
    int y;

    for (x = -rayon; x <= rayon; x++)
    {
        for (y = -rayon; y <= rayon; y++)
        {
            if (x * x + y * y <= rayon * rayon)
            {
                SDL_RenderDrawPoint(renderer, cx + x, cy + y);
            }
        }
    }
}

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
    SDL_bool continuer = SDL_TRUE;

    SDL_Rect corps;

    int homme_x = 400;
    int homme_y = 300;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur : SDL ne demarre pas : %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Un homme deplace au clavier",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGEUR_FENETRE,
        HAUTEUR_FENETRE,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL)
    {
        printf("Erreur creation fenetre : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (renderer == NULL)
    {
        printf("Erreur creation renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    printf("Commandes :\n");
    printf("Fleches du clavier : deplacer l'homme\n");
    printf("Echap ou q : quitter\n");

    while (continuer)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                continuer = SDL_FALSE;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE ||
                    event.key.keysym.sym == SDLK_q)
                {
                    continuer = SDL_FALSE;
                }
            }
        }

        const Uint8 *touches = SDL_GetKeyboardState(NULL);

        if (touches[SDL_SCANCODE_UP])
        {
            homme_y = homme_y - VITESSE;
        }

        if (touches[SDL_SCANCODE_DOWN])
        {
            homme_y = homme_y + VITESSE;
        }

        if (touches[SDL_SCANCODE_LEFT])
        {
            homme_x = homme_x - VITESSE;
        }

        if (touches[SDL_SCANCODE_RIGHT])
        {
            homme_x = homme_x + VITESSE;
        }

        /* Empêcher l'homme de sortir de l'écran */
        if (homme_x < 50)
        {
            homme_x = 50;
        }

        if (homme_x > LARGEUR_FENETRE - 50)
        {
            homme_x = LARGEUR_FENETRE - 50;
        }

        if (homme_y < 120)
        {
            homme_y = 120;
        }

        if (homme_y > HAUTEUR_FENETRE - 80)
        {
            homme_y = HAUTEUR_FENETRE - 80;
        }

        /* Fond noir */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        /* Sol */
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawLine(renderer, 0, 430, LARGEUR_FENETRE, 430);

        /* Tête */
        SDL_SetRenderDrawColor(renderer, 230, 190, 140, 255);
        dessiner_cercle_plein(renderer, homme_x, homme_y - 100, 25);

        /* Corps */
        corps.x = homme_x - 20;
        corps.y = homme_y - 75;
        corps.w = 40;
        corps.h = 70;

        SDL_SetRenderDrawColor(renderer, 0, 120, 255, 255);
        SDL_RenderFillRect(renderer, &corps);

        /* Bras fixes */
        SDL_SetRenderDrawColor(renderer, 230, 190, 140, 255);

        SDL_RenderDrawLine(renderer,
                           homme_x - 20, homme_y - 60,
                           homme_x - 55, homme_y - 25);

        SDL_RenderDrawLine(renderer,
                           homme_x + 20, homme_y - 60,
                           homme_x + 55, homme_y - 25);

        /* Jambes fixes */
        SDL_SetRenderDrawColor(renderer, 40, 40, 200, 255);

        SDL_RenderDrawLine(renderer,
                           homme_x - 10, homme_y - 5,
                           homme_x - 35, homme_y + 70);

        SDL_RenderDrawLine(renderer,
                           homme_x + 10, homme_y - 5,
                           homme_x + 35, homme_y + 70);

        /* Pieds fixes */
        SDL_RenderDrawLine(renderer,
                           homme_x - 35, homme_y + 70,
                           homme_x - 60, homme_y + 70);

        SDL_RenderDrawLine(renderer,
                           homme_x + 35, homme_y + 70,
                           homme_x + 60, homme_y + 70);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}