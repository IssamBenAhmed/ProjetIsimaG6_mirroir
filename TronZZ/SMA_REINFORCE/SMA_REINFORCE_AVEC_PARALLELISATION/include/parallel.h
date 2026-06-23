#ifndef PARALLEL_H
#define PARALLEL_H

/*
 * Fichier parallel.h
 *
 * Ici on annonce les fonctions de parallélisation.
 */

/*
 * Lance un entraînement parallèle.
 *
 * nb_threads : nombre de threads utilisés
 * episodes_par_thread : nombre de parties jouées par chaque thread
 */
void lancer_entrainement_parallele(int nb_threads, int episodes_par_thread);

#endif