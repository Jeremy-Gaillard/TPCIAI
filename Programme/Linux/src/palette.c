#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <mqueue.h>

#include "config.h"
#include "prod_utils.h"

int palette( mqd_t bal_erreur, mqd_t bal_log_disque, mqd_t bal_log_windows,
             sem_t sem_carton, sem_t sem_palette, sem_t sem_erreur_palette,
             statut_t shm_statut , lot_t shm_lot ){
	
	int nb_carton = 0;
	int nb_palette = 0;
	for ( ; ; ){
		sem_wait( &sem_carton );
		if ( nb_carton == 0 && shm_statut[ ST_PRESENCE_PALETTE ] != 1 ){
			
			gerer_erreur( ERR_ABSENCE_PALETTE, bal_erreur );
			sem_wait( &sem_erreur_palette );
		}/*end if palette absente*/
		
		nb_carton += 1;
		if ( nb_carton == PALETTE_PLEINE ){
			if ( shm_statut[ ST_FILM ] != 1 ){

				gerer_erreur( ERR_FILM_KO, bal_erreur );
				sem_wait( &sem_erreur_palette );	
			}/*end if film_KO*/
			
			nb_palette += 1;			
			/* ENVOIE MESSAGE LOGS
			QUAND CHAMPI AURA FINI*/
			 
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
