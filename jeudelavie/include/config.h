#ifndef CONFIG_H
#define CONFIG_H

/* dimensions logiques de la grille */
#define WIDTH 100
#define HEIGHT 100

/* taille initiale d'une cellule en pixels */
#define CELL_SIZE 7

/* dimensions dynamiques de la fenetre sdl */
#define WINDOW_WIDTH (WIDTH * CELL_SIZE)
#define WINDOW_HEIGHT (HEIGHT * CELL_SIZE)

/* delai de rafraichissement par defaut */
#define DELAY_MS 32

/* zoom minimum pour garder la grille a sa taille de base */
#define MIN_ZOOM ((float)WINDOW_WIDTH / ((float)WIDTH * CELL_SIZE))

#endif /* CONFIG_H */