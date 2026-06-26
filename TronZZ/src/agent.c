#include "../include/agent.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float random_float(void) { return (float)rand() / (float)RAND_MAX; }

#ifdef NO_REINFORCE

/* ===================================================================== *
 * LOGIQUE DE L'AGENT HEURISTIQUE (Pas d'apprentissage)                 *
 * ===================================================================== */
int choisir_action(float perception[PERCEPTION_SIZE]) {
  float z[3];
  float poids_espace = 2.0f;

  for (int i = 0; i < 3; i++) {
    if (perception[i] <= 1.0f) {
      z[i] = -999.0f; // Crainte extrême du mur
    } else {
      float perception_normalisee = perception[i] / (float)WIDTH;
      z[i] = (perception_normalisee * poids_espace) + (random_float() * 1.5f);
    }
  }

  float exp_z[3];
  float somme_exp = 0.0f;
  for (int i = 0; i < 3; i++) {
    exp_z[i] = exp(z[i]);
    somme_exp += exp_z[i];
  }

  float pi[3];
  for (int i = 0; i < 3; i++) {
    pi[i] = exp_z[i] / somme_exp;
  }

  float tirage = random_float();
  float probabilite_cumulee = 0.0f;
  for (int i = 0; i < 3; i++) {
    probabilite_cumulee += pi[i];
    if (tirage <= probabilite_cumulee) {
      return i;
    }
  }
  return ACTION_FORWARD;
}

#else

/* ===================================================================== *
 * LOGIQUE DE L'AGENT REINFORCE (Séquentiel & Parallèle)                 *
 * ===================================================================== */
#define ACTIVE_FEATURES 15
float theta[15][3];

void generer_phi(Perception p, float phi[15]) {
  phi[0] = p.distances_murs[0];
  phi[1] = p.distances_murs[1];
  phi[2] = p.distances_murs[2];
  phi[3] = p.densite_obstacles[0];
  phi[4] = p.densite_obstacles[1];
  phi[5] = p.densite_obstacles[2];
  phi[6] = p.densite_obstacles[3];
  phi[7] = (float)p.zone_adversaire_1;
  phi[8] = (float)p.zone_adversaire_2;
  phi[9] = (float)p.zone_adversaire_3;
  phi[10] = p.densite_obstacles[0] * p.densite_obstacles[1];
  phi[11] = p.distances_murs[0] * p.distances_murs[1];
  phi[12] = p.densite_obstacles[2] * p.densite_obstacles[3];
  phi[13] = p.distances_murs[0] * p.densite_obstacles[1];
  phi[14] = p.distances_murs[2] * p.distances_murs[1];
}

void init_theta(void) {
  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 3; j++) {
      theta[i][j] = (random_float() * 0.02f) - 0.01f;
    }
  }
}

void sauvegarder_theta(const char *filename) {
  FILE *f = fopen(filename, "wb");
  if (f) {
    fwrite(theta, sizeof(float), 45, f);
    fclose(f);
  }
}

void charger_theta(const char *filename) {
  FILE *f = fopen(filename, "rb");
  if (f) {
    if (fread(theta, sizeof(float), 45, f) != 45) {
      init_theta();
    }
    fclose(f);
  } else {
    init_theta();
  }
}

int choisir_action(Perception p, FrameMemoire *mem_frame) {
  float phi[15];
  generer_phi(p, phi);

  float z[3] = {0};
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < ACTIVE_FEATURES; i++) {
      z[j] += phi[i] * theta[i][j];
    }
  }

  float max_z = z[0];
  for (int j = 1; j < 3; j++) {
    if (z[j] > max_z)
      max_z = z[j];
  }

  float exp_z[3], somme_exp = 0.0f;
  for (int j = 0; j < 3; j++) {
    exp_z[j] = exp(z[j] - max_z);
    somme_exp += exp_z[j];
  }

  float pi[3];
  for (int j = 0; j < 3; j++) {
    pi[j] = exp_z[j] / somme_exp;
    if (mem_frame)
      mem_frame->probabilites[j] = pi[j];
  }

  float tirage = random_float();
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

float maj_theta(EpisodeMemoire *ep, float alpha, float gamma) {
  float G = 0.0f;
  for (int t = ep->taille - 1; t >= 0; t--) {
    G = ep->frames[t].recompense + gamma * G;
    for (int i = 0; i < ACTIVE_FEATURES; i++) {
      for (int j = 0; j < 3; j++) {
        float y_j = (ep->frames[t].action == j) ? 1.0f : 0.0f;
        float erreur = y_j - ep->frames[t].probabilites[j];
        theta[i][j] += alpha * ep->frames[t].phi[i] * erreur * G;
      }
    }
  }
  return G;
}

#endif /* NO_REINFORCE */