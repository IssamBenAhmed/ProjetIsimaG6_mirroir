#include "mj.h"
#include "agent.h"

int compter_voisins(int grille[WIDTH][HEIGHT], int x, int y){
    int compteur = 0;

    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){

            if(i == 0 && j == 0){
                continue;
            }

            int nx = (x + i + WIDTH) % WIDTH; // calcul la case
            int ny = (y + j + HEIGHT) % HEIGHT; 

            if(grille[nx][ny] == 1){
                compteur++;
            }
        }
    }

    return compteur;
}



void maj_synchrone(int grille_courante[WIDTH][HEIGHT], int grille_future[WIDTH][HEIGHT]){
    for (int i = 0 ; i < WIDTH ; i ++){
        for(int j = 0 ; j < HEIGHT ; j ++){
            int etat_current = grille_courante[i][j] ; 
            int nb_voisins_vivants = compter_voisins(grille_courante, i, j);
             grille_future[i][j] = is_alive( nb_voisins_vivants, etat_current,);
        }
    }

}
