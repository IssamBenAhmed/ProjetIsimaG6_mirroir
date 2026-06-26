#include "../include/agent.h"
#include "../include/config.h"
#include "../include/graphisme.h"
#include "../include/mj.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef PARALLEL
#include <threads.h> // Nécessaire pour thrd_t, thrd_create, etc.

typedef struct {
  int id_thread;
  long nb_episodes;
  unsigned int seed;
  float theta_local[15][3];
  long somme_frames;
  int max_frames;
  long victoires;
} ParamThread;

static void copier_theta(float source[15][3], float destination[15][3]) {
  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 3; j++) {
      destination[i][j] = source[i][j];
    }
  }
}

static int entrainer_thread(void *arg) {
  ParamThread *p = (ParamThread *)arg;

  int grid_local[WIDTH][HEIGHT];
  int pos_motos_local[MAX_MOTOS + 1][2];
  int dir_motos_local[MAX_MOTOS + 1];
  bool moto_alive_local[MAX_MOTOS + 1];

  EpisodeMemoire *memoires_local =
      calloc(MAX_MOTOS + 1, sizeof(EpisodeMemoire));
  if (memoires_local == NULL) {
    return 1;
  }

  p->somme_frames = 0;
  p->max_frames = 0;
  p->victoires = 0;

  for (long episode = 0; episode < p->nb_episodes; episode++) {
    initialiser_partie_thread(grid_local, pos_motos_local, dir_motos_local,
                              moto_alive_local, &p->seed);

    for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
      memoires_local[i].taille = 0;
    }

    bool partie_continue = true;
    int frames = 0;
    int dernier_vivant = 0;

    while (partie_continue) {
      bool vie_avant[MAX_MOTOS + 1];
      for (int i = 0; i <= MAX_MOTOS; i++) {
        vie_avant[i] = moto_alive_local[i];
      }

      mettre_a_jour_monde_entrainement_thread(
          grid_local, pos_motos_local, dir_motos_local, moto_alive_local,
          memoires_local, p->theta_local, &p->seed);
      frames++;

      int nb_vivants = 0;
      dernier_vivant = 0;

      for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
        if (vie_avant[i] && !moto_alive_local[i]) {
          if (memoires_local[i].taille > 0) {
            memoires_local[i].frames[memoires_local[i].taille - 1].recompense =
                -100.0f;
            maj_theta_thread(&memoires_local[i], ALPHA, 0.99f, p->theta_local);
            memoires_local[i].taille = 0;
          }
        }
        if (moto_alive_local[i]) {
          nb_vivants++;
          dernier_vivant = i;
        }
      }

      if (nb_vivants <= 1 || frames >= 2000) {
        partie_continue = false;
      }
    }

    p->somme_frames += frames;
    if (frames > p->max_frames) {
      p->max_frames = frames;
    }

    if (dernier_vivant >= CELL_PLAYER && dernier_vivant <= CELL_AI_3) {
      p->victoires++;
    }

    for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
      if (moto_alive_local[i] && memoires_local[i].taille > 0) {
        maj_theta_thread(&memoires_local[i], ALPHA, 0.99f, p->theta_local);
      }
    }

    if ((episode + 1) % 1000 == 0 || episode + 1 == p->nb_episodes) {
      printf("[thread %d] episode %ld / %ld termine\n", p->id_thread,
             episode + 1, p->nb_episodes);
      fflush(stdout);
    }
  }

  free(memoires_local);
  return 0;
}
#endif

int main(int argc, char *argv[]) {
  int grid[WIDTH][HEIGHT] = {0};
  int pos_motos[MAX_MOTOS + 1][2] = {0};
  int dir_motos[MAX_MOTOS + 1] = {0};
  bool moto_alive[MAX_MOTOS + 1] = {false};
  bool running = true;

  if (argc < 2) {
    fprintf(stderr, "usage: %s <mode>\n", argv[0]);
#ifdef NO_REINFORCE
    fprintf(stderr, "modes: %s\n", JOUEUR);
#else
    fprintf(stderr, "modes: %s, %s, trainp\n", JOUEUR, ENTRAINEMENT);
#endif
    return EXIT_FAILURE;
  }

#ifndef NO_REINFORCE
  EpisodeMemoire memoires[MAX_MOTOS + 1] = {0};
#endif

  /* ===================================================================== *
   * mode joueur (avec sdl et ecrans)                                      *
   * ===================================================================== */
  if (strcmp(argv[1], JOUEUR) == 0) {
#ifndef NO_REINFORCE
    charger_theta(filename_theta);
#endif

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      fprintf(stderr, "erreur sdl init : %s\n", SDL_GetError());
      return EXIT_FAILURE;
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
      fprintf(stderr, "erreur IMG_Init : %s\n", IMG_GetError());
      SDL_Quit();
      return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow(
        "smatron - arene sma", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
      fprintf(stderr, "erreur creation fenetre : %s\n", SDL_GetError());
      IMG_Quit();
      SDL_Quit();
      return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
      fprintf(stderr, "erreur creation renderer : %s\n", SDL_GetError());
      SDL_DestroyWindow(window);
      IMG_Quit();
      SDL_Quit();
      return EXIT_FAILURE;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (!initialiser_textures_motos(renderer)) {
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      IMG_Quit();
      SDL_Quit();
      return EXIT_FAILURE;
    }

    if (!afficher_ecran_titre(window, renderer)) {
      liberer_textures_motos();
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      IMG_Quit();
      SDL_Quit();
      return EXIT_SUCCESS;
    }

    srand((unsigned int)SDL_GetTicks());

    /* placement manuel du joueur humain a gauche, face a la droite */
    pos_motos[CELL_PLAYER][0] = 10;
    pos_motos[CELL_PLAYER][1] = HEIGHT / 2;
    dir_motos[CELL_PLAYER] = DIR_RIGHT;
    moto_alive[CELL_PLAYER] = true;
    grid[pos_motos[CELL_PLAYER][0]][pos_motos[CELL_PLAYER][1]] = CELL_PLAYER;

    /* placement des 3 ia aux differents coins de l'arene */
    pos_motos[CELL_AI_1][0] = WIDTH - 11;
    pos_motos[CELL_AI_1][1] = 10;
    dir_motos[CELL_AI_1] = DIR_LEFT;
    moto_alive[CELL_AI_1] = true;
    grid[pos_motos[CELL_AI_1][0]][pos_motos[CELL_AI_1][1]] = CELL_AI_1;

    pos_motos[CELL_AI_2][0] = 10;
    pos_motos[CELL_AI_2][1] = HEIGHT - 11;
    dir_motos[CELL_AI_2] = DIR_UP;
    moto_alive[CELL_AI_2] = true;
    grid[pos_motos[CELL_AI_2][0]][pos_motos[CELL_AI_2][1]] = CELL_AI_2;

    pos_motos[CELL_AI_3][0] = WIDTH - 11;
    pos_motos[CELL_AI_3][1] = HEIGHT - 11;
    dir_motos[CELL_AI_3] = DIR_UP;
    moto_alive[CELL_AI_3] = true;
    grid[pos_motos[CELL_AI_3][0]][pos_motos[CELL_AI_3][1]] = CELL_AI_3;

    Uint32 last_tick = SDL_GetTicks();

    while (running) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        capturer_evenements(&event, &dir_motos[CELL_PLAYER], &running);
      }

      Uint32 now = SDL_GetTicks();
      if (now - last_tick >= 2 * FRAME_DELAY_MS) {
        last_tick = now;

#ifdef NO_REINFORCE
        mettre_a_jour_monde(grid, pos_motos, dir_motos, moto_alive);
#else
        mettre_a_jour_monde(grid, pos_motos, dir_motos, moto_alive, memoires);
#endif

        int motos_vivantes = 0;
        for (int id = CELL_PLAYER; id <= CELL_AI_3; id++) {
          if (moto_alive[id]) {
            motos_vivantes++;
          }
        }

        if (motos_vivantes <= 1) {
          bool joueur_gagne = moto_alive[CELL_PLAYER];

          dessiner_arene(renderer, grid, pos_motos, dir_motos, moto_alive);
          dessiner_panneau_fin(renderer, joueur_gagne);
          SDL_RenderPresent(renderer);

          SDL_Delay(2500);
          running = false;
          continue;
        }
      }

      dessiner_arene(renderer, grid, pos_motos, dir_motos, moto_alive);
      SDL_Delay(16);
    }

    liberer_textures_motos();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
  }

#ifndef NO_REINFORCE
  /* ===================================================================== *
   * mode entrainement séquentiel (sans sdl, calcul pur, vitesse maximale) *
   * ===================================================================== */
  else if (strcmp(argv[1], ENTRAINEMENT) == 0) {
    charger_theta(filename_theta);
    printf("demarrage de l'entrainement intensif sans sdl...\n");

    bool check_existe = false;
    FILE *f_test = fopen("data/logs.csv", "r");
    if (f_test) {
      check_existe = true;
      fclose(f_test);
    }

    FILE *f_log = fopen("data/logs.csv", "a");
    if (f_log && !check_existe) {
      fprintf(f_log, "episode,temps_sec,survie_moyenne,survie_max,taux_"
                     "victoire,g0_moyen,pct_forward\n");
    }

    long somme_frames = 0;
    int max_frames = 0;
    float somme_g0 = 0.0f;
    int victoires = 0;
    long total_actions = 0;
    long total_forward = 0;

    time_t temps_debut_batch = time(NULL);
    int dernier_vainqueur = -1;

    long episode = 1;
    long episodes_totaux = 10000;

    if (argc >= 3) {
      episodes_totaux = strtol(argv[2], NULL, 10);
    }
    if (argc >= 4) {
      episode = strtol(argv[3], NULL, 10);
    }

    while (episode <= episodes_totaux && running) {
      initialiser_partie(grid, pos_motos, dir_motos, moto_alive);

      for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
        memoires[i].taille = 0;
      }

      bool episode_en_cours = true;
      int frames_ecoulees = 0;

      while (episode_en_cours) {
        bool vie_avant[MAX_MOTOS + 1];
        for (int i = 0; i <= MAX_MOTOS; i++)
          vie_avant[i] = moto_alive[i];

        mettre_a_jour_monde_entrainement(grid, pos_motos, dir_motos, moto_alive,
                                         memoires);
        frames_ecoulees++;

        int motos_vivantes = 0;
        dernier_vainqueur = 0;

        for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
          if (vie_avant[i] && !moto_alive[i]) {
            if (memoires[i].taille > 0) {
              memoires[i].frames[memoires[i].taille - 1].recompense = -100.0f;
              maj_theta(&memoires[i], ALPHA, 0.99f);
              memoires[i].taille = 0;
            }
          }
          if (moto_alive[i]) {
            motos_vivantes++;
            dernier_vainqueur = i;
          }
        }

        if (motos_vivantes <= 1 || frames_ecoulees >= 2000) {
          episode_en_cours = false;
        }
      }

      somme_frames += frames_ecoulees;
      if (frames_ecoulees > max_frames) {
        max_frames = frames_ecoulees;
      }

      if (dernier_vainqueur >= CELL_PLAYER && dernier_vainqueur <= CELL_AI_3) {
        victoires++;
      }

      for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
        if (moto_alive[i] && memoires[i].taille > 0) {
          float g0 = maj_theta(&memoires[i], ALPHA, 0.99f);
          if (i == dernier_vainqueur) {
            somme_g0 += g0;
          }
        }

        for (int t = 0; t < memoires[i].taille; t++) {
          total_actions++;
          if (memoires[i].frames[t].action == ACTION_FORWARD) {
            total_forward++;
          }
        }
      }

      if (episode % 100 == 0) {
        time_t temps_actuel = time(NULL);
        double temps_batch = difftime(temps_actuel, temps_debut_batch);

        float moyenne_frames = (float)somme_frames / 100.0f;
        float moyenne_g0 = somme_g0 / 100.0f;
        float taux_victoire = (float)victoires;
        float pct_forward =
            (total_actions > 0)
                ? ((float)total_forward / total_actions) * 100.0f
                : 33.3f;

        printf("[ep %ld] temps: %.0fs | survie moy: %.1f (max:%d) | win: "
               "%.0f%% | fwd: %.1f%%\n",
               episode, temps_batch, moyenne_frames, max_frames, taux_victoire,
               pct_forward);

        if (f_log) {
          fprintf(f_log, "%ld,%.0f,%.1f,%d,%.1f,%.2f,%.1f\n", episode,
                  temps_batch, moyenne_frames, max_frames, taux_victoire,
                  moyenne_g0, pct_forward);
          fflush(f_log);
        }

        somme_frames = 0;
        max_frames = 0;
        somme_g0 = 0.0f;
        victoires = 0;
        total_actions = 0;
        total_forward = 0;
        temps_debut_batch = time(NULL);
      }

      if (episode % 5000 == 0) {
        sauvegarder_theta(filename_theta);
      }

      episode++;
    }

    if (f_log)
      fclose(f_log);
    printf("entrainement termine avec succes.\n");
    sauvegarder_theta(filename_theta);

    return EXIT_SUCCESS;
  }

#ifdef PARALLEL
  /* ===================================================================== *
   * mode entrainement parallèle (multi-threadé thrd_create / thrd_join)   *
   * ===================================================================== */
  else if (strcmp(argv[1], "trainp") == 0) {
    charger_theta(filename_theta);

    long episodes_totaux = 10000;
    int nb_threads = 4;

    if (argc >= 3) {
      episodes_totaux = strtol(argv[2], NULL, 10);
    }
    if (argc >= 4) {
      nb_threads = atoi(argv[3]);
    }

    if (nb_threads <= 0) {
      nb_threads = 1;
    }
    if (nb_threads > 8) {
      nb_threads = 8;
    }

    printf("entrainement parallele : %ld episodes avec %d threads\n",
           episodes_totaux, nb_threads);

    thrd_t threads[8];
    ParamThread params[8];

    long episodes_par_thread = episodes_totaux / nb_threads;
    long reste = episodes_totaux % nb_threads;

    for (int t = 0; t < nb_threads; t++) {
      long nb_ep = episodes_par_thread;
      if (t < reste) {
        nb_ep++;
      }
      params[t].id_thread = t;
      params[t].nb_episodes = nb_ep;
      params[t].seed = (unsigned int)time(NULL) + (unsigned int)((t + 1) * 100);

      copier_theta(theta, params[t].theta_local);

      thrd_create(&threads[t], entrainer_thread, &params[t]);
    }

    for (int t = 0; t < nb_threads; t++) {
      thrd_join(threads[t], NULL);
    }

    for (int i = 0; i < 15; i++) {
      for (int j = 0; j < 3; j++) {
        float somme = 0.0f;
        for (int t = 0; t < nb_threads; t++) {
          somme += params[t].theta_local[i][j];
        }
        theta[i][j] = somme / (float)nb_threads;
      }
    }

    long somme_frames = 0;
    int max_frames = 0;
    long victoires = 0;

    for (int t = 0; t < nb_threads; t++) {
      somme_frames += params[t].somme_frames;
      victoires += params[t].victoires;
      if (params[t].max_frames > max_frames) {
        max_frames = params[t].max_frames;
      }
    }

    printf("entrainement parallele termine\n");
    printf("survie moyenne : %.2f frames\n",
           (float)somme_frames / (float)episodes_totaux);
    printf("survie max     : %d frames\n", max_frames);
    printf("victoires      : %.2f %%\n",
           ((float)victoires / (float)episodes_totaux) * 100.0f);

    sauvegarder_theta(filename_theta);
    return EXIT_SUCCESS;
  }
#endif /* PARALLEL */
#endif /* NO_REINFORCE */

  else {
    fprintf(stderr, "mode inconnu: %s\n", argv[1]);
#ifdef NO_REINFORCE
    fprintf(stderr, "modes: %s\n", JOUEUR);
#else
    fprintf(stderr, "modes: %s, %s, trainp\n", JOUEUR, ENTRAINEMENT);
#endif
    return EXIT_FAILURE;
  }
}
