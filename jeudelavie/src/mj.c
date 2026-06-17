#include "mj.h"
#include "agent.h"

/* compte les voisins vivants dans un voisinage torique */
int compter_voisins(int grille[WIDTH][HEIGHT], int x, int y){
    int compteur = 0;

    /* parcourt les huit cases autour de la cellule cible */
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){

            /* ignore la cellule centrale */
            if(i == 0 && j == 0){
                continue;
            }

            /* applique le tore pour rester dans la grille */
            int nx = (x + i + WIDTH) % WIDTH; // calcul la case
            int ny = (y + j + HEIGHT) % HEIGHT; 

            /* incrmente si le voisin est vivant */
            if(grille[nx][ny] == 1){
                compteur++;
            }
        }
    }

    return compteur;
}



/* calcule la prochaine generation cellule par cellule */
void maj_synchrone(int grille_courante[WIDTH][HEIGHT], int grille_future[WIDTH][HEIGHT]){
    /* parcourt toute la grille courante */
    for (int i = 0 ; i < WIDTH ; i ++){
        for(int j = 0 ; j < HEIGHT ; j ++){
            /* lit l'etat courant de la cellule */
            int etat_current = grille_courante[i][j] ; 
            /* calcule le nombre de voisins vivants */
            int nb_voisins_vivants = compter_voisins(grille_courante, i, j);
            /* applique la regle de l'agent sur la grille future */
            grille_future[i][j] = is_alive(nb_voisins_vivants, etat_current);
        }
    }

}
