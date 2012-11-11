#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include "config.h"

int carton(){
	int nb_piece = 0;
	int nb_carton = 0;
	int nb_rebus =0;
	int place_file_attente;
	time_t rawtime;
	struct tm * timeinfo;

	for( ; ; ){
		/* attente piece */
		sem_wait( &sem_piece ); 

		if ( nb_piece == 0 && shm_statut [ precence_carton ] != '1' ){
		/*si premiere piece et absence carton
		 envoie d'un message d'erreur avec hhmmss et type erreur
		 puis attente sur semaphore de reprise d'erreur*/
		 		
			struct erreur absence_carton;
  			time ( &rawtime );
  			timeinfo = localtime ( &rawtime );
			strftime ( absence_carton.heure, 7, "%H%M%S", timeinfo );
			absence_carton.id=2;
			
			mq_send( bal_erreur, absence_carton, sizeof( absence_carton ),
				1 );
			
			sem_wait( &sem_erreur_carton );
		}
		/*end of absence carton*/
		
		if ( shm_statut[ piece_OK ] == '1' ){
			nb_piece += 1;
			if ( nb_piece == CARTON_PLEIN ){
				if ( shm_statut[ imprimante ] != '1' ){
				/*si carton plein et imprimante HS
				 envoie d'un message d'erreur avec hhmmss et type erreur
				 puis attente sur semaphore de reprise d'erreur*/
				 
					struct erreur imprimante_KO;
					time ( &rawtime );
					timeinfo = localtime ( &rawtime );
					strftime ( imprimante_KO.heure, 7,
						"%H%M%S", timeinfo );
					imprimante_KO.id=3;

					mq_send( bal_erreur, imprimante_KO,
					 sizeof( imprimante_KO ), 1 );
					 
					sem_wait( &sem_erreur_carton ); 
				}
				/*end of if imprimante HS*/
				
				sem_getvalue( &sem_carton, &place_file_attente );
				if ( place_file_attente == 0 ){
				/*si trop de cartons dans la file d'attente
				 envoie d'un message d'erreur avec hhmmss et type erreur
				 puis attente sur semaphore de reprise d'erreur*/
				 
					struct erreur file_pleine;
		  			time ( &rawtime );
		  			timeinfo = localtime ( &rawtime );
					strftime ( file_pleine.heure, 7,
						"%H%M%S", timeinfo );
					imprimante_KO.id=3;

					mq_send( bal_erreur, file_pleine,
					 sizeof( file_pleine ), 1 );
					 
					 sem_wait( &sem_erreur_carton );
					 
				}
				/*end of if file attente pleine*/
				
				sem_post( &sem_carton );
				
				/* NOT IMPLEMENTED YET
				envoyer bal log_windoxs et bal_log_disque 
				l'info carton plein*/
				nb_piece = 0;
				nb_rebus = 0;
			}
			/*end of if carton plein*/
		
		}
		/*end of if piece OK*/
		
		else{
			nb_rebus +=1;
			if ( nb_rebus == MAX_REBUS ){
			/*si trop de mauvaise piece
			 envoie d'un message d'erreur avec hhmmss et type erreur
			 puis attente sur semaphore de reprise d'erreur
			 puis on jette le carton en cours*/
			 		
				struct erreur trop_rebus;
		  			time ( &rawtime );
		  			timeinfo = localtime ( &rawtime );
					strftime ( trop_rebus.heure, 7,
						"%H%M%S", timeinfo );
				trop_rebus.id=2;

				mq_send( bal_erreur, trop_rebus, 
					sizeof( trop_rebus ) , 1 );
			
				sem_wait( &sem_erreur_carton );
				nb_piece = 0;
				nb_rebus = 0;
			}
			/*end of if trop rebus atteint*/
		}
		/*end of if piece KO */
	}
	/*end of for(;;)*/ 	
}
