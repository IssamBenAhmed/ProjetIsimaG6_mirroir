/**
 * @file config.h
 * @brief Constantes de configuration globale, reglages et structures de perception pour TronZZ.
 */

#ifndef CONFIG_H
#define CONFIG_H

/**
 * @brief Chaine identifiant le mode Entrainement headless.
 */
#define ENTRAINEMENT "train"

/**
 * @brief Chaine identifiant le mode Joueur avec SDL2.
 */
#define JOUEUR "play"

/**
 * @brief Largeur de la grille logique.
 */
#define WIDTH 100

/**
 * @brief Hauteur de la grille logique.
 */
#define HEIGHT 100

/**
 * @brief Taille en pixels d'une case de la grille pour le rendu.
 */
#define CELL_SIZE 7

/**
 * @brief Largeur calculee de la fenetre SDL2.
 */
#define WINDOW_WIDTH (WIDTH * CELL_SIZE)

/**
 * @brief Hauteur calculee de la fenetre SDL2.
 */
#define WINDOW_HEIGHT (HEIGHT * CELL_SIZE)

/**
 * @brief Délai en millisecondes entre deux frames de jeu.
 */
#define FRAME_DELAY_MS 16

/**
 * @brief Taille du vecteur de perception directionnel (devant, gauche, droite).
 */
#define PERCEPTION_SIZE 3

/**
 * @brief Limite de vision en nombre de cases pour les capteurs et densites.
 */
#define LIMIT_VISION 15

/**
 * @brief Nombre maximal de motos dans l'arene (Joueur + IA).
 */
#define MAX_MOTOS 4

#define CELL_EMPTY 0        /**< Case vide de la grille. */
#define CELL_PLAYER 1       /**< Case occupee par le trajet du Joueur. */
#define CELL_AI_1 2         /**< Case occupee par le trajet de l'IA 1. */
#define CELL_AI_2 3         /**< Case occupee par le trajet de l'IA 2. */
#define CELL_AI_3 4         /**< Case occupee par le trajet de l'IA 3. */

#define DIR_UP 0            /**< Direction : Vers le haut. */
#define DIR_RIGHT 1         /**< Direction : Vers la droite. */
#define DIR_DOWN 2          /**< Direction : Vers le bas. */
#define DIR_LEFT 3          /**< Direction : Vers la gauche. */

#define ACTION_FORWARD 0    /**< Action : Aller tout droit. */
#define ACTION_LEFT 1       /**< Action : Tourner a gauche. */
#define ACTION_RIGHT 2      /**< Action : Tourner a droite. */

/**
 * @brief Chemin relatif vers le fichier de stockage binaire de la politique theta.
 */
#define filename_theta "data/cerveau.bin"

///////////////////////////////////////////////////////////////////////////////
//////////// PARTIE REINFORCE ET AGENT ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define MAX_FRAMES_EPISODE 10000    /**< Nombre max de frames dans un episode. */
#define MAX_FRAMES_MEMORY 10000     /**< Taille maximale de la memoire tampon. */
#define MAX_FRAMES_TOTAL 1000000    /**< Nombre limite d'etapes pour l'entrainement. */
#define TEMPS_ENTRE_RAPPORTS 20      /**< Intervalle temporel d'affichage des logs. */
#define ALPHA 0.0001f               /**< Taux d'apprentissage (learning rate) par defaut. */

/**
 * @struct Perception
 * @brief Structure de donnees regroupant les perceptions directionnelles et floues.
 */
typedef struct {
    float distances_murs[3] ;       /**< Rayons lineaires : [0]=devant, [1]=gauche, [2]=droite. */
    float densite_obstacles[4] ;    /**< Densite floue par quadrants : [0]=Avant-G, [1]=Avant-D, [2]=Arriere-G, [3]=Arriere-D. */
    int zone_adversaire_1 ;        /**< Zone (0 a 3) ou se trouve le premier adversaire. */
    int zone_adversaire_2 ;        /**< Zone (0 a 3) ou se trouve le second adversaire. */
    int zone_adversaire_3 ;        /**< Zone (0 a 3) ou se trouve le troisieme adversaire. */
} Perception ;

#endif
