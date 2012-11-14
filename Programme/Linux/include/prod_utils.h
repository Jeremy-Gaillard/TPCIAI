#ifndef PROD_UTILS_H
#define PROD_UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

#include "config.h"


typedef struct arg_carton
{
	statut_t shm_statut;

	sem_t* bal_erreur;
	sem_t* bal_log_win;
	sem_t* bal_log_disque;

  sem_t* sem_piece;
  sem_t* sem_carton;
  sem_t* sem_erreur_carton;

	sem_t* sem_AU;
} arg_carton_t;

typedef struct arg_palette
{
	statut_t shm_statut;
	lot_t shm_lot;

	/* sem_t* bal_erreur; */
	/* sem_t* bal_log_win; */
	/* sem_t* bal_log_disque; */

	sem_t* sem_carton;
	sem_t* sem_palette;
  sem_t* sem_erreur_palette;

	sem_t* sem_AU;
} arg_palette_t;

typedef struct arg_cariste
{
	lot_t shm_lot;

	sem_t* bal_erreur;
	sem_t* bal_log_win;
	sem_t* bal_log_disque;

	sem_t* palette;
	pthread_mutex_t* mutex_entrepot;
} arg_cariste_t;


void fin_production(int signum);

void arret_urgence_prod(int signum);


void AU_carton( int signum);
int carton( arg_carton_t args );

void AU_palette( int signum );
int palette();

int cariste();

void gerer_erreur( int erreur_id,
                   sem_t* sem_bal_erreur, sem_t* sem_bal_log_disque );

#endif
