/**
 * @file tests.c
 * @brief Suite de tests unitaires pour valider les composants critiques de
 * TronZZ.
 *
 * Ce fichier contient les tests unitaires essentiels demandés pour valider le
 * moteur physique, la perception, le calcul de probabilités Softmax et la
 * descente de gradient stochastique (REINFORCE).
 */

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "agent.h"
#include "config.h"
#include "mj.h"

/* Accès à la matrice globale de poids theta de l'agent pour vérification
 * directe */
extern float theta[15][3];

/* Seuil de tolérance pour la comparaison de nombres flottants */
#define EPSILON 1e-5f

/**
 * @brief Test de l'initialisation de la grille logique.
 */
void test_initialiser_grille(void) {
  int grille[WIDTH][HEIGHT];
  initialiser_grille(grille);

  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      assert(grille[i][j] == CELL_EMPTY);
    }
  }
  printf("  [OK] test_initialiser_grille\n");
}

/**
 * @brief Test de la fonction cause_mort (détection des collisions hors-grille
 * et obstacles).
 */
void test_cause_mort(void) {
  int grille[WIDTH][HEIGHT];
  initialiser_grille(grille);

  // Obstacle artificiel
  grille[10][20] = CELL_AI_1;

  // Cas 1 : Collision hors-limites
  assert(cause_mort(grille, -1, 5) == -1);
  assert(cause_mort(grille, WIDTH, 10) == -1);
  assert(cause_mort(grille, 5, -1) == -1);
  assert(cause_mort(grille, 5, HEIGHT) == -1);

  // Cas 2 : Collision avec un agent
  assert(cause_mort(grille, 10, 20) == CELL_AI_1);

  // Cas 3 : Case vide
  assert(cause_mort(grille, 5, 5) == CELL_EMPTY);

  printf("  [OK] test_cause_mort\n");
}

/**
 * @brief Test de l'extraction de caractéristiques (generer_phi).
 */
void test_generer_phi(void) {
  Perception p;
  p.distances_murs[0] = 0.5f;
  p.distances_murs[1] = 0.2f;
  p.distances_murs[2] = 0.8f;

  p.densite_obstacles[0] = 0.1f;
  p.densite_obstacles[1] = 0.4f;
  p.densite_obstacles[2] = 0.3f;
  p.densite_obstacles[3] = 0.9f;

  p.zone_adversaire_1 = 1;
  p.zone_adversaire_2 = 2;
  p.zone_adversaire_3 = 0;

  float phi[15];
  generer_phi(p, phi);

  // Vérification des caractéristiques de base (0 à 9)
  assert(fabsf(phi[0] - 0.5f) < EPSILON);
  assert(fabsf(phi[1] - 0.2f) < EPSILON);
  assert(fabsf(phi[2] - 0.8f) < EPSILON);
  assert(fabsf(phi[3] - 0.1f) < EPSILON);
  assert(fabsf(phi[4] - 0.4f) < EPSILON);
  assert(fabsf(phi[5] - 0.3f) < EPSILON);
  assert(fabsf(phi[6] - 0.9f) < EPSILON);
  assert(fabsf(phi[7] - 1.0f) < EPSILON);
  assert(fabsf(phi[8] - 2.0f) < EPSILON);
  assert(fabsf(phi[9] - 0.0f) < EPSILON);

  // Vérification des non-linéarités (10 à 14)
  assert(fabsf(phi[10] - (0.1f * 0.4f)) < EPSILON); // densite[0] * densite[1]
  assert(fabsf(phi[11] - (0.5f * 0.2f)) <
         EPSILON); // distances[0] * distances[1]
  assert(fabsf(phi[12] - (0.3f * 0.9f)) < EPSILON); // densite[2] * densite[3]
  assert(fabsf(phi[13] - (0.5f * 0.4f)) < EPSILON); // distances[0] * densite[1]
  assert(fabsf(phi[14] - (0.8f * 0.2f)) <
         EPSILON); // distances[2] * distances[1]

  printf("  [OK] test_generer_phi\n");
}

/**
 * @brief Test de la sélection d'actions Softmax stochastique.
 */
void test_choisir_action(void) {
  Perception p = {{0.5f, 0.5f, 0.5f}, {0.1f, 0.1f, 0.1f, 0.1f}, -1, -1, -1};
  FrameMemoire frame;

  // Initialisation
  init_theta();

  int action = choisir_action(p, &frame);

  // L'action doit être dans l'intervalle valide [0, 2]
  assert(action >= ACTION_FORWARD && action <= ACTION_RIGHT);

  // Les probabilités Softmax doivent sommer à 1.0
  float somme_probabilites =
      frame.probabilites[0] + frame.probabilites[1] + frame.probabilites[2];
  assert(fabsf(somme_probabilites - 1.0f) < EPSILON);

  // La structure frame mémoire doit être correctement remplie
  assert(frame.action == action);
  assert(fabsf(frame.recompense - 1.0f) <
         EPSILON); // Récompense par défaut de survie

  printf("  [OK] test_choisir_action\n");
}

/**
 * @brief Test mathématique de la mise à jour stochastique du gradient
 * (REINFORCE).
 */
void test_maj_theta(void) {
  // Étape 1 : Réinitialisation forcée de theta à 0.5 pour rendre le calcul
  // déterministe et prédictible
  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 3; j++) {
      theta[i][j] = 0.5f;
    }
  }

  // Étape 2 : Simulation d'un épisode de taille 2
  EpisodeMemoire ep;
  ep.taille = 2;

  // Frame t = 0 (début de partie)
  for (int i = 0; i < 15; i++)
    ep.frames[0].phi[i] = 0.1f;
  ep.frames[0].action = ACTION_LEFT; // Action 1
  ep.frames[0].probabilites[0] = 0.3f;
  ep.frames[0].probabilites[1] = 0.4f;
  ep.frames[0].probabilites[2] = 0.3f;
  ep.frames[0].recompense = 1.0f;

  // Frame t = 1 (mort au tour d'après)
  for (int i = 0; i < 15; i++)
    ep.frames[1].phi[i] = 0.2f;
  ep.frames[1].action = ACTION_FORWARD; // Action 0
  ep.frames[1].probabilites[0] = 0.5f;
  ep.frames[1].probabilites[1] = 0.2f;
  ep.frames[1].probabilites[2] = 0.3f;
  ep.frames[1].recompense = -100.0f;

  // Paramètres d'apprentissage
  float alpha = 0.0001f;
  float gamma = 0.99f;

  // Calcul manuel théorique :
  // A t = 1 (remontée) :
  //   G_1 = R_1 + gamma * 0 = -100.0f.
  //   Correction sur theta : theta_new[i][j] = theta_old[i][j] + alpha *
  //   phi_1[i] * (y_j - pi_j) * G_1 y_0 = 1, erreur_0 = 1.0 - 0.5 = 0.5 ->
  //   delta_theta[i][0] = 0.0001 * 0.2 * 0.5 * (-100) = -0.001f y_1 = 0,
  //   erreur_1 = 0.0 - 0.2 = -0.2 -> delta_theta[i][1] = 0.0001 * 0.2 * (-0.2)
  //   * (-100) = 0.0004f y_2 = 0, erreur_2 = 0.0 - 0.3 = -0.3 ->
  //   delta_theta[i][2] = 0.0001 * 0.2 * (-0.3) * (-100) = 0.0006f
  //
  // A t = 0 :
  //   G_0 = R_0 + gamma * G_1 = 1.0f + 0.99f * (-100.0f) = -98.0f.
  //   Correction sur theta : theta_new[i][j] += alpha * phi_0[i] * (y_j - pi_j)
  //   * G_0 y_0 = 0, erreur_0 = 0.0 - 0.3 = -0.3 -> delta_theta[i][0] += 0.0001
  //   * 0.1 * (-0.3) * (-98.0) = 0.000294f y_1 = 1, erreur_1 = 1.0 - 0.4 = 0.6
  //   -> delta_theta[i][1] += 0.0001 * 0.1 * (0.6) * (-98.0) = -0.000588f y_2 =
  //   0, erreur_2 = 0.0 - 0.3 = -0.3 -> delta_theta[i][2] += 0.0001 * 0.1 *
  //   (-0.3) * (-98.0) = 0.000294f
  //
  // Final attendu (depuis 0.5) :
  //   theta[i][0] = 0.5f - 0.001f + 0.000294f = 0.499294f
  //   theta[i][1] = 0.5f + 0.0004f - 0.000588f = 0.499812f
  //   theta[i][2] = 0.5f + 0.0006f + 0.000294f = 0.500894f

  float G0 = maj_theta(&ep, alpha, gamma);

  // Vérification du retour cumulé global
  assert(fabsf(G0 - (-98.0f)) < EPSILON);

  // Vérification des gradients sur chaque case de la matrice theta
  for (int i = 0; i < 15; i++) {
    assert(fabsf(theta[i][0] - 0.499294f) < EPSILON);
    assert(fabsf(theta[i][1] - 0.499812f) < EPSILON);
    assert(fabsf(theta[i][2] - 0.500894f) < EPSILON);
  }

  printf("  [OK] test_maj_theta (Mathématiques du Gradient de Politique)\n");
}

/**
 * @brief Test du raycasting géométrique de la perception de l'agent.
 */
void test_calculer_perception_rayons(void) {
  int grille[WIDTH][HEIGHT];
  initialiser_grille(grille);

  // Agent placé au centre (50, 50), dirigé vers le HAUT (DIR_UP)
  int agent_x = 50;
  int agent_y = 50;
  int agent_dir = DIR_UP;
  int mon_id = CELL_AI_1;

  Perception p;

  // Cas 1 : Grille complètement vide.
  // L'agent doit voir jusqu'à la limite de vision sans obstacle.
  calculer_perception(grille, agent_x, agent_y, agent_dir, mon_id, &p);

  assert(fabsf(p.distances_murs[0] - 1.0f) < EPSILON); // Tout droit (15/15)
  assert(fabsf(p.distances_murs[1] - 1.0f) < EPSILON); // À gauche (15/15)
  assert(fabsf(p.distances_murs[2] - 1.0f) < EPSILON); // À droite (15/15)

  // Cas 2 : Obstacle placé à 5 cases devant l'agent (y = 45, x = 50)
  // L'agent doit voir 4 cases vides, et heurter l'obstacle à la 5ème.
  grille[50][45] = CELL_AI_2;
  calculer_perception(grille, agent_x, agent_y, agent_dir, mon_id, &p);

  // La distance devant doit être de 4.0 / LIMIT_VISION (4/15)
  float distance_attendue = 4.0f / (float)LIMIT_VISION;
  assert(fabsf(p.distances_murs[0] - distance_attendue) < EPSILON);

  // Les côtés gauche/droite restent inchangés (1.0f)
  assert(fabsf(p.distances_murs[1] - 1.0f) < EPSILON);
  assert(fabsf(p.distances_murs[2] - 1.0f) < EPSILON);

  printf("  [OK] test_calculer_perception_rayons\n");
}

/**
 * @brief Point d'entrée de la suite de tests unitaires.
 */
int main(void) {
  printf("=== DÉBUT DES TESTS UNITAIRES TRONZZ ===\n");

  test_initialiser_grille();
  test_cause_mort();
  test_generer_phi();
  test_choisir_action();
  test_maj_theta();
  test_calculer_perception_rayons();

  printf("=== TOUS LES TESTS SE SONT DÉROULÉS AVEC SUCCÈS [PASSED] ===\n");
  return 0;
}
