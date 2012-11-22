#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUF_SIZE 256

/*Procédure appelée en cas d'erreur, on quitte le thread si c'est le cas*/
void error(const char* msg)
{
  perror(msg);
  exit(0);
}

/*Fonction chargée de se connecter au serveur pour établir la communication*/
int main(int argc, char* argv[])
{
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent* server;

  /*Verification de la bonne taille du buffer, s'il n'est pas de la bonne taille, on quitte le thread*/
  char buffer[BUF_SIZE];
  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }

  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  /*Verification de la bonne ouverture du socket, si ce n'est pas le cas, on envoi une erreur*/
  if (sockfd < 0)
    error("ERROR opening socket");

  server = gethostbyname(argv[1]);
  /*Verification de la bonne connection au serveur, si ce n'est pas le cas, on quitte le thread*/
  if (server == NULL) {
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }

  bzero( (char*)&serv_addr, sizeof(serv_addr) );

  serv_addr.sin_family = AF_INET;

  bcopy( (char*)server->h_addr,
         (char*)&serv_addr.sin_addr.s_addr,
         server->h_length );
  serv_addr.sin_port = htons(portno);

  /*Vérification de la connection, si ce n'est pas le cas, on avertit l'utilisateur*/
  if ( connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0 )
    error("ERROR connecting");
  printf("Please enter the message: ");
  bzero(buffer, BUF_SIZE);
  fgets(buffer, (BUF_SIZE-1), stdin);

  /*Ecriture du message à transmettre*/
  n = write( sockfd, buffer, strlen(buffer) );
  /*Vérification de la bonne écriture sur le socket, si ce n'est pas le cas, on avertit l'utlisateur*/
  if (n<0)
    error("ERROR writing to socket");

  /*Une fois l'écriture faire correctement, on passe à la lecture du socket*/
  printf("(done writing, reading...)\n");

  bzero(buffer, BUF_SIZE);
  n = read( sockfd, buffer, (BUF_SIZE-1) );
  /*Verification de la bonne lecture du socket, si ce n'est pas le cas, on avertit l'utilisateur*/
  if (n<0)
    error("ERROR reading from socket");
  printf("%s\n", buffer);

  close(sockfd);

  return 0;
}
