#include "../include/mj.h"
#include "../include/agent.h"

/* ---------------------------- initialiser la partie de jeu -----------------------------------*/
void initialiser_grille( int grille[WIDTH][HEIGHT]){
    for (int i = 0 ; i < WIDTH ; i++){
        for(int j = 0 ; j < HEIGHT ; j++){
            grille[i][j] = CELL_EMPTY;
        }
    }
}

void initialiser_partie (int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1]){
    initialiser_grille (grille); /* mettre la grille vide */

    int player_list[4] = {CELL_PLAYER, CELL_AI_1, CELL_AI_2, CELL_AI_3 };

    int k = 0 ;

    while(k<4){
        int x = rand()%WIDTH; /* la position aleatoire */
        int y = rand()%HEIGHT ; 
        if (grille[x][y] != CELL_EMPTY){ /* quand la case est deja remplie, on refait son positionnement */
            continue;
        }
        else{
            grille[x][y] = player_list[k]; /* sinon, on la situe */
            pos_motos[k+1][0] = x; /* on parametre la position initiale de la moto */
            pos_motos[k+1][1] = y;
            etats_vie[k+1] = true ; /* elle vit */
            dir_motos[k+1] = DIR_UP ; /* direction initiale (possiblement aleatoire) */
            k++ ;
        }     
    }
}
/*--------------------------------fin de l'initialisation----------------------------*/

/* on regarde l'etat du jeu et des motos */
int compter_moto_vivants(bool etats_vie[MAX_MOTOS + 1]){
    int i= 0; /* indice */
    int res = 0 ; /* compteur de motos en vie */
    for (i = 0 ; i < (MAX_MOTOS+1) ; i++){
        if (etats_vie[i]){ /* si la moto est en vie, on la compte */
            res ++ ; 
        }
    }
    return res ; 
}

int trouver_gagnant(bool etats_vie[MAX_MOTOS + 1]){
    int moto_vivant = compter_moto_vivants(etats_vie) ; /* compter le nombre de motos vivantes */
    if(moto_vivant == 2){
        for (int i = 1 ; i < (MAX_MOTOS+1) ; i++){ /* on ne rend pas compte de la case vide qui correspond a etat_vie[0] */
            if (etats_vie[i] != true){
                continue;
            }
            else{return i ; } 
        }
    }
    return -1; /* pas encore de gagnant */
}

bool gagnant_ou_pas(bool etats_vie[MAX_MOTOS + 1], EpisodeMemoire memoires[MAX_MOTOS + 1]){ 
    /* gestion de la fin de partie de l'entrainement ou du jeu */
    int gagner_ou_pas = trouver_gagnant(etats_vie) ; 
    if (gagner_ou_pas != -1){
        printf("le gagnant est %d \n", gagner_ou_pas);
        /* correction gahui : on utilise taille - 1 car l'index de memoire a deja avance */
        modifier_recompense(100.0f, &memoires[gagner_ou_pas].frames[memoires[gagner_ou_pas].taille - 1]); 
        return true;
    }    
    return false ;
}

int cause_mort( int grille[WIDTH][HEIGHT],  int x , int y){
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT){ /* elle a tape le mur : cas peu probable */
        return -1 ;
    }
    return grille[x][y]; /* renvoie le trajet que la moto a heurte et qui a cause sa mort */
} 

void modifier_recompense (float recompense, FrameMemoire * frame){ /* donner la recompense par rapport a l'action realisee par l'agent */
    frame->recompense += recompense;  
}

/*---------------------- la partie pour calculer la perception de la moto---------------------------------*/
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

static float calculer_densite_zone( int grille[WIDTH][HEIGHT],int x,int y,int dx1, int dy1, int dx2,int dy2){
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

            if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) { /* si hors de zone, on ignore */
                continue;
            }

            int cell = grille[nx][ny];

            if (cell == CELL_AI_1 ||cell == CELL_AI_2 ||cell == CELL_AI_3) {
                return cell; 
            }
        }
    }
    return -1; /* non adversaire */
}

void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction, Perception * perception ) {
    int dx, dy;
    obtenir_vecteur(direction, &dx, &dy);

    /* rotation matricielle correcte */
    int ldx = dy;   /* gauche */
    int ldy = -dx;
    int rdx = -dy;  /* droite */
    int rdy = dx;

    float distances[3] = {0.0, 0.0, 0.0};
    int rayons_dx[3] = {dx, ldx, rdx};
    int rayons_dy[3] = {dy, ldy, rdy};

    /* lancer de rayons factorise */
    for (int r = 0; r < 3; r++) {
        for (int i = 1; i <= LIMIT_VISION; i++) {
            int nx = x + i * rayons_dx[r];
            int ny = y + i * rayons_dy[r];

            if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) break;
            if (grille[nx][ny] != CELL_EMPTY) break;

            distances[r]++;
        }
    }

    perception->distances_murs[0] = distances[0]/LIMIT_VISION; /* rayon de vue devant */
    perception->distances_murs[1] = distances[1]/LIMIT_VISION; /* rayon de vue gauche */
    perception->distances_murs[2] = distances[2]/LIMIT_VISION; /* rayon de vue droite */

    /* zone floue */
    
    /* avant-gauche */
    perception->densite_obstacles[0] = calculer_densite_zone(grille, x, y,dx, dy,ldx, ldy);

    /* avant-droite */
    perception->densite_obstacles[1] = calculer_densite_zone(grille, x, y,dx, dy,rdx, rdy);

    /* arriere-gauche */
    perception->densite_obstacles[2] = calculer_densite_zone(grille, x, y,-dx, -dy,ldx, ldy);

    /* arriere-droite */
    perception->densite_obstacles[3] = calculer_densite_zone(grille, x, y,-dx, -dy,rdx, rdy);

    /* maintenant, on calcule la zone ou l'adversaire existe */

    int id; /* id de la moto dans une zone floue */

    /* avant-gauche */
    id = existence_adversaire_dans_un_zone(grille, x, y, dx, dy, ldx, ldy);
    assigner_zone_adversaire(id, 0,&perception->zone_adversaire_1,&perception->zone_adversaire_2, &perception->zone_adversaire_3);

    /* avant-droite */
    id = existence_adversaire_dans_un_zone(grille, x, y, dx, dy, rdx, rdy);
    assigner_zone_adversaire(id, 1, &perception->zone_adversaire_1,&perception->zone_adversaire_2,&perception->zone_adversaire_3);

    /* arriere-gauche */
    id = existence_adversaire_dans_un_zone(grille, x, y, -dx, -dy, ldx, ldy);
    assigner_zone_adversaire(id, 2,&perception->zone_adversaire_1,&perception->zone_adversaire_2,&perception->zone_adversaire_3);

    /* arriere-droite */
    id = existence_adversaire_dans_un_zone(grille, x, y, -dx, -dy, rdx, rdy);
    assigner_zone_adversaire(id, 3,&perception->zone_adversaire_1,&perception->zone_adversaire_2,&perception->zone_adversaire_3);
}
/*---------------------- fin de la partie pour calculer la perception de la moto ---------------------------------*/

/*----------------------------------- la partie de deroulement du jeu selon mode ---------------------------------*/
void nettoyer_trainee(int grille[WIDTH][HEIGHT], int id_moto) {
    for(int i = 0 ; i < WIDTH ; i++){
        for(int j = 0 ; j < HEIGHT ; j++){
            if( grille[i][j] == id_moto){
                grille[i][j] = CELL_EMPTY;
            }
        }
    }
}

/* mj pour le mode de jeu avec l'utilisateur */
void mettre_a_jour_monde(int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1], EpisodeMemoire memoires[MAX_MOTOS + 1]) {

    /* on boucle sur toutes les motos vivantes (joueur inclus : de 1 a 4) */
    for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
        if (!etats_vie[i]) continue;

        int x = pos_motos[i][0];
        int y = pos_motos[i][1];
        
        /* --- isolation de l'agent --- */
        /* seules les agents (id >= 2) utilisent la perception et ta fonction choisir_action */
        if (i >= CELL_AI_1) {
            Perception perception;
            calculer_perception(grille, x, y, dir_motos[i], &perception);
            
            /* correction gahui : ici on pointe vers [taille] (la case vide). pas de -1 sinon crash memoire des la frame 0 ! */
            int action = choisir_action(perception, &memoires[i].frames[memoires[i].taille]); 
            
            if (action == ACTION_LEFT) {
                dir_motos[i] = (dir_motos[i] + 3) % 4; /* rotation -90 degres */
            } else if (action == ACTION_RIGHT) {
                dir_motos[i] = (dir_motos[i] + 1) % 4; /* rotation +90 degres */
            }

            memoires[i].taille++; /* l'enregistrement est termine, on deplace le pointeur de taille */
        }
        /* le joueur (i == 1) ignore ce bloc. sa direction est geree par clavier. */

        /* --- deplacement commun (joueur + agents) --- */
        int dx, dy;
        obtenir_vecteur(dir_motos[i], &dx, &dy);

        int nx = x + dx;
        int ny = y + dy;

        /* verification de collision */
        if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && grille[nx][ny] == CELL_EMPTY) {
            grille[nx][ny] = i;
            pos_motos[i][0] = nx;
            pos_motos[i][1] = ny;
            if (i == CELL_PLAYER){
                continue;
            }
            
            /* correction gahui : on utilise taille - 1 pour modifier la frame qu'on a valide juste au dessus */
            modifier_recompense(1.0f, &memoires[i].frames[memoires[i].taille - 1]);
           
        } else {
            etats_vie[i] = false; /* la moto meurt */
            
            /* correction gahui : on utilise taille - 1 ici aussi */
            modifier_recompense(-100.0f, &memoires[i].frames[memoires[i].taille - 1]); 
            
            int cause  = cause_mort( grille ,nx, ny) ;
            if (cause == CELL_EMPTY){ //si le moto est mort par le mur, il ne dépend pas de récompense
                continue ;
            }
            if (cause == CELL_PLAYER){ //cas exception si le joueur est mort par lui-meme
                nettoyer_trainee(grille, i);
                continue ;
            }
            
            /* correction gahui : on utilise taille - 1 */
            modifier_recompense(50.0f, &memoires[i].frames[memoires[i].taille - 1] ) ;
            /* l'ia recoit une grande recompense pour tuer son adversaire */
            nettoyer_trainee(grille, i);
        }
    }
}

/* mj pour le mode d'entrainement de l'ia */
void mettre_a_jour_monde_entrainement(int grille[WIDTH][HEIGHT], int pos_motos[MAX_MOTOS + 1][2], int dir_motos[MAX_MOTOS + 1], bool etats_vie[MAX_MOTOS + 1], EpisodeMemoire memoires[MAX_MOTOS + 1]) {

    /* on boucle sur toutes les motos vivantes (joueur non inclus : de 1 a 4) */
    for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) { /* ici, le joueur est remplace par un agent */
        if (!etats_vie[i]) continue;

        int x = pos_motos[i][0];
        int y = pos_motos[i][1];
        Perception perception;
        calculer_perception(grille, x, y, dir_motos[i], &perception);
        
        /* correction gahui : pointage sur la nouvelle case [taille] */
        int action = choisir_action(perception, &memoires[i].frames[memoires[i].taille]); 
            
        if (action == ACTION_LEFT) {
            dir_motos[i] = (dir_motos[i] + 3) % 4; /* rotation -90 degres */
        } else if (action == ACTION_RIGHT) {
            dir_motos[i] = (dir_motos[i] + 1) % 4; /* rotation +90 degres */
        }

        memoires[i].taille++; /* validation */
        
        /* --- deplacement commun (agents) --- */
        int dx, dy;
        obtenir_vecteur(dir_motos[i], &dx, &dy);

        int nx = x + dx;
        int ny = y + dy;

        /* verification de collision */
        if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && grille[nx][ny] == CELL_EMPTY) {
            grille[nx][ny] = i;
            pos_motos[i][0] = nx;
            pos_motos[i][1] = ny;
            if (i == CELL_PLAYER){
                continue;
            }
            
            /* correction gahui : index de modification a taille - 1 */
            modifier_recompense(1.0f, &memoires[i].frames[memoires[i].taille - 1]);
        } 
        else {
            etats_vie[i] = false; /* la moto meurt */
            
            /* correction gahui : index de modification a taille - 1 */
            modifier_recompense(-100.0f, &memoires[i].frames[memoires[i].taille - 1]); 
            
            int cause  = cause_mort( grille ,nx, ny) ;
            if (cause == CELL_EMPTY){ /* morte par le mur */
                continue ;
            }
            
            /* correction gahui : index de modification a taille - 1 */
            modifier_recompense(50.0f, &memoires[i].frames[memoires[i].taille - 1] ) ;
            /* l'ia recoit une recompense pour tuer l'adversaire */
            nettoyer_trainee(grille, i);
        }
    }
}