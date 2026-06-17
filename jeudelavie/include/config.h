#ifndef CONFIG_H
#define CONFIG_H

// Dimensions logiques de la grille carrée
#define WIDTH 100
#define HEIGHT 100

// Taille initiale d'une cellule en pixels à l'écran
#define CELL_SIZE 7

// Dimensions dynamiques de la fenêtre SDL (ici 700x700)
#define WINDOW_WIDTH (WIDTH * CELL_SIZE)
#define WINDOW_HEIGHT (HEIGHT * CELL_SIZE)

// Délai de rafraîchissement par défaut (en millisecondes)
#define DELAY_MS 32

#endif // CONFIG_H