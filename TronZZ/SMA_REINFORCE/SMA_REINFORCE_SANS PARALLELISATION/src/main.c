#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../include/config.h"
#include "../include/graphisme.h"
#include "../include/mj.h"

int main(int argc, char *argv[])
{



    int grid[WIDTH][HEIGHT] = {0};
    int pos_motos[MAX_MOTOS + 1][2] = {0};
    int dir_motos[MAX_MOTOS + 1] = {0};
    bool moto_alive[MAX_MOTOS + 1] = {false};
    bool running = true;



    (void)argc;
    if (argv[1] == NULL) {
        fprintf(stderr, "Usage: %s <mode>\n", argv[0]);
        fprintf(stderr, "Modes: %s, %s\n", ENTRAINEMENT, JOUEUR);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], JOUEUR) == 0) {

         if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "erreur sdl init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
        }

        SDL_Window *window = SDL_CreateWindow(
            "SmaTron - Arène SMA",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN
        );


        if (window == NULL) {
            fprintf(stderr, "erreur creation fenetre : %s\n", SDL_GetError());
            SDL_Quit();
            return EXIT_FAILURE;
        }

        SDL_Renderer *renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );

        if (renderer == NULL) {
            fprintf(stderr, "erreur creation renderer : %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return EXIT_FAILURE;
        }

         /*
        * Permet d'utiliser les couleurs avec transparence.
        * Utile pour la grille, les effets lumineux et le panneau de fin.
        */
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        srand((unsigned int)SDL_GetTicks());

        /* Initialisation du Joueur Humain */
        pos_motos[CELL_PLAYER][0] = 10;
        pos_motos[CELL_PLAYER][1] = HEIGHT / 2;
        dir_motos[CELL_PLAYER] = DIR_RIGHT;
        moto_alive[CELL_PLAYER] = true;
        grid[pos_motos[CELL_PLAYER][0]][pos_motos[CELL_PLAYER][1]] = CELL_PLAYER;

        /* Initialisation de l'IA 1 */
        pos_motos[CELL_AI_1][0] = WIDTH - 11;
        pos_motos[CELL_AI_1][1] = 10;
        dir_motos[CELL_AI_1] = DIR_LEFT;
        moto_alive[CELL_AI_1] = true;
        grid[pos_motos[CELL_AI_1][0]][pos_motos[CELL_AI_1][1]] = CELL_AI_1;

        /* Initialisation de l'IA 2 */
        pos_motos[CELL_AI_2][0] = 10;
        pos_motos[CELL_AI_2][1] = HEIGHT - 11;
        dir_motos[CELL_AI_2] = DIR_UP;
        moto_alive[CELL_AI_2] = true;
        grid[pos_motos[CELL_AI_2][0]][pos_motos[CELL_AI_2][1]] = CELL_AI_2;

        /* Initialisation de l'IA 3 */
        pos_motos[CELL_AI_3][0] = WIDTH - 11;
        pos_motos[CELL_AI_3][1] = HEIGHT - 11;
        dir_motos[CELL_AI_3] = DIR_UP;
        moto_alive[CELL_AI_3] = true;
        grid[pos_motos[CELL_AI_3][0]][pos_motos[CELL_AI_3][1]] = CELL_AI_3;

        Uint32 last_tick = SDL_GetTicks();

        while (running) {
            SDL_Event event;

            while (SDL_PollEvent(&event)) {
                capturer_evenements(&event, &dir_motos[CELL_PLAYER], &running);
        
            

            Uint32 now = SDL_GetTicks();
            if (now - last_tick >= 2 * FRAME_DELAY_MS) {
                last_tick = now;

                mettre_a_jour_monde(grid, pos_motos, dir_motos, moto_alive);

                int motos_vivantes = 0;
                for (int id = CELL_PLAYER; id <= CELL_AI_3; id++) {
                    if (moto_alive[id]) {
                        motos_vivantes++;
                    }
                }

                if (motos_vivantes <= 1) {
                    dessiner_arene(renderer, grid);
                    dessiner_panneau_fin(renderer);
                    SDL_RenderPresent(renderer);

                    SDL_Delay(2000);

                    running = false;
                    continue;
                }
            }

            dessiner_arene(renderer, grid);
            SDL_Delay(16);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return EXIT_SUCCESS;
    }

    } 
    else if (strcmp(argv[1], ENTRAINEMENT) == 0) {
        
    } else {
        fprintf(stderr, "Mode inconnu: %s\n", argv[1]);
        fprintf(stderr, "Modes: %s, %s\n", ENTRAINEMENT, JOUEUR);
        return EXIT_FAILURE;
    }

    


    

    
}