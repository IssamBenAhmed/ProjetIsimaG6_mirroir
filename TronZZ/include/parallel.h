#ifndef PARALLEL_H
#define PARALLEL_H

#ifdef PARALLEL
#include "agent.h"
#include "config.h"
#include "mj.h"
#include <threads.h>

/**
 * @struct ParamThread
 * @brief Structure contenant les paramètres d'exécution propres à chaque thread
 * d'entraînement.
 */
typedef struct {
  int id_thread;     /**< Identifiant unique du thread */
  long nb_episodes;  /**< Nombre total d'épisodes à jouer par ce thread */
  unsigned int seed; /**< Graine aléatoire locale du thread */
  float theta_local[15]
                   [3]; /**< Copie locale des poids theta pour l'entraînement */
  long somme_frames; /**< Nombre cumulé de frames exécutées (statistiques) */
  int max_frames; /**< Nombre maximum de frames dans un épisode (statistiques)
                   */
  long victoires; /**< Nombre de victoires enregistrées (statistiques) */
} ParamThread;

/**
 * @brief Génère un nombre flottant aléatoire thread-safe.
 * @param seed Pointeur vers la graine du générateur aléatoire.
 * @return float Valeur aléatoire dans [0, 1]
 */
float random_float_thread(unsigned int *seed);

/**
 * @brief Génère un entier aléatoire thread-safe.
 * @param seed Pointeur vers la graine aléatoire.
 * @param max Valeur maximale de l'entier.
 * @return int Valeur aléatoire
 */
int random_int_thread(unsigned int *seed, int max);

/**
 * @brief Choisit une action de manière thread-safe selon une matrice locale.
 * @param p Perception courante.
 * @param mem_frame Pointeur vers la frame mémoire.
 * @param theta_local Matrice locale des poids theta.
 * @param seed Pointeur vers la graine du thread.
 * @return int Action choisie
 */
int choisir_action_thread(Perception p, FrameMemoire *mem_frame,
                          float theta_local[15][3], unsigned int *seed);

/**
 * @brief Calcule la mise à jour locale de la politique REINFORCE dans le
 * thread.
 * @param ep Pointeur vers l'épisode complété.
 * @param alpha Taux d'apprentissage.
 * @param gamma Coefficient d'atténuation.
 * @param theta_local Matrice locale de poids theta à modifier.
 * @return float Valeur cumulée des récompenses de l'épisode.
 */
float maj_theta_thread(EpisodeMemoire *ep, float alpha, float gamma,
                       float theta_local[15][3]);

/**
 * @brief Initialise une arène et positionne les motos de manière thread-safe.
 * @param grille La grille locale du thread.
 * @param pos_motos Les positions locales des motos.
 * @param dir_motos Les directions locales des motos.
 * @param etats_vie Les états locaux de vie.
 * @param seed Pointeur vers la graine locale.
 */
void initialiser_partie_thread(int grille[WIDTH][HEIGHT],
                               int pos_motos[MAX_MOTOS + 1][2],
                               int dir_motos[MAX_MOTOS + 1],
                               bool etats_vie[MAX_MOTOS + 1],
                               unsigned int *seed);

/**
 * @brief Met à jour le monde de jeu pour un pas de temps dans le thread de
 * manière thread-safe.
 * @param grille Grille locale.
 * @param pos_motos Positions locales.
 * @param dir_motos Directions locales.
 * @param etats_vie États de vie locaux.
 * @param memoires Mémoires des épisodes locaux.
 * @param theta_local Matrice locale de poids.
 * @param seed Graine locale.
 */
void mettre_a_jour_monde_entrainement_thread(
    int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2],
    int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1],
    EpisodeMemoire memoires[MAX_MOTOS + 1], float theta_local[15][3],
    unsigned int *seed);

/**
 * @brief Point d'entrée pour l'exécution d'un thread de calcul de
 * l'entraînement.
 * @param arg Paramètres du thread (pointeur vers ParamThread).
 * @return int Code de retour (0 = OK).
 */
int entrainer_thread(void *arg);

/**
 * @brief Copie les poids d'une matrice theta source vers une destination.
 * @param source Matrice source.
 * @param destination Matrice cible.
 */
void copier_theta(float source[15][3], float destination[15][3]);

#endif /* PARALLEL */

#endif /* PARALLEL_H */
