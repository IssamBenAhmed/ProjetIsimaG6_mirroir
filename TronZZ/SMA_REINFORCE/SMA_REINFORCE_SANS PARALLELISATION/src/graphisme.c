#include "../include/graphisme.h"

/*
 * Fichier graphisme.c
 * Ici on s'occupe seulement de l'affichage :
 * - belle grille style Tron
 * - couleurs des motos
 * - bordure
 * - compteur de temps écoulé
 * - panneau END
 */

/* Dessine un petit rectangle */
static void remplir_rect(SDL_Renderer *renderer, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

/* Dessine une grille plus belle style Tron */
static void dessiner_grille(SDL_Renderer *renderer)
{
    /* Petites lignes de base */
    SDL_SetRenderDrawColor(renderer, 10, 45, 75, 120);

    for (int x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
    }

    for (int y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
    }

    /* Lignes principales toutes les 5 cases */
    SDL_SetRenderDrawColor(renderer, 0, 180, 255, 160);

    for (int x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE * 5) {
        SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
    }

    for (int y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE * 5) {
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
    }

    /* Petits points lumineux aux intersections principales */
    SDL_SetRenderDrawColor(renderer, 0, 220, 255, 200);

    for (int x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE * 5) {
        for (int y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE * 5) {
            remplir_rect(renderer, x - 1, y - 1, 3, 3);
        }
    }
}

/* Dessine les limites de l'arène */
static void dessiner_bordure(SDL_Renderer *renderer)
{
    SDL_Rect bordure1 = {0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1};
    SDL_Rect bordure2 = {2, 2, WINDOW_WIDTH - 5, WINDOW_HEIGHT - 5};
    SDL_Rect bordure3 = {5, 5, WINDOW_WIDTH - 11, WINDOW_HEIGHT - 11};

    /* Bordure cyan forte */
    SDL_SetRenderDrawColor(renderer, 0, 240, 255, 255);
    SDL_RenderDrawRect(renderer, &bordure1);

    /* Bordure blanche transparente */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 170);
    SDL_RenderDrawRect(renderer, &bordure2);

    /* Bordure interne bleu foncé */
    SDL_SetRenderDrawColor(renderer, 0, 100, 160, 160);
    SDL_RenderDrawRect(renderer, &bordure3);

    /* Coins lumineux */
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

    SDL_RenderDrawLine(renderer, 0, 0, 40, 0);
    SDL_RenderDrawLine(renderer, 0, 0, 0, 40);

    SDL_RenderDrawLine(renderer, WINDOW_WIDTH - 1, 0, WINDOW_WIDTH - 41, 0);
    SDL_RenderDrawLine(renderer, WINDOW_WIDTH - 1, 0, WINDOW_WIDTH - 1, 40);

    SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT - 1, 40, WINDOW_HEIGHT - 1);
    SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT - 1, 0, WINDOW_HEIGHT - 41);

    SDL_RenderDrawLine(renderer, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1, WINDOW_WIDTH - 41, WINDOW_HEIGHT - 1);
    SDL_RenderDrawLine(renderer, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 41);
}

/* Choisit la couleur selon le type de cellule */
static void choisir_couleur_cellule(SDL_Renderer *renderer, int cellule)
{
    if (cellule == CELL_PLAYER) {
        SDL_SetRenderDrawColor(renderer, 0, 230, 255, 255);      /* bleu cyan */
    } else if (cellule == CELL_AI_1) {
        SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);      /* orange */
    } else if (cellule == CELL_AI_2) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 120, 255);      /* vert néon */
    } else {
        SDL_SetRenderDrawColor(renderer, 230, 80, 255, 255);     /* violet / rose */
    }
}

/* Choisit la couleur de la lueur autour d'une cellule */
static void choisir_couleur_lueur(SDL_Renderer *renderer, int cellule)
{
    if (cellule == CELL_PLAYER) {
        SDL_SetRenderDrawColor(renderer, 0, 90, 150, 150);
    } else if (cellule == CELL_AI_1) {
        SDL_SetRenderDrawColor(renderer, 150, 70, 0, 150);
    } else if (cellule == CELL_AI_2) {
        SDL_SetRenderDrawColor(renderer, 0, 120, 60, 150);
    } else {
        SDL_SetRenderDrawColor(renderer, 120, 35, 150, 150);
    }
}

/* Dessine une cellule avec un effet lumineux */
static void dessiner_cellule(SDL_Renderer *renderer, int x, int y, int cellule)
{
    SDL_Rect glow_rect;
    SDL_Rect case_rect;

    /*
     * glow_rect = rectangle un peu grand.
     * Il donne l'effet lumineux autour de la trace.
     */
    glow_rect.x = x * CELL_SIZE + 1;
    glow_rect.y = y * CELL_SIZE + 1;
    glow_rect.w = CELL_SIZE - 2;
    glow_rect.h = CELL_SIZE - 2;

    /*
     * case_rect = vraie trace colorée.
     * Elle est plus petite pour laisser voir la grille.
     */
    case_rect.x = x * CELL_SIZE + 3;
    case_rect.y = y * CELL_SIZE + 3;
    case_rect.w = CELL_SIZE - 6;
    case_rect.h = CELL_SIZE - 6;

    if (case_rect.w < 1) {
        case_rect.w = 1;
    }

    if (case_rect.h < 1) {
        case_rect.h = 1;
    }

    choisir_couleur_lueur(renderer, cellule);
    SDL_RenderFillRect(renderer, &glow_rect);

    choisir_couleur_cellule(renderer, cellule);
    SDL_RenderFillRect(renderer, &case_rect);
}

/*
 * Dessine un chiffre avec 7 segments.
 * Cela permet d'afficher le compteur sans SDL_ttf.
 */
static void dessiner_chiffre(SDL_Renderer *renderer, int chiffre, int x, int y, int taille)
{
    int epaisseur = taille;
    int longueur = 4 * taille;

    SDL_Rect a = {x + epaisseur, y, longueur, epaisseur};
    SDL_Rect b = {x + longueur + epaisseur, y + epaisseur, epaisseur, longueur};
    SDL_Rect c = {x + longueur + epaisseur, y + longueur + 2 * epaisseur, epaisseur, longueur};
    SDL_Rect d = {x + epaisseur, y + 2 * longueur + 2 * epaisseur, longueur, epaisseur};
    SDL_Rect e = {x, y + longueur + 2 * epaisseur, epaisseur, longueur};
    SDL_Rect f = {x, y + epaisseur, epaisseur, longueur};
    SDL_Rect g = {x + epaisseur, y + longueur + epaisseur, longueur, epaisseur};

    int segments[10][7] = {
        {1, 1, 1, 1, 1, 1, 0}, /* 0 */
        {0, 1, 1, 0, 0, 0, 0}, /* 1 */
        {1, 1, 0, 1, 1, 0, 1}, /* 2 */
        {1, 1, 1, 1, 0, 0, 1}, /* 3 */
        {0, 1, 1, 0, 0, 1, 1}, /* 4 */
        {1, 0, 1, 1, 0, 1, 1}, /* 5 */
        {1, 0, 1, 1, 1, 1, 1}, /* 6 */
        {1, 1, 1, 0, 0, 0, 0}, /* 7 */
        {1, 1, 1, 1, 1, 1, 1}, /* 8 */
        {1, 1, 1, 1, 0, 1, 1}  /* 9 */
    };

    if (chiffre < 0 || chiffre > 9) {
        return;
    }

    if (segments[chiffre][0]) {
        SDL_RenderFillRect(renderer, &a);
    }

    if (segments[chiffre][1]) {
        SDL_RenderFillRect(renderer, &b);
    }

    if (segments[chiffre][2]) {
        SDL_RenderFillRect(renderer, &c);
    }

    if (segments[chiffre][3]) {
        SDL_RenderFillRect(renderer, &d);
    }

    if (segments[chiffre][4]) {
        SDL_RenderFillRect(renderer, &e);
    }

    if (segments[chiffre][5]) {
        SDL_RenderFillRect(renderer, &f);
    }

    if (segments[chiffre][6]) {
        SDL_RenderFillRect(renderer, &g);
    }
}

/* Dessine les deux points entre minutes et secondes */
static void dessiner_deux_points(SDL_Renderer *renderer, int x, int y, int taille)
{
    remplir_rect(renderer, x, y + 3 * taille, taille, taille);
    remplir_rect(renderer, x, y + 7 * taille, taille, taille);
}

/*
 * Dessine le compteur de temps écoulé.
 * Format : MM:SS
 */
static void dessiner_compteur_temps(SDL_Renderer *renderer)
{
    static Uint32 temps_debut = 0;

    if (temps_debut == 0) {
        temps_debut = SDL_GetTicks();
    }

    Uint32 temps_actuel = SDL_GetTicks();
    Uint32 temps_ecoule = (temps_actuel - temps_debut) / 1000;

    int minutes = temps_ecoule / 60;
    int secondes = temps_ecoule % 60;

    if (minutes > 99) {
        minutes = 99;
        secondes = 59;
    }

    int m1 = minutes / 10;
    int m2 = minutes % 10;
    int s1 = secondes / 10;
    int s2 = secondes % 10;

    SDL_Rect ombre = {11, 11, 116, 38};
    SDL_Rect panneau = {8, 8, 116, 38};

    /* Fond du compteur */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 170);
    SDL_RenderFillRect(renderer, &ombre);

    SDL_SetRenderDrawColor(renderer, 5, 20, 35, 230);
    SDL_RenderFillRect(renderer, &panneau);

    SDL_SetRenderDrawColor(renderer, 0, 220, 255, 230);
    SDL_RenderDrawRect(renderer, &panneau);

    /* Chiffres */
    SDL_SetRenderDrawColor(renderer, 0, 240, 255, 255);

    dessiner_chiffre(renderer, m1, 18, 15, 2);
    dessiner_chiffre(renderer, m2, 34, 15, 2);

    dessiner_deux_points(renderer, 51, 15, 2);

    dessiner_chiffre(renderer, s1, 58, 15, 2);
    dessiner_chiffre(renderer, s2, 74, 15, 2);
}

void dessiner_arene(SDL_Renderer *renderer, int grille[WIDTH][HEIGHT])
{
    /*
     * Active la transparence.
     * Important pour les couleurs avec alpha.
     */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    /*
     * Fond bleu nuit.
     */
    SDL_SetRenderDrawColor(renderer, 3, 8, 18, 255);
    SDL_RenderClear(renderer);

    /*
     * Bandes sombres discrètes dans le fond.
     */
    for (int y = 0; y < WINDOW_HEIGHT; y += 40) {
        SDL_SetRenderDrawColor(renderer, 5, 15, 30, 80);
        remplir_rect(renderer, 0, y, WINDOW_WIDTH, 20);
    }

    /*
     * Grille de fond.
     */
    dessiner_grille(renderer);

    /*
     * Dessin des traces des motos.
     */
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            if (grille[x][y] != CELL_EMPTY) {
                dessiner_cellule(renderer, x, y, grille[x][y]);
            }
        }
    }

    /*
     * Bordure finale par-dessus tout.
     */
    dessiner_bordure(renderer);

    /*
     * Compteur de temps écoulé.
     */
    dessiner_compteur_temps(renderer);

    /*
     * Affichage à l'écran.
     */
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

void dessiner_panneau_fin(SDL_Renderer *renderer)
{
    SDL_Rect ombre = {WINDOW_WIDTH / 2 - 155, WINDOW_HEIGHT / 2 - 55, 310, 110};
    SDL_Rect panneau = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50, 300, 100};

    /*
     * Ombre derrière le panneau.
     */
    SDL_SetRenderDrawColor(renderer, 20, 0, 0, 220);
    SDL_RenderFillRect(renderer, &ombre);

    /*
     * Panneau central rouge sombre.
     */
    SDL_SetRenderDrawColor(renderer, 120, 0, 20, 245);
    SDL_RenderFillRect(renderer, &panneau);

    /*
     * Bordure lumineuse.
     */
    SDL_SetRenderDrawColor(renderer, 255, 40, 80, 255);
    SDL_RenderDrawRect(renderer, &panneau);

    SDL_Rect bordure_interne = {panneau.x + 4, panneau.y + 4, panneau.w - 8, panneau.h - 8};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);
    SDL_RenderDrawRect(renderer, &bordure_interne);

    /*
     * Dessin manuel des lettres E N D en gros pixels.
     */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    /* Lettre E */
    SDL_Rect e1 = {panneau.x + 50, panneau.y + 20, 10, 60};
    SDL_Rect e2 = {panneau.x + 50, panneau.y + 20, 40, 10};
    SDL_Rect e3 = {panneau.x + 50, panneau.y + 45, 30, 10};
    SDL_Rect e4 = {panneau.x + 50, panneau.y + 70, 40, 10};

    SDL_RenderFillRect(renderer, &e1);
    SDL_RenderFillRect(renderer, &e2);
    SDL_RenderFillRect(renderer, &e3);
    SDL_RenderFillRect(renderer, &e4);

    /* Lettre N */
    SDL_Rect n1 = {panneau.x + 120, panneau.y + 20, 10, 60};
    SDL_Rect n2 = {panneau.x + 160, panneau.y + 20, 10, 60};
    SDL_Rect n3 = {panneau.x + 130, panneau.y + 30, 15, 15};
    SDL_Rect n4 = {panneau.x + 145, panneau.y + 45, 15, 15};

    SDL_RenderFillRect(renderer, &n1);
    SDL_RenderFillRect(renderer, &n2);
    SDL_RenderFillRect(renderer, &n3);
    SDL_RenderFillRect(renderer, &n4);

    /* Lettre D */
    SDL_Rect d1 = {panneau.x + 200, panneau.y + 20, 10, 60};
    SDL_Rect d2 = {panneau.x + 200, panneau.y + 20, 30, 10};
    SDL_Rect d3 = {panneau.x + 200, panneau.y + 70, 30, 10};
    SDL_Rect d4 = {panneau.x + 230, panneau.y + 30, 10, 40};

    SDL_RenderFillRect(renderer, &d1);
    SDL_RenderFillRect(renderer, &d2);
    SDL_RenderFillRect(renderer, &d3);
    SDL_RenderFillRect(renderer, &d4);
}