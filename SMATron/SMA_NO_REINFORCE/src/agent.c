#include "../include/agent.h"

int choisir_action(float perception[PERCEPTION_SIZE]) {

    int best_action = ACTION_FORWARD; 
    float max_espace = perception[0]; 

    if (perception[1] > max_espace) {
        max_espace = perception[1];
        best_action = ACTION_LEFT;
    }

    if (perception[2] > max_espace) {
        max_espace = perception[2];
        best_action = ACTION_RIGHT;
    }

    return best_action;
}