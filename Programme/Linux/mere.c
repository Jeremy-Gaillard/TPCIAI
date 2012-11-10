#include <sys/types.h>
#include <sys/ipc.h>  
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>

#include <unistd.h> /*temporaire (sleep)*/

#include <stdio.h>
#include <stdlib.h>

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

/*Definitions des constantes locales*/
const char SEMCLA = 'D';
const char SEMPIE = 'E';
const char SEMCAR = 'F';
const char SEMPAL = 'G';
const char SEMERC = 'H';
const char SEMERP = 'I';
const char SEMAU = 'J';
const char SHMSTA = 'K';
const char SHMLOT = 'L';
const char SHMENT = 'M';


int main(int argc, char** argv)
{
	mqd_t bal_erreur, bal_log_disque, bal_log_windows; /*boîtes aux lettres*/
	int sem_clapet, sem_piece, sem_carton, sem_palette, 
		sem_erreur_carton, sem_erreur_palette, sem_AU;	/*id des semaphores*/
	pthread_mutex_t mutex_entrepot, mutex_lot; /*mutex*/
	int shm_statut, shm_lot, shm_entrepot; /*id des mémoires partagées*/
	
	pthread_t t_carton, t_palette, t_cariste, t_erreur, t_log_disque, 
		t_log_windows, t_commande_windows;
	
	/*Initialisation*/
	
	/*Boîtes aux lettres*/
	bal_erreur = mq_open( BALERR, O_CREAT | O_RDWR, MODERW, NULL);
	bal_log_disque = mq_open( BALDIS, O_CREAT | O_RDWR, MODERW, NULL);
	printf("%i\n", bal_log_disque);
	bal_log_windows = mq_open( BALWIN, O_CREAT | O_RDWR, MODERW, NULL);
	
	/*Sémaphores*/
		/*Création*/
	sem_clapet = semget( ftok(argv[0], SEMCLA), 1, IPC_CREAT | MODERW );
	sem_piece = semget( ftok(argv[0], SEMPIE), 1, IPC_CREAT | MODERW );
	sem_carton = semget( ftok(argv[0], SEMCAR), 1, IPC_CREAT | MODERW );
	sem_palette = semget( ftok(argv[0], SEMPAL), 1, IPC_CREAT | MODERW );
	sem_erreur_carton = semget( ftok(argv[0], SEMERC), 1, IPC_CREAT | MODERW );
	sem_erreur_palette = semget( ftok(argv[0], SEMERP), 1, IPC_CREAT | MODERW );
	sem_AU = semget( ftok(argv[0], SEMAU), 1, IPC_CREAT | MODERW );
	
		/*Initialisation*/
	semctl( sem_clapet, 0, SETVAL, 1 );
	semctl( sem_piece, 0, SETVAL, 0 );
	semctl( sem_carton, 0, SETVAL, 0 );
	semctl( sem_palette, 0, SETVAL, 0 );
	semctl( sem_erreur_carton, 0, SETVAL, 0 );
	semctl( sem_erreur_palette, 0, SETVAL, 0 );
	semctl( sem_AU, 0, SETVAL, 0 );
	
	/*Mutex*/
	pthread_mutex_init( &mutex_lot, NULL );
	pthread_mutex_init( &mutex_entrepot, NULL );
	
	/*Mémoire partagées*/
	shm_statut = shmget( ftok( argv[0], SHMSTA ), sizeof(char), IPC_CREAT | MODERW);
	shm_entrepot = shmget( ftok( argv[0], SHMENT ), sizeof(struct entrepot), IPC_CREAT | MODERW);
	shm_lot = shmget( ftok( argv[0], SHMLOT ), 2*sizeof(int), IPC_CREAT | MODERW);
	
	/*Threads*/
	/*pthread_create( &t_carton, NULL, carton, ? );*/
	pthread_create( &t_log_disque, NULL, (void*) log_disque, NULL );
	/*pthread_create( &t_log_windows, NULL, log_windows, ? );
	pthread_create( &t_palette, NULL, palette, ? );
	pthread_create( &t_cariste, NULL, cariste, ? );
	pthread_create( &t_erreur, NULL, erreur, ? );
	pthread_create( &t_commande_windows, NULL, commande_windows, ? );*/
	
	/*Moteur*/
	/*sleep( 5 );*/
	pthread_join( t_log_disque, NULL );
	/*pthread_join( t_commande_windows, NULL );*/
	
	/*Destruction*/
	
	/*Mémoire partagées*/
	shmctl( shm_lot, IPC_RMID, NULL);
	shmctl( shm_entrepot, IPC_RMID, NULL);
	shmctl( shm_statut, IPC_RMID, NULL);
	
	/*Mutex*/
	pthread_mutex_destroy( &mutex_entrepot );
	pthread_mutex_destroy( &mutex_lot );
	
	/*Sémaphores*/
	semctl( sem_AU, 0, IPC_RMID, NULL );
	semctl( sem_erreur_palette, 0, IPC_RMID, 0 );
	semctl( sem_erreur_carton, 0, IPC_RMID, 0 );
	semctl( sem_palette, 0, IPC_RMID, 0 );
	semctl( sem_carton, 0, IPC_RMID, 0 );
	semctl( sem_piece, 0, IPC_RMID, 0 );
	semctl( sem_clapet, 0, IPC_RMID, 0 );
	
	/*Boîtes aux lettres*/
	mq_unlink( BALWIN );
	mq_close( bal_log_windows );
	mq_unlink( BALDIS );
	mq_close( bal_log_disque );
	mq_unlink( BALERR );
	mq_close( bal_erreur );
	
	return 0;
}
