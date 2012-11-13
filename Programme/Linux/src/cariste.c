#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <mqueue.h>
#include <signal.h>

#include "config.h"
#include "prod_utils.h"
 
 int cariste( sem_t sem_palette, pthread_mutex_t mutex_entrepot, lot_t shm_lot ){
 	/*Création du Handler de fin de tâche et démasquage de SIGUSR2*/
	struct sigaction handler_USR2;
	handler_USR2.sa_handler = fin_simulation;
	sigaction ( SIGUSR2, &handler_USR2, NULL );
 	
 	/*Création du Handler d'arret d urgence et démasquage de SIGUSR1*/
	struct sigaction handler_USR1;
	handler_USR1.sa_handler = AU;
	sigaction ( SIGUSR1, &handler_USR1, NULL );
	
 	int nb_palette = 0;
	int i = 0;
	mqd_t bal_log_disque = mq_open(BALDIS, O_WRONLY);
	mqd_t bal_log_windows = mq_open(BALWIN, O_WRONLY);
 	for ( ; ; ){
		sem_wait( &sem_palette );
		nb_palette += 1;
		pthread_mutex_lock ( &mutex_entrepot );
		i = 0;
		for( ; shm_entrepot[ i ].id == 0 || i < 20 ; i += 1 ){
			shm_entrepot[ i ].id = nb_palette;
			if ( shm_lot[ LOT_A ] == 0 ){
				shm_entrepot[ i ].type = 'B';
			}
			else{
				shm_entrepot[ i ].type = 'A';
			}
			time_t rawtime;
			struct tm * timeinfo;	
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			strftime ( shm_entrepot[ i ].heure, 7, "%H%M%S", timeinfo );
		}/*palette rangee*/
		if ( i = 20 ){
			printf("j ai mange une palette. Om Nom Nom Nom \n Affectueusement le cariste\n");
		}
		pthread_mutex_unlock( &mutex_entrepot );
		
		/*envoi logs*/
		char heure[7];
		time_t rawtime;
		struct tm * timeinfo;
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		strftime ( heure, 7, "%H%M%S", timeinfo );

		char* message= malloc(28);/*nb palette(int=15) + heure (=6) +reste message (7) = 28*/

		sprintf(message, "L P %d %s", nb_palette,heure);
		mq_send( bal_log_disque, message, sizeof( message ), BAL_PRIO_ELSE );
		mq_send( bal_log_windows, message, sizeof( message ), BAL_PRIO_ELSE );
		/*fin envoi logs*/
		
		/*Fin de production d'un lot: mise a 0 du lot a produire*/
		if ( shm_lot[ LOT_A ] == nb_palette ){
			shm_lot [ LOT_A ] = 0;
			nb_palette = 0;
		}
		else if ( shm_lot[ LOT_A ] == 0 && shm_lot[ LOT_B ] == nb_palette ){
			shm_lot[ LOT_B ] == 0;
			nb_palette = 0;
		}
	}
 }
