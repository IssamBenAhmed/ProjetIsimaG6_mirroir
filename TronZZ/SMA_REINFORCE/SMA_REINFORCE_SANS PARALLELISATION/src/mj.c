#include "../include/mj.h"
#include "../include/agent.h"




/* initialiser la partie de jeu*/
void initialiser_grille( int grille[WIDTH][HEIGHT]){
    for (int i = 0 ; i < WIDTH ; i++){
        for(int j = 0 ; j < HEIGHT ; j++){
            grille[i][j] = CELL_EMPTY;
        }
    }
}

void initialiser_partie (int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1]){
    initialiser_grille (grille); //mettre la grille vide

    int player_list[4] = {CELL_PLAYER, CELL_AI_1, CELL_AI_2, CELL_AI_3 };

    int k = 0 ;

    while(k<4){
        int x = rand()%WIDTH; //la position aléatoire
        int y = rand()%HEIGHT ; 
        if (grille[x][y] != CELL_EMPTY){ //quand la case est déjà rempli, on refait sa positionnement
            continue;
        }
        else{
            grille[x][y] = player_list[k]; //sinon, on le situe
            pos_motos[k+1][0] = x; //on paramrtre la position initial de moto
            pos_motos[k+1][1] = y;
            etats_vie[k+1] = true ; //il vit 
            dir_motos[k+1] = DIR_UP ; // direction initial (possiblement aleatoire, si oui mais " rand()%4 " )
            k++ ;
        }     
    }

}
/*fin de initialisation*/

int compter_moto_vivants(bool etats_vie[MAX_MOTOS + 1]){
    int i= 0; //indice
    int res = 0 ; //compteur de moto en vie
    for (i = 0 ; i < (MAX_MOTOS+1) ; i++){
        if (etats_vie[i]){ //si le moto est en vie, on le compte
            res ++ ; 

        }
   

    }
    
    return res ; 
}

int trouver_gagnant(bool etats_vie[MAX_MOTOS + 1]){
    int moto_vivant = compter_moto_vivants(etats_vie) ; //compter le nombre de moto vivants
    if(moto_vivant ==2){
        for (int i = 1 ; i < (MAX_MOTOS+1) ; i++){ //on ne rend pas compte le case vide qui correspond a etat_vie[0]
            if (etats_vie[i] != true){
                continue;
            }
            else{return i ; } //

        }

    }
    return -1; //pas encore de gagnant 
}


int cause_mort( int grille[WIDTH][HEIGHT],  int x , int y){
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT){ // il a tapé le mur : cas peu probable
        return -1 ;
    }
    return grille[x][y]; //renvoie le trajet de moto ou le moto a heurté et meurt par celui-ci


} 

static void assigner_zone_adversaire(int id,int zone,int *z1,int *z2,int *z3){
    if (id == CELL_AI_1){*z1 = zone;}
    else if (id == CELL_AI_2) {*z2 = zone;}
    else if (id == CELL_AI_3) {*z3 = zone;}
}


static void obtenir_vecteur(int direction, int *dx, int *dy) {
    if (direction == DIR_UP) { *dx = 0; *dy = -1; }
    else if (direction == DIR_RIGHT) { *dx = 1; *dy = 0; }
    else if (direction == DIR_DOWN) { *dx = 0; *dy = 1; }
    else if (direction == DIR_LEFT) { *dx = -1; *dy = 0; }
}


static float calculer_densite_zone( int grille[WIDTH][HEIGHT],int x,int y,int dx1, int dy1, int dx2,int dy2)
{
    int obstacles = 0;
    int total = 0;

    for (int i = 1; i <= LIMIT_VISION; i++) {

        for (int j = 1; j <= i; j++) {

            int nx = x + i * dx1 + j * dx2;
            int ny = y + i * dy1 + j * dy2;

            if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) {
                continue;
            }

            total++;

            if (grille[nx][ny] != CELL_EMPTY) {
                obstacles++;
            }
        }
    }

    if (total == 0) {
        return 1.0f;
    }

    return (float)obstacles / (float)total;
}

int existence_adversaire_dans_un_zone ( int grille[WIDTH][HEIGHT],int x,int y,int dx1, int dy1, int dx2,int dy2 ){
    for (int i = 1; i <= LIMIT_VISION; i++) {

        for (int j = 1; j <= i; j++) {

            int nx = x + i * dx1 + j * dx2;
            int ny = y + i * dy1 + j * dy2;

            if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) { //si hors de zone, on ignore
                continue;
            }


            int cell = grille[nx][ny];

            if (cell == CELL_AI_1 ||cell == CELL_AI_2 ||cell == CELL_AI_3)
            {
                return cell; }

        }
    }
    return -1;//non adversaire

}



void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction, Perception * perception ) {
    int dx, dy;
    obtenir_vecteur(direction, &dx, &dy);

    // Rotation matricielle correcte
    int ldx = dy;   // Gauche
    int ldy = -dx;
    int rdx = -dy;  // Droite
    int rdy = dx;

    float distances[3] = {0.0, 0.0, 0.0};
    int rayons_dx[3] = {dx, ldx, rdx};
    int rayons_dy[3] = {dy, ldy, rdy};

    // Lancer de rayons factorisé
    for (int r = 0; r < 3; r++) {
        for (int i = 1; i <= LIMIT_VISION; i++) {
            int nx = x + i * rayons_dx[r];
            int ny = y + i * rayons_dy[r];

            if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) break;
            if (grille[nx][ny] != CELL_EMPTY) break;

            distances[r]++;
        }
    }

    perception->distances_murs[0] = distances[0]/LIMIT_VISION; // rayon de vue devant
    perception->distances_murs[1] = distances[1]/LIMIT_VISION; // rayon de vue gauche
    perception->distances_murs[2] = distances[2]/LIMIT_VISION; // rayon de vue droite

    int adversaire[3] ;

    // zone floue
    
    //Avant-gauche 
    perception->densite_obstacles[0] = calculer_densite_zone(grille, x, y,dx, dy,ldx, ldy);

    //Avant-droite 
    perception->densite_obstacles[1] = calculer_densite_zone(grille, x, y,dx, dy,rdx, rdy);

    // Arrière-gauche 
    perception->densite_obstacles[2] =calculer_densite_zone(grille, x, y,-dx, -dy,ldx, ldy);

    // Arrière-droite
    perception->densite_obstacles[3] =calculer_densite_zone(grille, x, y,-dx, -dy,rdx, rdy);

    //maintenant, on calcule la zone ou l'adversaire existe 

    int id; //id de moto dans une zone flou

    //Avant-gauche
    id = existence_adversaire_dans_un_zone(grille, x, y, dx, dy, ldx, ldy);
    assigner_zone_adversaire(id, 0,&perception->zone_adversaire_1,&perception->zone_adversaire_2, &perception->zone_adversaire_3);

    //Avant-droite
    id = existence_adversaire_dans_un_zone(grille, x, y, dx, dy, rdx, rdy);
    assigner_zone_adversaire(id, 1, &perception->zone_adversaire_1,&perception->zone_adversaire_2,&perception->zone_adversaire_3);

    // Arrière-gauche
    id = existence_adversaire_dans_un_zone(grille, x, y, -dx, -dy, ldx, ldy);
    assigner_zone_adversaire(id, 2,&perception->zone_adversaire_1,&perception->zone_adversaire_2,&perception->zone_adversaire_3);

    // Arrière-droite
    id = existence_adversaire_dans_un_zone(grille, x, y, -dx, -dy, rdx, rdy);
    assigner_zone_adversaire(id, 3,&perception->zone_adversaire_1,&perception->zone_adversaire_2,&perception->zone_adversaire_3);

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

void mettre_a_jour_monde(int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1], EpisodeMemoire memoires[MAX_MOTOS + 1]) {
    /*int gagner_ou_pas = trouver_gagnant(etats_vie) ;
    if (gagner_ou_pas !=-1){
        printf("le gagnant est %d \n", gagner_ou_pas);
        break ;
    }
    */
    // On boucle sur TOUTES les motos vivantes (Joueur inclus : de 1 à 4)
    for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
        if (!etats_vie[i]) continue;

        int x = pos_motos[i][0];
        int y = pos_motos[i][1];
        
        // --- ISOLATION DE L'AGENT ---
        // Seules les Agents (ID >= 2) utilisent la perception et ta fonction choisir_action
        if (i >= CELL_AI_1) {
            Perception perception;
            calculer_perception(grille, x, y, dir_motos[i], &perception);
            int action = choisir_action(perception, &memoires[i].frames[memoires[i].taille]); //venant de "agent.h"
            

            if (action == ACTION_LEFT) {
                dir_motos[i] = (dir_motos[i] + 3) % 4; // Rotation -90°
            } else if (action == ACTION_RIGHT) {
                dir_motos[i] = (dir_motos[i] + 1) % 4; // Rotation +90°
            }

            memoires[i].taille++;
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
            etats_vie[i] = false; //moto meurt
            int cause  = cause_mort( grille ,nx, ny) ;
            nettoyer_trainee(grille, i);
            //il faut donner récompense pour les moto tuant un autre moto
        }
    }
}


