// Programme SDL pour afficher et déplacer un sprite animé avec les flèches.

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Dimensions de la fenêtre SDL.
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Vitesse de déplacement du sprite à chaque pression de touche.
#define vitesse 5

// La feuille d'animation contient 9 images par ligne.
#define FRAME_COUNT 9

// La feuille est organisée en 4 lignes, une ligne par direction.
#define DIRECTION_COUNT 4

// Libère proprement les ressources SDL et termine le programme.
static void end_sdl(bool ok, const char *message, SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture, SDL_Texture *background_texture)
{
    // Affiche le message si une erreur doit être signalée.
    if (message != NULL) {
        fprintf(stderr, "%s\n", message);
    }

    // Détruit la texture si elle a été créée.
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
    }

    // Détruit la texture de fond si elle a été créée.
    if (background_texture != NULL) {
        SDL_DestroyTexture(background_texture);
    }

    // Détruit le renderer s'il existe.
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }

    // Détruit la fenêtre si elle existe.
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }

    // Ferme SDL_image puis SDL.
    IMG_Quit();
    SDL_Quit();

    // Termine le programme avec le code approprié.
    exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

// Charge la feuille de sprite et déplace l'image affichée selon les flèches.
static void display_movement(const char *image_path, const char *background_pic)
{
    // Pointeurs SDL principaux.
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Texture *background_texture = NULL;

    // Variables de gestion des événements et des rectangles SDL.
    SDL_Event event;
    SDL_Rect tab_src[DIRECTION_COUNT][FRAME_COUNT];
    SDL_Rect destination_rect;

    // Dimensions réelles de la feuille de sprite.
    int image_width = 0;
    int image_height = 0;

    // Index du frame courant et de la direction courante.
    int k = 0;
    int direction_index = 0;

    // Contrôle de la boucle principale.
    bool is_running = true;

    // Initialise SDL vidéo.
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        end_sdl(false, SDL_GetError(), NULL, NULL, NULL, NULL);
    }

    // Initialise SDL_image pour le PNG.
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        end_sdl(false, IMG_GetError(), NULL, NULL, NULL, NULL);
    }

    // Crée la fenêtre de rendu.
    window = SDL_CreateWindow(image_path,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == NULL) {
        end_sdl(false, "erreur ouverture window", window, renderer, texture, background_texture);
    }

    // Crée le renderer accéléré.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        end_sdl(false, "erreur init renderer", window, renderer, texture, background_texture);
    }

    // Charge la texture PNG depuis le disque.
    texture = IMG_LoadTexture(renderer, image_path);
    if (texture == NULL) {
        end_sdl(false, IMG_GetError(), window, renderer, texture, background_texture);
    }

    // Charge l'image de fond si elle existe, sinon on utilisera un fond uni.
    if (background_pic != NULL) {
        background_texture = IMG_LoadTexture(renderer, background_pic);
        if (background_texture == NULL) {
            SDL_ClearError();
        }
    }

    // Récupère la taille totale de la feuille de sprite.
    if (SDL_QueryTexture(texture, NULL, NULL, &image_width, &image_height) != 0) {
        end_sdl(false, SDL_GetError(), window, renderer, texture, background_texture);
    }

    // Calcule la taille d'un sprite individuel.
    const int sprite_width = image_width / FRAME_COUNT;
    const int sprite_height = image_height / DIRECTION_COUNT;

    // Prépare les rectangles source correspondant à chaque frame et direction.
    // Ordre des lignes dans la feuille : 0 = haut, 1 = gauche, 2 = bas, 3 = droite.
    for (int direction = 0; direction < DIRECTION_COUNT; direction++) {
        for (int frame = 0; frame < FRAME_COUNT; frame++) {
            tab_src[direction][frame].x = frame * sprite_width;
            tab_src[direction][frame].y = direction * sprite_height;
            tab_src[direction][frame].w = sprite_width;
            tab_src[direction][frame].h = sprite_height;
        }
    }

    // Place le sprite au centre de la fenêtre au démarrage.
    destination_rect.x = (WINDOW_WIDTH - sprite_width) / 2;
    destination_rect.y = (WINDOW_HEIGHT - sprite_height) / 2;
    destination_rect.w = sprite_width;
    destination_rect.h = sprite_height;

    // Affiche l'image initiale avant toute saisie clavier.
    SDL_RenderCopy(renderer, background_texture, NULL, NULL);
    SDL_RenderCopy(renderer, texture, &tab_src[direction_index][k], &destination_rect);
    SDL_RenderPresent(renderer);

    // Boucle principale de l'application.
    while (is_running) {
        // Traite tous les événements en attente.
        while (SDL_PollEvent(&event)) {
            // Ferme la fenêtre ou quitte avec Échap.
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                is_running = false;
                break;
            }

            // Ignore tout ce qui n'est pas une pression de touche.
            if (event.type != SDL_KEYDOWN) {
                continue;
            }

            // Met à jour position, direction et frame selon la touche pressée.
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    // Déplacement vers le haut et lecture de la ligne d'animation correspondante.
                    destination_rect.y -= vitesse;
                    direction_index = 0;
                    k = (k + 1) % FRAME_COUNT;
                    break;
                case SDLK_LEFT:
                    // Déplacement vers la gauche.
                    destination_rect.x -= vitesse;
                    direction_index = 1;
                    k = (k + 1) % FRAME_COUNT;
                    break;
                case SDLK_DOWN:
                    // Déplacement vers le bas.
                    destination_rect.y += vitesse;
                    direction_index = 2;
                    k = (k + 1) % FRAME_COUNT;
                    break;
                case SDLK_RIGHT:
                    // Déplacement vers la droite.
                    destination_rect.x += vitesse;
                    direction_index = 3;
                    k = (k + 1) % FRAME_COUNT;
                    break;
                default:
                    // Revenir à la pose immobile de face : 3e ligne, 1re colonne.
                    direction_index = 2;
                    k = 0;
                    break;
            }

            // Redessine l'écran avec la nouvelle frame du sprite.
            if (background_texture != NULL) {
                SDL_RenderCopy(renderer, background_texture, NULL, NULL);
            } else {
                SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
                SDL_RenderClear(renderer);
            }
            SDL_RenderCopy(renderer, texture, &tab_src[direction_index][k], &destination_rect);
            SDL_RenderPresent(renderer);
        }

        // Petite pause pour éviter une boucle trop gourmande.
        SDL_Delay(32);
    }

    // Sortie propre du programme.
    end_sdl(true, NULL, window, renderer, texture, background_texture);
}

// Point d'entrée du programme.
int main(void)
{
    // Lance l'animation avec la feuille de sprite du squelette.
    const char *image_path = "../img/sk/skeleton.png";
    const char *background_pic = "../img/sk/background.png";

    display_movement(image_path, background_pic);
    return EXIT_SUCCESS;
}