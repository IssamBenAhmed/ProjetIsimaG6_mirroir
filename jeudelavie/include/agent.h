#ifndef AGENT_H
#define AGENT_H

/**
 * @brief applique les regles de conway a une cellule.
 * @param nb_voisins nombre de voisins vivants autour de la cellule.
 * @param etat etat actuel de la cellule.
 * @return 1 si la cellule reste ou devient vivante, 0 sinon.
 */
int is_alive(int nb_voisins, int etat);

#endif