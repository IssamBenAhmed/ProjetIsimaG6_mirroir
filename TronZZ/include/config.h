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

/* Direction courante de la moto dans la grille */
#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3

#define ACTION_FORWARD 0
#define ACTION_LEFT 1
#define ACTION_RIGHT 2

#define filename_theta "data/cerveau.bin"

// Paramètres spécifiques à l'apprentissage REINFORCE
#define MAX_FRAMES_EPISODE 10000
#define MAX_FRAMES_MEMORY 10000
#define MAX_FRAMES_TOTAL 1000000
#define TEMPS_ENTRE_RAPPORTS 20
#define ALPHA 0.0001f

/*
 * Perception brute fournie à l'agent.
 */
typedef struct {
  // module 1 : vision linéaire (les rayons)
  float distances_murs[3]; // [0] = devant, [1] = Gauche, [2] = Droite

  // module 2 : vision globale (les zones)
  float densite_obstacles[4]; // [0] = Avant-G, [1] = Avant-D, [2] = Arrière-G,
                              // [3] = Arrière-D
  int zone_adversaire_1; // ID de la zone (0 à 3) où se trouve l'adversaire
  int zone_adversaire_2;
  int zone_adversaire_3;
} Perception;

#endif
