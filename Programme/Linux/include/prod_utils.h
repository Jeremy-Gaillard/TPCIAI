#ifndef PROD_UTILS_H
#define PROD_UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

#include "config.h"

/* struct passée en paramètre au lancement de carton, elle contient
   les ressources nécessaires au bon déroulement de carton */
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

/* struct passée en paramètre au lancement de palette, elle contient
   les ressources nécessaires au bon déroulement de palette */
typedef struct arg_palette
{
	statut_t* shm_statut;

	sem_t* sem_carton;
	sem_t* sem_palette;
	sem_t* sem_erreur_palette;
	
	pthread_mutex_t* mutex_erreur;

} arg_palette_t;

/* struct passée en paramètre au lancement de cariste, elle contient
   les ressources nécessaires au bon déroulement de cariste */
typedef struct arg_cariste
{
	lot_t* shm_lot;
	entrepot_t* shm_entrepot;

	sem_t* sem_palette;
	pthread_mutex_t* mutex_entrepot;
	
	pthread_mutex_t* mutex_disque;
	pthread_mutex_t* mutex_windows;
} arg_cariste_t;

/* Envoi message de log a bal_log_disque et bal_log_windows
   Celles-ci sont protégées par des mutex nommés identiquement
   Message de type "L C id_carton type_pièce %_pièces_défectueuses HHMMSS" */
void log_carton( mqd_t bal_log_disque, mqd_t bal_log_windows,
                 int carton_id, char type_piece, int nb_rebus, 
                 pthread_mutex_t* mutex_windows, pthread_mutex_t* mutex_disque);

/* Initialise les informations sur les commandes a réaliser
   cmd_A cmd_B, le type à produire type_piece, en regardant dans la mémoire shm_lot */
void init_carton( int* cmd_A, int* cmd_B, char* type_piece,
                  int* max_rebus, lot_t* shm_lot );

/* Tâche carton */
int carton( arg_carton_t* args );

/* Tâche palette */
int palette( arg_palette_t* args );

/* Envoi message de log a bal_log_disque et bal_log_windows
   Celles-ci sont protegées par des mutex nommés identiquement
   Message de type "L P id_palette type_pièce horaire" */
void log_cariste( mqd_t bal_log_disque, mqd_t bal_log_windows,
                 int palette_id, char type_piece , 
                 pthread_mutex_t* mutex_windows, pthread_mutex_t* mutex_disque);

/* Tâche cariste*/
int cariste( arg_cariste_t* args );

/* Envoi message de log à bal_log_erreur 
   Celle-ci est protégée par un mutex 
   Message de type "E id_erreur HHMMSS"*/
void gerer_erreur( int erreur_id, pthread_mutex_t* mutex_erreur );

#endif
