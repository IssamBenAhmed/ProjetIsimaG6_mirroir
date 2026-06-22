#include "../include/graphisme.h"

/* Dessine une grille légère pour rendre l'arène plus lisible */
static void dessiner_grille(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 20, 45, 65, 130);// Bleu foncé/sombre.

    for (int x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
    }

    for (int y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
    }
}

/* Dessine les limites de l'arène */
static void dessiner_bordure(SDL_Renderer *renderer)
{
    SDL_Rect bordure1 = {0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1};
    SDL_Rect bordure2 = {2, 2, WINDOW_WIDTH - 5, WINDOW_HEIGHT - 5};

    SDL_SetRenderDrawColor(renderer, 0, 220, 255, 255);//Bleu cyan
    SDL_RenderDrawRect(renderer, &bordure1);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);//blanc peu transparent 
    SDL_RenderDrawRect(renderer, &bordure2);
}

/* Choisit la couleur selon le type de cellule */
static void choisir_couleur_cellule(SDL_Renderer *renderer, int cellule)
{
    if (cellule == CELL_PLAYER) {
        SDL_SetRenderDrawColor(renderer, 0, 220, 255, 255);      // bleu cyan
    } else if (cellule == CELL_AI_1) {
        SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);      // orange
    } else if (cellule == CELL_AI_2) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 120, 255);      // vert néon
    } else {
        SDL_SetRenderDrawColor(renderer, 230, 80, 255, 255);     // violet / rose
    }
}

void dessiner_arene(SDL_Renderer *renderer, int grille[WIDTH][HEIGHT])
{
    SDL_Rect case_rect;
    SDL_Rect glow_rect;

    /* Fond bleu nuit, plus esthétique qu'un noir total */
    SDL_SetRenderDrawColor(renderer, 3, 8, 18, 255);
    SDL_RenderClear(renderer);

    /* Grille de fond */
    dessiner_grille(renderer);

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            if (grille[x][y] == CELL_EMPTY) {
                continue;
            }

            /*
             * Rectangle légèrement plus petit que la case.
             * Cela laisse apparaître la grille entre les traces.
             */
            case_rect.x = x * CELL_SIZE + 2;
            case_rect.y = y * CELL_SIZE + 2;
            case_rect.w = CELL_SIZE - 4;
            case_rect.h = CELL_SIZE - 4;

            /*
             * Petit rectangle plus grand et plus sombre autour de la trace.
             * Cela donne un effet lumineux simple.
             */
            glow_rect.x = x * CELL_SIZE + 1;
            glow_rect.y = y * CELL_SIZE + 1;
            glow_rect.w = CELL_SIZE - 2;
            glow_rect.h = CELL_SIZE - 2;

            if (grille[x][y] == CELL_PLAYER) {
                SDL_SetRenderDrawColor(renderer, 0, 80, 120, 180);
            } else if (grille[x][y] == CELL_AI_1) {
                SDL_SetRenderDrawColor(renderer, 120, 60, 0, 180);
            } else if (grille[x][y] == CELL_AI_2) {
                SDL_SetRenderDrawColor(renderer, 0, 100, 50, 180);
            } else {
                SDL_SetRenderDrawColor(renderer, 100, 30, 120, 180);
            }

            SDL_RenderFillRect(renderer, &glow_rect);

            choisir_couleur_cellule(renderer, grille[x][y]);
            SDL_RenderFillRect(renderer, &case_rect);
        }
    }

    /* Bordure finale par-dessus tout */
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
    SDL_Rect ombre = {WINDOW_WIDTH / 2 - 155, WINDOW_HEIGHT / 2 - 55, 310, 110};
    SDL_Rect panneau = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50, 300, 100};

    /* Ombre derrière le panneau */
    SDL_SetRenderDrawColor(renderer, 20, 0, 0, 220);
    SDL_RenderFillRect(renderer, &ombre);

    /* Panneau central rouge sombre */
    SDL_SetRenderDrawColor(renderer, 120, 0, 20, 245);
    SDL_RenderFillRect(renderer, &panneau);

    /* Bordure lumineuse */
    SDL_SetRenderDrawColor(renderer, 255, 40, 80, 255);
    SDL_RenderDrawRect(renderer, &panneau);

    SDL_Rect bordure_interne = {panneau.x + 4, panneau.y + 4, panneau.w - 8, panneau.h - 8};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);
    SDL_RenderDrawRect(renderer, &bordure_interne);

    /* Dessin manuel des lettres E N D en gros pixels */
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
} TU PEUX AMELIORER LESTHETIQUE PLUS que ça je veux qu'elle sera extraordinaire 