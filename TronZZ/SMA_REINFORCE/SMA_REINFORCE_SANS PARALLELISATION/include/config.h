#ifndef CONFIG_H
#define CONFIG_H

#define ENTRAINEMENT "train"
#define JOUEUR "play"

#define WIDTH 100
#define HEIGHT 100

#define CELL_SIZE 7
#define WINDOW_WIDTH (WIDTH * CELL_SIZE)
#define WINDOW_HEIGHT (HEIGHT * CELL_SIZE)

#define FRAME_DELAY_MS 16
#define PERCEPTION_SIZE 3
#define LIMIT_VISION 15
#define MAX_MOTOS 4

#define CELL_EMPTY 0
#define CELL_PLAYER 1
#define CELL_AI_1 2
#define CELL_AI_2 3
#define CELL_AI_3 4

/* Direction courante de la moto dans la grille.
 * Ce ne sont pas des actions: le demi-tour immediat reste interdit.
 */
#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3

#define ACTION_FORWARD 0
#define ACTION_LEFT 1
#define ACTION_RIGHT 2

#define filename_theta "../data/cerveau.bin"

///////////////////////////////////////////////////////////////////////////////
//////////// PARTIE REINFORCE ET AGENT ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define MAX_FRAMES_EPISODE 10000
/*
 * Perception brute fournie a l'agent.
 * Les distances, densites et zones adverses sont les entrees utilisees pour
 * construire le vecteur de caracteristiques phi.
 */
typedef struct {
    // module 1 : vision linéaire de précision (les rayons)
    float distances_murs[3] ; //[0] = devant , [1] = Gauche , [2] = Droite
    
    // module 2 : vision globale "floue" (les zones)
    float densite_obstacles[4] ;//[0] = Avant-G , [1] = Avant-D, [2] = Arrière-G , [3] = Arrière-D
    // densite obstacle calcule la proportion de case libre par rapport à la case de zone
    int zone_adversaire_1 ; //ID de la zone (0 a 3) ou se trouve l'adversaire
    int zone_adversaire_2 ;
    int zone_adversaire_3 ;
    
}Perception ;

#endif
