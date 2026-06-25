/**
 * @file mj.h
 * @brief En-tete du Maitre du Jeu (MJ) - regles de deplacement, calcul de perception et collisions.
 */

#ifndef MJ_H
#define MJ_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "config.h"
#include "agent.h"

/**
 * @brief Modifie (ajoute) la recompense associee a une frame specifique.
 * @param recompense Valeur de la recompense a ajouter.
 * @param frame Pointeur vers la frame memoire cible.
 */
void modifier_recompense (float recompense, FrameMemoire * frame) ;

/**
 * @brief Initialise toutes les cases de la grille de jeu a vide.
 * @param grille La grille 2D globale du jeu.
 */
void initialiser_grille( int grille[WIDTH][HEIGHT]) ;

/**
 * @brief Initialise une nouvelle partie (placement aleatoire des joueurs et IA).
 * @param grille La grille 2D globale.
 * @param pos_motos Tableau des positions courantes des motos.
 * @param dir_motos Tableau des directions courantes des motos.
 * @param etats_vie Tableau indiquant quels joueurs ou IA sont vivants.
 */
void initialiser_partie (int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1]) ;

/**
 * @brief Compte le nombre de motos actuellement en vie dans la partie.
 * @param etats_vie Tableau indiquant l'etat de vie de chaque entite.
 * @return Le nombre de motos encore vivantes.
 */
int compter_moto_vivants(bool etats_vie[MAX_MOTOS + 1]); 

/**
 * @brief Determine s'il y a un unique vainqueur a l'issue d'une confrontation.
 * @param etats_vie Tableau de l'etat de vie de chaque entite.
 * @return L'ID du vainqueur, ou -1 s'il n'y a pas encore d'unique vainqueur.
 */
int trouver_gagnant(bool etats_vie[MAX_MOTOS + 1]) ;

/**
 * @brief Traite la condition de fin de partie (victoire/defaite) et distribue les recompenses finales.
 * @param etats_vie Tableau de l'etat de vie des motos.
 * @param memoires Tableau d'historiques d'episodes des motos.
 * @return True si la partie est terminee, False sinon.
 */
bool gagnant_ou_pas(bool etats_vie[MAX_MOTOS + 1], EpisodeMemoire memoires[MAX_MOTOS + 1]);

/**
 * @brief Determine l'obstacle responsable de la mort d'une moto a une position donnee.
 * @param grille La grille 2D globale.
 * @param x Coordonnee X de la collision.
 * @param y Coordonnee Y de la collision.
 * @return L'ID de la cellule fautive (murs, trainee, etc.).
 */
int cause_mort( int grille[WIDTH][HEIGHT],  int x , int y); 

/**
 * @brief Cherche s'il existe un adversaire dans une zone triangulaire de perception.
 * @param grille La grille 2D globale.
 * @param x Coordonnee X de depart (position de la moto).
 * @param y Coordonnee Y de depart (position de la moto).
 * @param dx1 Vecteur X definissant le premier rayon de zone.
 * @param dy1 Vecteur Y definissant le premier rayon de zone.
 * @param dx2 Vecteur X definissant le second rayon de zone.
 * @param dy2 Vecteur Y definissant le second rayon de zone.
 * @param mon_id L'ID de l'agent qui effectue le calcul (pour s'exclure lui-meme).
 * @return L'ID de l'adversaire trouve, ou -1 sinon.
 */
int existence_adversaire_dans_un_zone(int grille[WIDTH][HEIGHT], int x, int y, int dx1, int dy1, int dx2, int dy2, int mon_id);

/**
 * @brief Calcule le vecteur de perception subjectif complet d'un agent.
 * @param grille La grille 2D globale.
 * @param x Coordonnee X de l'agent.
 * @param y Coordonnee Y de l'agent.
 * @param direction Direction courante de deplacement de l'agent.
 * @param mon_id ID de l'agent (pour s'exclure des calculs).
 * @param perception Pointeur vers la structure de perception de sortie a remplir.
 */
void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction, int mon_id, Perception * perception);

/**
 * @brief Met a jour les deplacements, perceptions, et verifications de collisions pour le mode JOUEUR.
 * @param grille La grille 2D globale.
 * @param pos_motos Tableau des positions.
 * @param dir_motos Tableau des directions.
 * @param etats_vie Tableau de l'etat de vie.
 * @param memoires Tableau d'historiques d'episodes des agents.
 */
void mettre_a_jour_monde(int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1], EpisodeMemoire memoires[MAX_MOTOS + 1]); 

/**
 * @brief Met a jour les deplacements et applique l'apprentissage pour le mode ENTRAINEMENT (sans joueur humain).
 * @param grille La grille 2D globale.
 * @param pos_motos Tableau des positions.
 * @param dir_motos Tableau des directions.
 * @param etats_vie Tableau de l'etat de vie.
 * @param memoires Tableau d'historiques d'episodes des agents.
 */
void mettre_a_jour_monde_entrainement(int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1], EpisodeMemoire memoires[MAX_MOTOS + 1]);

/**
 * @brief Nettoie/efface de la grille toutes les trainees laissees par une moto eliminee.
 * @param grille La grille 2D globale.
 * @param id_moto L'ID de la moto dont il faut effacer le trajet.
 */
void nettoyer_trainee(int grille[WIDTH][HEIGHT], int id_moto);

#endif