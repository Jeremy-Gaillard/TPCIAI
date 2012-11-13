#ifndef PROD_UTILS_H
#define PROD_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <semaphore.h>
#include "config.h"

typedef struct arg_carton
{
	statut_t* shm_statut;

	sem_t* bal_erreur;
	sem_t* bal_log_win;
	sem_t* bal_log_disque;

  sem_t* sem_piece;
  sem_t* sem_carton;
  sem_t* sem_erreur_carton;
} arg_carton_t;

void fin_production(int signum);

void arret_urgence_prod(int signum);

int carton( arg_carton_t args );

int palette();

int cariste();

void gerer_erreur( int erreur_id );

#endif
