#include "../include/mj.h"
#include "../include/agent.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* Helper commun pour obtenir le vecteur de direction */
static void obtenir_vecteur(int direction, int *dx, int *dy) {
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

/* --------------------------------------------------------------------- *
 * FONCTIONS COMMUNES                                                    *
 * --------------------------------------------------------------------- */
void initialiser_grille(int grille[WIDTH][HEIGHT]) {
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      grille[i][j] = CELL_EMPTY;
    }
  }
}

void initialiser_partie(int grille[WIDTH][HEIGHT],
                        int pos_motos[MAX_MOTOS + 1][2],
                        int dir_motos[MAX_MOTOS + 1],
                        bool etats_vie[MAX_MOTOS + 1]) {
  initialiser_grille(grille);

  int player_list[4] = {CELL_PLAYER, CELL_AI_1, CELL_AI_2, CELL_AI_3};
  int k = 0;

  while (k < 4) {
    int x = rand() % WIDTH;
    int y = rand() % HEIGHT;
    if (grille[x][y] != CELL_EMPTY) {
      continue;
    } else {
      grille[x][y] = player_list[k];
      pos_motos[k + 1][0] = x;
      pos_motos[k + 1][1] = y;
      etats_vie[k + 1] = true;
      dir_motos[k + 1] = DIR_UP;
      k++;
    }
  }
}

int compter_moto_vivants(bool etats_vie[MAX_MOTOS + 1]) {
  int res = 0;
  for (int i = 0; i < (MAX_MOTOS + 1); i++) {
    if (etats_vie[i]) {
      res++;
    }
  }
  return res;
}

int trouver_gagnant(bool etats_vie[MAX_MOTOS + 1]) {
  int moto_vivant = compter_moto_vivants(etats_vie);
  if (moto_vivant == 2) {
    for (int i = 1; i < (MAX_MOTOS + 1); i++) {
      if (etats_vie[i] == true) {
        return i;
      }
    }
  }
  return -1;
}

int cause_mort(int grille[WIDTH][HEIGHT], int x, int y) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
    return -1;
  }
  return grille[x][y];
}

void nettoyer_trainee(int grille[WIDTH][HEIGHT], int id_moto) {
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      if (grille[i][j] == id_moto) {
        grille[i][j] = CELL_EMPTY;
      }
    }
  }
}

#ifdef NO_REINFORCE

/* ===================================================================== *
 * IMPLÉMENTATION IA HEURISTIQUE (SMA_NO_REINFORCE)                      *
 * ===================================================================== */

void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction,
                         float perception[PERCEPTION_SIZE]) {
  int dx = 0, dy = 0;
  obtenir_vecteur(direction, &dx, &dy);

  int ldx = dy;
  int ldy = -dx;
  int rdx = -dy;
  int rdy = dx;

  int distances[3] = {0, 0, 0};
  int rayons_dx[3] = {dx, ldx, rdx};
  int rayons_dy[3] = {dy, ldy, rdy};

  for (int r = 0; r < 3; r++) {
    for (int i = 1; i < WIDTH; i++) {
      int nx = x + i * rayons_dx[r];
      int ny = y + i * rayons_dy[r];

      if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT)
        break;
      if (grille[nx][ny] != CELL_EMPTY)
        break;

      distances[r]++;
    }
  }

  perception[0] = (float)distances[0];
  perception[1] = (float)distances[1];
  perception[2] = (float)distances[2];
}

void mettre_a_jour_monde(int grille[WIDTH][HEIGHT],
                         int pos_motos[MAX_MOTOS + 1][2],
                         int dir_motos[MAX_MOTOS + 1],
                         bool etats_vie[MAX_MOTOS + 1]) {
  for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
    if (!etats_vie[i])
      continue;

    int x = pos_motos[i][0];
    int y = pos_motos[i][1];

    if (i >= CELL_AI_1) {
      float perception[PERCEPTION_SIZE] = {0};
      calculer_perception(grille, x, y, dir_motos[i], perception);

      int action = choisir_action(perception);

      if (action == ACTION_LEFT) {
        dir_motos[i] = (dir_motos[i] + 3) % 4;
      } else if (action == ACTION_RIGHT) {
        dir_motos[i] = (dir_motos[i] + 1) % 4;
      }
    }

    int dx = 0, dy = 0;
    obtenir_vecteur(dir_motos[i], &dx, &dy);

    int nx = x + dx;
    int ny = y + dy;

    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT &&
        grille[nx][ny] == CELL_EMPTY) {
      grille[nx][ny] = i;
      pos_motos[i][0] = nx;
      pos_motos[i][1] = ny;
    } else {
      etats_vie[i] = false;
      nettoyer_trainee(grille, i);
    }
  }
}

#else

/* ===================================================================== *
 * IMPLÉMENTATION REINFORCE (Séquentielle & Parallèle)                  *
 * ===================================================================== */

void modifier_recompense(float recompense, FrameMemoire *frame) {
  frame->recompense += recompense;
}

bool gagnant_ou_pas(bool etats_vie[MAX_MOTOS + 1],
                    EpisodeMemoire memoires[MAX_MOTOS + 1]) {
  int gagner_ou_pas = trouver_gagnant(etats_vie);
  if (gagner_ou_pas != -1) {
    printf("le gagnant est %d \n", gagner_ou_pas);
    modifier_recompense(
        100.0f,
        &memoires[gagner_ou_pas].frames[memoires[gagner_ou_pas].taille - 1]);
    return true;
  }
  return false;
}

static void assigner_zone_adversaire(int id_trouve, int zone, int mon_id,
                                     int *z1, int *z2, int *z3) {
  if (id_trouve < CELL_PLAYER || id_trouve > CELL_AI_3)
    return;

  int slot = 1;
  for (int k = CELL_PLAYER; k <= CELL_AI_3; k++) {
    if (k == mon_id)
      continue;
    if (k == id_trouve) {
      if (slot == 1)
        *z1 = zone;
      else if (slot == 2)
        *z2 = zone;
      else if (slot == 3)
        *z3 = zone;
      return;
    }
    slot++;
  }
}

static float calculer_densite_zone(int grille[WIDTH][HEIGHT], int x, int y,
                                   int dx1, int dy1, int dx2, int dy2) {
  int obstacles = 0;
  int total = 0;

  for (int i = 1; i <= LIMIT_VISION; i++) {
    for (int j = 1; j <= i; j++) {
      int nx = x + i * dx1 + j * dx2;
      int ny = y + i * dy1 + j * dy2;

      if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) {
        continue;
      }

      total++;
      if (grille[nx][ny] != CELL_EMPTY) {
        obstacles++;
      }
    }
  }

  if (total == 0) {
    return 1.0f;
  }
  return (float)obstacles / (float)total;
}

int existence_adversaire_dans_un_zone(int grille[WIDTH][HEIGHT], int x, int y,
                                      int dx1, int dy1, int dx2, int dy2,
                                      int mon_id) {
  for (int i = 1; i <= LIMIT_VISION; i++) {
    for (int j = 1; j <= i; j++) {
      int nx = x + i * dx1 + j * dx2;
      int ny = y + i * dy1 + j * dy2;
      if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) {
        continue;
      }
      int cell = grille[nx][ny];
      if (cell >= CELL_PLAYER && cell <= CELL_AI_3 && cell != mon_id) {
        return cell;
      }
    }
  }
  return -1;
}

void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction,
                         int mon_id, Perception *perception) {
  int dx = 0, dy = 0;
  obtenir_vecteur(direction, &dx, &dy);

  int ldx = dy;
  int ldy = -dx;
  int rdx = -dy;
  int rdy = dx;

  float distances[3] = {0.0f, 0.0f, 0.0f};
  int rayons_dx[3] = {dx, ldx, rdx};
  int rayons_dy[3] = {dy, ldy, rdy};

  for (int r = 0; r < 3; r++) {
    for (int i = 1; i <= LIMIT_VISION; i++) {
      int nx = x + i * rayons_dx[r];
      int ny = y + i * rayons_dy[r];
      if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT)
        break;
      if (grille[nx][ny] != CELL_EMPTY)
        break;
      distances[r]++;
    }
  }
  perception->distances_murs[0] = distances[0] / LIMIT_VISION;
  perception->distances_murs[1] = distances[1] / LIMIT_VISION;
  perception->distances_murs[2] = distances[2] / LIMIT_VISION;

  perception->densite_obstacles[0] =
      calculer_densite_zone(grille, x, y, dx, dy, ldx, ldy);
  perception->densite_obstacles[1] =
      calculer_densite_zone(grille, x, y, dx, dy, rdx, rdy);
  perception->densite_obstacles[2] =
      calculer_densite_zone(grille, x, y, -dx, -dy, ldx, ldy);
  perception->densite_obstacles[3] =
      calculer_densite_zone(grille, x, y, -dx, -dy, rdx, rdy);

  perception->zone_adversaire_1 = -1;
  perception->zone_adversaire_2 = -1;
  perception->zone_adversaire_3 = -1;

  int id;

  id =
      existence_adversaire_dans_un_zone(grille, x, y, dx, dy, ldx, ldy, mon_id);
  assigner_zone_adversaire(id, 0, mon_id, &perception->zone_adversaire_1,
                           &perception->zone_adversaire_2,
                           &perception->zone_adversaire_3);

  id =
      existence_adversaire_dans_un_zone(grille, x, y, dx, dy, rdx, rdy, mon_id);
  assigner_zone_adversaire(id, 1, mon_id, &perception->zone_adversaire_1,
                           &perception->zone_adversaire_2,
                           &perception->zone_adversaire_3);

  id = existence_adversaire_dans_un_zone(grille, x, y, -dx, -dy, ldx, ldy,
                                         mon_id);
  assigner_zone_adversaire(id, 2, mon_id, &perception->zone_adversaire_1,
                           &perception->zone_adversaire_2,
                           &perception->zone_adversaire_3);

  id = existence_adversaire_dans_un_zone(grille, x, y, -dx, -dy, rdx, rdy,
                                         mon_id);
  assigner_zone_adversaire(id, 3, mon_id, &perception->zone_adversaire_1,
                           &perception->zone_adversaire_2,
                           &perception->zone_adversaire_3);
}

void mettre_a_jour_monde(int grille[WIDTH][HEIGHT],
                         int pos_motos[MAX_MOTOS + 1][2],
                         int dir_motos[MAX_MOTOS + 1],
                         bool etats_vie[MAX_MOTOS + 1],
                         EpisodeMemoire memoires[MAX_MOTOS + 1]) {
  for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
    if (!etats_vie[i])
      continue;

    int x = pos_motos[i][0];
    int y = pos_motos[i][1];

    if (i >= CELL_AI_1) {
      Perception perception;
      calculer_perception(grille, x, y, dir_motos[i], i, &perception);

      FrameMemoire *frame_ptr = (memoires[i].taille < MAX_FRAMES_EPISODE)
                                    ? &memoires[i].frames[memoires[i].taille]
                                    : NULL;
      int action = choisir_action(perception, frame_ptr);

      if (action == ACTION_LEFT) {
        dir_motos[i] = (dir_motos[i] + 3) % 4;
      } else if (action == ACTION_RIGHT) {
        dir_motos[i] = (dir_motos[i] + 1) % 4;
      }

      if (memoires[i].taille < MAX_FRAMES_EPISODE) {
        memoires[i].taille++;
      }
    }

    int dx = 0, dy = 0;
    obtenir_vecteur(dir_motos[i], &dx, &dy);

    int nx = x + dx;
    int ny = y + dy;

    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT &&
        grille[nx][ny] == CELL_EMPTY) {
      grille[nx][ny] = i;
      pos_motos[i][0] = nx;
      pos_motos[i][1] = ny;
      if (i == CELL_PLAYER) {
        continue;
      }

      if (i >= CELL_AI_1 && memoires[i].taille > 0) {
        modifier_recompense(1.0f, &memoires[i].frames[memoires[i].taille - 1]);
      }
    } else {
      etats_vie[i] = false;

      if (i >= CELL_AI_1 && memoires[i].taille > 0) {
        modifier_recompense(-100.0f,
                            &memoires[i].frames[memoires[i].taille - 1]);
      }

      int cause = cause_mort(grille, nx, ny);
      if (cause == CELL_EMPTY) {
        continue;
      }
      if (cause == CELL_PLAYER) {
        nettoyer_trainee(grille, i);
        continue;
      }

      if (i >= CELL_AI_1 && memoires[i].taille > 0) {
        modifier_recompense(50.0f, &memoires[i].frames[memoires[i].taille - 1]);
      }
      nettoyer_trainee(grille, i);
    }
  }
}

void mettre_a_jour_monde_entrainement(int grille[WIDTH][HEIGHT],
                                      int pos_motos[MAX_MOTOS + 1][2],
                                      int dir_motos[MAX_MOTOS + 1],
                                      bool etats_vie[MAX_MOTOS + 1],
                                      EpisodeMemoire memoires[MAX_MOTOS + 1]) {
  for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
    if (!etats_vie[i])
      continue;

    int x = pos_motos[i][0];
    int y = pos_motos[i][1];
    Perception perception;
    calculer_perception(grille, x, y, dir_motos[i], i, &perception);

    FrameMemoire *frame_ptr = (memoires[i].taille < MAX_FRAMES_EPISODE)
                                  ? &memoires[i].frames[memoires[i].taille]
                                  : NULL;
    int action = choisir_action(perception, frame_ptr);

    if (action == ACTION_LEFT) {
      dir_motos[i] = (dir_motos[i] + 3) % 4;
    } else if (action == ACTION_RIGHT) {
      dir_motos[i] = (dir_motos[i] + 1) % 4;
    }

    if (memoires[i].taille < MAX_FRAMES_EPISODE) {
      memoires[i].taille++;
    }

    int dx = 0, dy = 0;
    obtenir_vecteur(dir_motos[i], &dx, &dy);

    int nx = x + dx;
    int ny = y + dy;

    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT &&
        grille[nx][ny] == CELL_EMPTY) {
      grille[nx][ny] = i;
      pos_motos[i][0] = nx;
      pos_motos[i][1] = ny;

      if (memoires[i].taille > 0) {
        modifier_recompense(1.0f, &memoires[i].frames[memoires[i].taille - 1]);
      }
    } else {
      etats_vie[i] = false;

      if (memoires[i].taille > 0) {
        modifier_recompense(-100.0f,
                            &memoires[i].frames[memoires[i].taille - 1]);
      }

      int cause = cause_mort(grille, nx, ny);
      if (cause == CELL_EMPTY) {
        continue;
      }

      if (memoires[i].taille > 0) {
        modifier_recompense(50.0f, &memoires[i].frames[memoires[i].taille - 1]);
      }
      nettoyer_trainee(grille, i);
    }
  }
}

#ifdef PARALLEL
/* ===================================================================== *
 * LOGIQUE MULTI-THREADING POUR L'ENTRAÎNEMENT PARALLÈLE                 *
 * ===================================================================== */
int random_int_thread(unsigned int *seed, int max) {
  if (max <= 0)
    return 0;
  *seed = (*seed * 17u) + 3u;
  return (int)(*seed % (unsigned int)max);
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
    obtenir_vecteur(dir_motos[i], &dx, &dy);

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
#endif /* PARALLEL */
#endif /* NO_REINFORCE */
