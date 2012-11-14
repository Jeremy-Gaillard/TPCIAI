#include <stdio.h>
#include <stdlib.h>

/* #include <mqueue.h> */
#include <semaphore.h>
#include <signal.h>
#include <time.h>

#include "config.h"
#include "prod_utils.h"


static sem_t* sem_AU;

void AU_palette(int signum)
{
	printf("PRODUCTION: PALETTE: ARRET D'URGENCE !\n");
	sem_wait(sem_AU);
}

int palette( arg_palette_t* args ){
         
	/* Récupération des ressources */
	/* mqd_t bal_log_disque = mq_open(BALDIS, O_WRONLY); */
	/* mqd_t bal_log_windows = mq_open(BALWIN, O_WRONLY); */

	statut_t* shm_statut = args.shm_statut;
	lot_t* shm_lot = args.shm_lot;

	sem_t* sem_bal_erreur = args.bal_erreur;
	/* sem_t* sem_bal_log_win = args.bal_log_win; */
	sem_t* sem_bal_log_disque = args.bal_log_disque;

	sem_t* sem_carton = args.sem_carton;
	sem_t* sem_palette = args.sem_palette;
	sem_t* sem_erreur_palette = args.sem_erreur_palette;

	sem_AU = args.sem_AU;

	/*Création du Handler de fin de tâche et démasquage de SIGUSR2*/
	struct sigaction handler_USR2;
	handler_USR2.sa_handler = fin_production;
	sigaction ( SIGUSR2, &handler_USR2, NULL );
 	
 	/*Création du Handler d'arret d urgence et démasquage de SIGUSR1*/
	struct sigaction handler_USR1;
	handler_USR1.sa_handler = AU_palette;
	sigaction ( SIGUSR1, &handler_USR1, NULL );

	/* Création des variables locales */
	int nb_carton = 0;
	int nb_palette = 0;

	for ( ; ; ){
		sem_wait( sem_carton );
		if ( nb_carton == 0 && (*shm_statut)[ ST_PRESENCE_PALETTE ] != 1 ){
			
			gerer_erreur( ERR_PAS_DE_PALETTE, sem_bal_erreur, sem_bal_log_disque );
			sem_wait( sem_erreur_palette );
		}/*end if palette absente*/
		
		nb_carton += 1;
		if ( nb_carton == PALETTE_PLEINE ){
			if ( (*shm_statut)[ ST_FILM ] != 1 ){

				gerer_erreur( ERR_FILM_KO, sem_bal_erreur, sem_bal_log_disque );
				sem_wait( sem_erreur_palette );	
			}/*end if film_KO*/
			
			nb_palette += 1;
			 
			sem_post( sem_palette );
			nb_carton = 0;
			
			/*On part du principe qu'on produit du A puis du B
			  ET que shm_lot[ LOT_A ] est mis à 0 une fois la
			  production finie*/
			if ( nb_palette == (*shm_lot)[ LOT_A ] ){
			/*Si productionde A finie*/
				nb_palette = 0;
			}
			else if ( nb_palette == (*shm_lot)[ LOT_B ] ){
				/*Si productionde B finie*/
				nb_palette = 0;
			}
		}
		/*end if palette pleine*/
	}
}
