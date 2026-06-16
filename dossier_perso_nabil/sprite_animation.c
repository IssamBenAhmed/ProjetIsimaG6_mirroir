// sprite animation fait par LAAMARCHI NABIL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Programme : Chien avec fond animé et obstacle plein

    Idée du programme :
    - On crée une fenêtre SDL.
    - On charge un fond avec fond.png.
    - Le fond se déplace lentement sur l'axe x.
    - On charge un chien avec fonddeux.png.
    - Le chien se déplace avec les flèches du clavier.
    - On dessine un obstacle plein vert.
    - Si le chien touche l'obstacle :
        l'obstacle devient rouge,
        le score augmente de 1,
        le chien ne traverse pas l'obstacle.
    - q ou Échap permet de quitter le programme.
*/

#define LARGEUR_FENETRE 1000
#define HAUTEUR_FENETRE 550

#define VITESSE_CHIEN 6
#define VITESSE_FOND 2

int main(void)
{
    SDL_Window *window = NULL;          // Fenêtre principale.
    SDL_Renderer *renderer = NULL;      // Outil pour dessiner dans la fenêtre.

    SDL_Texture *fond = NULL;           // Texture du fond.
    SDL_Texture *chien = NULL;          // Texture du chien.
    SDL_Texture *texte_score = NULL;    // Texture du texte du score.

    TTF_Font *font = NULL;              // Police utilisée pour écrire le score.

    SDL_Event event;                    // Variable qui reçoit les événements.
    SDL_bool continuer = SDL_TRUE;      // Variable qui garde le programme ouvert.

    SDL_Rect fond1;                     // Premier rectangle du fond.
    SDL_Rect fond2;                     // Deuxième rectangle du fond.
    SDL_Rect chien_rect;                // Rectangle du chien.
    SDL_Rect obstacle;                  // Rectangle de l'obstacle plein.
    SDL_Rect texte_rect;                // Rectangle du texte.

    SDL_Color couleur_texte = {255, 255, 255, 255}; // Couleur blanche pour le score.

    int position_fond_x = 0;            // Position horizontale du fond.
    int ancien_x;                       // Ancienne position x du chien.
    int ancien_y;                       // Ancienne position y du chien.
    int score = 0;                      // Score du joueur.
    int collision = 0;                  // Vaut 1 si le chien touche l'obstacle.
    int collision_deja_comptee = 0;     // Évite d'ajouter trop de points.

    char texte[50];                     // Texte qui contient le score.

    /*
        Démarrage de SDL.
    */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    /*
        Démarrage de SDL_image pour charger les images PNG.
    */
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        printf("Erreur IMG_Init : %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    /*
        Démarrage de SDL_ttf pour écrire le score.
    */
    if (TTF_Init() < 0)
    {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    /*
        Création de la fenêtre.
    */
    window = SDL_CreateWindow(
        "Chien avec fond anime - obstacle plein",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGEUR_FENETRE,
        HAUTEUR_FENETRE,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL)
    {
        printf("Erreur creation fenetre : %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    /*
        Création du renderer.
    */
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL)
    {
        printf("Erreur creation renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    /*
        Chargement des images.

        Il faut avoir dans le même dossier :
        fond.png
        fonddeux.png
        font.ttf
    */
    fond = IMG_LoadTexture(renderer, "fond.png");

    if (fond == NULL)
    {
        printf("Erreur chargement fond.png : %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    chien = IMG_LoadTexture(renderer, "fonddeux.png");

    if (chien == NULL)
    {
        printf("Erreur chargement fonddeux.png : %s\n", IMG_GetError());
        SDL_DestroyTexture(fond);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    /*
        Chargement de la police pour le score.
    */
    font = TTF_OpenFont("font.ttf", 32);

    if (font == NULL)
    {
        printf("Erreur chargement font.ttf : %s\n", TTF_GetError());
        SDL_DestroyTexture(chien);
        SDL_DestroyTexture(fond);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    /*
        Initialisation des deux fonds.

        On utilise deux images de fond côte à côte.
        Comme ça, quand le fond bouge, il n'y a pas de vide noir.
    */
    fond1.x = 0;
    fond1.y = 0;
    fond1.w = LARGEUR_FENETRE;
    fond1.h = HAUTEUR_FENETRE;

    fond2.x = LARGEUR_FENETRE;
    fond2.y = 0;
    fond2.w = LARGEUR_FENETRE;
    fond2.h = HAUTEUR_FENETRE;

    /*
        Initialisation du chien.
    */
    chien_rect.x = 100;
    chien_rect.y = 380;
    chien_rect.w = 120;
    chien_rect.h = 100;

    /*
        Initialisation de l'obstacle plein.
    */
    obstacle.x = 720;
    obstacle.y = 230;
    obstacle.w = 180;
    obstacle.h = 160;

    printf("Commandes :\n");
    printf("Fleches du clavier : deplacer le chien\n");
    printf("q ou Echap : quitter\n");

    /*
        Boucle principale.
    */
    while (continuer)
    {
        /*
            Lecture des événements.
        */
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

        /*
            On garde l'ancienne position du chien.

            Si le chien touche l'obstacle,
            on le remet à cette ancienne position.
        */
        ancien_x = chien_rect.x;
        ancien_y = chien_rect.y;

        /*
            Déplacement fluide avec les touches maintenues.
        */
        const Uint8 *touches = SDL_GetKeyboardState(NULL);

        if (touches[SDL_SCANCODE_UP])
        {
            chien_rect.y = chien_rect.y - VITESSE_CHIEN;
        }

        if (touches[SDL_SCANCODE_DOWN])
        {
            chien_rect.y = chien_rect.y + VITESSE_CHIEN;
        }

        if (touches[SDL_SCANCODE_LEFT])
        {
            chien_rect.x = chien_rect.x - VITESSE_CHIEN;
        }

        if (touches[SDL_SCANCODE_RIGHT])
        {
            chien_rect.x = chien_rect.x + VITESSE_CHIEN;
        }

        /*
            Empêcher le chien de sortir de la fenêtre.
        */
        if (chien_rect.x < 0)
        {
            chien_rect.x = 0;
        }

        if (chien_rect.y < 0)
        {
            chien_rect.y = 0;
        }

        if (chien_rect.x + chien_rect.w > LARGEUR_FENETRE)
        {
            chien_rect.x = LARGEUR_FENETRE - chien_rect.w;
        }

        if (chien_rect.y + chien_rect.h > HAUTEUR_FENETRE)
        {
            chien_rect.y = HAUTEUR_FENETRE - chien_rect.h;
        }

        /*
            Test de collision entre le chien et l'obstacle.
        */
        collision = SDL_HasIntersection(&chien_rect, &obstacle);

        if (collision)
        {
            /*
                Le chien ne peut pas traverser l'obstacle.
                Donc on revient à l'ancienne position.
            */
            chien_rect.x = ancien_x;
            chien_rect.y = ancien_y;

            /*
                On ajoute 1 point seulement au début du contact.
                Si on garde la touche appuyée, le score ne monte pas trop vite.
            */
            if (collision_deja_comptee == 0)
            {
                score++;
                collision_deja_comptee = 1;
                printf("Collision avec l'obstacle ! Score : %d\n", score);
            }
        }
        else
        {
            /*
                Quand le chien ne touche plus l'obstacle,
                on pourra recompter une prochaine collision.
            */
            collision_deja_comptee = 0;
        }

        /*
            Déplacement horizontal lent du fond.

            Le fond part vers la gauche.
        */
        position_fond_x = position_fond_x - VITESSE_FOND;

        if (position_fond_x <= -LARGEUR_FENETRE)
        {
            position_fond_x = 0;
        }

        fond1.x = position_fond_x;
        fond2.x = position_fond_x + LARGEUR_FENETRE;

        /*
            Création du texte du score.
            On détruit l'ancien texte avant d'en créer un nouveau.
        */
        if (texte_score != NULL)
        {
            SDL_DestroyTexture(texte_score);
            texte_score = NULL;
        }

        sprintf(texte, "Score : %d", score);

        SDL_Surface *surface_texte = TTF_RenderText_Blended(font, texte, couleur_texte);

        if (surface_texte != NULL)
        {
            texte_score = SDL_CreateTextureFromSurface(renderer, surface_texte);

            texte_rect.x = 20;
            texte_rect.y = 20;
            texte_rect.w = surface_texte->w;
            texte_rect.h = surface_texte->h;

            SDL_FreeSurface(surface_texte);
        }

        /*
            Affichage.
        */
        SDL_RenderClear(renderer);

        /*
            Afficher les deux fonds.
        */
        SDL_RenderCopy(renderer, fond, NULL, &fond1);
        SDL_RenderCopy(renderer, fond, NULL, &fond2);

        /*
            Afficher le chien.
        */
        SDL_RenderCopy(renderer, chien, NULL, &chien_rect);

        /*
            Dessiner l'obstacle plein.

            Vert : pas de collision.
            Rouge : collision.
        */
        if (collision)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }

        SDL_RenderFillRect(renderer, &obstacle);

        /*
            Afficher le score.
        */
        if (texte_score != NULL)
        {
            SDL_RenderCopy(renderer, texte_score, NULL, &texte_rect);
        }

        /*
            Montrer le résultat dans la fenêtre.
        */
        SDL_RenderPresent(renderer);

        /*
            Petite pause pour un mouvement fluide.
        */
        SDL_Delay(16);
    }

    /*
        Fermeture propre.
    */
    if (texte_score != NULL)
    {
        SDL_DestroyTexture(texte_score);
    }

    TTF_CloseFont(font);

    SDL_DestroyTexture(chien);
    SDL_DestroyTexture(fond);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
