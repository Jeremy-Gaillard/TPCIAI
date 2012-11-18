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
                  int palette_id, char type_piece ) {

		char heure[7];
		time_t rawtime;
		struct tm * timeinfo;
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		strftime ( heure, 7, "%H%M%S", timeinfo );

		log_t message;/*nb palette(int=15) + heure (=6) +reste message (7) = 28*/

		sprintf(message, "L P %d %c %s", palette_id, type_piece, heure);
		mq_send( bal_log_disque, message, sizeof( log_t ), BAL_PRIO_ELSE );
		mq_send( bal_log_windows, message, sizeof( log_t ), BAL_PRIO_ELSE );

}

int cariste( arg_cariste_t* args ){
	
	/* Récupération des ressources */

	mqd_t bal_log_disque = mq_open(BALDIS, O_WRONLY);
	mqd_t bal_log_windows = mq_open(BALWIN, O_WRONLY);

	lot_t* shm_lot = args->shm_lot;
	entrepot_t* shm_entrepot = args->shm_entrepot;

	pthread_mutex_t* mutex_entrepot = args->mutex_entrepot;
	sem_t* sem_palette = args->sem_palette;

	/* Création des variables locales */
 	int nb_palette = 0;
	int i = 0;
	char type_piece = 'A';

 	for ( ; ; ){
		sem_wait( sem_palette );
		type_piece = ( (*shm_lot)[LOT_A] > 0 ? 'A' : 'B' );

		nb_palette += 1;
		pthread_mutex_lock ( mutex_entrepot );
		i = 0;
		for( ; shm_entrepot->palettes[ i ].id != NO_PALETTE && i < 20 ; i += 1 );
		
		if ( i == 20 ){
			printf("j ai mange une palette. Om Nom Nom Nom \n Affectueusement le cariste\n");
		}
		else
		{
			shm_entrepot->palettes[ i ].id = nb_palette;
			shm_entrepot->palettes[i].type = type_piece;
			time_t rawtime;
			struct tm * timeinfo;	
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			strftime ( shm_entrepot->palettes[ i ].heure, 7, "%H%M%S", timeinfo );
		}/*palette rangee*/
		pthread_mutex_unlock( mutex_entrepot );
		
		log_cariste( bal_log_disque, bal_log_windows,
		             nb_palette, type_piece );
		
		/*Fin de production d'un lot: mise a 0 du lot a produire*/
		if ( (*shm_lot)[ LOT_A ] == nb_palette ){
			(*shm_lot) [ LOT_A ] = 0;
			nb_palette = 0;
		}
		else if ( (*shm_lot)[ LOT_A ] == 0 && (*shm_lot)[ LOT_B ] == nb_palette ){
			(*shm_lot)[ LOT_B ] = 0;
			nb_palette = 0;
		}
	}
 }
