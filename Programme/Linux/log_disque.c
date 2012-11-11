#include "log_disque.h"
#include "config.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>


void log_disque()
{
	FILE * fichier_log;
	int bal_log_disque = mq_open( BALDIS, O_RDONLY );
	int bla;
	log_t message = "";
	
	fichier_log = fopen(NOM_LOG, "w");
	
	while( message != TRAME_FIN )
	{
		struct mq_attr attr;
		mq_getattr(bal_log_disque, &attr);
		bla = mq_receive(bal_log_disque, (void*) &message, sizeof(log_t), NULL);
		/*TODO : erreur car sizeof log_t est trop petit -> réduire la limite (dans mq_attr) ou augmenter la taille de log_t*/
		/*Analyser le message ici*/
		printf("%li\n", attr.mq_msgsize);
		fprintf(fichier_log, "%s", message);
	}
	
	fclose(fichier_log);
	exit(0);
}

