#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "../include/config.h"
#include "../include/graphisme.h"
#include "../include/mj.h"
#include "../include/agent.h"

int main(int argc, char *argv[])
{
    int grid[WIDTH][HEIGHT] = {0};
    int pos_motos[MAX_MOTOS + 1][2] = {0};
    int dir_motos[MAX_MOTOS + 1] = {0};
    bool moto_alive[MAX_MOTOS + 1] = {false};
    bool running = true;

    if (argc < 2) {
        fprintf(stderr, "usage: %s <mode>\n", argv[0]);
        fprintf(stderr, "modes: %s, %s\n", ENTRAINEMENT, JOUEUR);
        return EXIT_FAILURE;
    }

    /* allocation de la memoire pour les agents */
    EpisodeMemoire memoires[MAX_MOTOS + 1] = {0};

    /* ===================================================================== *
     * mode joueur (avec sdl et ecrans)                                      *
     * ===================================================================== */
    if (strcmp(argv[1], JOUEUR) == 0) {
        
        charger_theta(filename_theta);
        
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            fprintf(stderr, "erreur sdl init : %s\n", SDL_GetError());
            return EXIT_FAILURE;
        }

        SDL_Window *window = SDL_CreateWindow(
            "smatron - arene sma",
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

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        if (!afficher_ecran_titre(window, renderer)) {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return EXIT_SUCCESS;
        }

        srand((unsigned int)SDL_GetTicks());

        pos_motos[CELL_PLAYER][0] = 10;
        pos_motos[CELL_PLAYER][1] = HEIGHT / 2;
        dir_motos[CELL_PLAYER] = DIR_RIGHT;
        moto_alive[CELL_PLAYER] = true;
        grid[pos_motos[CELL_PLAYER][0]][pos_motos[CELL_PLAYER][1]] = CELL_PLAYER;

        pos_motos[CELL_AI_1][0] = WIDTH - 11;
        pos_motos[CELL_AI_1][1] = 10;
        dir_motos[CELL_AI_1] = DIR_LEFT;
        moto_alive[CELL_AI_1] = true;
        grid[pos_motos[CELL_AI_1][0]][pos_motos[CELL_AI_1][1]] = CELL_AI_1;

        pos_motos[CELL_AI_2][0] = 10;
        pos_motos[CELL_AI_2][1] = HEIGHT - 11;
        dir_motos[CELL_AI_2] = DIR_UP;
        moto_alive[CELL_AI_2] = true;
        grid[pos_motos[CELL_AI_2][0]][pos_motos[CELL_AI_2][1]] = CELL_AI_2;

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
            }

            Uint32 now = SDL_GetTicks();

            if (now - last_tick >= 2 * FRAME_DELAY_MS) {
                last_tick = now;

                mettre_a_jour_monde(grid, pos_motos, dir_motos, moto_alive, memoires);

                int motos_vivantes = 0;

                for (int id = CELL_PLAYER; id <= CELL_AI_3; id++) {
                    if (moto_alive[id]) {
                        motos_vivantes++;
                    }
                }

                if (motos_vivantes <= 1) {
                    bool joueur_gagne = moto_alive[CELL_PLAYER];

                    dessiner_arene(renderer, grid);
                    dessiner_panneau_fin(renderer, joueur_gagne);
                    SDL_RenderPresent(renderer);

                    SDL_Delay(2500);

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

    /* ===================================================================== *
     * mode entrainement (sans sdl, calcul pur)                              *
     * ===================================================================== */
    } else if (strcmp(argv[1], ENTRAINEMENT) == 0) {
        
        charger_theta(filename_theta);
        printf("demarrage de l'entrainement intensif sans sdl...\n");

        time_t temps_debut = time(NULL);
        time_t dernier_rapport = temps_debut;
        
        int dernier_vainqueur = -1;
        float dernier_G0 = 0.0f;
        int derniere_duree = 0;

        long episode = 1;
        long episodes_totaux = 10000;

        /* ajout de la condition d'arret a 10 000 episodes */
        while (episode <= episodes_totaux && running) {
            
            initialiser_partie(grid, pos_motos, dir_motos, moto_alive);
            
            for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
                memoires[i].taille = 0;
            }

            bool episode_en_cours = true;
            int frames_ecoulees = 0;

            while (episode_en_cours) {
                bool vie_avant[MAX_MOTOS + 1];
                for (int i = 0; i <= MAX_MOTOS; i++) vie_avant[i] = moto_alive[i];

                mettre_a_jour_monde_entrainement(grid, pos_motos, dir_motos, moto_alive, memoires);
                frames_ecoulees++;

                int motos_vivantes = 0;
                dernier_vainqueur = 0; 

                for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
                    if (vie_avant[i] && !moto_alive[i]) {
                        if (memoires[i].taille > 0) {
                            memoires[i].frames[memoires[i].taille - 1].recompense = -100.0f;
                            float g0 = maj_theta(&memoires[i], 0.001f, 0.99f);
                            if (i == CELL_PLAYER) dernier_G0 = g0; 
                            memoires[i].taille = 0;
                        }
                    }
                    if (moto_alive[i]) {
                        motos_vivantes++;
                        dernier_vainqueur = i;
                    }
                }

                if (motos_vivantes <= 1 || frames_ecoulees >= 2000) {
                    episode_en_cours = false;
                }
            }

            /* stockage de la duree de la partie pour l'affichage */
            derniere_duree = frames_ecoulees;

            for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
                if (moto_alive[i] && memoires[i].taille > 0) {
                    float g0 = maj_theta(&memoires[i], 0.001f, 0.99f);
                    if (i == dernier_vainqueur) dernier_G0 = g0;
                }
            }

            if (episode % 5000 == 0) {
                sauvegarder_theta(filename_theta);
            }

            time_t temps_actuel = time(NULL);
            if (temps_actuel - dernier_rapport >= TEMPS_ENTRE_RAPPORTS) {
                long sec_totales = (long)(temps_actuel - temps_debut);
                
                /* affichage de la duree in-game ajoute */
                printf("[chronometre: %02ldh %02ldm %02lds] episode n°%ld | vainqueur id %d | duree: %d frames | g_0: %.2f\n",
                       sec_totales / 3600, (sec_totales % 3600) / 60, sec_totales % 60,
                       episode, dernier_vainqueur, derniere_duree, dernier_G0);
                       
                dernier_rapport = temps_actuel;
                sauvegarder_theta(filename_theta); 
            }

            episode++;
        }

        /* sauvegarde finale obligatoire a la fin de la boucle */
        printf("entrainement termine avec succes (%ld episodes).\n", episode - 1);
        sauvegarder_theta(filename_theta);

        return EXIT_SUCCESS;

    } else {
        fprintf(stderr, "mode inconnu: %s\n", argv[1]);
        fprintf(stderr, "modes: %s, %s\n", ENTRAINEMENT, JOUEUR);
        return EXIT_FAILURE;
    }
}