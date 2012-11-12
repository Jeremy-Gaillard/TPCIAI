#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include "config.h"
#include "prod_utils.h"

void gerer_erreur( int erreur_id, mqd_t bal_erreur )
{
	struct erreur err;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime ( err.heure, 7, "%H%M%S", timeinfo );
	err.id = erreur_id;
			
	mq_send( bal_erreur, err, sizeof( err ), 1 );
}
