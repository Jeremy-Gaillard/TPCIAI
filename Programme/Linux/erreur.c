#include "erreur.h"
#include "config.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <fcntl.h>
#include <pthread.h>

#include <stdlib.h>
#include <string.h>


void erreur(statut_t* shm_statut)
{
	int bal_log_erreur = mq_open( BALERR, O_RDONLY );
	int bal_log_windows = mq_open( BALWIN, O_RDONLY );
	int bal_log_disque = mq_open( BALDIS, O_RDONLY );
	erreur_t message;	
	
	do
	{
		mq_receive(bal_log_erreur, (void*) &message, sizeof(erreur_t), NULL);
		/*Analyser le message ici*/
		*shm_statut[ST_CLAPET_OUVERT] = 0;
		strcat( "E ", (char *)message);
		mq_send(bal_log_windows, message, sizeof(log_t), 1);
		mq_send(bal_log_disque, message, sizeof(log_t), 1);
	}
	while( strcmp(message, TRAME_FIN) );
	
	pthread_exit(0);
}


