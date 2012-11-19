#include "erreur.h"
#include "config.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void erreur(struct arg_erreur *ipc)
{
	mqd_t bal_log_erreur = mq_open( BALERR, O_RDONLY );
	mqd_t bal_log_windows = mq_open( BALWIN, O_WRONLY );
	mqd_t bal_log_disque = mq_open( BALDIS, O_WRONLY );
	erreur_t message;
	statut_t* shm_statut = ipc->statut;	
	pthread_mutex_t* mutex_disque = ipc->mutex_disque;
	pthread_mutex_t* mutex_windows = ipc->mutex_windows;
	
	do
	{
		mq_receive(bal_log_erreur, message, sizeof(erreur_t), NULL);
		if( message != TRAME_FIN )
		{
			(*shm_statut)[ST_CLAPET_OUVERT] = 0;
			log_t log;
			sprintf( log, "E %s", message );
			pthread_mutex_lock ( mutex_windows );
			mq_send(bal_log_windows, log, sizeof(log_t), BAL_PRIO_ELSE);
			pthread_mutex_unlock ( mutex_windows );
			pthread_mutex_lock ( mutex_disque );
			mq_send(bal_log_disque, log, sizeof(log_t), BAL_PRIO_ELSE);
			pthread_mutex_unlock ( mutex_disque );
		}
	}
	while( strcmp(message, TRAME_FIN) );
	printf("Fin thread erreur\n");
	
	pthread_exit(0);
}


