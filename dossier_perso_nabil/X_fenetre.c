//X_fenetre fait par LAAMARCHI NABIL 

/*

    Idee du programme :
    - On cree plusieurs fenetres SDL.
    - On les place pour former la lettre X.
    - Les fleches deplacent toutes les fenetres.
    - La touche a agrandit toutes les fenetres.
    - La touche r reduit toutes les fenetres.
    - La touche espace replace les fenetres en X.
    - q ou Echap ferme le programme.
*/

#include <SDL2/SDL.h> 
#include <stdio.h>
#include <stdlib.h>


#define NB_PAR_DIAGONALE 15 //Nombre de fenetres sur une diagonale.

#define NB_FENETRES 29 //Nombre total de fenetres.


#define LARGEUR_INIT 110 //Largeur initial de chaque fenetre.
#define HAUTEUR_INIT 70 //Hauteur initial de chaque fenetre.


#define LARGEUR_MIN 70 //Largeur minimal, Quand on reduit avec r, on ne descend pas en dessous de cette valeurs.
#define HAUTEUR_MIN 50 //Hauteur minimal, Quand on reduit avec r, on ne descend pas en dessous de cette valeurs.


#define MARGE 40 // Marge entre le X et les bords de l'ecran.Cela evite de coller les fenetres au bord de l'ecran.

#define DEPLACEMENT 20 // Nombre de pixels utilises pour deplacer les fenetres avec les fleches.

/*
    Quantite ajoutee ou enlevee quand on agrandit / reduit.
*/
#define AGRANDIR_LARGEUR 15 //Largeur ajouté/enlevée quand on agrandit/réduit
#define AGRANDIR_HAUTEUR 10 //Hauteur ajouté/enlevée quand on agrandit/réduit

int main(void)
{
    
    SDL_Window *fenetres[NB_FENETRES] = {NULL}; //tableau de 29 fenetres qui ne sont pas encore crées.

    SDL_bool continuer = SDL_TRUE; // pour continuer le programme ou l'arreter.

    SDL_Event event;// pour recevoir les evenements.

    SDL_DisplayMode screen; //screen contient la taille de l'écran(largeur et hauteur).

    int i;          /* compteur de boucle */
    int k;          /* numero de la fenetre qu'on place */
    int x;          /* position horizontale d'une fenetre */
    int y;          /* position verticale d'une fenetre */
    int largeur;    /* largeur actuelle d'une fenetre */
    int hauteur;    /* hauteur actuelle d'une fenetre */

    
    int pas_x; //La distance horizontale entre deux fenetres.
    int pas_y; //La distance verticale entre deux fenetres.

    
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
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE //fenetres visibles et redimensionnées.
        ); 

        /*
            Si une fenetre n'a pas ete creee,
            on detruit toutes les fenetres deja creees
            puis on quitte proprement.
        */
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

    
    pas_x = (screen.w - 2 * MARGE - LARGEUR_INIT) / (NB_PAR_DIAGONALE - 1);
    pas_y = (screen.h - 2 * MARGE - HAUTEUR_INIT) / (NB_PAR_DIAGONALE - 1);

    k = 0;
    for (i = 0; i < NB_PAR_DIAGONALE; i++) // premiére diagonale.
    {
        x = MARGE + i * pas_x;
        y = MARGE + i * pas_y;

        SDL_SetWindowPosition(fenetres[k], x, y); //Placer la fenetre k à la position calculée.

        k++;//Passer à la fenetre suivante.
    }

    for (i = 0; i < NB_PAR_DIAGONALE; i++)// deuxiéme diagonale.
    {
        if (i == NB_PAR_DIAGONALE / 2) //on saut le milieu sinon on aurait deux fenetres au centre.
        {
            continue;
        }

        x = screen.w - MARGE - LARGEUR_INIT - i * pas_x;
        y = MARGE + i * pas_y;

        SDL_SetWindowPosition(fenetres[k], x, y);//Placer la fenetre k à la position calculée.

        k++;//Passer à la fenetre suivante.
    }

    while (continuer)
    {
        /*
            Lire tous les evenements SDL.
        */
        while (SDL_PollEvent(&event))
        {
            /*
                Si l'utilisateur clique sur la croix d'une fenetre,
                on quitte le programme.
            */
            if (event.type == SDL_QUIT)
            {
                continuer = SDL_FALSE;
            }

            /*
                Si une touche du clavier est appuyee.
            */
            if (event.type == SDL_KEYDOWN)
            {
                /*
                    q ou Echap :
                    quitter le programme.
                */
                if (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_ESCAPE)
                {
                    continuer = SDL_FALSE;
                }

                /*
                    Fleche haut :
                    deplacer toutes les fenetres vers le haut.

                    Pour monter, y diminue.
                */
                if (event.key.keysym.sym == SDLK_UP)
                {
                    for (i = 0; i < NB_FENETRES; i++)
                    {
                        SDL_GetWindowPosition(fenetres[i], &x, &y);
                        SDL_SetWindowPosition(fenetres[i], x, y - DEPLACEMENT);
                    }
                }

                /*
                    Fleche bas :
                    deplacer toutes les fenetres vers le bas.

                    Pour descendre, y augmente.
                */
                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    for (i = 0; i < NB_FENETRES; i++)
                    {
                        SDL_GetWindowPosition(fenetres[i], &x, &y);
                        SDL_SetWindowPosition(fenetres[i], x, y + DEPLACEMENT);
                    }
                }

                /*
                    Fleche gauche :
                    deplacer toutes les fenetres vers la gauche.

                    Pour aller a gauche, x diminue.
                */
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    for (i = 0; i < NB_FENETRES; i++)
                    {
                        SDL_GetWindowPosition(fenetres[i], &x, &y);
                        SDL_SetWindowPosition(fenetres[i], x - DEPLACEMENT, y);
                    }
                }

                /*
                    Fleche droite :
                    deplacer toutes les fenetres vers la droite.

                    Pour aller a droite, x augmente.
                */
                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    for (i = 0; i < NB_FENETRES; i++)
                    {
                        SDL_GetWindowPosition(fenetres[i], &x, &y);
                        SDL_SetWindowPosition(fenetres[i], x + DEPLACEMENT, y);
                    }
                }

                /*
                    Touche a :
                    agrandir toutes les fenetres.
                */
                if (event.key.keysym.sym == SDLK_a)
                {
                    for (i = 0; i < NB_FENETRES; i++)
                    {
                        /*
                            Recuperer la taille actuelle de la fenetre.
                        */
                        SDL_GetWindowSize(fenetres[i], &largeur, &hauteur);

                        /*
                            Augmenter la taille.
                        */
                        largeur = largeur + AGRANDIR_LARGEUR;
                        hauteur = hauteur + AGRANDIR_HAUTEUR;

                        /*
                            Appliquer la nouvelle taille.
                        */
                        SDL_SetWindowSize(fenetres[i], largeur, hauteur);
                    }
                }

                /*
                    Touche r :
                    reduire toutes les fenetres.
                */
                if (event.key.keysym.sym == SDLK_r)
                {
                    for (i = 0; i < NB_FENETRES; i++)
                    {
                        SDL_GetWindowSize(fenetres[i], &largeur, &hauteur);

                        largeur = largeur - AGRANDIR_LARGEUR;
                        hauteur = hauteur - AGRANDIR_HAUTEUR;

                        /*
                            limite de largeur.
                        */
                        if (largeur < LARGEUR_MIN)
                        {
                            largeur = LARGEUR_MIN;
                        }

                        /*
                            limite de hauteur.
                        */
                        if (hauteur < HAUTEUR_MIN)
                        {
                            hauteur = HAUTEUR_MIN;
                        }

                        SDL_SetWindowSize(fenetres[i], largeur, hauteur);
                    }
                }

                /*
                    Touche espace :
                    replacer les fenetres en forme de X.

                    Important :
                    on utilise la taille actuelle des fenetres.
                    Comme ca, meme si on a agrandi ou reduit,
                    le X est replace correctement.
                */
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    /*
                        On prend la taille actuelle de la premiere fenetre.
                    */
                    SDL_GetWindowSize(fenetres[0], &largeur, &hauteur);

                    /*
                        Recalculer les pas avec la taille actuelle.
                    */
                    pas_x = (screen.w - 2 * MARGE - largeur) / (NB_PAR_DIAGONALE - 1);
                    pas_y = (screen.h - 2 * MARGE - hauteur) / (NB_PAR_DIAGONALE - 1);

                    k = 0;

                    /*
                        Replacer la premiere diagonale :
                        haut gauche vers bas droite.
                    */
                    for (i = 0; i < NB_PAR_DIAGONALE; i++)
                    {
                        x = MARGE + i * pas_x;
                        y = MARGE + i * pas_y;

                        SDL_SetWindowPosition(fenetres[k], x, y);

                        k++;
                    }

                    /*
                        Replacer la deuxieme diagonale :
                        haut droite vers bas gauche.
                    */
                    for (i = 0; i < NB_PAR_DIAGONALE; i++)
                    {
                        /*
                            On saute encore le milieu.
                        */
                        if (i == NB_PAR_DIAGONALE / 2)
                        {
                            continue;
                        }

                        x = screen.w - MARGE - largeur - i * pas_x;
                        y = MARGE + i * pas_y;

                        SDL_SetWindowPosition(fenetres[k], x, y);

                        k++;
                    }
                }
            }
        }

        /*
            Petite pause.

            Sans cette pause, la boucle tourne trop vite
            et le processeur travaille beaucoup pour rien.
        */
        SDL_Delay(10);
    }

    /*
        On detruit toutes les fenetres une par une.
    */
    for (i = 0; i < NB_FENETRES; i++)
    {
        if (fenetres[i] != NULL)
        {
            SDL_DestroyWindow(fenetres[i]);
            fenetres[i] = NULL;
        }
    }

    /*
        On ferme SDL.
    */
    SDL_Quit();
    return 0;
}

   
                   