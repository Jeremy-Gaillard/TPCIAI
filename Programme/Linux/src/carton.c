#include <stdio.h>
#include <stdlib.h>

#include <mqueue.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>

#include "config.h"
#include "prod_utils.h"

/*Fonction qui envoi les logs prvenant de carton aux threads de log_windows et de log_disque*/
void log_carton( mqd_t bal_log_disque, mqd_t bal_log_windows,
                 int carton_id, int palette_id, char type_piece, int nb_rebus,
                 pthread_mutex_t* mutex_windows, pthread_mutex_t* mutex_disque ) {

	char heure[7];
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime ( heure, 7, "%H%M%S", timeinfo );

	int pourcent_rebus = (100*nb_rebus)/(CARTON_PLEIN + nb_rebus);
	log_t message;

	sprintf(message, "L C %d %d %c %d %s",
	        carton_id, palette_id, type_piece, pourcent_rebus, heure);

        /*Envoi des log à bal_log_disque*/
	while ( pthread_mutex_lock( mutex_disque ) );
	mq_send( bal_log_disque, message, sizeof( log_t ),
	         BAL_PRIO_ELSE );
	pthread_mutex_unlock( mutex_disque );

        /*Envoi des log à bal_log_windows*/
	while ( pthread_mutex_lock( mutex_windows ) );
	mq_send( bal_log_windows, message, sizeof( log_t ),
	         BAL_PRIO_ELSE );
	pthread_mutex_unlock( mutex_windows );
}

/*Procédure chargée d'initialiser les IPC utiles à carton*/
void init_carton( int* cmd_A, int* cmd_B, char* type_piece,
                  int* max_rebus, lot_t* shm_lot ) {

	*cmd_A = (*shm_lot)[LOT_A];
	*cmd_B = (*shm_lot)[LOT_B];
	*type_piece = ( (*cmd_A) > 0) ? 'A' : 'B';
	*max_rebus = CARTON_PLEIN * (*shm_lot)[REBUS] / 100;
}

int carton( arg_carton_t* args ){

	/* Récupération des ressources */
	mqd_t bal_log_disque = mq_open(BALDIS, O_WRONLY);
	mqd_t bal_log_windows = mq_open(BALWIN, O_WRONLY);

	statut_t* shm_statut = args->shm_statut;
	lot_t* shm_lot = args->shm_lot;

	sem_t* sem_piece = args->sem_piece;
	sem_t* sem_carton = args->sem_carton;
	sem_t* sem_erreur_carton = args->sem_erreur_carton;
	
	pthread_mutex_t* mutex_disque = args->mutex_disque;
	pthread_mutex_t* mutex_windows = args->mutex_windows;
	pthread_mutex_t* mutex_erreur = args->mutex_erreur;

	/* Création des variables locales */
	int debut_prod = 1;
	int nb_piece = 0;
	int nb_carton = 0;
	int nb_rebus = 0;
	int max_rebus;
	int nb_palette = 0;
	int cmd_A;
	int cmd_B;
	char type_piece;
	int place_file_attente;

	for( ; ; ){
	
		while ( nb_piece == 0 && (*shm_statut)[ST_PRESENCE_CARTON] != 1 ){
			/* Si première piece et absence carton
			   envoi d'un message d'erreur avec hhmmss et type erreur
			   puis attente sur sémaphore de reprise d'erreur */
		 		
			gerer_erreur(ERR_PAS_DE_CARTON, mutex_erreur );
			while ( sem_wait ( sem_erreur_carton ) );
		}
		/*end of absence carton*/
		
		/* attente piece */
		while ( sem_wait ( sem_piece ) ); 

		if (debut_prod) {
			init_carton(&cmd_A, &cmd_B, &type_piece, &max_rebus, shm_lot);
			debut_prod = 0;
		}
		
		if ( (*shm_statut)[ ST_PIECE ] == 1 ){
			nb_piece += 1;
			if ( nb_piece == CARTON_PLEIN ){
				while ( (*shm_statut)[ST_IMPRIMANTE] != 1 ){
					/* Si carton plein et imprimante HS
					   envoi d'un message d'erreur avec hhmmss et type erreur
					   puis attente sur sémaphore de reprise d'erreur */

					gerer_erreur(ERR_IMPRIMANTE_KO, mutex_erreur);
					while ( sem_wait( sem_erreur_carton ) );
				}
				/*end of if imprimante HS*/
				
				sem_getvalue( sem_carton, &place_file_attente );
				while ( place_file_attente == MAX_CARTON_FILE ){
					/* Si trop de cartons dans la file d'attente
					   envoi d'un message d'erreur avec hhmmss et type erreur
					   puis attente sur sémaphore de reprise d'erreur */
				 
					gerer_erreur(ERR_FILE_D_ATTENTE, mutex_erreur);
					while ( sem_wait( sem_erreur_carton ) );
					sem_getvalue( sem_carton, &place_file_attente );
				}
				/*end of if file attente pleine*/
				
				sem_post( sem_carton );
				nb_carton++;
				
				log_carton(bal_log_disque, bal_log_windows,
				           nb_carton, nb_palette + 1, type_piece, nb_rebus,
				           mutex_windows, mutex_disque);

				nb_piece = 0;
				nb_rebus = 0;

				if (nb_carton==PALETTE_PLEINE) {
					nb_carton=0;
					nb_palette++;

					if ( type_piece=='A' && nb_palette==cmd_A ) {
						type_piece = 'B';
						nb_palette = 0;
					}
					/* Plus de pièce à produire : on ferme le clapet
					   et on met à 0 le lot de production
					   (commande_windows utilise LOT_A et LOT_B pour vérifier
					   s'il reste des trucs à produire à la reprise) */
					else if ( type_piece=='B' && nb_palette==cmd_B ) {
						(*shm_statut)[ST_CLAPET_OUVERT] = 0;
						(*shm_lot)[LOT_A] = 0;
						(*shm_lot)[LOT_B] = 0;
						(*shm_lot)[REBUS] = 0;
					}
				}
					
			}
			/*end of if carton plein*/
		
		}
		/*end of if piece OK*/
		
		else{
			nb_rebus +=1;
			if ( nb_rebus >= max_rebus ){
				/* Si trop de mauvaises pièces
				   envoi d'un message d'erreur avec hhmmss et type erreur
				   puis attente sur sémaphore de reprise d'erreur
				   puis on jette le carton en cours */
			 	
				gerer_erreur(ERR_TROP_DE_REBUS, mutex_erreur);
				while ( sem_wait( sem_erreur_carton ) );
				/* printf("Fin erreur rebus\n"); */

				nb_piece = 0;
				nb_rebus = 0;
			}
			/*end of if trop rebus atteint*/
		}
		/*end of if piece KO */
	}
	/*end of for(;;)*/ 	
}
