#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/agent.h"
#include "../include/config.h"

/* Matrice de poids partagee par toutes les decisions de l'agent. */
float theta[10][3];

/* Retourne un flottant uniforme dans [0, 1]. */
float random_float(void) {
    return (float)rand() / (float)RAND_MAX;
   
}

/*
 * Convertit la perception brute en vecteur de caracteristiques phi.
 * L'algorithme travaille ensuite uniquement sur ces 10 valeurs.
 */
void generer_phi(Perception p, float phi[10]) {
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
}

/* Initialise theta avec de petites valeurs aleatoires pour eviter un biais initial. */
void init_theta(void) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 3; j++) {
            theta[i][j] = (random_float() * 0.02f) - 0.01f;
        }
    }
}

/* Ecrit theta tel quel dans un fichier binaire. */
void sauvegarder_theta(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(theta, sizeof(float), 30, f);
        fclose(f);
    }
}

/* Recharge theta depuis le fichier, sinon repart avec une initialisation fraiche. */
void charger_theta(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f) {
        fread(theta, sizeof(float), 30, f);
        fclose(f);
    } else {
        init_theta();
    }
}

/*
 * Choisit une action pendant l'entrainement.
 * Etapes:
 * 1. construire phi;
 * 2. calculer les scores z;
 * 3. appliquer un softmax;
 * 4. tirer une action selon les probabilites;
 * 5. memoriser la frame pour la mise a jour ulterieure.
 */
int choisir_action(Perception p, FrameMemoire *mem_frame) {
    float phi[10];
    generer_phi(p, phi);

    /* Score lineaire de chaque action. Z=theta^T * phi. */
    float z[3] = {0};
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 10; i++) {
            z[j] += phi[i] * theta[i][j];
        }
    }

    /* Masquage heuristique: empeche l'action avant si la voie est trop proche. (no suicide) */
    if (phi[0] <= 0.1f) z[0] = -999.0f;

    /* Stabilisation numerique du softmax.  */
    float max_z = z[0];
    for (int j = 1; j < 3; j++) {
        if (z[j] > max_z) max_z = z[j];
    }
//On retire max_z à toutes les valeurs de z 
    float exp_z[3], somme_exp = 0.0f;
    for (int j = 0; j < 3; j++) {
        exp_z[j] = exp(z[j] - max_z);
        somme_exp += exp_z[j];
    }

    float pi[3];
    for (int j = 0; j < 3; j++) {
        pi[j] = exp_z[j] / somme_exp;
        if (mem_frame) mem_frame->probabilites[j] = pi[j];
    }

    /* Tirage aleatoire selon la distribution calculee. */
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

    /* Sauvegarde de la frame pour la phase de mise a jour. */
    if (mem_frame) {
        for (int i = 0; i < 10; i++) {
            mem_frame->phi[i] = phi[i];
        }
        mem_frame->action = action_choisie;
        mem_frame->recompense = 1.0f;
    }

    return action_choisie;
}

/*
 * Met a jour theta apres la fin d'un episode.
 * On remonte l'historique a l'envers pour calculer les retours G_t,
 * puis on applique la regle REINFORCE sur chaque poids.
 */
void maj_theta(EpisodeMemoire *ep, float alpha, float gamma) {
    float G = 0.0f;
    for (int t = ep->taille - 1; t >= 0; t--) {
        /* Retour cumule a partir de la frame t. */
        G = ep->frames[t].recompense + gamma * G;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 3; j++) {
                /* Cible one-hot de l'action effectivement jouee. */
                float y_j = (ep->frames[t].action == j) ? 1.0f : 0.0f;
                /* Erreur entre l'action choisie et la proba predite. */
                float erreur = y_j - ep->frames[t].probabilites[j];
                /* Mise a jour proportionnelle a l'etat, a l'erreur et au retour. */
                theta[i][j] += alpha * ep->frames[t].phi[i] * erreur * G;
            }
        }
    }
}