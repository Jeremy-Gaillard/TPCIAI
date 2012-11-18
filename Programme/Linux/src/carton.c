#include <stdio.h>
#include <stdlib.h>

#include <mqueue.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>

#include "config.h"
#include "prod_utils.h"


int carton( arg_carton_t* args ){

	/* Récupération des ressources */
	mqd_t bal_log_disque = mq_open(BALDIS, O_WRONLY);
	mqd_t bal_log_windows = mq_open(BALWIN, O_WRONLY);

	statut_t* shm_statut = args->shm_statut;
	lot_t* shm_lot = args->shm_lot;

	sem_t* sem_piece = args->sem_piece;
	sem_t* sem_carton = args->sem_carton;
	sem_t* sem_erreur_carton = args->sem_erreur_carton;
	
	pthread_mutex_t* mutex_disque = args->mutex_disque;
	pthread_mutex_t* mutex_windows = args->mutex_windows;
	pthread_mutex_t* mutex_erreur = args->mutex_erreur;
	

	/* Création des variables locales */
	int nb_piece = 0;
	int nb_carton = 0;
	int nb_rebus = 0;
	int max_rebus = CARTON_PLEIN * (*shm_lot)[REBUS] / 100;
	int place_file_attente;

	for( ; ; ){
		/* attente piece */
		sem_wait( sem_piece ); 

		if ( nb_piece == 0 && (*shm_statut)[ST_PRESENCE_CARTON] != 1 ){
			/*si premiere piece et absence carton
			  envoi d'un message d'erreur avec hhmmss et type erreur
			  puis attente sur semaphore de reprise d'erreur*/
		 		
			gerer_erreur(ERR_PAS_DE_CARTON, mutex_erreur );
			sem_wait( sem_erreur_carton );
		}
		/*end of absence carton*/
		
		if ( (*shm_statut)[ ST_PIECE ] == 1 ){
			nb_piece += 1;
			if ( nb_piece == CARTON_PLEIN ){
				if ( (*shm_statut)[ST_IMPRIMANTE] != 1 ){
					/*si carton plein et imprimante HS
					  envoie d'un message d'erreur avec hhmmss et type erreur
					  puis attente sur semaphore de reprise d'erreur*/

					gerer_erreur(ERR_IMPRIMANTE_KO, mutex_erreur);
					sem_wait( sem_erreur_carton );
				}
				/*end of if imprimante HS*/
				
				sem_getvalue( sem_carton, &place_file_attente );
				if ( place_file_attente == 10 ){
					/*si trop de cartons dans la file d'attente
					  envoie d'un message d'erreur avec hhmmss et type erreur
					  puis attente sur semaphore de reprise d'erreur*/
				 
					gerer_erreur(ERR_FILE_D_ATTENTE, mutex_erreur);
					sem_wait( sem_erreur_carton );
				}
				/*end of if file attente pleine*/
				
				sem_post( sem_carton );
				
				/*envoi logs*/
				char heure[7];
				time_t rawtime;
				struct tm * timeinfo;
				time ( &rawtime );
				timeinfo = localtime ( &rawtime );
				strftime ( heure, 7, "%H%M%S", timeinfo );
				int pourcent_rebus = (100*nb_rebus)/MAX_REBUS;
				log_t message;
				sprintf(message, "L C %d %d %s", nb_carton,pourcent_rebus,heure);

				pthread_mutex_lock( mutex_disque );
				mq_send( bal_log_disque, message, sizeof( log_t ),
				         BAL_PRIO_ELSE );
				pthread_mutex_unlock( mutex_disque );
				
				pthread_mutex_lock( mutex_windows );
				mq_send( bal_log_windows, message, sizeof( log_t ),
				         BAL_PRIO_ELSE );
				pthread_mutex_unlock( mutex_windows );
				/*fin envoi logs*/
				
				nb_piece = 0;
				nb_rebus = 0;
				if (++nb_carton==PALETTE_PLEINE)
					nb_carton=0;
			}
			/*end of if carton plein*/
		
		}
		/*end of if piece OK*/
		
		else{
			nb_rebus +=1;
			if ( nb_rebus >= max_rebus ){
				/*si trop de mauvaise piece
				  envoie d'un message d'erreur avec hhmmss et type erreur
				  puis attente sur semaphore de reprise d'erreur
				  puis on jette le carton en cours*/
			 	
				gerer_erreur(ERR_TROP_DE_REBUS, mutex_erreur);
				sem_wait( sem_erreur_carton );
				printf("Fin erreur rebus\n");

				nb_piece = 0;
				nb_rebus = 0;
			}
			/*end of if trop rebus atteint*/
		}
		/*end of if piece KO */
	}
	/*end of for(;;)*/ 	
}
