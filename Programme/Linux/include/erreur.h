#ifndef ERREUR_H
#define ERREUR_H

#include "config.h"
#include <pthread.h>

typedef struct arg_erreur
{
	pthread_mutex_t* mutex_disque;
	pthread_mutex_t* mutex_windows;
	statut_t* statut;
} arg_erreur_t;
	
void erreur(arg_erreur_t* ipc);

#endif

