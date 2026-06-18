#include "../include/mj.h"
#include "agent.h"

void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction, float perception[PERCEPTION_SIZE]) {
	
	int devant = 0, gauche = 0, droite = 0; 

    int dx, dy;

    // vecteur devant selon direction de moto
    if(direction == 0){ dx = 1; dy = 0; }
    else if(direction == 1){ dx = -1; dy = 0; }
    else if(direction == 2){ dx = 0; dy = 1; }
    else { dx = 0; dy = -1; }

    // gauche = rotation -90°
    int ldx = -dy;
    int ldy = dx;

    // droite = rotation +90°
    int rdx = dy;
    int rdy = -dx;

    // perception devant
    for(int i = 1; i < WIDTH; i++){
        int nx = x + i * dx; //calcul de coordonnée de case devant de moto
        int ny = y + i * dy;

        if(nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) break;//quand la case est hors de zone , on s'arrete
        if(grille[nx][ny] != 0) break; //quand la case est bloqué , on s'arrete

        devant++;
    }

    // gauche
    for(int i = 1; i < WIDTH; i++){
        int nx = x + i * ldx; //calcul de coordonnée de case gauche de moto
        int ny = y + i * ldy;

        if(nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) break;//quand la case est hors de zone , on s'arrete
        if(grille[nx][ny] != 0) break; //quand la case est bloqué , on s'arrete


        gauche++;
    }

    // droite
    for(int i = 1; i < WIDTH; i++){
        int nx = x + i * rdx;//calcul de coordonnée de case droite de moto
        int ny = y + i * rdy;

        if(nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) break;//quand la case est hors de zone , on s'arrete
        if(grille[nx][ny] != 0) break; //quand la case est bloqué , on s'arrete


        droite++;
    }

    perception[0] = devant;
    perception[1] = gauche;
    perception[2] = droite;
}

void mettre_a_jour_monde(int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1]) {
	int i = 0;
    for(i = 0; i < MAX_MOTOS+1; i++){
       
    int x = pos_motos[i][0] ;
    int y = pos_motos[i][1];
    int direction = dir_motos[i];
    float perception[3]={0};
    calculer_perception(grille, x, y,direction, perception); //on regarde si à la vue de moto, il y a des places libres
    if (perception[0] ==0 && perception[1] ==0 && perception[2] ==0){//il est bloqué partout
            etats_vie[i] = 0; //le moto mort 
            nettoyer_trainee(grille, i); // on nettoie son trajet
    }
    else {
            int action = choisir_action(perception); 
            //agent choisi, en fonction de sa perception, à quelle action qu'il va choisir : 0= tout droite ; 1 = gauche ; 2 = droite
            int dx, dy;

            // vecteur devant selon direction de moto
            if(direction == 0){ dx = 1; dy = 0; }
            else if(direction == 1){ dx = -1; dy = 0; }
            else if(direction == 2){ dx = 0; dy = 1; }
            else { dx = 0; dy = -1; }

            // gauche = rotation -90°
            int ldx = -dy;
            int ldy = dx;

            // droite = rotation +90°
            int rdx = dy;
            int rdy = -dx;

            if (action == 0) { nx = x + dx; ny = y + dy; } //tout droit
            if (action == 1) { nx = x + ldx; ny = y + ldy; }//gauche
            if (action == 2) { nx = x + rdx; ny = y + rdy; } //droite

            // vérification sécurité
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && grille[nx][ny] == 0) {

                grille[nx][ny] = i;    // nouvelle position

                pos_motos[i][0] = nx;
                pos_motos[i][1] = ny;
            } 
            else {
            etats_vie[i] = false;
            nettoyer_trainee(grille, i);
            }


        }

    }

}

void nettoyer_trainee(int grille[WIDTH][HEIGHT], int id_moto) {
	for(int i = 0 ; i < WIDTH ; i++){
        for(int j = 0 ; j < HEIGHT ; j++){
            if( grille[i][j] == id_moto){ //si il y a le trajet de moto mort, on le vide
                grille[i][j] == 0;
            }
        }
    }

}
