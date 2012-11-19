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
	lot_t* shm_lot;


	sem_t* sem_piece;
	sem_t* sem_carton;
	sem_t* sem_erreur_carton;
	
	pthread_mutex_t* mutex_disque;
	pthread_mutex_t* mutex_windows;
	pthread_mutex_t* mutex_erreur;	

} arg_carton_t;

typedef struct arg_palette
{
	statut_t* shm_statut;
	lot_t* shm_lot;

	sem_t* sem_carton;
	sem_t* sem_palette;
	sem_t* sem_erreur_palette;
	
	pthread_mutex_t* mutex_erreur;

} arg_palette_t;

typedef struct arg_cariste
{
	lot_t* shm_lot;
	entrepot_t* shm_entrepot;

	sem_t* sem_palette;
	pthread_mutex_t* mutex_entrepot;
	
	pthread_mutex_t* mutex_disque;
	pthread_mutex_t* mutex_windows;
} arg_cariste_t;


void init_prod( sem_t* sem_arret_urgence );


void log_carton( mqd_t bal_log_disque, mqd_t bal_log_windows,
                 int carton_id, char type_piece, int nb_rebus, 
                 pthread_mutex_t* mutex_windows, pthread_mutex_t* mutex_disque);

void init_carton( int* cmd_A, int* cmd_B, char* type_piece,
                  lot_t* shm_lot );

int carton( arg_carton_t* args );


int palette( arg_palette_t* args );


void log_cariste( mqd_t bal_log_disque, mqd_t bal_log_windows,
                 int palette_id, char type_piece , 
                 pthread_mutex_t* mutex_windows, pthread_mutex_t* mutex_disque);

int cariste( arg_cariste_t* args );


void gerer_erreur( int erreur_id, pthread_mutex_t* mutex_erreur );

#endif
