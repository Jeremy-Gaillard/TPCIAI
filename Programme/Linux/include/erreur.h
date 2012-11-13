#ifndef ERREUR_H
#define ERREUR_H

#include "config.h"
#include <semaphore.h>

struct arg_erreur
{
	statut_t* statut;
	sem_t* bal_erreur;
	sem_t* bal_log_win;
	sem_t* bal_log_disque;
};
	
void erreur(struct arg_erreur* ipc/*statut_t* shm_statut, sem_t* erreur, sem_t* windows, sem_t* disque*/);

#endif

