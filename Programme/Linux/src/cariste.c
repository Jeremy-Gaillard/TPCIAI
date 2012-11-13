#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <mqueue.h>

#include "config.h"
#include "prod_utils.h"
 
 int cariste( mqd_t bal_erreur, mqd_t bal_log_disque, mqd_t bal_log_windows,
              sem_t sem_palette, pthread_mutex_t mutex_entrepot, lot_t shm_lot ){
 	int nb_palette = 0;
	int i = 0;
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
		
		/*ENVOYER LES MESSAGES DE LOG!*/
		
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
