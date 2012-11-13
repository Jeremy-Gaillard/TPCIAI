#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include "config.h"
#include "prod_utils.h"

int carton( arg_carton_t args ){

	int nb_piece = 0;
	int nb_carton = 0;
	int nb_rebus =0;
	int place_file_attente;
	mqd_t bal_log_disque = mq_open(BALDIS, O_WRONLY);
	mqd_t bal_log_windows = mq_open(BALWIN, O_WRONLY);

	statut_t* shm_statut = args.shm_statut;

	sem_t* sem_bal_erreur = args.bal_erreur;
	sem_t* sem_bal_log_win = args.bal_log_win;
	sem_t* sem_bal_log_disque = args.bal_log_disque;

  sem_t* sem_piece = args.sem_piece;
  sem_t* sem_carton = args.sem_carton;
  sem_t* sem_erreur_carton = args.sem_erreur_carton;
  
	for( ; ; ){
		/* attente piece */
		sem_wait( sem_piece ); 

		if ( nb_piece == 0 && shm_statut[ST_PRESENCE_CARTON] != 1 ){
		/*si premiere piece et absence carton
		 envoi d'un message d'erreur avec hhmmss et type erreur
		 puis attente sur semaphore de reprise d'erreur*/
		 		
			gerer_erreur(ERR_PAS_DE_CARTON);
			sem_wait( sem_erreur_carton );
		}
		/*end of absence carton*/
		
		if ( shm_statut[ ST_PIECE ] == 1 ){
			nb_piece += 1;
			if ( nb_piece == CARTON_PLEIN ){
				if ( shm_statut[ST_IMPRIMANTE] != 1 ){
				/*si carton plein et imprimante HS
				 envoie d'un message d'erreur avec hhmmss et type erreur
				 puis attente sur semaphore de reprise d'erreur*/

					gerer_erreur(ERR_IMPRIMANTE_KO );
					sem_wait( sem_erreur_carton );
				}
				/*end of if imprimante HS*/
				
				sem_getvalue( sem_carton, &place_file_attente );
				if ( place_file_attente == 0 ){
				/*si trop de cartons dans la file d'attente
				 envoie d'un message d'erreur avec hhmmss et type erreur
				 puis attente sur semaphore de reprise d'erreur*/
				 
					gerer_erreur(ERR_FILE_D_ATTENTE );
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
				char* message= malloc(30);/*id erreur(int=15) + heure (=6) + +erreur (2) +reste ressage (7) = 16*/
				sprintf(message, "L C %d %d %s", nb_carton,pourcent_rebus,heure);

				mq_send( bal_log_disque, message, sizeof( message ), BAL_PRIO_ELSE );
        sem_post( sem_bal_log_disque );
				mq_send( bal_log_windows, message, sizeof( message ), BAL_PRIO_ELSE );
        sem_post( sem_bal_log_win );
				/*fin envoi logs*/
				
				nb_piece = 0;
				nb_rebus = 0;
				nb_carton = (nb_carton==PALETTE_PLEINE) ? 0 : nb_carton+1;
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
			 	
				gerer_erreur(ERR_TROP_DE_REBUS );
				sem_wait( sem_erreur_carton );

				nb_piece = 0;
				nb_rebus = 0;
			}
			/*end of if trop rebus atteint*/
		}
		/*end of if piece KO */
	}
	/*end of for(;;)*/ 	
}
