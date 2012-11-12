#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <mqueue.h>
#include <fcntl.h>

#include <unistd.h> /*temporaire (sleep)*/

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include "log_disque.h"
#include "config.h"

/*Definitions de constantes de config.h*/
const mode_t MODERW = 0666;
const log_t TRAME_FIN = "Fin";
const int MESSAGE_TYPE_STD = 1;
const int MESSAGE_TYPE_FIN = 2;

const char * BALERR = "/bal_erreur";
const char * BALDIS = "/bal_disque";
const char * BALWIN = "/bal_windows";

const char * NOM_LOG = "log.txt";


int main(int argc, char** argv)
{
	mqd_t bal_erreur, bal_log_disque, bal_log_windows; /*boîtes aux lettres*/
	sem_t sem_clapet, sem_piece, sem_carton, sem_palette, 
		sem_erreur_carton, sem_erreur_palette, sem_AU;	/*semaphores*/
	pthread_mutex_t mutex_entrepot, mutex_lot; /*mutex*/
	statut_t * shm_statut;
	lot_t * shm_lot;
	entrepot_t * shm_entrepot;
	
	pthread_t t_carton, t_palette, t_cariste, t_erreur, t_log_disque, 
		t_log_windows, t_commande_windows;
	
	/*Initialisation*/
	
	/*Boîtes aux lettres*/
	struct mq_attr attributs_err;
	attributs_err.mq_flags = 0;
	attributs_err.mq_maxmsg = 100;	/*A changer en fonction de la bal ?*/
	attributs_err.mq_msgsize = (long int) sizeof(erreur_t);
	attributs_err.mq_curmsgs = 0;

	bal_erreur = mq_open( BALERR, O_CREAT | O_RDWR, MODERW, &attributs_err );

	struct mq_attr attr_err;
	mq_getattr(bal_erreur, &attr_err);
	printf("Taille orig %lx\n", attributs_err.mq_maxmsg);
	printf("Taille BAL %lx\n", attr_err.mq_maxmsg);

	struct mq_attr attributs_log;
	attributs_log.mq_flags = 0;
	attributs_log.mq_maxmsg = 100;	/*A changer en fonction de la bal ?*/
	attributs_log.mq_msgsize = (long int) sizeof(log_t);/* (long int) sizeof(erreur_t); */
	attributs_log.mq_curmsgs = 0;
	
	bal_log_disque = mq_open( BALDIS, O_CREAT | O_RDWR, MODERW, &attributs_log );
	
	bal_log_windows = mq_open( BALWIN, O_CREAT | O_RDWR, MODERW, &attributs_log );

	struct mq_attr attr;
	mq_getattr(bal_erreur, &attr);
	printf("%li\n", (long int)sizeof(erreur_t));
	printf("Taille BAL %lx\n", attr.mq_msgsize);
	/*la définition des attributs ne marche pas, cela n'a aucun sens (probleme avec mon OS ?)*/
	
	/*Sémaphores*/
	sem_init( &sem_clapet, 0, 1 );
	sem_init( &sem_piece, 0, 0 );
	sem_init( &sem_carton, 0, 0 );
	sem_init( &sem_palette, 0, 0 );
	sem_init( &sem_erreur_carton, 0, 0 );
	sem_init( &sem_erreur_palette, 0, 0 );
	sem_init( &sem_AU, 0, 0 );
	
	/*Mutex*/
	pthread_mutex_init( &mutex_lot, NULL );
	pthread_mutex_init( &mutex_entrepot, NULL );
	
	/*Mémoire partagées*/
	shm_statut = malloc( sizeof( statut_t ) );
	shm_entrepot = malloc( sizeof( entrepot_t ) );
	shm_lot = malloc( sizeof( lot_t ) );
	
	/*Threads*/
	/*pthread_create( &t_carton, NULL, carton, ? );*/
	/*pthread_create( &t_log_disque, NULL, (void*) log_disque, NULL );
	/*pthread_create( &t_log_windows, NULL, log_windows, ? );
	pthread_create( &t_palette, NULL, palette, ? );
	pthread_create( &t_cariste, NULL, cariste, ? );
	pthread_create( &t_erreur, NULL, erreur, ? );
	pthread_create( &t_commande_windows, NULL, commande_windows, ? );*/
	
	/*Moteur*/
	/*sleep( 5 );*/
	/*pthread_join( t_log_disque, NULL );
	/*pthread_join( t_commande_windows, NULL );*/
	
	/*Destruction*/
	
	/*Mémoire partagées*/
	free( shm_lot );
	free( shm_entrepot );
	free( shm_statut );
	
	/*Mutex*/
	pthread_mutex_destroy( &mutex_entrepot );
	pthread_mutex_destroy( &mutex_lot );
	
	/*Sémaphores*/
	sem_destroy( &sem_AU );
	sem_destroy( &sem_erreur_palette );
	sem_destroy( &sem_erreur_carton );
	sem_destroy( &sem_palette );
	sem_destroy( &sem_carton );
	sem_destroy( &sem_piece );
	sem_destroy( &sem_clapet );
	
	/*Boîtes aux lettres*/
	mq_unlink( BALWIN );
	mq_close( bal_log_windows );
	mq_unlink( BALDIS );
	mq_close( bal_log_disque );
	mq_unlink( BALERR );
	mq_close( bal_erreur );
	
	return 0;
}
