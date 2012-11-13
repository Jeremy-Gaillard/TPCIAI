#ifndef ERREUR_H
#define ERREUR_H

#include "config.h"
#include <semaphore.h>

typedef struct arg_erreur
{
	statut_t* statut;
	sem_t* bal_erreur;
	sem_t* bal_log_win;
	sem_t* bal_log_disque;
} arg_erreur_t;
	
void erreur(arg_erreur_t* ipc);

#endif

