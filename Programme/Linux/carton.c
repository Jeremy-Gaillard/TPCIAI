#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "config.h"
#include "prod_utils.h"

int carton( mqd_t bal_erreur, mdq_t bal_log_disque, mdq_t bal_log_windows,
            sem_t sem_piece, sem_t sem_carton, sem_t sem_erreur_carton,
            statut_t shm_statut ){
	int nb_piece = 0;
	int nb_carton = 0;
	int nb_rebus =0;
	int place_file_attente;

	for( ; ; ){
		/* attente piece */
		sem_wait( &sem_piece ); 

		if ( nb_piece == 0 && shm_statut[ST_PRESENCE_CARTON] != 1 ){
		/*si premiere piece et absence carton
		 envoi d'un message d'erreur avec hhmmss et type erreur
		 puis attente sur semaphore de reprise d'erreur*/
		 		
			gerer_erreur_carton(ERR_PAS_DE_CARTON, bal_erreur, sem_erreur_carton);
			sem_wait( &sem_erreur_carton );
		}
		/*end of absence carton*/
		
		if ( shm_statut[ ST_PIECE_OK ] == 1 ){
			nb_piece += 1;
			if ( nb_piece == CARTON_PLEIN ){
				if ( shm_statut[ST_IMPRIMANTE] != 1 ){
				/*si carton plein et imprimante HS
				 envoie d'un message d'erreur avec hhmmss et type erreur
				 puis attente sur semaphore de reprise d'erreur*/

					gerer_erreur_carton(ERR_IMPRIMANTE_KO, bal_erreur, sem_erreur_carton);
					sem_wait( &sem_erreur_carton );
				}
				/*end of if imprimante HS*/
				
				sem_getvalue( &sem_carton, &place_file_attente );
				if ( place_file_attente == 0 ){
				/*si trop de cartons dans la file d'attente
				 envoie d'un message d'erreur avec hhmmss et type erreur
				 puis attente sur semaphore de reprise d'erreur*/
				 
					gerer_erreur_carton(ERR_FILE_D_ATTENTE, bal_erreur, sem_erreur_carton);
					sem_wait( &sem_erreur_carton );
				}
				/*end of if file attente pleine*/
				
				sem_post( &sem_carton );
				
				/* NOT IMPLEMENTED YET
				envoyer bal log_windoxs et bal_log_disque 
				l'info carton plein*/
				nb_piece = 0;
				nb_rebus = 0;
				nb_carton = (nb_carton==PALETTE_PLEINE) ? 0 : nb_carton;
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
			 	
				gerer_erreur_carton(ERR_TROP_DE_REBUS, bal_erreur, sem_erreur_carton);
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
