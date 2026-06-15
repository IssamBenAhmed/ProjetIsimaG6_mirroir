#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FENETRES 60

#define LARGEUR_ECRAN 1200
#define HAUTEUR_ECRAN 700

#define ORIGINE_X_DEBUT 250
#define ORIGINE_Y_DEBUT 180

#define DEPLACEMENT 20

#define ECHELLE_MIN 50
#define ECHELLE_MAX 180
#define PAS_ECHELLE 10

#define SANS_BORDURE 1

int main(void)
{
    SDL_Window *fenetres[MAX_FENETRES] = {NULL};
    SDL_Surface *surface = NULL;

    SDL_Event event;
    SDL_bool continuer = SDL_TRUE;

    int base_x[MAX_FENETRES];
    int base_y[MAX_FENETRES];
    int base_w[MAX_FENETRES];
    int base_h[MAX_FENETRES];

    Uint8 rouge[MAX_FENETRES];
    Uint8 vert[MAX_FENETRES];
    Uint8 bleu[MAX_FENETRES];

    int origine_x = ORIGINE_X_DEBUT;
    int origine_y = ORIGINE_Y_DEBUT;
    int echelle = 100;

    int n = 0;
    int i;
    int x;
    int y;
    int w;
    int h;

    int roue_x[8] = {-35, 0, 35, 35, 35, 0, -35, -35};
    int roue_y[8] = {-35, -55, -35, 0, 35, 55, 35, 0};

    int centre_roue_arriere_x = 120;
    int centre_roue_arriere_y = 300;

    int centre_roue_avant_x = 520;
    int centre_roue_avant_y = 300;

    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

    if (SANS_BORDURE)
    {
        flags = flags | SDL_WINDOW_BORDERLESS;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur : SDL ne demarre pas : %s\n", SDL_GetError());
        return 1;
    }

    /*
        1. Construction logique de la trottinette
        Chaque morceau sera une fenêtre.
    */

    /* Roue arrière */
    for (i = 0; i < 8; i++)
    {
        base_x[n] = centre_roue_arriere_x + roue_x[i];
        base_y[n] = centre_roue_arriere_y + roue_y[i];
        base_w[n] = 45;
        base_h[n] = 30;

        rouge[n] = 20;
        vert[n] = 20;
        bleu[n] = 20;

        n++;
    }

    /* Roue avant */
    for (i = 0; i < 8; i++)
    {
        base_x[n] = centre_roue_avant_x + roue_x[i];
        base_y[n] = centre_roue_avant_y + roue_y[i];
        base_w[n] = 45;
        base_h[n] = 30;

        rouge[n] = 20;
        vert[n] = 20;
        bleu[n] = 20;

        n++;
    }

    /* Plateau de la trottinette */
    for (i = 0; i < 8; i++)
    {
        base_x[n] = 150 + i * 45;
        base_y[n] = 230;
        base_w[n] = 55;
        base_h[n] = 25;

        rouge[n] = 180;
        vert[n] = 180;
        bleu[n] = 180;

        n++;
    }

    /* Barre verticale du guidon */
    for (i = 0; i < 5; i++)
    {
        base_x[n] = 520;
        base_y[n] = 190 - i * 38;
        base_w[n] = 28;
        base_h[n] = 42;

        rouge[n] = 120;
        vert[n] = 120;
        bleu[n] = 120;

        n++;
    }

    /* Guidon horizontal */
    for (i = 0; i < 4; i++)
    {
        base_x[n] = 455 + i * 45;
        base_y[n] = 15;
        base_w[n] = 55;
        base_h[n] = 25;

        rouge[n] = 80;
        vert[n] = 80;
        bleu[n] = 80;

        n++;
    }

    /* Fourche avant */
    for (i = 0; i < 3; i++)
    {
        base_x[n] = 485 + i * 12;
        base_y[n] = 225 + i * 20;
        base_w[n] = 28;
        base_h[n] = 45;

        rouge[n] = 120;
        vert[n] = 120;
        bleu[n] = 120;

        n++;
    }

    /* Support arrière vers le plateau */
    for (i = 0; i < 2; i++)
    {
        base_x[n] = 120 + i * 35;
        base_y[n] = 250 - i * 15;
        base_w[n] = 45;
        base_h[n] = 25;

        rouge[n] = 120;
        vert[n] = 120;
        bleu[n] = 120;

        n++;
    }

    printf("Nombre de fenetres utilisees : %d\n", n);

    /*
        2. Création des fenêtres
    */
    for (i = 0; i < n; i++)
    {
        char titre[100];

        sprintf(titre, "Trottinette morceau %d", i + 1);

        fenetres[i] = SDL_CreateWindow(
            titre,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            base_w[i],
            base_h[i],
            flags
        );

        if (fenetres[i] == NULL)
        {
            printf("Erreur creation fenetre %d : %s\n", i + 1, SDL_GetError());

            for (int j = 0; j < i; j++)
            {
                SDL_DestroyWindow(fenetres[j]);
            }

            SDL_Quit();
            return 1;
        }
    }

    /*
        3. Placement initial + couleur de chaque fenêtre
    */
    for (i = 0; i < n; i++)
    {
        x = origine_x + base_x[i] * echelle / 100;
        y = origine_y + base_y[i] * echelle / 100;
        w = base_w[i] * echelle / 100;
        h = base_h[i] * echelle / 100;

        if (w < 20)
        {
            w = 20;
        }

        if (h < 20)
        {
            h = 20;
        }

        SDL_SetWindowPosition(fenetres[i], x, y);
        SDL_SetWindowSize(fenetres[i], w, h);

        surface = SDL_GetWindowSurface(fenetres[i]);

        if (surface != NULL)
        {
            SDL_FillRect(
                surface,
                NULL,
                SDL_MapRGB(surface->format, rouge[i], vert[i], bleu[i])
            );

            SDL_UpdateWindowSurface(fenetres[i]);
        }
    }

    printf("\nCommandes :\n");
    printf("  Fleches : deplacer la trottinette\n");
    printf("  a       : agrandir\n");
    printf("  r       : reduire\n");
    printf("  espace  : remettre au depart\n");
    printf("  q/Echap : quitter\n\n");

    /*
        4. Boucle événementielle
    */
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
                    origine_y = origine_y - DEPLACEMENT;
                }

                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    origine_y = origine_y + DEPLACEMENT;
                }

                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    origine_x = origine_x - DEPLACEMENT;
                }

                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    origine_x = origine_x + DEPLACEMENT;
                }

                if (event.key.keysym.sym == SDLK_a)
                {
                    echelle = echelle + PAS_ECHELLE;

                    if (echelle > ECHELLE_MAX)
                    {
                        echelle = ECHELLE_MAX;
                    }
                }

                if (event.key.keysym.sym == SDLK_r)
                {
                    echelle = echelle - PAS_ECHELLE;

                    if (echelle < ECHELLE_MIN)
                    {
                        echelle = ECHELLE_MIN;
                    }
                }

                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    origine_x = ORIGINE_X_DEBUT;
                    origine_y = ORIGINE_Y_DEBUT;
                    echelle = 100;
                }

                /*
                    Après chaque touche, on replace et redimensionne toutes les fenêtres.
                */
                for (i = 0; i < n; i++)
                {
                    x = origine_x + base_x[i] * echelle / 100;
                    y = origine_y + base_y[i] * echelle / 100;
                    w = base_w[i] * echelle / 100;
                    h = base_h[i] * echelle / 100;

                    if (w < 20)
                    {
                        w = 20;
                    }

                    if (h < 20)
                    {
                        h = 20;
                    }

                    SDL_SetWindowPosition(fenetres[i], x, y);
                    SDL_SetWindowSize(fenetres[i], w, h);

                    surface = SDL_GetWindowSurface(fenetres[i]);

                    if (surface != NULL)
                    {
                        SDL_FillRect(
                            surface,
                            NULL,
                            SDL_MapRGB(surface->format, rouge[i], vert[i], bleu[i])
                        );

                        SDL_UpdateWindowSurface(fenetres[i]);
                    }
                }
            }
        }

        SDL_Delay(10);
    }

    /*
        5. Fermeture propre
    */
    for (i = 0; i < n; i++)
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