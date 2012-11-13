#include "log_windows.h"
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


void envoyer(log_t message)
{}

void log_windows(sem_t* windows)
{
	sem_t sem_bal_log_windows = *windows;
	int bal_log_windows = mq_open( BALWIN, O_RDONLY );
	log_t message;	
	
	do
	{
		int retour = -5;
		sem_getvalue( &sem_bal_log_windows, &retour );
		sem_wait(&sem_bal_log_windows);
		printf("Sem pas content: %i\n",retour);
		mq_receive(bal_log_windows, (void*) &message, sizeof(log_t), NULL);
		/*Analyser le message ici*/
		envoyer(message);
	}
	while( strcmp(message, TRAME_FIN) );
	printf("Fin log win\n");
	
	pthread_exit(0);
}


