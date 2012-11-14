#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>
#include <time.h>

#include "config.h"
#include "prod_utils.h"

static sem_t* sem_AU;

void init_prod( sem_t* sem_arret_urgence )
{
	sem_AU = sem_arret_urgence;
}

void arret_urgence_prod(int signum) {
	printf("PRODUCTION: ARRÊT D'URGENCE !\n");
	sem_wait(sem_AU);
}

void fin_production(int signum)
{
	printf("PRODUCTION: signal %d reçu\n", signum);
	pthread_exit( 0 );
}

void gerer_erreur( int erreur_id,
                   sem_t* sem_bal_erreur, sem_t* sem_bal_log_disque )
{
	printf("err: %d\n", erreur_id);
	char heure[7];
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime ( heure, 7, "%H%M%S", timeinfo );
	char* message_erreur= malloc(22);/*id erreur(int=15) + heure (=6) + 1 = 22*/
	sprintf(message_erreur, "%d %s", erreur_id,heure);

	char* message_log= malloc(22);/*2+ id erreur(int=15) + heure (=6) + 1 = 22*/
	sprintf(message_log, "E %d %s", erreur_id,heure);
	mqd_t bal_erreur = mq_open(BALERR, O_WRONLY);
	mqd_t bal_log_disque = mq_open(BALDIS, O_WRONLY);
	mq_send( bal_erreur, message_erreur, sizeof( message_erreur ),
	         BAL_PRIO_ERREUR );
	sem_post(sem_bal_erreur);
	mq_send( bal_log_disque, message_log, sizeof( message_log ),
	         BAL_PRIO_ERREUR );
	sem_post(sem_bal_log_disque);
}
