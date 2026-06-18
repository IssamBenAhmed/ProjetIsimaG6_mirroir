#include <stdlib.h>
#include <math.h>
#include "../include/config.h"

// fonction utilitaire pour generer un float aleatoire entre zero et un
float random_float(void) {
    return (float)rand() / (float)RAND_MAX;
}

int choisir_action(float perception[PERCEPTION_SIZE]) {
    // 1. Le tableau Z (les logits) liés à la perception
    float z[3];
    float poids_espace = 2.0f; // L'importance donnée à l'espace libre

    for (int i = 0; i < 3; i++) {
        // La "Crainte du mur" : si le mur est à 1 case ou moins
        if (perception[i] <= 1.0f) {
            z[i] = -999.0f; // Score infiniment bas -> Proba Softmax proche de 0%
        } 
        else {
            // Le score Z dépend du nombre de cases libres + un peu de hasard (exploration)
            z[i] = (perception[i] * poids_espace) + (random_float() * 1.5f);
        }
    }

    // 2. Calcul des probabilités avec la formule Softmax
    float exp_z[3];
    float somme_exp = 0.0f;

    for (int i = 0; i < 3; i++) {
        exp_z[i] = exp(z[i]);
        somme_exp += exp_z[i];
    }

    // Calcul final de pi k
    float pi[3];
    for (int i = 0; i < 3; i++) {
        pi[i] = exp_z[i] / somme_exp;
    }

    // 3. Extraction de la décision par tirage aléatoire biaisé
    float tirage = random_float();
    float probabilite_cumulee = 0.0f;

    for (int i = 0; i < 3; i++) {
        probabilite_cumulee += pi[i];
        if (tirage <= probabilite_cumulee) {
            return i; // Renvoie l'action choisie
        }
    }

    // Sécurité au cas où l'imprécision des float fausse la somme
 
    return ACTION_FORWARD;
}