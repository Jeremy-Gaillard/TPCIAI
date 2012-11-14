#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>

#include "config.h"
#include "prod_utils.h"
 
int cariste( arg_cariste_t* args ){
	
	/* Récupération des ressources */

	mqd_t bal_log_disque = mq_open(BALDIS, O_WRONLY);
	mqd_t bal_log_windows = mq_open(BALWIN, O_WRONLY);

	lot_t* shm_lot = args->shm_lot;
	entrepot_t* shm_entrepot = args->shm_entrepot;

	pthread_mutex_t* mutex_entrepot = args->mutex_entrepot;
	sem_t* sem_palette = args->sem_palette;

	/* sem_t* sem_bal_erreur = args->bal_erreur; */
	sem_t* sem_bal_log_win = args->bal_log_win;
	sem_t* sem_bal_log_disque = args->bal_log_disque;

 	/*Création du Handler de fin de tâche et démasquage de SIGUSR2*/
	struct sigaction handler_USR2;
	handler_USR2.sa_handler = fin_production;
	sigaction ( SIGUSR2, &handler_USR2, NULL );
 	
 	/* /\*Création du Handler d'arret d urgence et démasquage de SIGUSR1*\/ */
	/* struct sigaction handler_USR1; */
	/* handler_USR1.sa_handler = AU; */
	/* sigdelset( &handler_USR1.sa_mask, SIGUSR2 ); */
	/* sigaction ( SIGUSR1, &handler_USR1, NULL ); */

	/* Création des variables locales */
 	int nb_palette = 0;
	int i = 0;

 	for ( ; ; ){
		sem_wait( sem_palette );
		nb_palette += 1;
		pthread_mutex_lock ( mutex_entrepot );
		i = 0;
		for( ; shm_entrepot->palettes[ i ].id == 0 || i < 20 ; i += 1 ){
			shm_entrepot->palettes[ i ].id = nb_palette;
			if ( (*shm_lot)[ LOT_A ] == 0 ){
				shm_entrepot->palettes[ i ].type = 'B';
			}
			else{
				shm_entrepot->palettes[ i ].type = 'A';
			}
			time_t rawtime;
			struct tm * timeinfo;	
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			strftime ( shm_entrepot->palettes[ i ].heure, 7, "%H%M%S", timeinfo );
		}/*palette rangee*/
		if ( i == 20 ){
			printf("j ai mange une palette. Om Nom Nom Nom \n Affectueusement le cariste\n");
		}
		pthread_mutex_unlock( mutex_entrepot );
		
		/*envoi logs*/
		char heure[7];
		time_t rawtime;
		struct tm * timeinfo;
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		strftime ( heure, 7, "%H%M%S", timeinfo );

		log_t* message_log= malloc(sizeof(log_t));
		sprintf(*message_log, "L P %d %s", nb_palette,heure);
		
		sem_post( sem_bal_log_disque );
		mq_send( bal_log_disque, *message_log, sizeof( *message_log ), BAL_PRIO_ELSE );
		sem_post( sem_bal_log_win );
		mq_send( bal_log_windows, *message_log, sizeof( *message_log ), BAL_PRIO_ELSE );
		/*fin envoi logs*/
		
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
