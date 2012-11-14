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
static statut_t* shm_statut;
static pthread_t t_envoi_piece;

/*INUTILISE
void fin_simulation(int signum)
{
	printf("Simulation catching : %i\n", signum);
	pthread_kill( t_envoi_piece, SIGUSR2 );
	pthread_join( t_envoi_piece, NULL );
	printf("Envoi piece termine! Fin simulation");
	pthread_exit( 0 );
}

void fin_envoi_piece(int signum)
{
	printf("Envoi piece catching : %i\n", signum);
	pthread_exit( 0 );
}*/

void envoi_piece(sem_t* sem_piece)
{
	/*Création du Handler de fin de tâche et démasquage de SIGUSR2*/
	/*struct sigaction handler_USR2;
	handler_USR2.sa_handler = fin_envoi_piece;
	sigaction ( SIGUSR2, &handler_USR2, NULL );*/
	for( ; ; )
	{
		sem_post( sem_piece );
		sleep(1);
	}
	pthread_exit( 0 );
}

void simulation(arg_simulation_t* ipc)
{
	/*Création du Handler de fin de tâche et démasquage de SIGUSR2*/
	/*struct sigaction handler_USR2;
	handler_USR2.sa_handler = fin_simulation;
	sigaction ( SIGUSR2, &handler_USR2, NULL );*/
	
	shm_statut = ipc->statut;
	sem_clapet = ipc->clapet;
	sem_t* sem_piece = ipc->piece;
	sem_t* sem_disque = ipc->disque;
	sem_t* sem_windows = ipc->windows;
	sem_t* sem_erreur = ipc->erreur;

	int bal_log_disque = mq_open( BALDIS, O_WRONLY );
	char commande[20];
	
	for( ; ; )
	{
		scanf("%s", commande);
		if( !strcmp(commande, "logd") )
		{
			mq_send(bal_log_disque, "L C 10 666666", sizeof(log_t), BAL_PRIO_ELSE);
			sem_post(sem_disque);
		}
		if( !strcmp(commande, "logdf") )
		{
			mq_send(bal_log_disque, TRAME_FIN, sizeof(log_t), BAL_PRIO_FIN);
			sem_post(sem_disque);
		}
		else if( !strcmp(commande, "presence_carton") )
		{
			*(shm_statut)[ ST_PRESENCE_CARTON ] = 1;
		}
		else if( !strcmp(commande, "absence_carton") )
		{
			*(shm_statut)[ ST_PRESENCE_CARTON ] = 0;
		}
		else if( !strcmp(commande, "presence_palette") )
		{
			*(shm_statut)[ ST_PRESENCE_PALETTE ] = 1;
		}
		else if( !strcmp(commande, "absence_palette") )
		{
			*(shm_statut)[ ST_PRESENCE_PALETTE ] = 0;
		}
		else if( !strcmp(commande, "imprimante_OK") )
		{
			*(shm_statut)[ ST_IMPRIMANTE ] = 1;
		}
		else if( !strcmp(commande, "err_imprimante_KO") )
		{
			*(shm_statut)[ ST_IMPRIMANTE ] = 0;
		}
		else if( !strcmp(commande, "presence_carton") )
		{
			*(shm_statut)[ ST_PRESENCE_CARTON ] = 1;
		}
		else if( !strcmp(commande, "absence_carton") )
		{
			*(shm_statut)[ ST_PRESENCE_CARTON ] = 0;
		}
		else if( !strcmp(commande, "piece_OK") )
		{
			*(shm_statut)[ ST_PIECE ] = 1;
		}
		else if( !strcmp(commande, "piece_KO") )
		{
			*(shm_statut)[ ST_PIECE ] = 0;
		}
		else if( !strcmp(commande, "film_OK") )
		{
			*(shm_statut)[ ST_FILM ] = 1;
		}
		else if( !strcmp(commande, "film_KO") )
		{
			*(shm_statut)[ ST_FILM ] = 0;
		}
		else if( !strcmp(commande, "clapet_ouvert") )
		{
			*(shm_statut)[ ST_CLAPET_OUVERT ] = 1;
			sem_post( sem_clapet );
			pthread_join( t_envoi_piece, NULL );
		}
		else if( !strcmp(commande, "clapet_ferme") )
		{
			*(shm_statut)[ ST_CLAPET_OUVERT ] = 0;
		}
		if( !strcmp(commande, "q") )
		{
			break;
		}
	}
	pthread_exit(0);
}
