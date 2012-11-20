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

void log_disque()
{
	/*Recuperation du fichier, ouverture de la boite aux lettres*/
	FILE * fichier_log;
	int bal_log_disque = mq_open( BALDIS, O_RDONLY );
	log_t message;
	message[0] = '\0';
	
	fichier_log = fopen(NOM_LOG, "w");/*Ouverture du fichier de log*/
	fprintf(fichier_log, "Début de session\n");
	
	/*Tant que le message reçu n'est pas une trame de fin*/
	while( strcmp(message, TRAME_FIN) )
	{
		/*Reception du message*/
		mq_receive(bal_log_disque, message, sizeof(log_t), NULL);
		log_t log;
		log[0] = '\0';
		printf("%s\n", message);
		
		/*Si c'est un message de fin, on quitte, sinon, on continue*/
		if( !strcmp(message, TRAME_FIN) )
			fprintf(fichier_log, "Fin de session\n");
		else
		{
			switch (message[0])/*Analyse du message*/
			{
				case 'L':/*Si c'est un log*/
					switch (message[2])
					{
						case 'C':/*Si c'est un carton produit*/
							strcat( log, message);
							break;
						case 'P':/*Si c'est une palette produite*/
							strcat( log, message);
							break;
						default:/*Si c'est un message eronne*/
							log[0] = '\0';
							break;
					}
					break;
				case 'E':/*Si c'est une erreur*/
					switch (message[2] - '0')/*Ecriture du log en fonction de l'erreur detectee*/
					{
						case ERR_AU:
							sprintf( log, "Erreur : arrêt d'urgence" );
							break;
						case ERR_TROP_DE_REBUS:
							sprintf( log, "Erreur : trop de rebus" );
							break;
						case ERR_PAS_DE_CARTON:
							sprintf( log, "Erreur : plus de carton");
							break;
						case ERR_IMPRIMANTE_KO:
							sprintf( log, "Erreur : imprimante hors service" );
							break;
						case ERR_FILE_D_ATTENTE:
							sprintf( log, "Erreur : trop de carton en attente d'être empaletté" );
							break;
						case ERR_PAS_DE_PALETTE:
							sprintf( log, "Erreur : plus de palette");
							break;
						case ERR_FILM_KO:
							sprintf( log, "Erreur : dysfonction du film palette");
							break;
						default:
							log[0] = '\0';
							break;
					}
					break;
				default:/*Si c'est un message eronne*/
					log[0] = '\0';
					break;
			}
			if( log[0] != '\0' )/*Si le log n'est pas vide, on l'ecrit dans le fichier de log*/
			{
				fprintf(fichier_log, "%s\n", log);
			}
		}
	}
	
	fclose(fichier_log);/*Fermeture du fichier de log*/
	pthread_exit(0);
}

