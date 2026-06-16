// programme sdl pour afficher et deplacer un sprite anime avec les fleches.

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// dimensions de la fenetre sdl.
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// vitesse de deplacement du sprite a chaque pression de touche.
#define vitesse 5

// la feuille d'animation contient 9 images par ligne.
#define FRAME_COUNT 9

// la feuille est organisee en 4 lignes, une ligne par direction.
#define DIRECTION_COUNT 4

/**
 * @brief libere les ressources sdl et termine le programme.
 *
 * @param ok indique si la sortie est normale.
 * @param message message d'erreur a afficher, ou NULL.
 * @param window fenetre sdl a detruire, ou NULL.
 * @param renderer renderer sdl a detruire, ou NULL.
 * @param texture texture principale a detruire, ou NULL.
 * @param background_texture texture de fond a detruire, ou NULL.
 */
static void end_sdl(bool ok, const char *message, SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture, SDL_Texture *background_texture)
{
    // affiche le message si une erreur doit etre signalee.
    if (message != NULL) {
        fprintf(stderr, "%s\n", message);
    }

    // detruit la texture si elle a ete creee.
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
    }

    // detruit la texture de fond si elle a ete creee.
    if (background_texture != NULL) {
        SDL_DestroyTexture(background_texture);
    }

    // detruit le renderer s'il existe.
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }

    // detruit la fenetre si elle existe.
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }

    // ferme sdl_image puis sdl.
    IMG_Quit();
    SDL_Quit();

    // termine le programme avec le code approprie.
    exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

/**
 * @brief charge la feuille de sprite et deplace l'image affichee selon les fleches.
 *
 * @param image_path chemin vers la feuille de sprite.
 * @param background_pic chemin vers le fond, ou NULL si aucun fond n'est souhaite.
 */
static void display_movement(const char *image_path, const char *background_pic)
{
    // pointeurs sdl principaux.
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Texture *background_texture = NULL;

    // variables de gestion des evenements et des rectangles sdl.
    SDL_Event event;
    SDL_Rect tab_src[DIRECTION_COUNT][FRAME_COUNT];
    SDL_Rect destination_rect;

    // dimensions reelles de la feuille de sprite.
    int image_width = 0;
    int image_height = 0;

    // index du frame courant et de la direction courante.
    int k = 0;
    int direction_index = 0;

    // controle de la boucle principale.
    bool is_running = true;

    // initialise sdl video.
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        end_sdl(false, SDL_GetError(), NULL, NULL, NULL, NULL);
    }

    // initialise sdl_image pour les formats utilises par le projet.
    if ((IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)) == 0) {
        end_sdl(false, IMG_GetError(), NULL, NULL, NULL, NULL);
    }

    // cree la fenetre de rendu.
    window = SDL_CreateWindow(image_path,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        end_sdl(false, "erreur ouverture window", window, renderer, texture, background_texture);
    }

    // cree le renderer accelere.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        end_sdl(false, "erreur init renderer", window, renderer, texture, background_texture);
    }

    // charge la texture png depuis le disque.
    texture = IMG_LoadTexture(renderer, image_path);
    if (texture == NULL) {
        end_sdl(false, IMG_GetError(), window, renderer, texture, background_texture);
    }

    // charge l'image de fond si elle existe, sinon on utilisera un fond uni.
    if (background_pic != NULL) {
        background_texture = IMG_LoadTexture(renderer, background_pic);
        if (background_texture == NULL) {
            SDL_ClearError();
        }
    }

    // recupere la taille totale de la feuille de sprite.
    if (SDL_QueryTexture(texture, NULL, NULL, &image_width, &image_height) != 0) {
        end_sdl(false, SDL_GetError(), window, renderer, texture, background_texture);
    }

    // calcule la taille d'un sprite individuel.
    const int sprite_width = image_width / FRAME_COUNT;
    const int sprite_height = image_height / DIRECTION_COUNT;

    // prepare les rectangles source correspondant a chaque frame et direction.
    // ordre des lignes dans la feuille : 0 = haut, 1 = gauche, 2 = bas, 3 = droite.
    for (int direction = 0; direction < DIRECTION_COUNT; direction++) {
        for (int frame = 0; frame < FRAME_COUNT; frame++) {
            tab_src[direction][frame].x = frame * sprite_width;
            tab_src[direction][frame].y = direction * sprite_height;
            tab_src[direction][frame].w = sprite_width;
            tab_src[direction][frame].h = sprite_height;
        }
    }

    // place le sprite au centre de la fenetre au demarrage.
    destination_rect.x = (WINDOW_WIDTH - sprite_width) / 2;
    destination_rect.y = (WINDOW_HEIGHT - sprite_height) / 2;
    destination_rect.w = sprite_width;
    destination_rect.h = sprite_height;

    // affiche l'image initiale avant toute saisie clavier.
    SDL_RenderCopy(renderer, background_texture, NULL, NULL);
    SDL_RenderCopy(renderer, texture, &tab_src[direction_index][k], &destination_rect);
    SDL_RenderPresent(renderer);

    // boucle principale de l'application.
    while (is_running) {
        // traite tous les evenements en attente.
        while (SDL_PollEvent(&event)) {
            // ferme la fenetre ou quitte avec echap.
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                is_running = false;
                break;
            }

            // ignore tout ce qui n'est pas une pression de touche.
            if (event.type != SDL_KEYDOWN) {
                continue;
            }

            // met a jour position, direction et frame selon la touche pressee.
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    // deplacement vers le haut et lecture de la ligne d'animation correspondante.
                    destination_rect.y -= vitesse;
                    direction_index = 0;
                    k = (k + 1) % FRAME_COUNT;
                    break;
                case SDLK_LEFT:
                    // deplacement vers la gauche.
                    destination_rect.x -= vitesse;
                    direction_index = 1;
                    k = (k + 1) % FRAME_COUNT;
                    break;
                case SDLK_DOWN:
                    // deplacement vers le bas.
                    destination_rect.y += vitesse;
                    direction_index = 2;
                    k = (k + 1) % FRAME_COUNT;
                    break;
                case SDLK_RIGHT:
                    // deplacement vers la droite.
                    destination_rect.x += vitesse;
                    direction_index = 3;
                    k = (k + 1) % FRAME_COUNT;
                    break;
                default:
                    // revenir a la pose immobile de face : 3e ligne, 1re colonne.
                    direction_index = 2;
                    k = 0;
                    break;
            }

            // redessine l'ecran avec la nouvelle frame du sprite.
            if (background_texture != NULL) {
                SDL_RenderCopy(renderer, background_texture, NULL, NULL);
            } else {
                SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
                SDL_RenderClear(renderer);
            }
            SDL_RenderCopy(renderer, texture, &tab_src[direction_index][k], &destination_rect);
            SDL_RenderPresent(renderer);
        }

        // petite pause pour eviter une boucle trop gourmande.
        SDL_Delay(32);
    }

    // sortie propre du programme.
    end_sdl(true, NULL, window, renderer, texture, background_texture);
}

/**
 * @brief point d'entree du programme.
 *
 * @return EXIT_SUCCESS si l'execution se termine correctement.
 */
int main(void)
{
    // lance l'animation avec la feuille de sprite du squelette.
    const char *image_path = "../img/sk/skeleton.png";
    const char *background_pic = "../img/sk/background.jpg";

    display_movement(image_path, background_pic);
    return EXIT_SUCCESS;
}