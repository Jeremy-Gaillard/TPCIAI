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
#include "prod_utils.h"
#include "server.h"


static sem_t sem_AU;

void fin_thread(int signum)
{
	printf("Signal %d reçu\n", signum);
	if( signum == SIGUSR2 )
		pthread_exit( 0 );
}

void arret_urgence_prod(int signum) {
	printf("PRODUCTION: ARRÊT D'URGENCE !\n");
	sem_wait(&sem_AU);
}


int main(int argc, char** argv)
{
	mqd_t bal_erreur, bal_log_disque, bal_log_windows; /*boîtes aux lettres*/
	sem_t sem_clapet, sem_piece, sem_carton, sem_palette, 
		sem_erreur_carton, sem_erreur_palette;	/*semaphores*/
	pthread_mutex_t mutex_entrepot; /*mutex*/
	statut_t * shm_statut;	
	lot_t * shm_lot;	
	entrepot_t * shm_entrepot;
	
	pthread_t t_carton, t_palette, t_cariste, t_erreur, t_log_disque, 
		t_log_windows, t_commande_windows, t_simulation, t_envoi_piece;
	
	/*----------------------------------------------------Initialisation--------------------------------------------------*/

	//server(32768);
	
	/*priorite de la mere*/
	struct sched_param mere_param;
	mere_param.sched_priority = PRIO_MERE;
	pthread_setschedparam(pthread_self(), SCHED_FIFO, &mere_param);
	
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
	sem_init( &sem_clapet, 0, 0 );
	sem_init( &sem_piece, 0, 0 );
	sem_init( &sem_carton, 0, 0 );
	sem_init( &sem_palette, 0, 0 );
	sem_init( &sem_erreur_carton, 0, 0 );
	sem_init( &sem_erreur_palette, 0, 0 );
	sem_init( &sem_AU, 0, 0 );
	
	
	/*Mutex*/
	pthread_mutex_init( &mutex_entrepot, NULL );
	
	/*Mémoire partagées*/
	shm_statut = malloc( sizeof( statut_t ) );
	shm_entrepot = malloc( sizeof( entrepot_t ) );
	shm_lot = malloc( sizeof( lot_t ) );
	
	/*initialisation des mémoires*/
	int i;
	for ( i = 0; i < STATUT_SIZE; i++)
		(*shm_statut)[i] = 1;
	
	for ( i = 0; i < 20; i++)
		shm_entrepot->palettes[i].id = NO_PALETTE;
		
	for ( i = 0; i < 3; i++)
		(*shm_lot)[i] = 0;
		
	/*Création du handler d'arrêt d'urgence*/
	struct sigaction handler_USR1;
	handler_USR1.sa_handler = arret_urgence_prod;
	sigdelset( &handler_USR1.sa_mask, SIGUSR2 );
	sigaction( SIGUSR1, &handler_USR1, NULL );
	
	/*Création du Handler de fin de tâche et démasquage de SIGUSR2*/
	struct sigaction handler_USR2;
	handler_USR2.sa_handler = fin_thread;
	sigaction ( SIGUSR2, &handler_USR2, NULL );
		
	/*Threads*/
	
	arg_carton_t carton_arg;
	carton_arg.shm_statut = shm_statut;
	carton_arg.shm_lot = shm_lot;
	carton_arg.sem_piece = &sem_piece;
	carton_arg.sem_carton = &sem_carton;
	carton_arg.sem_erreur_carton = &sem_erreur_carton;
	pthread_create( &t_carton, NULL, (void*) carton, (void*) &carton_arg );
	
	pthread_create( &t_log_disque, NULL, (void*) log_disque, NULL );
	
	pthread_create( &t_log_windows, NULL, (void*) log_windows, NULL );
	
	arg_palette_t palette_arg;
	palette_arg.shm_statut = shm_statut;
	palette_arg.shm_lot = shm_lot;
	palette_arg.sem_carton = &sem_carton;
	palette_arg.sem_palette = &sem_palette;
  	palette_arg.sem_erreur_palette = &sem_erreur_palette;
	pthread_create( &t_palette, NULL, (void*) palette, (void*) &palette_arg );
	
	arg_simulation_t simulation_arg;
	simulation_arg.statut = shm_statut;
	simulation_arg.clapet = &sem_clapet;
	simulation_arg.AU = &sem_AU;
	simulation_arg.t_carton = t_carton;
	simulation_arg.t_palette = t_palette;
	pthread_create( &t_simulation, NULL, (void*) simulation, (void*) &simulation_arg );
	
	arg_cariste_t cariste_arg;
	cariste_arg.shm_lot = shm_lot;
	cariste_arg.shm_entrepot = shm_entrepot;
	cariste_arg.sem_palette = &sem_palette;
	cariste_arg.mutex_entrepot = &mutex_entrepot;
	pthread_create( &t_cariste, NULL, (void*) cariste, (void*) &cariste_arg );
	
	arg_erreur_t erreur_arg;
	erreur_arg.statut = shm_statut;
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
	
	arg_envoi_piece_t envoi_piece_arg;
	envoi_piece_arg.statut = shm_statut;
	envoi_piece_arg.piece = &sem_piece;
	
	pthread_create( &t_envoi_piece, NULL, (void*) envoi_piece, (void*) &envoi_piece_arg );
	 
	/*priorites*/
	struct sched_param erreur_param;
	struct sched_param carton_param;
	struct sched_param log_windows_param;
	struct sched_param palette_param;
	struct sched_param cariste_param;
	struct sched_param commande_windows_param;
	struct sched_param log_disque_param;
	erreur_param.sched_priority = PRIO_ERREUR;
	carton_param.sched_priority = PRIO_CARTON;
	log_windows_param.sched_priority = PRIO_LOG_WINDOWS;
	palette_param.sched_priority = PRIO_PALETTE;
	cariste_param.sched_priority = PRIO_CARISTE;
	commande_windows_param.sched_priority = PRIO_COMMANDE_WINDOWS;
	log_disque_param.sched_priority = PRIO_LOG_DISQUE;
	
	pthread_setschedparam(t_erreur, SCHED_FIFO, &erreur_param);
	pthread_setschedparam(t_carton, SCHED_FIFO, &carton_param);
	pthread_setschedparam(t_log_windows, SCHED_FIFO, &log_windows_param);
	pthread_setschedparam(t_palette, SCHED_FIFO, &palette_param);
	pthread_setschedparam(t_cariste, SCHED_FIFO, &cariste_param);
	pthread_setschedparam(t_commande_windows, SCHED_FIFO, &commande_windows_param);
	pthread_setschedparam(t_log_disque, SCHED_FIFO, &log_disque_param);
	
	/*---------------------------------------------------------Moteur----------------------------------------------------------------*/
	pthread_join( t_commande_windows, NULL );
	/*--------------------------------------------------------Destruction-------------------------------------------------------------*/
	
	pthread_kill( t_simulation, SIGUSR2 );
	pthread_join(t_simulation, NULL);
	
	pthread_kill(t_envoi_piece, SIGUSR2);
	pthread_join(t_envoi_piece, NULL);
	
	mq_send(bal_erreur, TRAME_FIN, sizeof(erreur_t), BAL_PRIO_FIN);
	pthread_join( t_erreur, NULL );
	
	mq_send(bal_log_disque, TRAME_FIN, sizeof(log_t), BAL_PRIO_FIN);
	pthread_join( t_log_disque, NULL );
	
	mq_send(bal_log_windows, TRAME_FIN, sizeof(log_t), BAL_PRIO_FIN);
	pthread_join( t_log_windows, NULL );
	
	pthread_kill(t_carton, SIGUSR2);
	pthread_join(t_carton, NULL);

	pthread_kill(t_palette, SIGUSR2);
	pthread_join(t_palette, NULL);
	
	pthread_kill(t_cariste, SIGUSR2);
	pthread_join(t_cariste, NULL);
	
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
	
	/*Boîtes aux lettres*/
	mq_unlink( BALWIN );
	mq_close( bal_log_windows );
	mq_unlink( BALDIS );
	mq_close( bal_log_disque );
	mq_unlink( BALERR );
	mq_close( bal_erreur );

	return 0;
}
