#ifndef CONFIG_H
#define CONFIG_H

#define WIDTH 100
#define HEIGHT 100

#define CELL_SIZE 7
#define WINDOW_WIDTH (WIDTH * CELL_SIZE)
#define WINDOW_HEIGHT (HEIGHT * CELL_SIZE)

#define FRAME_DELAY_MS 16
#define PERCEPTION_SIZE 3
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

#endif
