#include "log_disque.h"
#include "config.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>


void log_disque()
{
	FILE * fichier_log;
	int bal_log_disque = mq_open( BALDIS, O_RDONLY );
	log_t message;
	message[0] = '\0';
	
	fichier_log = fopen(NOM_LOG, "w");
	
	while( strcmp(message, TRAME_FIN) )
	{
		mq_receive(bal_log_disque, (void*) &message, sizeof(log_t), NULL);
		/*Analyser le message ici*/
		printf("%s\n", message);
		fprintf(fichier_log, "log_disque : %s\n", message);
		if( strcmp(message, TRAME_FIN) )
			message[0] = '\0';
	}
	
	fclose(fichier_log);
	exit(0);
}

