#ifndef AGENT_H
#define AGENT_H

#include "config.h"

/* Taille fixe de l'historique d'un episode pour l'apprentissage REINFORCE. */
#define MAX_FRAMES_EPISODE 10000


/*
 * Memoire d'une frame:
 * - phi : etat transforme en variables numeriques
 * - action : action effectivement choisie
 * - probabilites : sortie du softmax au moment du choix
 * - recompense : retour immediat associe a la frame
 */
typedef struct {
    float phi[15];
    int action;
    float probabilites[3];
    float recompense;
} FrameMemoire;

/* Historique complet d'un episode pour recalculer les retours G_t. */
typedef struct {
    FrameMemoire frames[MAX_FRAMES_EPISODE];
    int taille;
} EpisodeMemoire;

/* Construit le vecteur phi a partir de la perception brute. */
void generer_phi(Perception p, float phi[15]);
/* Initialise theta avec de petites valeurs aleatoires. */
void init_theta(void);
/* Sauvegarde la matrice theta sur disque. */
void sauvegarder_theta(const char *filename);
/* Charge theta depuis un fichier, ou reinitialise si le fichier est absent. */
void charger_theta(const char *filename);
/* Fournit un alea uniforme dans [0, 1]. */
float random_float(void);
/* Choisit une action pendant l'apprentissage et remplit la memoire de frame. */
int choisir_action(Perception p, FrameMemoire *mem_frame);
/* Met a jour theta a partir d'un episode complet ET RENVOIS LE SCORE TOTAL G_0 */
float maj_theta(EpisodeMemoire *ep, float alpha, float gamma);

#endif