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
	/* Récuperation de l'heure au format HHMMSS */
	char heure[7];
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime ( heure, 7, "%H%M%S", timeinfo );
	
	/* Création de message erreur : type_erreur hhmmss */
	erreur_t message_erreur;
	sprintf(message_erreur, "%d %s", erreur_id, heure);

	/* Envoi du message a la bal d'erreur
	   Celle-ci est protégée par un mutex : mutex_erreur */
	mqd_t bal_erreur = mq_open(BALERR, O_WRONLY);
	while ( pthread_mutex_lock( mutex_erreur ) );
	mq_send( bal_erreur, message_erreur, sizeof( erreur_t ),
	         BAL_PRIO_ELSE );
	pthread_mutex_unlock( mutex_erreur );
}
