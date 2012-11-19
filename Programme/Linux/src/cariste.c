#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>

#include "config.h"
#include "prod_utils.h"

void log_cariste( mqd_t bal_log_disque, mqd_t bal_log_windows,
                  int palette_id, char type_piece, pthread_mutex_t* mutex_windows,
                  pthread_mutex_t* mutex_disque) {

	char heure[7];
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime ( heure, 7, "%H%M%S", timeinfo );

	log_t message;/*nb palette(int=15) + heure (=6) +reste message (7) = 28*/

	sprintf(message, "L P %d %c %s", palette_id, type_piece, heure);
	pthread_mutex_lock( mutex_disque );
	mq_send( bal_log_disque, message, sizeof( log_t ), BAL_PRIO_ELSE );
	pthread_mutex_unlock( mutex_disque );
		
	pthread_mutex_lock( mutex_windows );
	mq_send( bal_log_windows, message, sizeof( log_t ), BAL_PRIO_ELSE );
	pthread_mutex_unlock( mutex_windows );
}

void ranger_palette( int place, int nb_palette, char type_piece,
                     entrepot_t* shm_entrepot ) {

	shm_entrepot->palettes[place].id = nb_palette;
	shm_entrepot->palettes[place].type = type_piece;
	time_t rawtime;
	struct tm * timeinfo;	
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime ( shm_entrepot->palettes[ place ].heure, 7, "%H%M%S", timeinfo );

}


int cariste( arg_cariste_t* args ){
	
	/* Récupération des ressources */

	mqd_t bal_log_disque = mq_open(BALDIS, O_WRONLY);
	mqd_t bal_log_windows = mq_open(BALWIN, O_WRONLY);

	lot_t* shm_lot = args->shm_lot;
	entrepot_t* shm_entrepot = args->shm_entrepot;

	pthread_mutex_t* mutex_entrepot = args->mutex_entrepot;
	pthread_mutex_t* mutex_disque = args->mutex_disque;
	pthread_mutex_t* mutex_windows = args->mutex_windows;
	sem_t* sem_palette = args->sem_palette;

	/* Création des variables locales */
	int nb_palette = 0;
	char type_piece = 'A';
	int i;

	for ( ; ; ){
		sem_wait( sem_palette );

		nb_palette += 1;
		if ( type_piece=='A' && nb_palette==(*shm_lot)[LOT_A] ) {
			type_piece = 'B';
			nb_palette = 0;
		}

		pthread_mutex_lock ( mutex_entrepot );
		for( i=0 ; shm_entrepot->palettes[ i ].id != NO_PALETTE && i < TAILLE_ENTREPOT ; i++ );
		
		if ( i == TAILLE_ENTREPOT ){
			/* Erreur non supportée : on suppose notre entrepôt de taille infinie.
			   Le cariste a mangé la palette pour que personne ne remarque rien.
			   Brave bête. */
		}
		else
			ranger_palette( i, nb_palette, type_piece, shm_entrepot );

		pthread_mutex_unlock( mutex_entrepot );
		
		log_cariste( bal_log_disque, bal_log_windows,
		             nb_palette, type_piece, mutex_windows, mutex_disque);

	}
}
