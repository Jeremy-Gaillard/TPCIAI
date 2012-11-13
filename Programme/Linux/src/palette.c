#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <mqueue.h>

#include "config.h"
#include "prod_utils.h"

int palette( sem_t sem_carton, sem_t sem_palette, sem_t sem_erreur_palette,
             statut_t shm_statut , lot_t shm_lot ){
	
	int nb_carton = 0;
	int nb_palette = 0;
	for ( ; ; ){
		sem_wait( &sem_carton );
		if ( nb_carton == 0 && shm_statut[ ST_PRESENCE_PALETTE ] != 1 ){
			
			gerer_erreur( ERR_ABSENCE_PALETTE);
			sem_wait( &sem_erreur_palette );
		}/*end if palette absente*/
		
		nb_carton += 1;
		if ( nb_carton == PALETTE_PLEINE ){
			if ( shm_statut[ ST_FILM ] != 1 ){

				gerer_erreur( ERR_FILM_KO );
				sem_wait( &sem_erreur_palette );	
			}/*end if film_KO*/
			
			nb_palette += 1;

			/*envoi logs*/
			char heure[7];
			time_t rawtime;
			struct tm * timeinfo;
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			strftime ( heure, 7, "%H%M%S", timeinfo );
	
			char* message= malloc(28);/*nb palette(int=15) + heure (=6) +reste message (7) = 28*/

			sprintf(message, "L P %d %s", nb_palette,heure);
			mqd_t bal_log_disque = mq_open(BALDIS, O_WRONLY);
			mqd_t bal_log_windows = mq_open(BALWIN, O_WRONLY);
			mq_send( bal_log_disque, message, sizeof( message ), BAL_PRIO_ELSE );
			mq_send( bal_log_windows, message, sizeof( message ), BAL_PRIO_ELSE );
			/*fin envoi logs*/
			 
			sem_post( &sem_palette );
			nb_carton = 0;
			
			/*On part du principe qu'on produit du A puis du B
			  ET que shm_lot[ LOT_A ] est mis à 0 une fois la
			  production finie*/
			if ( nb_palette == shm_lot[ LOT_A ] ){
			/*Si productionde A finie*/
				nb_palette = 0;
			}
			else if ( nb_palette == shm_lot[ LOT_B ] ){
				/*Si productionde B finie*/
				nb_palette = 0;
			}
		}
		/*end if palette pleine*/
	}
}
