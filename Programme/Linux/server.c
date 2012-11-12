#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 256

void wait_order();

void error(const char* msg)
{
  perror(msg);
  exit(0);
}

  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[BUF_SIZE];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  
int main(int argc, char* argv[])
{
  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }
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

void wait_order()
{
  newsockfd = accept( sockfd,
                      (struct sockaddr*)&cli_addr,
                      &clilen );

  if (newsockfd < 0)
    error("ERROR on accept");

  bzero(buffer, BUF_SIZE);
  n = read( newsockfd, buffer, (BUF_SIZE-1) );
  if (n<0)
    error("ERROR reading from socket");

  printf("Here is the message: %s\n", buffer);
  
  char resp[] = "I got your message\n";
  
  n = write( newsockfd, resp, sizeof(resp) );
  if (n<0)
    error("ERROR writing to socket");
}






