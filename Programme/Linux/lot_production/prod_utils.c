#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mqueue.h>
#include <semaphore.h>
#include "config.h"
#include "prod_utils.h"

void gerer_erreur( int erreur_id, mqd_t bal_erreur )
{
	time_t rawtime;
	struct tm * timeinfo;

	struct erreur err;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime ( err.heure, 7, "%H%M%S", timeinfo );
	err.id = erreur_id;

	const char * message = "ERREUR!!! AAAAAAH!";

	mq_send( bal_erreur, message, sizeof( message ), BAL_PRIO_ERREUR );
}
