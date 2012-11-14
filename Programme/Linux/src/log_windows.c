#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>


#include "server.h"
#include "log_windows.h"
#include "config.h"

static int my_socket;

void envoyer(log_t message)
{
	int n = write( socket, message, sizeof(log_t) );
	if (n<0)
		error("ERROR writing to socket");
}

void init_log_windows(int asocket)
{
	printf("%d",sizeof(log_t));
	error("");
	my_socket = asocket;
}

void log_windows(sem_t* windows)
{
	sem_t* sem_bal_log_windows = windows;
	int bal_log_windows = mq_open( BALWIN, O_RDONLY );
	log_t message;	
	
	do
	{
		sem_wait(sem_bal_log_windows);
		mq_receive(bal_log_windows, (void*) &message, sizeof(log_t), NULL);
		envoyer(message);
	}
	while( strcmp(message, TRAME_FIN) );
	printf("Fin log win\n");
	
	pthread_exit(0);
}


