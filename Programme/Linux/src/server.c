#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "server.h"
#include "log_windows.h"


#define MSG_MAX_SIZE 256

void wait_order();

void error(const char* msg)
{
	perror(msg);
	exit(0);
}

	//void envoyer(log_t message);

int server(int portno)
{
	/*pthread_t t_server;
	pthread_create( &t_server, NULL, (void*) host_server, (void*) portno );
	pthread_join(t_server, NULL);*/
	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	
	printf("Launching server...\n"); 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	bzero( (char*)&serv_addr, sizeof(serv_addr) );

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if ( bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0 )
		error("ERROR on binding");

	/*
	int listen(int sockfd, int backlog);
	The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow. If a connection request arrives when the queue is full, the client may receive an error with an indication of ECONNREFUSED or, if the underlying protocol supports retransmission, the request may be ignored so that a later reattempt at connection succeeds.
	*/
	listen(sockfd, 1); // 5?

	clilen = sizeof(cli_addr);
	
	printf("Waiting for connection...\n");
	newsockfd = accept( sockfd,
						(struct sockaddr*)&cli_addr,
						&clilen );
	
	if (newsockfd < 0)
		error("ERROR on accept 1");
	
	pthread_t t_receive;
	pthread_create( &t_receive, NULL, (void*) wait_order, (void*) newsockfd );
	/*
	printf("First socket aqcuired.\n");
	
	newsockfd = accept( sockfd,
						(struct sockaddr*)&cli_addr,
						&clilen );
	
	if (newsockfd < 0)
		error("ERROR on accept 2");
	*/
	init_log_windows(newsockfd);
	
	printf("Connection initiated.\n");
	
	//printf(".%d\n",newsockfd);
	
	envoyer("INIT..\n");
	
/*
	client(portno-1);
	while(1) {
		wait_order();
	}
*/
/*
	pthread_join(t_receive, NULL);
	close(newsockfd);
	close(sockfd);
*/
	return 0;
}

void process_message(char * message)
{
	
}

void wait_order(int newsockfd)
{
	char msg_buffer[MSG_MAX_SIZE];
	while (1)
	{
		bzero(msg_buffer, MSG_MAX_SIZE);
		int n = read( newsockfd, msg_buffer, (MSG_MAX_SIZE-1) );
		if (n<0)
			error("Connection interrupted.");
		printf("Here is the message: %s\n", msg_buffer);
		process_message(msg_buffer);
		//envoyer("TEST!!!");
	}

}


