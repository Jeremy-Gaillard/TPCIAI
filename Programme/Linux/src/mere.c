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
#include <string.h>

#include "log_disque.h"
#include "config.h"
#include "simulation.h"
#include "log_windows.h"
#include "erreur.h"

/*Definitions de constantes de config.h*/



int main(int argc, char** argv)
{
	mqd_t bal_erreur, bal_log_disque, bal_log_windows; /*boîtes aux lettres*/
	sem_t sem_clapet, sem_piece, sem_carton, sem_palette, 
		sem_erreur_carton, sem_erreur_palette, sem_AU, sem_bal_log_disque,
		sem_bal_log_windows, sem_bal_erreur;	/*semaphores*/
	pthread_mutex_t mutex_entrepot; /*mutex*/
	statut_t * shm_statut;
	lot_t * shm_lot;	
	entrepot_t * shm_entrepot;
	
	pthread_t t_carton, t_palette, t_cariste, t_erreur, t_log_disque, 
		t_log_windows, t_commande_windows, t_simulation;
	
	/*Initialisation*/
	
	/*Boîtes aux lettres*/
	struct mq_attr attributs_err;
	attributs_err.mq_flags = 0;
	attributs_err.mq_maxmsg = 10;	/*ATTENTION : 10 est le maximum toléré pour un processus n'étant pas privilégié*/
	attributs_err.mq_msgsize = (long int) sizeof(erreur_t);
	attributs_err.mq_curmsgs = 0;

	bal_erreur = mq_open( BALERR, O_CREAT | O_RDWR, MODERW, &attributs_err );
	
	int errbuf = errno;

	struct mq_attr attr_err;
	mq_getattr(bal_erreur, &attr_err);

	struct mq_attr attributs_log;
	attributs_log.mq_flags = 0;
	attributs_log.mq_maxmsg = 10;	/*ATTENTION : 10 est le maximum toléré pour un processus n'étant pas privilégié*/
	attributs_log.mq_msgsize = (long int) sizeof(log_t);
	attributs_log.mq_curmsgs = 0;
	
	bal_log_disque = mq_open( BALDIS, O_CREAT | O_RDWR, MODERW, &attributs_log );
	
	bal_log_windows = mq_open( BALWIN, O_CREAT | O_RDWR, MODERW, &attributs_log );

	struct mq_attr attr;
	mq_getattr(bal_erreur, &attr);
	
	/*Sémaphores*/
	sem_init( &sem_clapet, 0, 1 );
	sem_init( &sem_piece, 0, 0 );
	sem_init( &sem_carton, 0, 0 );
	sem_init( &sem_palette, 0, 0 );
	sem_init( &sem_erreur_carton, 0, 0 );
	sem_init( &sem_erreur_palette, 0, 0 );
	sem_init( &sem_AU, 0, 0 );
	sem_init( &sem_bal_log_disque, 10, 0);
	sem_init( &sem_bal_log_windows, 10, 0);
	sem_init( &sem_bal_erreur, 10, 0);
	
	/*Mutex*/
	pthread_mutex_init( &mutex_entrepot, NULL );
	
	/*Mémoire partagées*/
	shm_statut = malloc( sizeof( statut_t ) );
	shm_entrepot = malloc( sizeof( entrepot_t ) );
	shm_lot = malloc( sizeof( lot_t ) );
	
	/*initialisation des mémoires*/
	int i;
	for ( i = 0; i < STATUT_SIZE; i++)
		*shm_statut[i] = 1;
	/**shm_statut[ST_CLAPET_OUVERT] = 1; /* clapet ouvert*/
	
	for ( i = 0; i < 20; i++)
		shm_entrepot->palettes[i].id = NO_PALETTE;
		
	for ( i = 0; i < 2; i++)
		*shm_lot[i] = 0;
	
	/*Threads*/
	/*pthread_create( &t_carton, NULL, carton, ? );*/
	pthread_create( &t_log_disque, NULL, (void*) log_disque, NULL );
	pthread_create( &t_simulation, NULL, (void*) simulation, (void*) shm_statut );
	pthread_create( &t_log_windows, NULL, log_windows, NULL );
	/*pthread_create( &t_palette, NULL, palette, ? );
	pthread_create( &t_cariste, NULL, cariste, ? );*/
	pthread_create( &t_erreur, NULL, erreur, NULL );
	/*pthread_create( &t_commande_windows, NULL, commande_windows, ? );*/
	
	/*Moteur*/
	/*sleep( 5 );*/
	pthread_join( t_log_disque, NULL );
	pthread_join( t_simulation, NULL );
	/*pthread_join( t_commande_windows, NULL );*/
	
	/*Destruction*/
	
	/*Mémoire partagées*/
	free( shm_lot );
	free( shm_entrepot );
	free( shm_statut );
	
	/*Mutex*/
	pthread_mutex_destroy( &mutex_entrepot );
	
	/*Sémaphores*/
	sem_destroy( &sem_AU );
	sem_destroy( &sem_erreur_palette );
	sem_destroy( &sem_erreur_carton );
	sem_destroy( &sem_palette );
	sem_destroy( &sem_carton );
	sem_destroy( &sem_piece );
	sem_destroy( &sem_clapet );
	sem_destroy( &sem_bal_log_disque );
	sem_destroy( &sem_bal_log_windows );
	sem_destroy( &sem_bal_erreur );
	
	/*Boîtes aux lettres*/
	mq_unlink( BALWIN );
	mq_close( bal_log_windows );
	mq_unlink( BALDIS );
	mq_close( bal_log_disque );
	mq_unlink( BALERR );
	mq_close( bal_erreur );
	
	return 0;
}
