#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define NB_PAR_DIAGONALE 15
#define NB_FENETRES (2 * NB_PAR_DIAGONALE - 1)

#define LARGEUR_INIT 110
#define HAUTEUR_INIT 70

#define LARGEUR_MIN 70
#define HAUTEUR_MIN 50

#define MARGE 40
#define DEPLACEMENT 20

#define AGRANDIR_LARGEUR 15
#define AGRANDIR_HAUTEUR 10

int main(void)
{
    SDL_Window *fenetres[NB_FENETRES] = {NULL};

    SDL_bool continuer = SDL_TRUE;
    SDL_Event event;
    SDL_DisplayMode screen;

    int i;
    int k;
    int x;
    int y;
    int largeur;
    int hauteur;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur : SDL ne demarre pas : %s\n", SDL_GetError());
        return 1;
    }

    if (SDL_GetCurrentDisplayMode(0, &screen) != 0)
    {
        printf("Erreur : impossible de recuperer la taille de l'ecran\n");
        SDL_Quit();
        return 1;
    }

    printf("Taille de l'ecran : %d x %d\n", screen.w, screen.h);
    printf("Nombre de fenetres : %d\n", NB_FENETRES);

    /* Creation des fenetres */
    for (i = 0; i < NB_FENETRES; i++)
    {
        char titre[100];

        sprintf(titre, "X fenetre numero %d", i + 1);

        fenetres[i] = SDL_CreateWindow(
            titre,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            LARGEUR_INIT,
            HAUTEUR_INIT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );

        if (fenetres[i] == NULL)
        {
            printf("Erreur creation fenetre %d : %s\n", i + 1, SDL_GetError());

            for (k = 0; k < i; k++)
            {
                SDL_DestroyWindow(fenetres[k]);
            }

            SDL_Quit();
            return 1;
        }
    }

    /* Placement initial en forme de X */
    k = 0;

    /* Première diagonale : haut gauche vers bas droite */
    for (i = 0; i < NB_PAR_DIAGONALE; i++)
    {
        double t = (double)i / (NB_PAR_DIAGONALE - 1);

        x = MARGE + t * (screen.w - 2 * MARGE - LARGEUR_INIT);
        y = MARGE + t * (screen.h - 2 * MARGE - HAUTEUR_INIT);

        SDL_SetWindowPosition(fenetres[k], x, y);

        k++;
    }

    /* Deuxième diagonale : haut droite vers bas gauche */
    for (i = 0; i < NB_PAR_DIAGONALE; i++)
    {
        double t;

        if (i == NB_PAR_DIAGONALE / 2)
        {
            continue;
        }

        t = (double)i / (NB_PAR_DIAGONALE - 1);

        x = screen.w - MARGE - LARGEUR_INIT - t * (screen.w - 2 * MARGE - LARGEUR_INIT);
        y = MARGE + t * (screen.h - 2 * MARGE - HAUTEUR_INIT);

        SDL_SetWindowPosition(fenetres[k], x, y);

        k++;
    }

    printf("\nCommandes :\n");
    printf("  Fleches : deplacer toutes les fenetres\n");
    printf("  a       : agrandir toutes les fenetres\n");
    printf("  r       : reduire toutes les fenetres\n");
    printf("  espace  : replacer les fenetres en X\n");
    printf("  q/Echap : quitter\n\n");

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
                if (event.key.keysym.sym == SDLK_q ||
                    event.key.keysym.sym == SDLK_ESCAPE)
                {
                    continuer = SDL_FALSE;
                }

                if (event.key.keysym.sym == SDLK_UP)
                {
                    for (i = 0; i < NB_FENETRES; i++)
                    {
                        SDL_GetWindowPosition(fenetres[i], &x, &y);
                        SDL_SetWindowPosition(fenetres[i], x, y - DEPLACEMENT);
                    }
                }

                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    for (i = 0; i < NB_FENETRES; i++)
                    {
                        SDL_GetWindowPosition(fenetres[i], &x, &y);
                        SDL_SetWindowPosition(fenetres[i], x, y + DEPLACEMENT);
                    }
                }

                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    for (i = 0; i < NB_FENETRES; i++)
                    {
                        SDL_GetWindowPosition(fenetres[i], &x, &y);
                        SDL_SetWindowPosition(fenetres[i], x - DEPLACEMENT, y);
                    }
                }

                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    for (i = 0; i < NB_FENETRES; i++)
                    {
                        SDL_GetWindowPosition(fenetres[i], &x, &y);
                        SDL_SetWindowPosition(fenetres[i], x + DEPLACEMENT, y);
                    }
                }

                if (event.key.keysym.sym == SDLK_a)
                {
                    for (i = 0; i < NB_FENETRES; i++)
                    {
                        SDL_GetWindowSize(fenetres[i], &largeur, &hauteur);

                        largeur = largeur + AGRANDIR_LARGEUR;
                        hauteur = hauteur + AGRANDIR_HAUTEUR;

                        SDL_SetWindowSize(fenetres[i], largeur, hauteur);
                    }
                }

                if (event.key.keysym.sym == SDLK_r)
                {
                    for (i = 0; i < NB_FENETRES; i++)
                    {
                        SDL_GetWindowSize(fenetres[i], &largeur, &hauteur);

                        largeur = largeur - AGRANDIR_LARGEUR;
                        hauteur = hauteur - AGRANDIR_HAUTEUR;

                        if (largeur < LARGEUR_MIN)
                        {
                            largeur = LARGEUR_MIN;
                        }

                        if (hauteur < HAUTEUR_MIN)
                        {
                            hauteur = HAUTEUR_MIN;
                        }

                        SDL_SetWindowSize(fenetres[i], largeur, hauteur);
                    }
                }

                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    SDL_GetWindowSize(fenetres[0], &largeur, &hauteur);

                    k = 0;

                    for (i = 0; i < NB_PAR_DIAGONALE; i++)
                    {
                        double t = (double)i / (NB_PAR_DIAGONALE - 1);

                        x = MARGE + t * (screen.w - 2 * MARGE - largeur);
                        y = MARGE + t * (screen.h - 2 * MARGE - hauteur);

                        SDL_SetWindowPosition(fenetres[k], x, y);

                        k++;
                    }

                    for (i = 0; i < NB_PAR_DIAGONALE; i++)
                    {
                        double t;

                        if (i == NB_PAR_DIAGONALE / 2)
                        {
                            continue;
                        }

                        t = (double)i / (NB_PAR_DIAGONALE - 1);

                        x = screen.w - MARGE - largeur - t * (screen.w - 2 * MARGE - largeur);
                        y = MARGE + t * (screen.h - 2 * MARGE - hauteur);

                        SDL_SetWindowPosition(fenetres[k], x, y);

                        k++;
                    }
                }
            }
        }

        SDL_Delay(10);
    }

    for (i = 0; i < NB_FENETRES; i++)
    {
        if (fenetres[i] != NULL)
        {
            SDL_DestroyWindow(fenetres[i]);
            fenetres[i] = NULL;
        }
    }

    SDL_Quit();

    return 0;
}