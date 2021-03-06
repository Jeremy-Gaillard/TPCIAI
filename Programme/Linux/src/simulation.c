#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#include <errno.h>

#include "simulation.h"
#include "commande_windows.h"

static pthread_t t_envoi_piece;

/*Procédure chargée d'envoyer les pièces sur la chaine de production*/
void envoi_piece(arg_envoi_piece_t* ipc)
{
	statut_t* shm_statut = ipc->statut;
	sem_t* sem_piece = ipc->piece;
	sem_t* sem_clapet = ipc->clapet;
	for( ; ; )
	{
                /*Si le clapet est ouvert, on envoi les pieces, sinon, on attend son ouverture*/
		if( (*shm_statut)[ST_CLAPET_OUVERT] == 1 )
		{
			sem_post( sem_piece );
			sleep(1);
		}
		else
			sem_wait( sem_clapet );
	}
	pthread_exit( 0 );
}

/*Procédure chargée d'interpréter les commandes utilisateurs et de les traduire pour le système*/
void simulation(arg_simulation_t* ipc)
{
	statut_t* shm_statut = ipc->statut;
	sem_t* sem_clapet = ipc->clapet;
	pthread_t t_carton = ipc->t_carton;
	pthread_t t_palette = ipc->t_palette;

	char commande[20];
	
	for( ; ; )
	{
                /*Récupération de la commande entrée par l'utilisateur*/
		scanf("%s", commande);
                /*Cette partie est chargée de modifier le bon segment de mémoire dans shm_status en fonction de la commande utilisateur*/
		if( !strcmp(commande, "presence_carton") )
		{
			(*shm_statut)[ ST_PRESENCE_CARTON ] = 1;
		}
		else if( !strcmp(commande, "absence_carton") )
		{
			(*shm_statut)[ ST_PRESENCE_CARTON ] = 0;
		}
		else if( !strcmp(commande, "presence_palette") )
		{
			(*shm_statut)[ ST_PRESENCE_PALETTE ] = 1;
		}
		else if( !strcmp(commande, "absence_palette") )
		{
			(*shm_statut)[ ST_PRESENCE_PALETTE ] = 0;
		}
		else if( !strcmp(commande, "imprimante_OK") )
		{
			(*shm_statut)[ ST_IMPRIMANTE ] = 1;
		}
		else if( !strcmp(commande, "err_imprimante_KO") )
		{
			(*shm_statut)[ ST_IMPRIMANTE ] = 0;
		}
		else if( !strcmp(commande, "piece_OK") )
		{
			(*shm_statut)[ ST_PIECE ] = 1;
		}
		else if( !strcmp(commande, "piece_KO") )
		{
			(*shm_statut)[ ST_PIECE ] = 0;
		}
		else if( !strcmp(commande, "film_OK") )
		{
			(*shm_statut)[ ST_FILM ] = 1;
		}
		else if( !strcmp(commande, "film_KO") )
		{
			(*shm_statut)[ ST_FILM ] = 0;
		}
                /*En cas d'arret d'urgence, on envoie SIGUSR1 aux threads carton et palette*/
		else if( !strcmp(commande, "AU") )
		{
			pthread_kill( t_carton, SIGUSR1 );
			pthread_kill( t_palette, SIGUSR1 );
		}
	}
	pthread_exit(0);
}
