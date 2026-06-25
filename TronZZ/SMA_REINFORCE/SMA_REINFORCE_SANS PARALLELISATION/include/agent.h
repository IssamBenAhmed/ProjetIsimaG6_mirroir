/**
 * @file agent.h
 * @brief Gestion de l'IA (matrice de poids theta, choix de l'action et apprentissage REINFORCE).
 */

#ifndef AGENT_H
#define AGENT_H

#include "config.h"

/**
 * @brief Taille fixe de l'historique d'un episode pour l'apprentissage REINFORCE.
 */
#define MAX_FRAMES_EPISODE 10000

/**
 * @struct FrameMemoire
 * @brief Representation de l'historique d'une etape (frame) de jeu.
 */
typedef struct {
    float phi[15];          /**< Le vecteur de caractéristiques de taille 15 associe a la perception. */
    int action;             /**< L'action qui a ete reellement choisie par l'agent. */
    float probabilites[3];  /**< Le vecteur de distribution de probabilités issu de la formule Softmax. */
    float recompense;       /**< La recompense immediate associee a cette etape. */
} FrameMemoire;

/**
 * @struct EpisodeMemoire
 * @brief Historique complet d'un episode utilise pour la propagation arriere du gradient.
 */
typedef struct {
    FrameMemoire frames[MAX_FRAMES_EPISODE]; /**< Tableau statique des frames enregistrees durant la partie. */
    int taille;                              /**< Nombre effectif d'etapes ecoulees dans cet episode. */
} EpisodeMemoire;

/**
 * @brief Traduit la structure logique de perception en un vecteur mathematique abstrait phi.
 * @param p La structure de perception fournie par le MJ.
 * @param phi Le vecteur flottant de sortie a remplir.
 */
void generer_phi(Perception p, float phi[15]);

/**
 * @brief Initialise la matrice de poids theta avec de petites valeurs aleatoires et asymetriques.
 */
void init_theta(void);

/**
 * @brief Sauvegarde la matrice theta courante sur le disque dur.
 * @param filename Chemin vers le fichier binaire de destination.
 */
void sauvegarder_theta(const char *filename);

/**
 * @brief Charge la matrice theta depuis le disque, ou l'initialise si absente.
 * @param filename Chemin vers le fichier de sauvegarde binaire.
 */
void charger_theta(const char *filename);

/**
 * @brief Generateur de nombre pseudo-aleatoire uniforme dans [0.0, 1.0].
 * @return Un flottant pseudo-aleatoire uniforme.
 */
float random_float(void);

/**
 * @brief Choisit une action par tirage stochastique biaise apres calcul du Softmax et memorise l'etat.
 * @param p La perception courante de l'agent.
 * @param mem_frame Pointeur vers la frame a remplir dans l'historique (NULL si aucun enregistrement).
 * @return L'action choisie (ACTION_FORWARD, ACTION_LEFT, ACTION_RIGHT).
 */
int choisir_action(Perception p, FrameMemoire *mem_frame);

/**
 * @brief Applique la descente de gradient de politique (REINFORCE) a la fin d'un episode.
 * @param ep Pointeur vers l'episode complet.
 * @param alpha Le taux d'apprentissage (learning rate).
 * @param gamma Le facteur d'attenuation temporelle.
 * @return Le score total G_0 de l'episode pour le suivi statistique.
 */
float maj_theta(EpisodeMemoire *ep, float alpha, float gamma);

#endif