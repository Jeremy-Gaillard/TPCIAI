#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>


#include "server.h"
#include "log_windows.h"
#include "config.h"

static int my_socket;

// Envoie un message au client
void envoyer(log_t message)
{
	int len = strlen(message);
	char buffer[len+2];
	strcpy(buffer,message);
	buffer[len] = '\n';
	buffer[len+1] = '\0';
	int n = write( my_socket, buffer, len+1 );
	if (n<0)
		error("ERROR writing to socket");
}

void init_log_windows(int asocket)
{
	my_socket = asocket;
}

void log_windows()
{
	/*Recuperation de la boite aux lettres*/
	int bal_log_windows = mq_open( BALWIN, O_RDONLY );
	log_t message;	
	
	do/*Envoi de message tant qu'on ne reçoit pas de trame de fin*/
	{
		/*Reception du message du superviseur*/
		mq_receive(bal_log_windows, message, sizeof(log_t), NULL);
		if( strcmp(message, TRAME_FIN) ) /*Si ce n'est pas un message de fin...*/
			envoyer(message);
	}
	while( strcmp(message, TRAME_FIN) );
	printf("Fin log win\n");
	
	pthread_exit(0);
}


