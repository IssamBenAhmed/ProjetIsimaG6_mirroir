#include "../include/agent.h"

/* applique les regles de survie et de naissance de conway */
int is_alive(int nb_voisins, int etat) {
  /* une cellule vivante reste vivante avec deux ou trois voisins */
  if (etat == 1) {
    if (nb_voisins == 2 || nb_voisins == 3) {
      return 1;
    } else {
      return 0;
    }
  }

  /* une cellule morte devient vivante avec exactement trois voisins */
  if (etat == 0) {
    if (nb_voisins == 3) {
      return 1;
    } else {
      return 0;
    }
  }

  /* securite de retour par defaut */
  return 0;
}