#ifndef AGENT_H
#define AGENT_H

#include "config.h"

#define MAX_FRAMES_EPISODE 10000

/**
 * @struct FrameMemoire
 * @brief Structure stockant les informations d'une frame (état, action,
 * probabilités, récompense) pour l'apprentissage REINFORCE.
 */
typedef struct {
  float phi[15];         /**< Vecteur de caractéristiques de l'état */
  int action;            /**< Action prise par l'agent */
  float probabilites[3]; /**< Probabilités calculées par le Softmax */
  float recompense;      /**< Récompense obtenue à cette frame */
} FrameMemoire;

/**
 * @struct EpisodeMemoire
 * @brief Historique complet des frames d'un épisode pour l'apprentissage.
 */
typedef struct {
  FrameMemoire frames[MAX_FRAMES_EPISODE]; /**< Liste des frames de l'épisode */
  int taille; /**< Nombre de frames enregistrées */
} EpisodeMemoire;

/**
 * @brief Génère un nombre flottant pseudo-aléatoire dans l'intervalle [0, 1].
 * @return float Valeur aléatoire
 */
float random_float(void);

#ifdef NO_REINFORCE
/**
 * @brief Choisit une action pour l'IA heuristique simple sans apprentissage.
 * @param perception Tableau des distances perçues par l'agent [devant, gauche,
 * droite].
 * @return int Action choisie (ACTION_FORWARD, ACTION_LEFT, ACTION_RIGHT)
 */
int choisir_action(float perception[PERCEPTION_SIZE]);
#else
/**
 * @brief Génère le vecteur de caractéristiques phi à partir d'une perception
 * brute.
 * @param p La perception courante de l'agent.
 * @param phi Le vecteur de caractéristiques de sortie à remplir.
 */
void generer_phi(Perception p, float phi[15]);

/**
 * @brief Initialise les poids theta de l'agent avec de petites valeurs
 * aléatoires.
 */
void init_theta(void);

/**
 * @brief Sauvegarde les poids theta dans un fichier binaire.
 * @param filename Chemin vers le fichier de sauvegarde.
 */
void sauvegarder_theta(const char *filename);

/**
 * @brief Charge les poids theta depuis un fichier binaire.
 * @param filename Chemin vers le fichier de sauvegarde.
 */
void charger_theta(const char *filename);

/**
 * @brief Choisit une action selon la politique courante (Softmax) et remplit la
 * mémoire de frame.
 * @param p La perception courante.
 * @param mem_frame Pointeur vers la structure de mémoire de frame à remplir.
 * @return int Action choisie
 */
int choisir_action(Perception p, FrameMemoire *mem_frame);

/**
 * @brief Met à jour les poids theta à l'aide de la règle de mise à jour
 * REINFORCE à la fin d'un épisode.
 * @param ep Pointeur vers la mémoire de l'épisode complété.
 * @param alpha Taux d'apprentissage.
 * @param gamma Facteur d'atténuation.
 * @return float Le score total (G0) de l'épisode.
 */
float maj_theta(EpisodeMemoire *ep, float alpha, float gamma);

extern float theta[15][3]; /**< Matrice globale de poids theta de l'agent */

#ifdef PARALLEL
/**
 * @brief Génère un nombre flottant pseudo-aléatoire thread-safe.
 * @param seed Pointeur vers la graine du générateur aléatoire.
 * @return float Valeur aléatoire dans [0, 1]
 */
float random_float_thread(unsigned int *seed);

/**
 * @brief Choisit une action de manière thread-safe selon une matrice de poids
 * locale.
 * @param p La perception courante.
 * @param mem_frame Pointeur vers la structure de mémoire de frame.
 * @param theta_local Matrice locale de poids theta.
 * @param seed Pointeur vers la graine aléatoire du thread.
 * @return int Action choisie
 */
int choisir_action_thread(Perception p, FrameMemoire *mem_frame,
                          float theta_local[15][3], unsigned int *seed);

/**
 * @brief Met à jour les poids theta locaux de manière thread-safe à la fin d'un
 * épisode.
 * @param ep Pointeur vers la mémoire de l'épisode complété.
 * @param alpha Taux d'apprentissage.
 * @param gamma Facteur d'atténuation.
 * @param theta_local Matrice locale de poids theta à mettre à jour.
 * @return float Le score total (G0) de l'épisode.
 */
float maj_theta_thread(EpisodeMemoire *ep, float alpha, float gamma,
                       float theta_local[15][3]);
#endif /* PARALLEL */
#endif /* NO_REINFORCE */

#endif /* AGENT_H */