#ifndef PROD_UTILS_H
#define PROD_UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

#include "config.h"

/*struct passee en parametre au lancement de carton, elle contient
les ressources necessaire au bon deroulement de carton*/
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

/*struct passee en parametre au lancement de palette, elle contient
les ressources necessaire au bon deroulement de palette*/
typedef struct arg_palette
{
	statut_t* shm_statut;

	sem_t* sem_carton;
	sem_t* sem_palette;
	sem_t* sem_erreur_palette;
	
	pthread_mutex_t* mutex_erreur;

} arg_palette_t;

/*struct passee en parametre au lancement de cariste, elle contient
les ressources necessaire au bon deroulement de cariste*/
typedef struct arg_cariste
{
	lot_t* shm_lot;
	entrepot_t* shm_entrepot;

	sem_t* sem_palette;
	pthread_mutex_t* mutex_entrepot;
	
	pthread_mutex_t* mutex_disque;
	pthread_mutex_t* mutex_windows;
} arg_cariste_t;

/*envoi message de log a bal_log_disque et bal_log_windows
celles ci protegees par des mutex nommee identiquement
message de type "L C id_carton type_pièce %_pièces_défectueuses HHMMSS"*/
void log_carton( mqd_t bal_log_disque, mqd_t bal_log_windows,
                 int carton_id, char type_piece, int nb_rebus, 
                 pthread_mutex_t* mutex_windows, pthread_mutex_t* mutex_disque);

/*initialise les informations sur les commandes a realiser
cmd_A cmd_B, le type a produire type_piece, en regardant dans la memoire shm_lot*/
void init_carton( int* cmd_A, int* cmd_B, char* type_piece,
                  int* max_rebus, lot_t* shm_lot );

/*tache carton*/
int carton( arg_carton_t* args );

/*tache palette*/
int palette( arg_palette_t* args );

/*envoi message de log a bal_log_disque et bal_log_windows
celles ci protegees par des mutex nommee identiquement
message de type "L P id_palette type_pièce horaire"*/
void log_cariste( mqd_t bal_log_disque, mqd_t bal_log_windows,
                 int palette_id, char type_piece , 
                 pthread_mutex_t* mutex_windows, pthread_mutex_t* mutex_disque);

/*tache cariste*/
int cariste( arg_cariste_t* args );

/*envoi message de log a bal_log_erreur 
celle ci protegee par un mutex 
message de type "E id_erreur HHMMSS"*/
void gerer_erreur( int erreur_id, pthread_mutex_t* mutex_erreur );

#endif
