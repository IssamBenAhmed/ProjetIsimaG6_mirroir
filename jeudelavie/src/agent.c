#include "../include/agent.h"

int is_alive(int nb_voisins, int etat) {
    if (etat == 1) {
        if (nb_voisins == 2 || nb_voisins == 3) {
            return 1;
        }
        else {
            return 0;
        }
    }
    
    if (etat == 0) {
        if (nb_voisins == 3) {
            return 1;
        }
        else {
            return 0;
        }
    }
    
    // Sécurité obligatoire pour satisfaire le compilateur
    return 0; 
}