#include "../include/graphisme.h"

/*
 * Fichier graphisme.c
 * Rendu graphique plus sobre :
 * - fond sombre
 * - grille discrète
 * - bordure lumineuse
 * - traces plus visibles
 * - panneau END propre
 */

/* Petit outil pour dessiner rapidement un rectangle plein */
static void remplir_rect(SDL_Renderer *renderer, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

/* Dessine une grille discrète */
static void dessiner_grille(SDL_Renderer *renderer)
{
    /*
     * Petite grille très discrète.
     * Elle aide à voir les cases sans dominer l'affichage.
     */
    SDL_SetRenderDrawColor(renderer, 8, 22, 35, 45);

    for (int x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
    }

    for (int y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
    }

    /*
     * Grande grille toutes les 5 cases.
     * Elle donne une structure à l'arène, mais reste douce.
     */
    SDL_SetRenderDrawColor(renderer, 0, 120, 180, 70);

    for (int x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE * 5) {
        SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
    }

    for (int y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE * 5) {
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
    }
}

/* Dessine une bordure lumineuse mais pas trop agressive */
static void dessiner_bordure(SDL_Renderer *renderer)
{
    SDL_Rect bordure_ext = {0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1};
    SDL_Rect bordure_int = {3, 3, WINDOW_WIDTH - 7, WINDOW_HEIGHT - 7};

    SDL_SetRenderDrawColor(renderer, 0, 220, 255, 255);
    SDL_RenderDrawRect(renderer, &bordure_ext);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 90);
    SDL_RenderDrawRect(renderer, &bordure_int);

    /*
     * Coins lumineux courts.
     * Cela donne un style arène sans surcharger tout le contour.
     */
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 230);

    SDL_RenderDrawLine(renderer, 0, 0, 35, 0);
    SDL_RenderDrawLine(renderer, 0, 0, 0, 35);

    SDL_RenderDrawLine(renderer, WINDOW_WIDTH - 1, 0, WINDOW_WIDTH - 36, 0);
    SDL_RenderDrawLine(renderer, WINDOW_WIDTH - 1, 0, WINDOW_WIDTH - 1, 35);

    SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT - 1, 35, WINDOW_HEIGHT - 1);
    SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT - 1, 0, WINDOW_HEIGHT - 36);

    SDL_RenderDrawLine(renderer, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1,
                       WINDOW_WIDTH - 36, WINDOW_HEIGHT - 1);
    SDL_RenderDrawLine(renderer, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1,
                       WINDOW_WIDTH - 1, WINDOW_HEIGHT - 36);
}

/* Couleur principale des traces */
static void choisir_couleur_cellule(SDL_Renderer *renderer, int cellule)
{
    if (cellule == CELL_PLAYER) {
        SDL_SetRenderDrawColor(renderer, 0, 230, 255, 255);      /* bleu cyan */
    } else if (cellule == CELL_AI_1) {
        SDL_SetRenderDrawColor(renderer, 255, 150, 0, 255);      /* orange */
    } else if (cellule == CELL_AI_2) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 120, 255);      /* vert néon */
    } else {
        SDL_SetRenderDrawColor(renderer, 230, 70, 255, 255);     /* violet */
    }
}

/* Couleur de la lueur autour des traces */
static void choisir_couleur_lueur(SDL_Renderer *renderer, int cellule)
{
    if (cellule == CELL_PLAYER) {
        SDL_SetRenderDrawColor(renderer, 0, 120, 180, 80);
    } else if (cellule == CELL_AI_1) {
        SDL_SetRenderDrawColor(renderer, 180, 80, 0, 80);
    } else if (cellule == CELL_AI_2) {
        SDL_SetRenderDrawColor(renderer, 0, 160, 70, 80);
    } else {
        SDL_SetRenderDrawColor(renderer, 160, 40, 180, 80);
    }
}

/* Dessine une case occupée par une moto ou une trace */
static void dessiner_cellule(SDL_Renderer *renderer, int x, int y, int cellule)
{
    SDL_Rect glow_rect;
    SDL_Rect core_rect;
    SDL_Rect centre_rect;

    /*
     * Lueur : presque toute la case.
     */
    glow_rect.x = x * CELL_SIZE;
    glow_rect.y = y * CELL_SIZE;
    glow_rect.w = CELL_SIZE;
    glow_rect.h = CELL_SIZE;

    /*
     * Trace principale : plus grande que l'ancienne version.
     * Avant, CELL_SIZE - 6 rendait les traces trop petites.
     */
    core_rect.x = x * CELL_SIZE + 1;
    core_rect.y = y * CELL_SIZE + 1;
    core_rect.w = CELL_SIZE - 2;
    core_rect.h = CELL_SIZE - 2;

    /*
     * Petit centre plus clair pour donner un effet lumineux.
     */
    centre_rect.x = x * CELL_SIZE + CELL_SIZE / 3;
    centre_rect.y = y * CELL_SIZE + CELL_SIZE / 3;
    centre_rect.w = CELL_SIZE / 3;
    centre_rect.h = CELL_SIZE / 3;

    if (core_rect.w < 1) {
        core_rect.w = 1;
    }

    if (core_rect.h < 1) {
        core_rect.h = 1;
    }

    if (centre_rect.w < 1) {
        centre_rect.w = 1;
    }

    if (centre_rect.h < 1) {
        centre_rect.h = 1;
    }

    choisir_couleur_lueur(renderer, cellule);
    SDL_RenderFillRect(renderer, &glow_rect);

    choisir_couleur_cellule(renderer, cellule);
    SDL_RenderFillRect(renderer, &core_rect);

    /*
     * Petit éclat au centre.
     */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 120);
    SDL_RenderFillRect(renderer, &centre_rect);
}

/* Dessine l'arène complète */
void dessiner_arene(SDL_Renderer *renderer, int grille[WIDTH][HEIGHT])
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    /*
     * Fond très sombre.
     * Moins de bruit visuel que les bandes répétées.
     */
    SDL_SetRenderDrawColor(renderer, 2, 5, 12, 255);
    SDL_RenderClear(renderer);

    /*
     * Léger rectangle intérieur pour donner de la profondeur.
     */
    SDL_SetRenderDrawColor(renderer, 4, 10, 22, 255);
    remplir_rect(renderer, 6, 6, WINDOW_WIDTH - 12, WINDOW_HEIGHT - 12);

    dessiner_grille(renderer);

    /*
     * Dessin des motos et des traces.
     */
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            if (grille[x][y] != CELL_EMPTY) {
                dessiner_cellule(renderer, x, y, grille[x][y]);
            }
        }
    }

    dessiner_bordure(renderer);

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
    SDL_Rect ombre = {WINDOW_WIDTH / 2 - 160, WINDOW_HEIGHT / 2 - 60, 320, 120};
    SDL_Rect panneau = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50, 300, 100};
    SDL_Rect bordure_interne = {panneau.x + 5, panneau.y + 5, panneau.w - 10, panneau.h - 10};

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    /*
     * Ombre.
     */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
    SDL_RenderFillRect(renderer, &ombre);

    /*
     * Panneau sombre rouge.
     */
    SDL_SetRenderDrawColor(renderer, 80, 0, 18, 245);
    SDL_RenderFillRect(renderer, &panneau);

    /*
     * Bordures.
     */
    SDL_SetRenderDrawColor(renderer, 255, 50, 90, 255);
    SDL_RenderDrawRect(renderer, &panneau);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 120);
    SDL_RenderDrawRect(renderer, &bordure_interne);

    /*
     * Lettres END.
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