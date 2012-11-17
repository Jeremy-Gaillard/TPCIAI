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
	statut_t* shm_statut;


  sem_t* sem_piece;
  sem_t* sem_carton;
  sem_t* sem_erreur_carton;

} arg_carton_t;

typedef struct arg_palette
{
	statut_t* shm_statut;
	lot_t* shm_lot;

	sem_t* sem_carton;
	sem_t* sem_palette;
  sem_t* sem_erreur_palette;

} arg_palette_t;

typedef struct arg_cariste
{
	lot_t* shm_lot;
	entrepot_t* shm_entrepot;

	sem_t* sem_palette;
	pthread_mutex_t* mutex_entrepot;
} arg_cariste_t;

void init_prod( sem_t* sem_arret_urgence );
void arret_urgence_prod(int signum);

void fin_production(int signum);

int carton( arg_carton_t* args );

int palette( arg_palette_t* args );

int cariste( arg_cariste_t* args );

void gerer_erreur( int erreur_id );

#endif
