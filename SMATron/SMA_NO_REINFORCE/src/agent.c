#include <stdlib.h>
#include <math.h>
#include "../include/config.h"

// fonction utilitaire pour generer un float aleatoire entre zero et un
float random_float() {
    return (float)rand() / (float)RAND_MAX;
}

int choisir_action(float perception[PERCEPTION_SIZE]) {
    (void)perception; // on l ignore pour cette version aleatoire

    // le tableau z
    // rempli avec des valeurs arbitraires aleatoires pour tester
    // sera adapte grace au REINFORCE dans la version suivante
    // les valeurs sont comprises entre 0 et 5
    // z[0] = score pour tout droit
    // z[1] = score pour gauche
    // z[2] = score pour droites
    float z[3];
    z[ACTION_FORWARD] = random_float() * 5.0f; // score pour tout droit
    z[ACTION_LEFT]    = random_float() * 5.0f; // score pour gauche
    z[ACTION_RIGHT]   = random_float() * 5.0f; // score pour droite

    // calcul des probabilites avec la formule softmax
    float exp_z[3];
    float somme_exp = 0.0f;

    
    for (int i = 0; i < 3; i++) {
        exp_z[i] = exp(z[i]);
        somme_exp += exp_z[i];
    }

    // calcul final de pi k
    float pi[3];
    for (int i = 0; i < 3; i++) {
        pi[i] = exp_z[i] / somme_exp;
    }

    // extraction de la decision par tirage aleatoire selon les probabilites pi k
    // on tire un float aleatoire entre 0 et 1
    // on parcourt le tableau des probabilites cumulees pour determiner l action choisie
    // par exemple si pi[0] = 0.5, pi[1] = 0.3, pi[2] = 0.2
    // alors les intervalles sont:
    // [0.0, 0.5) -> ACTION_FORWARD
    // [0.5, 0.8) -> ACTION_LEFT
    // [0.8, 1.0) -> ACTION_RIGHT

    
    float tirage = random_float();
    float probabilite_cumulee = 0.0f;

    for (int i = 0; i < 3; i++) {
        probabilite_cumulee += pi[i];
        if (tirage <= probabilite_cumulee) {
            return i; // renvoie l action choisie
        }
    }

    // securite evite un plantage si float imprecis
    return ACTION_FORWARD;
}