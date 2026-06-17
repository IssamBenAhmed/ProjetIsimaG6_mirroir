#include "graphique.h"

#include <stdio.h>
#include <stdlib.h>

/* initialise sdl puis cree la fenetre principale */
SDL_Window *creer_fenetre(int largeur, int hauteur)
{
    SDL_Window *window = NULL;

    /* demarre le sous-systeme video de sdl */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Erreur SDL_Init : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /* cree une fenetre redimensionnable */
    window = SDL_CreateWindow(
        "Jeu de la Vie - SMA",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        largeur,
        hauteur,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    /* arrete en cas d'echec de creation */
    if (window == NULL)
    {
        SDL_Log("Erreur SDL_CreateWindow : %s", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    return window;
}

/* cree le renderer associe a la fenetre */
SDL_Renderer *creer_renderer(SDL_Window *window)
{
    SDL_Renderer *renderer = NULL;

    /* verifie que la fenetre existe avant de continuer */
    if (window == NULL)
    {
        SDL_Log("Erreur : la fenetre est NULL");
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    /* cree un renderer accelere par le materiel */
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    /* libere proprement la fenetre si la creation echoue */
    if (renderer == NULL)
    {
        SDL_Log("Erreur SDL_CreateRenderer : %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    return renderer;
}

/* dessine chaque cellule de la grille a l'ecran */
void dessiner_grille(SDL_Renderer *renderer,
                     int grille[WIDTH][HEIGHT],
                     float zoom,
                     int cam_x,
                     int cam_y)
{
    SDL_Rect cellule;
    int taille_affichee;
    int x;
    int y;

    /* calcule la taille visible d'une cellule */
    taille_affichee = (int)(CELL_SIZE * zoom);

    /* garantit une taille minimale */
    if (taille_affichee < 1)
    {
        taille_affichee = 1;
    }

    /* parcourt toute la grille logique */
    for (x = 0; x < WIDTH; x++)
    {
        for (y = 0; y < HEIGHT; y++)
        {
            /* positionne le carre correspondant a la cellule */
            cellule.x = x * taille_affichee + cam_x;
            cellule.y = y * taille_affichee + cam_y;
            cellule.w = taille_affichee;
            cellule.h = taille_affichee;

            /* cellule vivante en blanc, cellule morte en noir */
            if (grille[x][y] == 1)
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

/* gere les clics, le deplacement et le zoom */
void gerer_interaction(SDL_Event *event,
                       int grille[WIDTH][HEIGHT],
                       float *zoom,
                       int *cam_x,
                       int *cam_y)
{
    static int clic_gauche_appuye = 0;
    static int clic_gauche_en_drag = 0;
    static int ancien_x = 0;
    static int ancien_y = 0;

    int souris_x;
    int souris_y;
    int taille_affichee;
    int largeur_grille_affichee;
    int hauteur_grille_affichee;
    int cam_x_min;
    int cam_y_min;

    int local_x;
    int local_y;

    int case_x;
    int case_y;

    /* calcule la taille courante d'une cellule */
    taille_affichee = (int)(CELL_SIZE * (*zoom));

    /* evite une taille nulle a l'affichage */
    if (taille_affichee < 1)
    {
        taille_affichee = 1;
    }

    /* calcule les limites de la camera pour rester dans la grille */
    largeur_grille_affichee = WIDTH * taille_affichee;
    hauteur_grille_affichee = HEIGHT * taille_affichee;
    cam_x_min = WINDOW_WIDTH - largeur_grille_affichee;
    cam_y_min = WINDOW_HEIGHT - hauteur_grille_affichee;

    if (cam_x_min > 0)
    {
        cam_x_min = 0;
    }

    if (cam_y_min > 0)
    {
        cam_y_min = 0;
    }

    /* clics souris et appuis */
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        /* clic gauche: prepare soit un toggle soit un drag */
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            clic_gauche_appuye = 1;
            clic_gauche_en_drag = 0;
            souris_x = event->button.x;
            souris_y = event->button.y;

            ancien_x = souris_x;
            ancien_y = souris_y;

            /* calcule la cellule visee pour un toggle possible au relachement */
            local_x = souris_x - *cam_x;
            local_y = souris_y - *cam_y;

            if (local_x >= 0 && local_y >= 0)
            {
                case_x = local_x / taille_affichee;
                case_y = local_y / taille_affichee;
            }
        }
    }

    /* relachement du clic gauche */
    if (event->type == SDL_MOUSEBUTTONUP)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            if (!clic_gauche_en_drag)
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
                        grille[case_x][case_y] = 1 - grille[case_x][case_y];
                    }
                }
            }

            clic_gauche_appuye = 0;
            clic_gauche_en_drag = 0;
        }
    }

    /* deplacement de la souris avec clic gauche maintenu */
    if (event->type == SDL_MOUSEMOTION)
    {
        if (clic_gauche_appuye)
        {
            /* active le drag si la souris a bouge d'au moins un pixel */
            if (event->motion.xrel != 0 || event->motion.yrel != 0)
            {
                clic_gauche_en_drag = 1;
            }

            /* translate la vue selon le mouvement de souris */
            *cam_x = *cam_x + event->motion.x - ancien_x;
            *cam_y = *cam_y + event->motion.y - ancien_y;

            /* bloque la camera aux bords de la grille */
            if (*cam_x > 0)
            {
                *cam_x = 0;
            }
            else if (*cam_x < cam_x_min)
            {
                *cam_x = cam_x_min;
            }

            if (*cam_y > 0)
            {
                *cam_y = 0;
            }
            else if (*cam_y < cam_y_min)
            {
                *cam_y = cam_y_min;
            }

            ancien_x = event->motion.x;
            ancien_y = event->motion.y;
        }
    }

    /* molette: ajuste le zoom */
    if (event->type == SDL_MOUSEWHEEL)
    {
        if (event->wheel.y > 0)
        {
            *zoom = *zoom + 0.1f;
        }

        if (event->wheel.y < 0 && *zoom > MIN_ZOOM)
        {
            *zoom = *zoom - 0.1f;
        }

        /* empeche tout zoom en dessous de la taille de base de la grille */
        if (*zoom < MIN_ZOOM)
        {
            *zoom = MIN_ZOOM;
        }
    }
}

