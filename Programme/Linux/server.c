#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MSG_MAX_SIZE 256

void wait_order();

void error(const char* msg)
{
  perror(msg);
  exit(0);
}

int sockfd, newsockfd, portno;
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;
int n;

int main(int argc, char* argv[])
{
  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }
  printf("Launching server...\n"); 
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  bzero( (char*)&serv_addr, sizeof(serv_addr) );
  portno = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if ( bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0 )
    error("ERROR on binding");

  listen(sockfd, 5); // 5?
  /*
int listen(int sockfd, int backlog);
The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow. If a connection request arrives when the queue is full, the client may receive an error with an indication of ECONNREFUSED or, if the underlying protocol supports retransmission, the request may be ignored so that a later reattempt at connection succeeds.
*/

  clilen = sizeof(cli_addr);

  while(1) {
    wait_order();
  }

  close(newsockfd);
  close(sockfd);

  return 0;
}

char * get_response_for(char * message, char * response)
{
  strcpy(response,"I got your message\n");
}

void wait_order()
{
  char msg_buffer[MSG_MAX_SIZE]; //msg_buffer[MSG_MAX_SIZE-1] = '\0';
  char resp_buffer[MSG_MAX_SIZE]; //resp_buffer[MSG_MAX_SIZE-1] = '\0';
  while (1)
  {
	  cont:
	  printf("Waiting for connection...\n");
	  newsockfd = accept( sockfd,
						  (struct sockaddr*)&cli_addr,
						  &clilen );

	  if (newsockfd < 0)
		error("ERROR on accept");
		
	  printf("Connection initiated.\n");
	  while (1)
	  {
		  bzero(msg_buffer, MSG_MAX_SIZE);
		  n = read( newsockfd, msg_buffer, (MSG_MAX_SIZE-1) );
		  if (n<0)
			//error("ERROR reading from socket");
		  { printf("Connection interrupted.\n"); goto cont; }
		  
		  printf("Here is the message: %s\n", msg_buffer);
		  
		  //char resp[] = "I got your message\n";
		  //char resp[MSG_MAX_SIZE] 
		  get_response_for(msg_buffer, resp_buffer);
		  
		  //n = write( newsockfd, resp_buffer, sizeof(resp_buffer) );
		  n = write( newsockfd, resp_buffer, strlen(resp_buffer) );
		  if (n<0)
			error("ERROR writing to socket");
	  }
	}

}


