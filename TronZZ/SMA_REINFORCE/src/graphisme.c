#include "../include/graphisme.h"

/* Petit max pour éviter les tailles négatives */
static int max_int(int a, int b)
{
    if (a > b) {
        return a;
    }

    return b;
}

/* Couleur principale selon le type de cellule */
static void couleur_cellule(int cellule, int *r, int *g, int *b)
{
    if (cellule == CELL_PLAYER) {
        *r = 0;
        *g = 220;
        *b = 255;
    } else if (cellule == CELL_AI_1) {
        *r = 255;
        *g = 140;
        *b = 0;
    } else if (cellule == CELL_AI_2) {
        *r = 0;
        *g = 255;
        *b = 120;
    } else {
        *r = 230;
        *g = 80;
        *b = 255;
    }
}

/* Dessine un fond bleu nuit avec quelques petites étoiles */
static void dessiner_fond(SDL_Renderer *renderer, int frame)
{
    SDL_SetRenderDrawColor(renderer, 3, 8, 18, 255);
    SDL_RenderClear(renderer);

    /* Petites étoiles fixes, juste pour donner de la profondeur */
    for (int i = 0; i < 80; i++) {
        int x = (i * 73 + frame / 3) % WINDOW_WIDTH;
        int y = (i * 41) % WINDOW_HEIGHT;
        int alpha = 30 + (i % 4) * 25;

        SDL_SetRenderDrawColor(renderer, 80, 200, 255, alpha);
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

/* Dessine une grille style Tron */
static void dessiner_grille(SDL_Renderer *renderer)
{
    for (int x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE) {
        if ((x / CELL_SIZE) % 5 == 0) {
            SDL_SetRenderDrawColor(renderer, 0, 180, 255, 90);
        } else {
            SDL_SetRenderDrawColor(renderer, 20, 60, 90, 55);
        }

        SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
    }

    for (int y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE) {
        if ((y / CELL_SIZE) % 5 == 0) {
            SDL_SetRenderDrawColor(renderer, 0, 180, 255, 90);
        } else {
            SDL_SetRenderDrawColor(renderer, 20, 60, 90, 55);
        }

        SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
    }
}

/* Ajoute de fines lignes horizontales, comme un écran futuriste */
static void dessiner_scanlines(SDL_Renderer *renderer)
{
    for (int y = 0; y < WINDOW_HEIGHT; y += 4) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 12);
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
    }
}

/* Bordure extérieure très lumineuse */
static void dessiner_bordure(SDL_Renderer *renderer)
{
    for (int i = 0; i < 7; i++) {
        SDL_Rect glow = {
            i,
            i,
            WINDOW_WIDTH - 1 - 2 * i,
            WINDOW_HEIGHT - 1 - 2 * i
        };

        SDL_SetRenderDrawColor(renderer, 0, 220, 255, 35 - i * 4);
        SDL_RenderDrawRect(renderer, &glow);
    }

    SDL_Rect bordure1 = {0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1};
    SDL_Rect bordure2 = {3, 3, WINDOW_WIDTH - 7, WINDOW_HEIGHT - 7};

    SDL_SetRenderDrawColor(renderer, 0, 240, 255, 255);
    SDL_RenderDrawRect(renderer, &bordure1);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 170);
    SDL_RenderDrawRect(renderer, &bordure2);
}

/* Dessine une cellule avec un vrai effet néon */
static void dessiner_cellule_neon(SDL_Renderer *renderer, int x, int y, int cellule, int frame)
{
    int r;
    int g;
    int b;

    int px = x * CELL_SIZE;
    int py = y * CELL_SIZE;

    int pulse = frame % 40;

    if (pulse > 20) {
        pulse = 40 - pulse;
    }

    couleur_cellule(cellule, &r, &g, &b);

    /*
     * Plusieurs rectangles autour de la case.
     * Plus on se rapproche du centre, plus la lumière est forte.
     */
    for (int couche = 4; couche >= 1; couche--) {
        SDL_Rect glow = {
            px + 2 - couche,
            py + 2 - couche,
            CELL_SIZE - 4 + 2 * couche,
            CELL_SIZE - 4 + 2 * couche
        };

        int alpha = 18 + (5 - couche) * 22 + pulse;

        SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
        SDL_RenderFillRect(renderer, &glow);
    }

    /* Corps principal de la trace */
    int marge = 3;

    if (CELL_SIZE < 10) {
        marge = 1;
    }

    SDL_Rect corps = {
        px + marge,
        py + marge,
        max_int(1, CELL_SIZE - 2 * marge),
        max_int(1, CELL_SIZE - 2 * marge)
    };

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &corps);

    /* Petit reflet blanc pour rendre la trace plus brillante */
    SDL_Rect reflet = {
        corps.x + 2,
        corps.y + 2,
        max_int(1, corps.w / 2),
        max_int(1, corps.h / 2)
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 70);
    SDL_RenderFillRect(renderer, &reflet);

    /* Contour blanc fin */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 90);
    SDL_RenderDrawRect(renderer, &corps);

    /* Petit point lumineux au centre */
    if (CELL_SIZE >= 10) {
        SDL_Rect coeur = {
            px + CELL_SIZE / 2 - 1,
            py + CELL_SIZE / 2 - 1,
            3,
            3
        };

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);
        SDL_RenderFillRect(renderer, &coeur);
    }
}

void dessiner_arene(SDL_Renderer *renderer, int grille[WIDTH][HEIGHT])
{
    static int frame = 0;

    /*
     * Très important :
     * Sans ça, les couleurs avec alpha comme 180 ou 90
     * ne donnent pas un vrai effet transparent.
     */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    dessiner_fond(renderer, frame);
    dessiner_grille(renderer);

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            if (grille[x][y] != CELL_EMPTY) {
                dessiner_cellule_neon(renderer, x, y, grille[x][y], frame);
            }
        }
    }

    dessiner_scanlines(renderer);
    dessiner_bordure(renderer);

    SDL_RenderPresent(renderer);

    frame++;
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

/* Petit bloc utilisé pour dessiner les lettres */
static void dessiner_bloc(SDL_Renderer *renderer, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

/* Dessine le panneau de fin en style néon */
void dessiner_panneau_fin(SDL_Renderer *renderer)
{
    static int frame_fin = 0;

    int pulse = frame_fin % 50;

    if (pulse > 25) {
        pulse = 50 - pulse;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Rect ombre = {
        WINDOW_WIDTH / 2 - 170,
        WINDOW_HEIGHT / 2 - 70,
        340,
        140
    };

    SDL_Rect panneau = {
        WINDOW_WIDTH / 2 - 155,
        WINDOW_HEIGHT / 2 - 55,
        310,
        110
    };

    /* Fond sombre transparent derrière le panneau */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer, &ombre);

    /* Glow rouge autour du panneau */
    for (int i = 0; i < 8; i++) {
        SDL_Rect glow = {
            panneau.x - i,
            panneau.y - i,
            panneau.w + 2 * i,
            panneau.h + 2 * i
        };

        SDL_SetRenderDrawColor(renderer, 255, 20, 80, 45 - i * 4);
        SDL_RenderDrawRect(renderer, &glow);
    }

    /* Panneau central */
    SDL_SetRenderDrawColor(renderer, 70, 0, 20, 235);
    SDL_RenderFillRect(renderer, &panneau);

    /* Bordure rouge brillante */
    SDL_SetRenderDrawColor(renderer, 255, 40 + pulse * 4, 90 + pulse * 3, 255);
    SDL_RenderDrawRect(renderer, &panneau);

    SDL_Rect bordure_interne = {
        panneau.x + 5,
        panneau.y + 5,
        panneau.w - 10,
        panneau.h - 10
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 160);
    SDL_RenderDrawRect(renderer, &bordure_interne);

    /*
     * Lettres END en gros pixels.
     * On dessine d'abord une ombre rouge, puis le texte blanc.
     */

    /* Ombre du texte */
    SDL_SetRenderDrawColor(renderer, 255, 0, 60, 120);

    /* E */
    dessiner_bloc(renderer, panneau.x + 48, panneau.y + 23, 12, 60);
    dessiner_bloc(renderer, panneau.x + 48, panneau.y + 23, 45, 12);
    dessiner_bloc(renderer, panneau.x + 48, panneau.y + 47, 35, 12);
    dessiner_bloc(renderer, panneau.x + 48, panneau.y + 71, 45, 12);

    /* N */
    dessiner_bloc(renderer, panneau.x + 119, panneau.y + 23, 12, 60);
    dessiner_bloc(renderer, panneau.x + 162, panneau.y + 23, 12, 60);
    dessiner_bloc(renderer, panneau.x + 132, panneau.y + 35, 15, 15);
    dessiner_bloc(renderer, panneau.x + 147, panneau.y + 50, 15, 15);

    /* D */
    dessiner_bloc(renderer, panneau.x + 202, panneau.y + 23, 12, 60);
    dessiner_bloc(renderer, panneau.x + 202, panneau.y + 23, 35, 12);
    dessiner_bloc(renderer, panneau.x + 202, panneau.y + 71, 35, 12);
    dessiner_bloc(renderer, panneau.x + 235, panneau.y + 35, 12, 36);

    /* Texte blanc par-dessus, légèrement décalé */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    /* E */
    dessiner_bloc(renderer, panneau.x + 45, panneau.y + 20, 12, 60);
    dessiner_bloc(renderer, panneau.x + 45, panneau.y + 20, 45, 12);
    dessiner_bloc(renderer, panneau.x + 45, panneau.y + 44, 35, 12);
    dessiner_bloc(renderer, panneau.x + 45, panneau.y + 68, 45, 12);

    /* N */
    dessiner_bloc(renderer, panneau.x + 116, panneau.y + 20, 12, 60);
    dessiner_bloc(renderer, panneau.x + 159, panneau.y + 20, 12, 60);
    dessiner_bloc(renderer, panneau.x + 129, panneau.y + 32, 15, 15);
    dessiner_bloc(renderer, panneau.x + 144, panneau.y + 47, 15, 15);

    /* D */
    dessiner_bloc(renderer, panneau.x + 199, panneau.y + 20, 12, 60);
    dessiner_bloc(renderer, panneau.x + 199, panneau.y + 20, 35, 12);
    dessiner_bloc(renderer, panneau.x + 199, panneau.y + 68, 35, 12);
    dessiner_bloc(renderer, panneau.x + 232, panneau.y + 32, 12, 36);

    frame_fin++;
}