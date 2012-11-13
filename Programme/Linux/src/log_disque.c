#include "log_disque.h"
#include "config.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

void log_disque(sem_t disque)
{
	FILE * fichier_log;
	int bal_log_disque = mq_open( BALDIS, O_RDONLY );
	log_t message;
	message[0] = '\0';
	
	fichier_log = fopen(NOM_LOG, "w");
	fprintf(fichier_log, "Début de session\n");
	
	sem_t sem_bal_log_disque = disque;
	
	/*int idx = 2;
	int idxn;
	char buff[10];*/
	
	while( strcmp(message, TRAME_FIN) )
	{
		sem_wait(&sem_bal_log_disque);
		mq_receive(bal_log_disque, (void*) &message, sizeof(log_t), NULL);
		/*Analyser le message ici*/
		log_t log = "";
		
		if( !strcmp(message, TRAME_FIN) )
			fprintf(fichier_log, "Fin de session\n");
		else
		{
			switch (message[0])
			{
				case 'L':
					switch (message[2])
					{
						case 'C':
							/*idx = 2;
							buff[0] = '\0';
							strcat( log, "Id carton : ");
							idxn = strchr( message+idx, ' ');
							strncpy( buff, message+idx, idxn-idx);
							strcat( log, buff);*/
							strcat( log, message);
							break;
						case 'P':
							strcat( log, message);
							break;
						default:
							log[0] = '\0';
							break;
					}
					break;
				case 'E':
					strcat( log, "Erreur : ");
					switch (message[2] - '0')
					{
						case ERR_AU:
							strcat( log, "arrêt d'urgence\n");
							break;
						case ERR_TROP_DE_REBUS:
							strcat( log, "trop de rebus\n");
							break;
						case ERR_PAS_DE_CARTON:
							strcat( log, "plus de carton\n");
							break;
						case ERR_IMPRIMANTE_KO:
							strcat( log, "imprimante hors service\n");
							break;
						case ERR_FILE_D_ATTENTE:
							strcat( log, "trop de carton en attente d'être empaletté\n");
							break;
						case ERR_PAS_DE_PALETTE:
							strcat( log, "plus de palette\n");
							break;
						case ERR_FILM_KO:
							strcat( log, "dysfonction du film palette\n");
							break;
						default:
							log[0] = '\0';
							break;
					}
				default:
					log[0] = '\0';
					break;
			}	
			if( log[0] != '\0' )
				fprintf(fichier_log, "log_disque : %s\n", log);
		}
	}
	
	fclose(fichier_log);
	pthread_exit(0);
}

