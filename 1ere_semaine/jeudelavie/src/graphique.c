
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

    int souris_x;//la position x de la souris au moment du clic
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
    
    /* on teste si l'evenement correspond a un appui sur un bouton de la souris */
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        /* clic gauche: prepare soit un toggle soit un drag */

        /* on verifie que le bouton appuye est le clic gauche */
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            /* indique que le clic gauche est actuellement maintenu */
            clic_gauche_appuye = 1;

            /* au debut du clic, on suppose que ce n'est pas encore un deplacement */
            clic_gauche_en_drag = 0;

            /* on recupere la position x de la souris au moment du clic */
            souris_x = event->button.x;

            /* on recupere la position y de la souris au moment du clic */
            souris_y = event->button.y;

            /* on memorise la position x de depart pour calculer ensuite le deplacement */
            ancien_x = souris_x;

            /* on memorise la position y de depart pour calculer ensuite le deplacement */
            ancien_y = souris_y;

            /* calcule la cellule visee pour un toggle possible au relachement */

            /* on convertit la position souris en position locale dans la grille selon la camera x */
            local_x = souris_x - *cam_x;

            /* on convertit la position souris en position locale dans la grille selon la camera y */
            local_y = souris_y - *cam_y;

            /* on verifie que la souris est bien dans une zone positive de la grille */
            if (local_x >= 0 && local_y >= 0)
            {
                /* on transforme la position locale x en numero de case de la grille */
                case_x = local_x / taille_affichee;

                /* on transforme la position locale y en numero de case de la grille */
                case_y = local_y / taille_affichee;
            }
        }
    }

    /* relachement du clic gauche */

    /* on teste si l'evenement correspond au relachement d'un bouton de souris */
    if (event->type == SDL_MOUSEBUTTONUP)
    {
        /* on verifie que le bouton relache est le clic gauche */
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            /* si la souris n'a pas ete deplacee, alors c'est un vrai clic simple */
            if (!clic_gauche_en_drag)
            {
                /* on recupere la position x de la souris au moment du relachement */
                souris_x = event->button.x;

                /* on recupere la position y de la souris au moment du relachement */
                souris_y = event->button.y;

                /* on enleve le decalage de la camera pour obtenir la position dans la grille */
                local_x = souris_x - *cam_x;

                /* on enleve le decalage de la camera pour obtenir la position dans la grille */
                local_y = souris_y - *cam_y;

                /* on verifie que la position locale n'est pas negative */
                if (local_x >= 0 && local_y >= 0)
                {
                    /* on calcule la colonne de la cellule cliquee */
                    case_x = local_x / taille_affichee;

                    /* on calcule la ligne de la cellule cliquee */
                    case_y = local_y / taille_affichee;

                    /* on verifie que la cellule cliquee est bien dans les limites de la grille */
                    if (case_x >= 0 && case_x < WIDTH &&
                        case_y >= 0 && case_y < HEIGHT)
                    {
                        /* on inverse l'etat de la cellule : 0 devient 1, et 1 devient 0 */
                        grille[case_x][case_y] = 1 - grille[case_x][case_y];
                    }
                }
            }

            /* le clic gauche n'est plus maintenu */
            clic_gauche_appuye = 0;

            /* on remet le drag a zero pour le prochain clic */
            clic_gauche_en_drag = 0;
        }
    }

    /* deplacement de la souris avec clic gauche maintenu */

    /* on teste si l'evenement correspond a un mouvement de la souris */
    if (event->type == SDL_MOUSEMOTION)
    {
        /* on deplace la camera seulement si le clic gauche est maintenu */
        if (clic_gauche_appuye)
        {
            /* active le drag si la souris a bouge d'au moins un pixel */

            /* xrel et yrel indiquent le deplacement relatif de la souris depuis le dernier mouvement */
            if (event->motion.xrel != 0 || event->motion.yrel != 0)
            {
                /* la souris a bouge, donc ce n'est plus un clic simple mais un deplacement */
                clic_gauche_en_drag = 1;
            }

            /* translate la vue selon le mouvement de souris */

            /* on modifie le decalage horizontal de la camera selon le mouvement de la souris */
            *cam_x = *cam_x + event->motion.x - ancien_x;

            /* on modifie le decalage vertical de la camera selon le mouvement de la souris */
            *cam_y = *cam_y + event->motion.y - ancien_y;

            /* bloque la camera aux bords de la grille */

            /* si la camera va trop a droite, on la remet a zero */
            if (*cam_x > 0)
            {
                /* la camera ne doit pas depasser le bord gauche de la grille */
                *cam_x = 0;
            }

            /* sinon, si la camera va trop a gauche, on la bloque a la limite minimale */
            else if (*cam_x < cam_x_min)
            {
                /* cam_x_min represente la limite maximale vers la gauche */
                *cam_x = cam_x_min;
            }

            /* si la camera va trop en bas, on la remet a zero */
            if (*cam_y > 0)
            {
                /* la camera ne doit pas depasser le bord haut de la grille */
                *cam_y = 0;
            }

            /* sinon, si la camera va trop en haut, on la bloque a la limite minimale */
            else if (*cam_y < cam_y_min)
            {
                /* cam_y_min represente la limite maximale vers le haut */
                *cam_y = cam_y_min;
            }

            /* on memorise la nouvelle position x de la souris pour le prochain mouvement */
            ancien_x = event->motion.x;

            /* on memorise la nouvelle position y de la souris pour le prochain mouvement */
            ancien_y = event->motion.y;
        }
    }

    /* molette: ajuste le zoom */

    /* on teste si l'evenement correspond a l'utilisation de la molette */
    if (event->type == SDL_MOUSEWHEEL)
    {
        /* si la molette monte, on augmente le zoom */
        if (event->wheel.y > 0)
        {
            /* on agrandit la taille d'affichage de la grille */
            *zoom = *zoom + 0.1f;
        }

        /* si la molette descend et que le zoom est encore superieur au minimum */
        if (event->wheel.y < 0 && *zoom > MIN_ZOOM)
        {
            /* on reduit la taille d'affichage de la grille */
            *zoom = *zoom - 0.1f;
        }

        /* empeche tout zoom en dessous de la taille de base de la grille */

        /* securite : si le zoom devient trop petit, on le remet au minimum autorise */
        if (*zoom < MIN_ZOOM)
        {
            /* MIN_ZOOM est la valeur minimale du zoom */
            *zoom = MIN_ZOOM;
        }
    }
}




