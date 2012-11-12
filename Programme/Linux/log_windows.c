#include "log_windows.h"
#include "config.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <fcntl.h>
#include <pthread.h>

#include <stdlib.h>
#include <string.h>


void envoyer(log_t message)
{}

void log_windows()
{
	int bal_log_windows = mq_open( BALWIN, O_RDONLY );
	log_t message;	
	
	do
	{
		mq_receive(bal_log_windows, (void*) &message, sizeof(log_t), NULL);
		/*Analyser le message ici*/
		envoyer(message);
	}
	while( strcmp(message, TRAME_FIN) );
	
	pthread_exit(0);
}


