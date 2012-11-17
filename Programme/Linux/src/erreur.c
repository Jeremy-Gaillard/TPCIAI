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
	
	do
	{
		mq_receive(bal_log_erreur, (void*) &message, sizeof(erreur_t), NULL);
		*shm_statut[ST_CLAPET_OUVERT] = 0;
		log_t* log = malloc( sizeof(log_t) );
		log_t* log2 = malloc( sizeof(log_t) );
		sprintf( *log, "E %s", message );
		sprintf( *log2, "E %s", message );
		mq_send(bal_log_windows, *log, sizeof(log_t), BAL_PRIO_ELSE);
		mq_send(bal_log_disque, *log2, sizeof(log_t), BAL_PRIO_ELSE);
	}
	while( strcmp(message, TRAME_FIN) );
	printf("Fin thread erreur\n");
	
	pthread_exit(0);
}


