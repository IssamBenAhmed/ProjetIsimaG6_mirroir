#ifdef PARALLEL

#include "../include/parallel.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* Helper pour obtenir le vecteur de direction local au thread */
static void obtenir_vecteur_thread(int direction, int *dx, int *dy) {
  if (direction == DIR_UP) {
    *dx = 0;
    *dy = -1;
  } else if (direction == DIR_RIGHT) {
    *dx = 1;
    *dy = 0;
  } else if (direction == DIR_DOWN) {
    *dx = 0;
    *dy = 1;
  } else if (direction == DIR_LEFT) {
    *dx = -1;
    *dy = 0;
  } else {
    *dx = 0;
    *dy = 0;
  }
}

float random_float_thread(unsigned int *seed) {
  *seed = (*seed) * 1103515245u + 12345u;
  return (float)((*seed >> 8) % 1000000) / 1000000.0f;
}

int random_int_thread(unsigned int *seed, int max) {
  if (max <= 0)
    return 0;
  *seed = (*seed * 17u) + 3u;
  return (int)(*seed % (unsigned int)max);
}

int choisir_action_thread(Perception p, FrameMemoire *mem_frame,
                          float theta_local[15][3], unsigned int *seed) {
  float phi[15];
  generer_phi(p, phi);

  float z[3] = {0};
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 15; i++) {
      z[j] += phi[i] * theta_local[i][j];
    }
  }

  float max_z = z[0];
  for (int j = 1; j < 3; j++) {
    if (z[j] > max_z)
      max_z = z[j];
  }

  float exp_z[3], somme_exp = 0.0f;
  for (int j = 0; j < 3; j++) {
    exp_z[j] = expf(z[j] - max_z);
    somme_exp += exp_z[j];
  }

  float pi[3];
  for (int j = 0; j < 3; j++) {
    pi[j] = exp_z[j] / somme_exp;
    if (mem_frame)
      mem_frame->probabilites[j] = pi[j];
  }

  float tirage = random_float_thread(seed);
  float proba_cumul = 0.0f;
  int action_choisie = ACTION_FORWARD;

  for (int j = 0; j < 3; j++) {
    proba_cumul += pi[j];
    if (tirage <= proba_cumul) {
      action_choisie = j;
      break;
    }
  }

  if (mem_frame) {
    for (int i = 0; i < 15; i++) {
      mem_frame->phi[i] = phi[i];
    }
    mem_frame->action = action_choisie;
    mem_frame->recompense = 1.0f;
  }
  return action_choisie;
}

float maj_theta_thread(EpisodeMemoire *ep, float alpha, float gamma,
                       float theta_local[15][3]) {
  float G = 0.0f;
  for (int t = ep->taille - 1; t >= 0; t--) {
    G = ep->frames[t].recompense + gamma * G;
    for (int i = 0; i < 15; i++) {
      for (int j = 0; j < 3; j++) {
        float y_j = (ep->frames[t].action == j) ? 1.0f : 0.0f;
        float erreur = y_j - ep->frames[t].probabilites[j];
        theta_local[i][j] += alpha * ep->frames[t].phi[i] * erreur * G;
      }
    }
  }
  return G;
}

void initialiser_partie_thread(int grille[WIDTH][HEIGHT],
                               int pos_motos[MAX_MOTOS + 1][2],
                               int dir_motos[MAX_MOTOS + 1],
                               bool etats_vie[MAX_MOTOS + 1],
                               unsigned int *seed) {
  initialiser_grille(grille);

  for (int i = 0; i <= MAX_MOTOS; i++) {
    etats_vie[i] = false;
  }

  int player_list[4] = {CELL_PLAYER, CELL_AI_1, CELL_AI_2, CELL_AI_3};
  int k = 0;

  while (k < 4) {
    int x = random_int_thread(seed, WIDTH);
    int y = random_int_thread(seed, HEIGHT);

    if (grille[x][y] != CELL_EMPTY) {
      continue;
    }

    grille[x][y] = player_list[k];
    pos_motos[k + 1][0] = x;
    pos_motos[k + 1][1] = y;
    etats_vie[k + 1] = true;
    dir_motos[k + 1] = DIR_UP;
    k++;
  }
}

void mettre_a_jour_monde_entrainement_thread(
    int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2],
    int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1],
    EpisodeMemoire memoires[MAX_MOTOS + 1], float theta_local[15][3],
    unsigned int *seed) {
  for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
    if (!etats_vie[i])
      continue;

    int x = pos_motos[i][0];
    int y = pos_motos[i][1];

    Perception perception;
    calculer_perception(grille, x, y, dir_motos[i], i, &perception);

    int action = choisir_action_thread(
        perception, &memoires[i].frames[memoires[i].taille], theta_local, seed);

    if (action == ACTION_LEFT) {
      dir_motos[i] = (dir_motos[i] + 3) % 4;
    } else if (action == ACTION_RIGHT) {
      dir_motos[i] = (dir_motos[i] + 1) % 4;
    }

    memoires[i].taille++;

    int dx = 0, dy = 0;
    obtenir_vecteur_thread(dir_motos[i], &dx, &dy);

    int nx = x + dx;
    int ny = y + dy;

    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT &&
        grille[nx][ny] == CELL_EMPTY) {
      grille[nx][ny] = i;
      pos_motos[i][0] = nx;
      pos_motos[i][1] = ny;

      modifier_recompense(1.0f, &memoires[i].frames[memoires[i].taille - 1]);
    } else {
      etats_vie[i] = false;

      modifier_recompense(-100.0f, &memoires[i].frames[memoires[i].taille - 1]);

      int cause = cause_mort(grille, nx, ny);
      if (cause != CELL_EMPTY) {
        modifier_recompense(50.0f, &memoires[i].frames[memoires[i].taille - 1]);
      }
      nettoyer_trainee(grille, i);
    }
  }
}

int entrainer_thread(void *arg) {
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

void copier_theta(float source[15][3], float destination[15][3]) {
  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 3; j++) {
      destination[i][j] = source[i][j];
    }
  }
}

#endif
