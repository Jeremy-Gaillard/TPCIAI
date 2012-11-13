#ifndef COMMANDE_WINDOWS_H
#define COMMANDE_WINDOWS_H

#include "config.h"
#include <semaphore.h>
#include <pthread.h>

typedef struct arg_commande_windows
{
	statut_t* shm_statut;
	lot_t* shm_lot;
	entrepot_t* shm_entrepot;
	pthread_mutex_t* entrepot;
	sem_t* palette;
	sem_t* carton;
	sem_t* AU;
	sem_t* clapet;
} arg_commande_windows_t;

void reprise(int erreur_id);
void terminaison();
void commander_lot(int nb_A, int nb_B);
void expedier_lot(int nb_A, int nb_B);
void commande_windows(arg_commande_windows_t* ipc);

#endif

