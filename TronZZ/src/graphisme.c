#include "graphisme.h"
#include <SDL2/SDL_image.h>
#include <math.h>
static SDL_Texture *textures_motos[MAX_MOTOS + 1] = {NULL};

// les chemins des fichiers images
static const char *fichiers_motos[MAX_MOTOS + 1] = {
    NULL, "assets/moto_joueur.png", "assets/moto_ia1.png",
    "assets/moto_ia2.png", "assets/moto_ia3.png"};
/*
 * graphisme.c
 * Style Tron :
 * - écran titre avant le jeu
 * - fond bleu sombre
 * - grille cyan visible
 * - traces lumineuses
 * - bordure néon
 * - panneau WIN / LOSE
 */

typedef struct Couleur {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
} Couleur;

static void set_couleur(SDL_Renderer *renderer,
                        Couleur c) // imposer une couleur
{
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

static void remplir_rect(SDL_Renderer *renderer, int x, int y, int w,
                         int h) // remplir un rectangle complet
{
  SDL_Rect rect = {x, y, w, h};
  SDL_RenderFillRect(renderer, &rect);
}

static void
remplir_rect_limite(SDL_Renderer *renderer, int x, int y, int w,
                    int h) // remplir un rectangle sans depasser les limites
{
  if (x < 0) {
    w += x;
    x = 0;
  }

  if (y < 0) {
    h += y;
    y = 0;
  }

  if (x + w > WINDOW_WIDTH) {
    w = WINDOW_WIDTH - x;
  }

  if (y + h > WINDOW_HEIGHT) {
    h = WINDOW_HEIGHT - y;
  }

  if (w <= 0 || h <= 0) {
    return;
  }

  remplir_rect(renderer, x, y, w, h);
}

static Couleur couleur_principale(int cellule) {
  if (cellule == CELL_PLAYER) {
    return (Couleur){0, 240, 255, 255}; // Bleu clair
  } else if (cellule == CELL_AI_1) {
    return (Couleur){255, 160, 0, 255}; // orange nèon
  } else if (cellule == CELL_AI_2) {
    return (Couleur){0, 255, 120, 255}; // vert néon
  }

  return (Couleur){
      240, 80, 255,
      255}; // violet/rose néon (=couleur lumineuse comme brillée dans le noir)
}

static Couleur
couleur_lueur(int cellule) //(autour de la moto ou de sa trace, on voit une
                           // lumière colorée qui brille un peu.)
{
  if (cellule == CELL_PLAYER) {
    return (Couleur){0, 160, 255, 160}; //(bleu transparente)
  } else if (cellule == CELL_AI_1) {
    return (Couleur){255, 110, 0, 160}; //(orange transparente)
  } else if (cellule == CELL_AI_2) {
    return (Couleur){0, 230, 100, 160}; //(vert transparente)
  }

  return (Couleur){220, 60, 255, 160}; //(violet transparente)
}

// true si toutes les images sont chargées et false s'il ya une erreur
bool initialiser_textures_motos(SDL_Renderer *renderer) {
  for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
    SDL_Surface *surface =
        IMG_Load(fichiers_motos[i]); // on charge la moto sur la surface

    if (surface == NULL) {
      fprintf(stderr, "erreur chargement image %s : %s\n", fichiers_motos[i],
              IMG_GetError());
      return false;
    }

    /*
     * Rend le blanc transparent.
     * Utile si l'image a encore un fond blanc.
     */
    Uint32 blanc = SDL_MapRGB(
        surface->format, 255, 255,
        255); // crée la couleur blanche dans le format de l'image.Unit32 est
              // utilisée dans SDL pour representer une couleur
    SDL_SetColorKey(surface, SDL_TRUE,
                    blanc); // Dans l’image surface, tous les pixels qui ont la
                            // couleur blanc deviennent transparents.

    textures_motos[i] = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (textures_motos[i] == NULL) {
      fprintf(stderr, "erreur creation texture %s : %s\n", fichiers_motos[i],
              SDL_GetError());
      return false;
    }

    SDL_SetTextureBlendMode(textures_motos[i], SDL_BLENDMODE_BLEND);
  }

  return true;
}

void liberer_textures_motos(void) {
  for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
    if (textures_motos[i] != NULL) {
      SDL_DestroyTexture(textures_motos[i]);
      textures_motos[i] = NULL;
    }
  }
}

static double angle_direction(int direction) {
  if (direction == DIR_RIGHT) {
    return 0.0;
  } else if (direction == DIR_DOWN) {
    return 90.0;
  } else if (direction == DIR_LEFT) {
    return 180.0;
  }

  return -90.0;
}

static void dessiner_tete_moto(SDL_Renderer *renderer, int x, int y,
                               int direction, int cellule) {
  if (textures_motos[cellule] == NULL) // L'image n'a pas ete chargée
  {
    return;
  }

  int taille = CELL_SIZE * 5;

  SDL_Rect dest = {x * CELL_SIZE - (taille - CELL_SIZE) / 2,
                   y * CELL_SIZE - (taille - CELL_SIZE) / 2, taille, taille};

  SDL_RenderCopyEx(renderer, textures_motos[cellule], NULL, &dest,
                   angle_direction(direction), NULL, SDL_FLIP_NONE);
}
static void dessiner_fond(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 3, 8, 18, 255); // fond noir bleu trés sombre
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 6, 18, 38,
                         255); // couleur sombre un peu plus clair
  remplir_rect(renderer, 5, 5, WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10);

  SDL_SetRenderDrawColor(renderer, 0, 35, 60,
                         180); // bleu sombre semi transparent

  for (int y = 0; y < WINDOW_HEIGHT; y += 14) {
    SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
  }
}

static void dessiner_grille(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 90, 130,
                         120); // bleu cyan sombre transparent

  for (int x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE) {
    SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
  }

  for (int y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE) {
    SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
  }

  SDL_SetRenderDrawColor(renderer, 0, 220, 255, 190); // bleu clair lumineux

  for (int x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE * 5) {
    SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
  }

  for (int y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE * 5) {
    SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
  }

  SDL_SetRenderDrawColor(renderer, 180, 245, 255,
                         140); // cyan tres clair transparent

  for (int x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE * 10) {
    SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
  }

  for (int y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE * 10) {
    SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
  }
}

static void dessiner_bordure(SDL_Renderer *renderer) {
  for (int i = 0; i < 4; i++) {
    SDL_Rect rect = {i, i, WINDOW_WIDTH - 1 - 2 * i, WINDOW_HEIGHT - 1 - 2 * i};

    SDL_SetRenderDrawColor(renderer, 0, 220, 255, 220 - i * 40);
    SDL_RenderDrawRect(renderer, &rect);
  }

  SDL_Rect interne = {6, 6, WINDOW_WIDTH - 13, WINDOW_HEIGHT - 13};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);
  SDL_RenderDrawRect(renderer, &interne);

  int l = CELL_SIZE * 9;

  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

  SDL_RenderDrawLine(renderer, 0, 0, l, 0);
  SDL_RenderDrawLine(renderer, 0, 0, 0, l);

  SDL_RenderDrawLine(renderer, WINDOW_WIDTH - 1, 0, WINDOW_WIDTH - l, 0);
  SDL_RenderDrawLine(renderer, WINDOW_WIDTH - 1, 0, WINDOW_WIDTH - 1, l);

  SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT - 1, l, WINDOW_HEIGHT - 1);
  SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT - 1, 0, WINDOW_HEIGHT - l);

  SDL_RenderDrawLine(renderer, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1,
                     WINDOW_WIDTH - l, WINDOW_HEIGHT - 1);

  SDL_RenderDrawLine(renderer, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1,
                     WINDOW_WIDTH - 1, WINDOW_HEIGHT - l);
}

static void dessiner_cellule(SDL_Renderer *renderer, int x, int y,
                             int cellule) {
  int px = x * CELL_SIZE;
  int py = y * CELL_SIZE;

  Couleur principale = couleur_principale(cellule);
  Couleur lueur = couleur_lueur(cellule);

  // Calcul de l'onde temporelle et spatiale pour l'effet "Bling-Bling"
  Uint32 ticks = SDL_GetTicks();
  float vague = sinf((x + y) * 0.4f - ticks * 0.008f);
  float intensite = (vague + 1.0f) / 2.0f; // Oscille entre 0.0 et 1.0

  // 1. Lueur externe pulsante en opacité et en taille
  lueur.a = (Uint8)(70 + intensite * 110); // Alpha varie entre 70 et 180
  set_couleur(renderer, lueur);

  int taille_lueur = (vague > 0.0f) ? 4 : 2; // La lueur s'élargit par vagues
  remplir_rect_limite(renderer, px - taille_lueur / 2, py - taille_lueur / 2,
                      CELL_SIZE + taille_lueur, CELL_SIZE + taille_lueur);

  // 2. Trace principale stable
  set_couleur(renderer, principale);
  remplir_rect_limite(renderer, px, py, CELL_SIZE, CELL_SIZE);

  // 3. Noyau d'énergie central ultra-lumineux et scintillant
  Uint8 alpha_reflet = (Uint8)(100 + intensite * 155);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha_reflet);
  remplir_rect_limite(renderer, px + CELL_SIZE / 4, py + CELL_SIZE / 4,
                      CELL_SIZE / 2, CELL_SIZE / 2);
}

/*
 * Dessine le mot TRON avec des rectangles.
 * On évite SDL_ttf et les images.
 */
static void dessiner_mot_tron(SDL_Renderer *renderer, int x, int y, int s) {
  /* Lettre T */
  remplir_rect(renderer, x, y, 5 * s, s);
  remplir_rect(renderer, x + 2 * s, y, s, 7 * s);

  x += 7 * s;

  /* Lettre R */
  remplir_rect(renderer, x, y, s, 7 * s);
  remplir_rect(renderer, x, y, 5 * s, s);
  remplir_rect(renderer, x, y + 3 * s, 5 * s, s);
  remplir_rect(renderer, x + 4 * s, y + s, s, 2 * s);
  remplir_rect(renderer, x + 2 * s, y + 4 * s, s, s);
  remplir_rect(renderer, x + 3 * s, y + 5 * s, s, s);
  remplir_rect(renderer, x + 4 * s, y + 6 * s, s, s);

  x += 7 * s;

  /* Lettre O */
  remplir_rect(renderer, x, y, 5 * s, s);
  remplir_rect(renderer, x, y + 6 * s, 5 * s, s);
  remplir_rect(renderer, x, y, s, 7 * s);
  remplir_rect(renderer, x + 4 * s, y, s, 7 * s);

  x += 7 * s;

  /* Lettre N */
  remplir_rect(renderer, x, y, s, 7 * s);
  remplir_rect(renderer, x + 4 * s, y, s, 7 * s);
  remplir_rect(renderer, x + s, y + s, s, s);
  remplir_rect(renderer, x + 2 * s, y + 2 * s, s, s);
  remplir_rect(renderer, x + 3 * s, y + 3 * s, s, s);
  remplir_rect(renderer, x + 3 * s, y + 4 * s, s, s);
  remplir_rect(renderer, x + 3 * s, y + 5 * s, s, s);
}

/*
 * Dessine un petit bouton START sans utiliser de police.
 */
static void dessiner_bouton_start(SDL_Renderer *renderer, int x, int y) {
  SDL_Rect bouton = {x, y, 220, 45};

  SDL_SetRenderDrawColor(renderer, 0, 60, 90, 220);
  SDL_RenderFillRect(renderer, &bouton);

  SDL_SetRenderDrawColor(renderer, 0, 240, 255, 255);
  SDL_RenderDrawRect(renderer, &bouton);

  SDL_Rect interne = {x + 4, y + 4, 212, 37};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 120);
  SDL_RenderDrawRect(renderer, &interne);

  /*
   * Petit symbole play.
   */
  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

  SDL_RenderDrawLine(renderer, x + 80, y + 14, x + 80, y + 31);
  SDL_RenderDrawLine(renderer, x + 80, y + 14, x + 100, y + 22);
  SDL_RenderDrawLine(renderer, x + 80, y + 31, x + 100, y + 22);

  /*
   * Ligne décorative.
   */
  SDL_RenderDrawLine(renderer, x + 120, y + 22, x + 185, y + 22);
  SDL_RenderDrawLine(renderer, x + 185, y + 22, x + 175, y + 14);
  SDL_RenderDrawLine(renderer, x + 185, y + 22, x + 175, y + 30);
}

/*
 * Écran titre avant le début du jeu.
 */
bool afficher_ecran_titre(SDL_Window *window, SDL_Renderer *renderer) {
  SDL_Event event;
  bool attendre = true;

  SDL_SetWindowTitle(window, "SmaTron - Appuie sur Entree ou Espace");
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  while (attendre) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        return false;
      }

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          return false;
        }

        if (event.key.keysym.sym == SDLK_RETURN ||
            event.key.keysym.sym == SDLK_SPACE) {
          attendre = false;
        }
      }
    }

    /*
     * Fond et grille.
     */
    dessiner_fond(renderer);
    dessiner_grille(renderer);

    /*
     * Panneau central.
     */
    SDL_Rect panneau = {WINDOW_WIDTH / 2 - 260, WINDOW_HEIGHT / 2 - 150, 520,
                        300};

    for (int i = 0; i < 6; i++) {
      SDL_Rect glow = {panneau.x - i * 4, panneau.y - i * 4, panneau.w + i * 8,
                       panneau.h + i * 8};

      SDL_SetRenderDrawColor(renderer, 0, 220, 255, 150 - i * 20);
      SDL_RenderDrawRect(renderer, &glow);
    }

    SDL_SetRenderDrawColor(renderer, 0, 8, 20, 230);
    SDL_RenderFillRect(renderer, &panneau);

    SDL_SetRenderDrawColor(renderer, 0, 240, 255, 255);
    SDL_RenderDrawRect(renderer, &panneau);

    SDL_Rect interne = {panneau.x + 8, panneau.y + 8, panneau.w - 16,
                        panneau.h - 16};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 120);
    SDL_RenderDrawRect(renderer, &interne);

    /*
     * Lueur derrière le logo.
     */
    SDL_SetRenderDrawColor(renderer, 0, 150, 255, 45);
    remplir_rect(renderer, WINDOW_WIDTH / 2 - 190, WINDOW_HEIGHT / 2 - 105, 380,
                 120);

    /*
     * Logo TRON.
     */
    SDL_SetRenderDrawColor(renderer, 0, 245, 255, 255);
    dessiner_mot_tron(renderer, WINDOW_WIDTH / 2 - 170, WINDOW_HEIGHT / 2 - 90,
                      12);

    /*
     * Bouton start.
     */
    dessiner_bouton_start(renderer, WINDOW_WIDTH / 2 - 110,
                          WINDOW_HEIGHT / 2 + 70);

    dessiner_bordure(renderer);

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }

  SDL_SetWindowTitle(window, "SmaTron");
  return true;
}

void dessiner_arene(SDL_Renderer *renderer, int grille[WIDTH][HEIGHT],
                    int pos_motos[MAX_MOTOS + 1][2],
                    int dir_motos[MAX_MOTOS + 1],
                    bool etats_vie[MAX_MOTOS + 1]) {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  dessiner_fond(renderer);
  dessiner_grille(renderer);

  /*
   * On dessine d'abord les traces.
   */
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      if (grille[x][y] != CELL_EMPTY) {
        dessiner_cellule(renderer, x, y, grille[x][y]);
      }
    }
  }

  /*
   * Ensuite on dessine les vraies images des motos sur la tete.
   */
  for (int id = CELL_PLAYER; id <= CELL_AI_3; id++) {
    if (etats_vie[id]) {
      dessiner_tete_moto(renderer, pos_motos[id][0], pos_motos[id][1],
                         dir_motos[id], id);
    }
  }

  dessiner_bordure(renderer);

  SDL_RenderPresent(renderer);
}
void capturer_evenements(SDL_Event *event, int *direction_joueur,
                         bool *running) {
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
  } else if (event->key.keysym.sym == SDLK_RIGHT &&
             *direction_joueur != DIR_LEFT) {
    *direction_joueur = DIR_RIGHT;
  } else if (event->key.keysym.sym == SDLK_DOWN &&
             *direction_joueur != DIR_UP) {
    *direction_joueur = DIR_DOWN;
  } else if (event->key.keysym.sym == SDLK_LEFT &&
             *direction_joueur != DIR_RIGHT) {
    *direction_joueur = DIR_LEFT;
  }
}

static void dessiner_mot_win(SDL_Renderer *renderer, int x, int y) {
  /* W */
  remplir_rect(renderer, x, y, 10, 70);
  remplir_rect(renderer, x + 40, y, 10, 70);
  remplir_rect(renderer, x + 10, y + 50, 10, 20);
  remplir_rect(renderer, x + 20, y + 35, 10, 25);
  remplir_rect(renderer, x + 30, y + 50, 10, 20);

  x += 80;

  /* I */
  remplir_rect(renderer, x, y, 50, 10);
  remplir_rect(renderer, x + 20, y, 10, 70);
  remplir_rect(renderer, x, y + 60, 50, 10);

  x += 80;

  /* N */
  remplir_rect(renderer, x, y, 10, 70);
  remplir_rect(renderer, x + 45, y, 10, 70);
  remplir_rect(renderer, x + 12, y + 10, 12, 15);
  remplir_rect(renderer, x + 24, y + 25, 12, 15);
  remplir_rect(renderer, x + 35, y + 40, 12, 15);
}

static void dessiner_mot_lose(SDL_Renderer *renderer, int x, int y) {
  /* L */
  remplir_rect(renderer, x, y, 10, 70);
  remplir_rect(renderer, x, y + 60, 55, 10);

  x += 75;

  /* O */
  remplir_rect(renderer, x, y, 55, 10);
  remplir_rect(renderer, x, y + 60, 55, 10);
  remplir_rect(renderer, x, y, 10, 70);
  remplir_rect(renderer, x + 45, y, 10, 70);

  x += 80;

  /* S */
  remplir_rect(renderer, x, y, 55, 10);
  remplir_rect(renderer, x, y, 10, 35);
  remplir_rect(renderer, x, y + 30, 55, 10);
  remplir_rect(renderer, x + 45, y + 30, 10, 40);
  remplir_rect(renderer, x, y + 60, 55, 10);

  x += 80;

  /* E */
  remplir_rect(renderer, x, y, 10, 70);
  remplir_rect(renderer, x, y, 55, 10);
  remplir_rect(renderer, x, y + 30, 45, 10);
  remplir_rect(renderer, x, y + 60, 55, 10);
}

void dessiner_panneau_fin(SDL_Renderer *renderer, bool joueur_gagne) {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
  remplir_rect(renderer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  SDL_Rect panneau = {WINDOW_WIDTH / 2 - 230, WINDOW_HEIGHT / 2 - 85, 460, 170};

  /*
   * Couleur différente selon le résultat.
   */
  if (joueur_gagne) {
    for (int i = 0; i < 8; i++) {
      SDL_Rect glow = {panneau.x - i * 4, panneau.y - i * 4, panneau.w + i * 8,
                       panneau.h + i * 8};

      SDL_SetRenderDrawColor(renderer, 0, 220, 255, 160 - i * 18);
      SDL_RenderDrawRect(renderer, &glow);
    }

    SDL_SetRenderDrawColor(renderer, 0, 15, 30, 250);
  } else {
    for (int i = 0; i < 8; i++) {
      SDL_Rect glow = {panneau.x - i * 4, panneau.y - i * 4, panneau.w + i * 8,
                       panneau.h + i * 8};

      SDL_SetRenderDrawColor(renderer, 255, 0, 80, 160 - i * 18);
      SDL_RenderDrawRect(renderer, &glow);
    }

    SDL_SetRenderDrawColor(renderer, 35, 0, 15, 250);
  }

  SDL_RenderFillRect(renderer, &panneau);

  if (joueur_gagne) {
    SDL_SetRenderDrawColor(renderer, 0, 240, 255, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 40, 90, 255);
  }

  SDL_RenderDrawRect(renderer, &panneau);

  SDL_Rect interne = {panneau.x + 8, panneau.y + 8, panneau.w - 16,
                      panneau.h - 16};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 160);
  SDL_RenderDrawRect(renderer, &interne);

  /*
   * Lignes décoratives.
   */
  if (joueur_gagne) {
    SDL_SetRenderDrawColor(renderer, 0, 240, 255, 180);
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 0, 80, 180);
  }

  remplir_rect(renderer, panneau.x + 20, panneau.y + 20, panneau.w - 40, 5);
  remplir_rect(renderer, panneau.x + 20, panneau.y + panneau.h - 25,
               panneau.w - 40, 5);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  if (joueur_gagne) {
    dessiner_mot_win(renderer, panneau.x + 115, panneau.y + 50);
  } else {
    dessiner_mot_lose(renderer, panneau.x + 55, panneau.y + 50);
  }
}