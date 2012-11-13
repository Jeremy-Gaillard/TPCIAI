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

void erreur(statut_t* shm_statut, sem_t erreur, sem_t windows, sem_t disque)
{
	int bal_log_erreur = mq_open( BALERR, O_RDONLY );
	int bal_log_windows = mq_open( BALWIN, O_RDONLY );
	int bal_log_disque = mq_open( BALDIS, O_RDONLY );
	erreur_t message;
	erreur_t log = "E ";
	sem_t sem_bal_erreur = erreur;
	sem_t sem_bal_log_windows = windows;
	sem_t sem_bal_log_disque = disque;	
	
	
	do
	{
		sem_wait(&sem_bal_erreur);
		mq_receive(bal_log_erreur, (void*) &message, sizeof(erreur_t), NULL);
		/*Analyser le message ici*/
		*shm_statut[ST_CLAPET_OUVERT] = 0;
		strcat( log, message );
		mq_send(bal_log_windows, log, sizeof(log_t), 1);
		sem_post(&sem_bal_log_windows);
		mq_send(bal_log_disque, log, sizeof(log_t), 1);
		sem_post(&sem_bal_log_disque);
	}
	while( strcmp(message, TRAME_FIN) );
	printf("Fin thread erreur\n");
	
	pthread_exit(0);
}


