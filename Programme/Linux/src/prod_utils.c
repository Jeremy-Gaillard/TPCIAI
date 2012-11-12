#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mqueue.h>
#include <semaphore.h>
#include "config.h"
#include "prod_utils.h"

void gerer_erreur( int erreur_id, mqd_t bal_erreur )
{
	char heure[7];
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime ( heure, 7, "%H%M%S", timeinfo );
	char* message= malloc(22);/*id erreur(int=15) + heure (=6) + 1 = 16*/
	sprintf(message, "%d|%s", id,heure);

	mq_send( bal_erreur, message, sizeof( message ), BAL_PRIO_ERREUR );
}
