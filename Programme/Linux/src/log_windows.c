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
{/*
	//printf("%d\n",socket);
	printf("Sending to %d\n",my_socket);
	printf("%c\n",message[len-2]);
	//message[len-2] = '\n';
	//message[len-1] = '\0'; // TODO: fix possible overflow
	printf("%s\n",message);*/
	int len = strlen(message);
	char buffer[len+1]; // alloca POWA
	strcpy(buffer,message);
	buffer[len-1] = '\n';
	//printf("%s\n",buffer);
	int n = write( my_socket, buffer, len );
	if (n<0)
		error("ERROR writing to socket");
}

void init_log_windows(int asocket)
{
	//printf("%d",sizeof(log_t));
	//error("");
	//printf(":%d",asocket);
	my_socket = asocket;
}

void log_windows()
{
	int bal_log_windows = mq_open( BALWIN, O_RDONLY );
	log_t message;	
	
	do
	{
		mq_receive(bal_log_windows, message, sizeof(log_t), NULL);
		//envoyer(message);
	}
	while( strcmp(message, TRAME_FIN) );
	printf("Fin log win\n");
	
	pthread_exit(0);
}


