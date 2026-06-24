/*
 * main.c
 * point d'entree principal du projet smatron.
 * gere deux flux d'execution distincts via les arguments du terminal (argc/argv) :
 * - mode "play" : boucle en temps reel avec rendu graphique sdl2, a 60 fps.
 * - mode "train" : boucle purement mathematique, sans limitation de temps,
 * optimisee pour executer des milliers d'episodes par minute avec journalisation csv.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/config.h"
#include "../include/graphisme.h"
#include "../include/mj.h"
#include "../include/agent.h"

int main(int argc, char *argv[])
{
    /* * allocation des structures de donnees fondamentales de l'environnement (le monde).
     * on utilise des tableaux statiques (sur la pile/stack) car leur taille est connue a la compilation.
     * l'initialisation a {0} ou {false} garantit qu'il n'y a pas de donnees poubelles en ram.
     */
    int grid[WIDTH][HEIGHT] = {0};
    
    /* * tableau des positions (x,y). la taille est max_motos + 1 car on ignore l'index 0.
     * cela permet de faire correspondre l'identifiant de la moto (ex: cell_player = 1) 
     * directement avec son index dans le tableau (pos_motos[1]).
     */
    int pos_motos[MAX_MOTOS + 1][2] = {0};
    int dir_motos[MAX_MOTOS + 1] = {0};
    bool moto_alive[MAX_MOTOS + 1] = {false};
    bool running = true;

    /* securite : empeche le programme de crasher si l'utilisateur oublie l'argument */
    if (argc < 2) {
        fprintf(stderr, "usage: %s <mode>\n", argv[0]);
        fprintf(stderr, "modes: %s, %s\n", ENTRAINEMENT, JOUEUR);
        return EXIT_FAILURE;
    }

    /* * allocation du "cerveau a court terme" pour chaque agent.
     * episodememoire contient un tableau de frames (etats, actions, probabilites).
     * c'est ce tampon qui sera lu a l'envers lors de la mort pour calculer le gradient.
     */
    EpisodeMemoire memoires[MAX_MOTOS + 1] = {0};

    /* ===================================================================== *
     * mode joueur (avec sdl et ecrans)                                      *
     * ===================================================================== */
    if (strcmp(argv[1], JOUEUR) == 0) {
        
        /* * injection de la matrice de poids (theta) depuis le disque dur vers la ram.
         * sans cela, les ia joueraient avec des probabilites purement aleatoires.
         */
        charger_theta(filename_theta);
        
        /* initialisation du sous-systeme video de la bibliotheque sdl2 */
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            fprintf(stderr, "erreur sdl init : %s\n", SDL_GetError());
            return EXIT_FAILURE;
        }
         if ((IMG_Init(IMG_INIT_WEBP) & IMG_INIT_WEBP) == 0) {
            fprintf(stderr, "erreur IMG_Init : %s\n", IMG_GetError());
            SDL_Quit();
            return EXIT_FAILURE;
        }

        /* * creation de la fenetre au centre de l'ecran. 
         * sdl_window_shown force son affichage immediat.
         */
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

        /* * creation du renderer (le moteur de dessin physique).
         * sdl_renderer_accelerated : utilise la carte graphique (gpu) plutot que le processeur (cpu).
         * sdl_renderer_presentvsync : synchronise la boucle d'affichage avec la frequence 
         * de l'ecran (souvent 60hz) pour eviter les dechirements d'image (screen tearing).
         */
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

        /* * active le calcul du canal alpha (transparence) pour les fonctions de dessin.
         * c'est ce flag specifique qui permet de dessiner les halos lumineux neon 
         * en superposant des couches de couleurs semi-transparentes.
         */
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        if (!initialiser_textures_motos(renderer)) {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
            return EXIT_FAILURE;
        }
        /* * la fonction afficher_ecran_titre bloque l'execution jusqu'a ce que 
         * le joueur appuie sur entree. elle renvoie false s'il ferme la fenetre.
         */
        if (!afficher_ecran_titre(window, renderer)) {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return EXIT_SUCCESS;
        }
        /* * srand initialise la graine (seed) du generateur de nombres pseudo-aleatoires.
        * LA FONCTION RAND() N'EST PAS THREAD-SAFE, POUR LE PARRALÉLISME IL FAUT UTILISER UN GENERATEUR DE NOMBRES ALEATOIRES DEDIE.
         * sdl_getticks() renvoie les millisecondes depuis l'init de la sdl, 
         * garantissant une graine differente a chaque lancement.
         */
        srand((unsigned int)SDL_GetTicks());

        /* placement manuel du joueur humain a gauche, face a la droite */
        pos_motos[CELL_PLAYER][0] = 10;
        pos_motos[CELL_PLAYER][1] = HEIGHT / 2;
        dir_motos[CELL_PLAYER] = DIR_RIGHT;
        moto_alive[CELL_PLAYER] = true;
        grid[pos_motos[CELL_PLAYER][0]][pos_motos[CELL_PLAYER][1]] = CELL_PLAYER;

        /* placement des 3 ia aux differents coins de l'arene */
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

        /* * last_tick stocke le temps exact (en ms) avant d'entrer dans la boucle.
         * cela servira a calculer le "delta time" pour cadencer la vitesse du jeu.
         */
        Uint32 last_tick = SDL_GetTicks();

        while (running) {
            SDL_Event event;

            /* * sdl_pollevent depile les evenements (clavier, souris, fenetre) stockes par l'os.
             * la boucle while la vide completement a chaque frame pour eviter 
             * l'input lag (retard entre la touche et l'action).
             */
            while (SDL_PollEvent(&event)) {
                capturer_evenements(&event, &dir_motos[CELL_PLAYER], &running);
            }

            Uint32 now = SDL_GetTicks();

            /* * limiteur de cadence logique (game tick).
             * on met a jour les positions de la grille uniquement si assez de temps s'est ecoule 
             * (ici 2 * frame_delay_ms). cela dissocie la vitesse du jeu du framerate visuel.
             */
            if (now - last_tick >= 2 * FRAME_DELAY_MS) {
                last_tick = now;

                /* la fonction coeur de la dynamique du jeu (collisions, mouvements) */
                mettre_a_jour_monde(grid, pos_motos, dir_motos, moto_alive, memoires);

                /* comptage des survivants pour declencher la condition de fin de partie */
                int motos_vivantes = 0;
                for (int id = CELL_PLAYER; id <= CELL_AI_3; id++) {
                    if (moto_alive[id]) {
                        motos_vivantes++;
                    }
                }

                if (motos_vivantes <= 1) {
                    bool joueur_gagne = moto_alive[CELL_PLAYER];

                    /* rendu du dernier etat (le crash visuel) et du texte win/lose */
                    dessiner_arene(renderer, grid, pos_motos, dir_motos, moto_alive);
                    dessiner_panneau_fin(renderer, joueur_gagne);
                    SDL_RenderPresent(renderer);

                    /* pause bloquante de 2.5 secondes pour laisser le joueur lire l'ecran */
                    SDL_Delay(2500);

                    running = false;
                    continue; /* force la sortie de la boucle */
                }
            }

            /* rendu visuel a chaque frame (meme si la position n'a pas change) */
            dessiner_arene(renderer, grid, pos_motos, dir_motos, moto_alive);
            SDL_Delay(16); /* ~60 images par secondes (1000ms / 60 = 16.6) */
        }

        /* liberation rigoureuse de la memoire gpu et destruction des contextes */
        liberer_textures_motos();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return EXIT_SUCCESS;

    /* ===================================================================== *
     * mode entrainement (sans sdl, calcul pur, vitesse maximale)            *
     * ===================================================================== */
    } else if (strcmp(argv[1], ENTRAINEMENT) == 0) {
        
        charger_theta(filename_theta);
        printf("demarrage de l'entrainement intensif sans sdl...\n");

        /* * test preliminaire de l'existence du fichier csv.
         * on l'ouvre en lecture ("r"). s'il s'ouvre, c'est qu'il existe deja, 
         * on evite donc de reecrire la ligne d'en-tete (colonnes) au milieu des donnees.
         */
        bool check_existe = false;
        FILE *f_test = fopen("data/logs.csv", "r");
        if (f_test) {
            check_existe = true;
            fclose(f_test);
        }

        /* * ouverture du fichier d'historisation en mode "a" (append / ajout).
         * contrairement au mode "w" qui detruit l'ancien fichier, "a" place 
         * le curseur a la toute fin du texte pour concatener la suite de l'entrainement.
         */
        FILE *f_log = fopen("data/logs.csv", "a");
        if (f_log && !check_existe) {
            fprintf(f_log, "episode,temps_sec,survie_moyenne,survie_max,taux_victoire,g0_moyen,pct_forward\n");
        }

        /* accumulateurs de statistiques. ils lisseront la variance innee de l'exploration aleatoire */
        long somme_frames = 0;
        int max_frames = 0;
        float somme_g0 = 0.0f;
        int victoires = 0;
        long total_actions = 0;
        long total_forward = 0;
        
        /* time(null) renvoie l'heure unix (secondes depuis 1970). sert de point de depart absolu */
        time_t temps_debut_batch = time(NULL);

        int dernier_vainqueur = -1;

        long episode = 1;
        long episodes_totaux = 10000;
        
                /*
                * long strtol( const char * theString, char ** end, int base ); 
                * Cette fonction, signifiant string to long, permet de transformer une chaîne de caractères,
                * contenant la représentation textuelle d'un entier, en un entier type long
                */

            if (argc >= 3) {//nombre d'episodes a executer avant l'arret du programme (pour le batch)

                episodes_totaux = strtol(argv[2], NULL, 10);
                
            }

            if (argc >= 4) {//numero de l'episode a partir duquel reprendre l'entrainement (utile pour les batchs interrompus)

                episode = strtol(argv[3], NULL, 10);

            }

        while (episode <= episodes_totaux && running) {
            
            initialiser_partie(grid, pos_motos, dir_motos, moto_alive);
            
            /* raz du tampon memoire pour eviter que les actions de l'episode n-1 polluent l'episode n */
            for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
                memoires[i].taille = 0;
            }

            bool episode_en_cours = true;
            int frames_ecoulees = 0;

            while (episode_en_cours) {
                /* * copie temporel de l'etat de vie a t-1.
                 * c'est necessaire car la fonction mettre_a_jour modifie l'etat.
                 * pour punir un agent, on doit savoir s'il vient EXACTEMENT de mourir a CETTE frame.
                 */
                bool vie_avant[MAX_MOTOS + 1];
                for (int i = 0; i <= MAX_MOTOS; i++) vie_avant[i] = moto_alive[i];

                /* appel de la fonction dediee ou la case joueur est remplacee par une 4eme ia */
                mettre_a_jour_monde_entrainement(grid, pos_motos, dir_motos, moto_alive, memoires);
                frames_ecoulees++;

                int motos_vivantes = 0;
                dernier_vainqueur = 0; 

                for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
                    
                    /* detection d'un crash exclusif a l'instant t */
                    if (vie_avant[i] && !moto_alive[i]) {
                        if (memoires[i].taille > 0) {
                            
                            /* * punition fatale : on ecrase la derniere recompense de la memoire (+1) 
                             * par le malus critique de mort (-100). l'ia comprendra que l'action 
                             * qui a mene a ce point precis etait catastrophique.
                             */
                            memoires[i].frames[memoires[i].taille - 1].recompense = -100.0f;
                            
                            /* * declenchement de l'algorithme reinforce.
                             * parametre 2 (0.001f) = alpha (learning rate) : micro-pas pour ne pas desequilibrer la matrice d'un coup.
                             * parametre 3 (0.99f) = gamma (discount factor) : l'ia considere qu'une recompense dans 100 frames 
                             * vaut presque autant qu'une recompense immediate, l'aidant a anticiper le piege.
                             */
                            maj_theta(&memoires[i], ALPHA, 0.99f);
                            
                            /* * remise a 0 instantanee de la memoire.
                             * empeche que le cadavre ne continue de recalculer des gradients aux frames suivantes.
                             */
                            memoires[i].taille = 0; 
                        }
                    }
                    
                    if (moto_alive[i]) {
                        motos_vivantes++;
                        dernier_vainqueur = i; /* garde en memoire l'id du dernier potentiel survivant */
                    }
                }

                /* * la limite de 2000 frames est le timeout (early stopping technique).
                 * elle empeche deux agents extremement performants de tourner en boucle infiniment 
                 * sans se toucher, ce qui ferait diverger le gradient vers l'infini (g_t geant).
                 */
                if (motos_vivantes <= 1 || frames_ecoulees >= 2000) {
                    episode_en_cours = false;
                }
            }

            /* --- fin de la partie : collecte silencieuse des metriques --- */
            
            somme_frames += frames_ecoulees;
            if (frames_ecoulees > max_frames) {
                max_frames = frames_ecoulees;
            }
            
            /* une victoire n'est comptee que si l'agent a reellement ecrase tous les autres (pas de double crash) */
            if (dernier_vainqueur >= CELL_PLAYER && dernier_vainqueur <= CELL_AI_3) {
                victoires++;
            }

            /* recompense finale des survivants de l'arret du temps (victoire ou timeout) */
            for (int i = CELL_PLAYER; i <= CELL_AI_3; i++) {
                if (moto_alive[i] && memoires[i].taille > 0) {
                    
                    /* ici, il n'y a pas le malus de -100. on consolide le comportement victorieux */
                    float g0 = maj_theta(&memoires[i], ALPHA, 0.99f);
                    if (i == dernier_vainqueur) {
                        somme_g0 += g0;
                    }
                }
                
                /* * audit des probabilites strategiques.
                 * on scanne la memoire pour voir la proportion d'actions "aller tout droit".
                 * un agent bien entraine arrete de zigzagger et priviliegie la ligne droite (souvent >90%).
                 */
                for (int t = 0; t < memoires[i].taille; t++) {
                    total_actions++;
                    if (memoires[i].frames[t].action == ACTION_FORWARD) {
                        total_forward++;
                    }
                }
            }

            /* --- bilan d'evaluation : affichage et ecriture disque tous les 100 episodes --- */
            if (episode % 100 == 0) {
                
                /* difftime calcule la difference de secondes reelles entre le debut du bloc et maintenant */
                time_t temps_actuel = time(NULL);
                double temps_batch = difftime(temps_actuel, temps_debut_batch);

                /* conversion des sommes en moyennes flottantes (lissage stochastique) */
                float moyenne_frames = (float)somme_frames / 100.0f;
                float moyenne_g0 = somme_g0 / 100.0f;
                float taux_victoire = (float)victoires; 
                float pct_forward = (total_actions > 0) ? ((float)total_forward / total_actions) * 100.0f : 33.3f;

                /* rendu console formaté pour suivre l'evolution en direct sans inonder le terminal */
                printf("[ep %ld] temps: %.0fs | survie moy: %.1f (max:%d) | win: %.0f%% | fwd: %.1f%%\n", 
                       episode, temps_batch, moyenne_frames, max_frames, taux_victoire, pct_forward);

                if (f_log) {
                    fprintf(f_log, "%ld,%.0f,%.1f,%d,%.1f,%.2f,%.1f\n", 
                            episode, temps_batch, moyenne_frames, max_frames, taux_victoire, moyenne_g0, pct_forward);
                    
                    /* * fflush est crucial en data engineering !
                     * le langage c stocke les printf/fprintf dans une memoire tampon invisible (le buffer).
                     * fflush force le vidage de ce tampon vers le ssd immediatement.
                     * ainsi, meme si tu coupes violemment le programme (ctrl+c), ton csv est 100% sauve.
                     */
                    fflush(f_log); 
                }

                /* reinitialisation rigoureuse des compteurs statistiques pour les 100 episodes suivants */
                somme_frames = 0;
                max_frames = 0;
                somme_g0 = 0.0f;
                victoires = 0;
                total_actions = 0;
                total_forward = 0;
                temps_debut_batch = time(NULL);
            }

            /* sauvegarde binaire du cerveau regulierement au cas ou le pc plante */
            if (episode % 5000 == 0) {
                sauvegarder_theta(filename_theta);
            }

            episode++;
        }

        /* proprete de sortie : fermeture du fichier et sauvegarde ultime des matrices theta */
        if (f_log) fclose(f_log);
        printf("entrainement termine avec succes.\n");
        sauvegarder_theta(filename_theta);

        return EXIT_SUCCESS;

    
    } else {
        /* le programme se referme net si l'argument ne correspond ni a play ni a train */
        fprintf(stderr, "mode inconnu: %s\n", argv[1]);
        fprintf(stderr, "modes: %s, %s\n", ENTRAINEMENT, JOUEUR);
        return EXIT_FAILURE;
    }
}