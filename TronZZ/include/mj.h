#ifndef MJ_H
#define MJ_H

#include "agent.h"
#include "config.h"
#include <stdbool.h>

/**
 * @brief Initialise la grille logique avec des cases vides.
 * @param grille Matrice représentant la grille logique du jeu.
 */
void initialiser_grille(int grille[WIDTH][HEIGHT]);

/**
 * @brief Initialise une nouvelle partie en plaçant aléatoirement les motos sur
 * la grille.
 * @param grille La grille de jeu.
 * @param pos_motos Tableau stockant les positions (x, y) de chaque moto.
 * @param dir_motos Tableau des directions de chaque moto.
 * @param etats_vie Tableau de l'état de vie (vivant/mort) de chaque moto.
 */
void initialiser_partie(int grille[WIDTH][HEIGHT],
                        int pos_motos[MAX_MOTOS + 1][2],
                        int dir_motos[MAX_MOTOS + 1],
                        bool etats_vie[MAX_MOTOS + 1]);

/**
 * @brief Supprime la traînée d'une moto morte de la grille de jeu.
 * @param grille La grille de jeu.
 * @param id_moto Identifiant de la moto à nettoyer.
 */
void nettoyer_trainee(int grille[WIDTH][HEIGHT], int id_moto);

/**
 * @brief Compte le nombre de motos encore vivantes dans la partie.
 * @param etats_vie Tableau de l'état de vie de chaque moto.
 * @return int Le nombre de motos vivantes
 */
int compter_moto_vivants(bool etats_vie[MAX_MOTOS + 1]);

/**
 * @brief Trouve l'identifiant de la moto gagnante s'il en reste une.
 * @param etats_vie Tableau de l'état de vie de chaque moto.
 * @return int L'identifiant du gagnant, ou -1 si la partie continue
 */
int trouver_gagnant(bool etats_vie[MAX_MOTOS + 1]);

/**
 * @brief Détermine la cause de la mort en identifiant ce que la moto a percuté.
 * @param grille La grille de jeu.
 * @param x Coordonnée X de la collision.
 * @param y Coordonnée Y de la collision.
 * @return int L'identifiant de l'obstacle percuté, ou -1 en cas de collision
 * hors-grille
 */
int cause_mort(int grille[WIDTH][HEIGHT], int x, int y);

#ifdef NO_REINFORCE
/**
 * @brief Calcule le vecteur de perception heuristique à 3 distances pour un
 * agent.
 * @param grille La grille de jeu.
 * @param x Coordonnée X de l'agent.
 * @param y Coordonnée Y de l'agent.
 * @param direction Direction de l'agent.
 * @param perception Tableau à remplir avec les distances calculées [devant,
 * gauche, droite].
 */
void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction,
                         float perception[PERCEPTION_SIZE]);

/**
 * @brief Met à jour le monde de jeu d'une frame (mouvements, collisions) pour
 * le mode heuristique.
 * @param grille La grille de jeu.
 * @param pos_motos Positions des motos.
 * @param dir_motos Directions des motos.
 * @param etats_vie État de vie des motos.
 */
void mettre_a_jour_monde(int grille[WIDTH][HEIGHT],
                         int pos_motos[MAX_MOTOS + 1][2],
                         int dir_motos[MAX_MOTOS + 1],
                         bool etats_vie[MAX_MOTOS + 1]);
#else
/**
 * @brief Gère la fin de partie (récompense de victoire et affichage).
 * @param etats_vie Tableau de l'état de vie de chaque moto.
 * @param memoires Tableau des mémoires d'épisodes pour chaque moto.
 * @return true Si un gagnant a été désigné, false sinon
 */
bool gagnant_ou_pas(bool etats_vie[MAX_MOTOS + 1],
                    EpisodeMemoire memoires[MAX_MOTOS + 1]);

/**
 * @brief Calcule la perception REINFORCE complète (distances, densité
 * d'obstacles, zones adversaires).
 * @param grille La grille de jeu.
 * @param x Coordonnée X de l'agent.
 * @param y Coordonnée Y de l'agent.
 * @param direction Direction courante de l'agent.
 * @param mon_id Identifiant de l'agent.
 * @param perception Pointeur vers la structure de perception à remplir.
 */
void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction,
                         int mon_id, Perception *perception);

/**
 * @brief Met à jour le monde de jeu d'une frame avec enregistrement des frames
 * pour REINFORCE (mode interactif).
 * @param grille La grille de jeu.
 * @param pos_motos Positions des motos.
 * @param dir_motos Directions des motos.
 * @param etats_vie État de vie des motos.
 * @param memoires Mémoires d'épisodes des agents IA.
 */
void mettre_a_jour_monde(int grille[WIDTH][HEIGHT],
                         int pos_motos[MAX_MOTOS + 1][2],
                         int dir_motos[MAX_MOTOS + 1],
                         bool etats_vie[MAX_MOTOS + 1],
                         EpisodeMemoire memoires[MAX_MOTOS + 1]);

/**
 * @brief Met à jour le monde de jeu d'une frame (mouvements, collisions) pour
 * l'entraînement séquentiel REINFORCE.
 * @param grille La grille de jeu.
 * @param pos_motos Positions des motos.
 * @param dir_motos Directions des motos.
 * @param etats_vie État de vie des motos.
 * @param memoires Mémoires d'épisodes de tous les agents IA.
 */
void mettre_a_jour_monde_entrainement(int grille[WIDTH][HEIGHT],
                                      int pos_motos[MAX_MOTOS + 1][2],
                                      int dir_motos[MAX_MOTOS + 1],
                                      bool etats_vie[MAX_MOTOS + 1],
                                      EpisodeMemoire memoires[MAX_MOTOS + 1]);

void modifier_recompense(float recompense, FrameMemoire *frame);

#endif /* NO_REINFORCE */

#endif /* MJ_H */