#include "../include/mj.h"
#include "../include/agent.h"

// Fonction utilitaire interne pour avoir les bons vecteurs partout
static void obtenir_vecteur(int direction, int *dx, int *dy) {
    if (direction == DIR_UP) { *dx = 0; *dy = -1; }
    else if (direction == DIR_RIGHT) { *dx = 1; *dy = 0; }
    else if (direction == DIR_DOWN) { *dx = 0; *dy = 1; }
    else if (direction == DIR_LEFT) { *dx = -1; *dy = 0; }
}

void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction, float perception[PERCEPTION_SIZE]) {
    int dx, dy;
    obtenir_vecteur(direction, &dx, &dy);

    // Rotation matricielle correcte
    int ldx = dy;   // Gauche
    int ldy = -dx;
    int rdx = -dy;  // Droite
    int rdy = dx;

    int distances[3] = {0, 0, 0};
    int rayons_dx[3] = {dx, ldx, rdx};
    int rayons_dy[3] = {dy, ldy, rdy};

    // Lancer de rayons factorisé
    for (int r = 0; r < 3; r++) {
        for (int i = 1; i < WIDTH; i++) {
            int nx = x + i * rayons_dx[r];
            int ny = y + i * rayons_dy[r];

            if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) break;
            if (grille[nx][ny] != CELL_EMPTY) break;

            distances[r]++;
        }
    }

    perception[0] = distances[0];
    perception[1] = distances[1];
    perception[2] = distances[2];
}

void nettoyer_trainee(int grille[WIDTH][HEIGHT], int id_moto) {
    for(int i = 0 ; i < WIDTH ; i++){
        for(int j = 0 ; j < HEIGHT ; j++){
            if( grille[i][j] == id_moto){
                grille[i][j] = CELL_EMPTY;
            }
        }
    }
}

void mettre_a_jour_monde(int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1]) {
    // On boucle sur TOUTES les motos vivantes (Joueur inclus : de 1 à 4)
    for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
        if (!etats_vie[i]) continue;

        int x = pos_motos[i][0];
        int y = pos_motos[i][1];
        
        // --- ISOLATION DE L'AGENT ---
        // Seules les Agents (ID >= 2) utilisent la perception et ta fonction choisir_action
        if (i >= CELL_AI_1) {
            float perception[PERCEPTION_SIZE] = {0};
            calculer_perception(grille, x, y, dir_motos[i], perception);

            int action = choisir_action(perception);

            if (action == ACTION_LEFT) {
                dir_motos[i] = (dir_motos[i] + 3) % 4; // Rotation -90°
            } else if (action == ACTION_RIGHT) {
                dir_motos[i] = (dir_motos[i] + 1) % 4; // Rotation +90°
            }
        }
        // Le Joueur (i == 1) ignore ce bloc. Sa direction est gérée par Nabil via le clavier.
        // -------------------------

        // --- DEPLACEMENT COMMUN (Joueur + Agents) ---
        int dx, dy;
        obtenir_vecteur(dir_motos[i], &dx, &dy);

        int nx = x + dx;
        int ny = y + dy;

        // Verification de collision
        if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && grille[nx][ny] == CELL_EMPTY) {
            grille[nx][ny] = i;
            pos_motos[i][0] = nx;
            pos_motos[i][1] = ny;
        } else {
            etats_vie[i] = false;
            nettoyer_trainee(grille, i);
        }
    }
}