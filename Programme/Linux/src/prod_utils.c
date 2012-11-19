#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>
#include <time.h>

#include "config.h"
#include "prod_utils.h"

void gerer_erreur( int erreur_id, pthread_mutex_t* mutex_erreur )
{
	
	printf("err: %d\n", erreur_id);
	char heure[7];
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime ( heure, 7, "%H%M%S", timeinfo );
	erreur_t message_erreur;
	sprintf(message_erreur, "%d %s", erreur_id, heure);

	mqd_t bal_erreur = mq_open(BALERR, O_WRONLY);
	pthread_mutex_lock( mutex_erreur );
	mq_send( bal_erreur, message_erreur, sizeof( erreur_t ),
	         BAL_PRIO_ELSE );
	pthread_mutex_unlock( mutex_erreur );
}
