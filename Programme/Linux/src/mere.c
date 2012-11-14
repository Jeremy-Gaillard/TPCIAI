#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <mqueue.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>


#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

#include "log_disque.h"
#include "config.h"
#include "simulation.h"
#include "log_windows.h"
#include "erreur.h"
#include "commande_windows.h"




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
	
	/*Définition d'un comportement de masquage de signal*/
	struct sigaction mask;
	mask.sa_handler = SIG_IGN;

	/*Masquage de SIGUSR1*/
	sigaction( SIGUSR1, &mask, NULL );
	/*Masquage de SIGUSR2*/
	sigaction( SIGUSR2, &mask, NULL );
	
	/*Boîtes aux lettres*/
	struct mq_attr attributs_err;
	attributs_err.mq_flags = 0;
	attributs_err.mq_maxmsg = 10;	/*ATTENTION : 10 est le maximum toléré pour un processus n'étant pas privilégié*/
	attributs_err.mq_msgsize = (long int) sizeof(erreur_t);
	attributs_err.mq_curmsgs = 0;

	bal_erreur = mq_open( BALERR, O_CREAT | O_RDWR, MODERW, &attributs_err );
	


	struct mq_attr attributs_log;
	attributs_log.mq_flags = 0;
	attributs_log.mq_maxmsg = 10;	/*ATTENTION : 10 est le maximum toléré pour un processus n'étant pas privilégié*/
	attributs_log.mq_msgsize = (long int) sizeof(log_t);
	attributs_log.mq_curmsgs = 0;
	
	bal_log_disque = mq_open( BALDIS, O_CREAT | O_RDWR, MODERW, &attributs_log );
	
	bal_log_windows = mq_open( BALWIN, O_CREAT | O_RDWR, MODERW, &attributs_log );

	
	/*Sémaphores*/	
	sem_init( &sem_clapet, 0, 1 );
	sem_init( &sem_piece, 0, 0 );
	sem_init( &sem_carton, 0, 0 );
	sem_init( &sem_palette, 0, 0 );
	sem_init( &sem_erreur_carton, 0, 0 );
	sem_init( &sem_erreur_palette, 0, 0 );
	sem_init( &sem_AU, 0, 0 );
	sem_init( &sem_bal_log_disque, 0, 0);
	sem_init( &sem_bal_log_windows, 0, 0);
	sem_init( &sem_bal_erreur, 0, 0);
	
	
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
	/**shm_statut[ST_CLAPET_OUVERT] = 1; /*clapet ouvert*/
	
	for ( i = 0; i < 20; i++)
		shm_entrepot->palettes[i].id = NO_PALETTE;
		
	for ( i = 0; i < 2; i++)
		*shm_lot[i] = 0;
	
	/*Threads*/
	/*pthread_create( &t_carton, NULL, carton, ? );*/


	arg_simulation_t simulation_arg;
	simulation_arg.statut = shm_statut;
	simulation_arg.erreur = &sem_bal_erreur;
	simulation_arg.windows = &sem_bal_log_windows;
	simulation_arg.disque = &sem_bal_log_disque;
	simulation_arg.clapet = &sem_clapet;
	simulation_arg.piece = &sem_piece;
	pthread_create( &t_log_disque, NULL, (void*) log_disque, (void*) &sem_bal_log_disque );
	
	pthread_create( &t_simulation, NULL, (void*) simulation, (void*) &simulation_arg );
	
	pthread_create( &t_log_windows, NULL, (void*) log_windows, (void*) &sem_bal_log_windows );
	/*pthread_create( &t_palette, NULL, palette, ? );
	pthread_create( &t_cariste, NULL, cariste, ? );*/
	
	arg_erreur_t erreur_arg;
	erreur_arg.statut = shm_statut;
	erreur_arg.bal_erreur = &sem_bal_erreur;
	erreur_arg.bal_log_win = &sem_bal_log_windows;
	erreur_arg.bal_log_disque = &sem_bal_log_disque;
	pthread_create( &t_erreur, NULL, (void*) erreur, (void*) &erreur_arg );
	
	arg_commande_windows_t windows_arg;
	windows_arg.shm_statut = shm_statut;
	windows_arg.shm_lot = shm_lot;
	windows_arg.shm_entrepot = shm_entrepot;
	windows_arg.entrepot = &mutex_entrepot;
	windows_arg.palette = &sem_erreur_palette;
	windows_arg.carton = &sem_erreur_carton;
	windows_arg.AU = &sem_AU;
	windows_arg.clapet = &sem_clapet;
	pthread_create( &t_commande_windows, NULL, (void*) commande_windows, (void*) &windows_arg );
	
	/*Moteur*/
	pthread_join( t_commande_windows, NULL );
	pthread_kill( t_simulation, SIGUSR2 );
	
	/*Destruction*/
	
	mq_send(bal_erreur, TRAME_FIN, sizeof(erreur_t), 2);
	sem_post(&sem_bal_erreur);
	pthread_join( t_erreur, NULL );
	
	mq_send(bal_log_disque, TRAME_FIN, sizeof(log_t), 2 );
	sem_post(&sem_bal_log_disque);
	pthread_join( t_log_disque, NULL );
	
	mq_send(bal_log_windows, TRAME_FIN, sizeof(log_t), 2 );
	sem_post(&sem_bal_log_windows);
	pthread_join( t_log_windows, NULL );
	
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
