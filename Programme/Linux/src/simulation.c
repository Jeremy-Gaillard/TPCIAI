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

static sem_t* sem_clapet;
static pthread_t t_envoi_piece;

void envoi_piece(arg_envoi_piece_t* ipc)
{
	statut_t* shm_statut = ipc->statut;
	sem_t* sem_piece = ipc->piece;
	for( ; ; )
	{
		if( (*shm_statut)[ST_CLAPET_OUVERT] == 1 )
		{
			sleep(1);
			sem_post( sem_piece );
		}
		else
			sem_wait( sem_clapet );
	}
	pthread_exit( 0 );
}

void simulation(arg_simulation_t* ipc)
{
	statut_t* shm_statut = ipc->statut;
	sem_clapet = ipc->clapet;
	pthread_t t_carton = ipc->t_carton;
	pthread_t t_palette = ipc->t_palette;

	int bal_log_disque = mq_open( BALDIS, O_WRONLY );
	char commande[20];
	
	for( ; ; )
	{
		scanf("%s", commande);
		if( !strcmp(commande, "logd") )
		{
			mq_send(bal_log_disque, "L C 10 666666", sizeof(log_t), BAL_PRIO_ELSE);
		}
		if( !strcmp(commande, "logdf") )
		{
			mq_send(bal_log_disque, TRAME_FIN, sizeof(log_t), BAL_PRIO_FIN);
		}
		else if( !strcmp(commande, "presence_carton") )
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
		else if( !strcmp(commande, "presence_carton") )
		{
			(*shm_statut)[ ST_PRESENCE_CARTON ] = 1;
		}
		else if( !strcmp(commande, "absence_carton") )
		{
			(*shm_statut)[ ST_PRESENCE_CARTON ] = 0;
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
		else if( !strcmp(commande, "clapet_ouvert") )
		{
			(*shm_statut)[ ST_CLAPET_OUVERT ] = 1;
			sem_post( sem_clapet );
			pthread_join( t_envoi_piece, NULL );
		}
		else if( !strcmp(commande, "clapet_ferme") )
		{
			(*shm_statut)[ ST_CLAPET_OUVERT ] = 0;
		}
		else if( !strcmp(commande, "AU") )
		{
			pthread_kill( t_carton, SIGUSR1 );
			pthread_kill( t_palette, SIGUSR1 );
		}
		if( !strcmp(commande, "q") )
		{
			break;
		}
	}
	pthread_exit(0);
}
