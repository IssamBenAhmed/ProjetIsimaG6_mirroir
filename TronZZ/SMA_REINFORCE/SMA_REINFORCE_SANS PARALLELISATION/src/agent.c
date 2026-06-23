#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/agent.h"
#include "../include/config.h"

/* limite l'apprentissage aux 3 premiers capteurs (les rayons) suite aux erreurs rapportees par mon prof.
 * les poids des autres capteurs resteront figes a 0 mathematiquement.
 * la taille de la matrice reste a 10x3 pour garantir la compatibilite du fichier binaire.
 */
#define ACTIVE_FEATURES 3 

/* matrice de poids theta partagee par tous les agents en apprentissage */
float theta[10][3];

/* retourne un flottant pseudo-aleatoire uniforme entre 0.0 et 1.0 */
float random_float(void) {
    return (float)rand() / (float)RAND_MAX;
}

/*
 * extraction des caracteristiques.
 * traduit la structure logique perception en un vecteur mathematique abstrait phi.
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

/* initialise theta avec de petites valeurs asymetriques (-0.01 a 0.01).
 * evite le blocage du gradient avec une matrice remplie de zeros.
 */
void init_theta(void) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 3; j++) {
            theta[i][j] = (random_float() * 0.02f) - 0.01f;
        }
    }
}

/* exporte la matrice theta en brut (120 octets) sur le disque dur */
void sauvegarder_theta(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(theta, sizeof(float), 30, f);
        fclose(f);
    }
}

/* injecte les poids du disque dur vers la ram. recommence a zero si introuvable. */
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
 * prise de decision de l'agent.
 * calcule les probabilites d'action via softmax et memorise le contexte.
 * l'anti-suicide est retire suite aux erreurs rapportees par le professeur.
 */
int choisir_action(Perception p, FrameMemoire *mem_frame) {
    float phi[10];
    generer_phi(p, phi);

    /* calcul des scores lineaires z (produit scalaire) */
    float z[3] = {0};
    for (int j = 0; j < 3; j++) {
        /* la boucle s'arrete a active_features (3). les entrees suivantes sont ignorees. */
        for (int i = 0; i < ACTIVE_FEATURES; i++) {
            z[j] += phi[i] * theta[i][j];
        }
    }

    /* stabilisation numerique pour eviter un overflow avec exp() */
    float max_z = z[0];
    for (int j = 1; j < 3; j++) {
        if (z[j] > max_z) max_z = z[j];
    }

    /* calcul des probabilites avec la formule softmax */
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

    /* tirage pseudo-aleatoire proportionnel */
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

    /* sauvegarde de la frame en ram pour la future descente de gradient */
    if (mem_frame) {
        for (int i = 0; i < 10; i++) {
            mem_frame->phi[i] = phi[i];
        }
        mem_frame->action = action_choisie;
        mem_frame->recompense = 1.0f; /* recompense unitaire de survie par defaut */
    }

    return action_choisie;
}

/*
 * apprentissage et mise a jour des poids.
 * parcourt l'historique de la mort vers la naissance pour propager les recompenses.
 * retourne le score total g_0 pour les statistiques.
 */
float maj_theta(EpisodeMemoire *ep, float alpha, float gamma) {
    float G = 0.0f;
    
    /* remontee dans le temps : de la fin de l'episode vers le debut */
    for (int t = ep->taille - 1; t >= 0; t--) {
        
        /* calcul du retour g_t avec l'attenuation (gamma) */
        G = ep->frames[t].recompense + gamma * G;
        
        /* mise a jour ciblee (active_features uniquement) */
        for (int i = 0; i < ACTIVE_FEATURES; i++) {
            for (int j = 0; j < 3; j++) {
                
                /* cible (1.0 si l'action a ete jouee, 0.0 sinon) */
                float y_j = (ep->frames[t].action == j) ? 1.0f : 0.0f;
                
                /* difference entre le choix reel et la prediction */
                float erreur = y_j - ep->frames[t].probabilites[j];
                
                /* formule du gradient de politique */
                theta[i][j] += alpha * ep->frames[t].phi[i] * erreur * G;
            }
        }
    }
    
    /* retourne l'evaluation globale de la partie (g a t=0) */
    return G;
}