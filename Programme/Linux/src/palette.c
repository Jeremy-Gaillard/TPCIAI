#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <mqueue.h>
#include <signal.h>

#include "config.h"
#include "prod_utils.h"

int palette( sem_t sem_carton, sem_t sem_palette, sem_t sem_erreur_palette,
             statut_t shm_statut , lot_t shm_lot ){
         
         /*Création du Handler de fin de tâche et démasquage de SIGUSR2*/
	struct sigaction handler_USR2;
	handler_USR2.sa_handler = fin_simulation;
	sigaction ( SIGUSR2, &handler_USR2, NULL );
 	
 	/*Création du Handler d'arret d urgence et démasquage de SIGUSR1*/
	struct sigaction handler_USR1;
	handler_USR1.sa_handler = AU;
	sigaction ( SIGUSR1, &handler_USR1, NULL );	
	
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
