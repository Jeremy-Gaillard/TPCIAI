#include "config.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>      
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>

#include <unistd.h> /*temporaire (sleep)*/

#include <stdio.h>


const char BALERR = 'A';
const char BALDIS = 'B';
const char BALWIN = 'C';
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
	pid_t bal_erreur, bal_log_disque, bal_log_windows; /*id des boites aux lettres*/
	pid_t sem_clapet, sem_piece, sem_carton, sem_palette, 
		sem_erreur_carton, sem_erreur_palette, sem_AU;	/*id des semaphores*/
	pthread_mutex_t mutex_entrepot, mutex_lot; /*mutex*/
	pid_t shm_statut, shm_lot, shm_entrepot; /*id des mémoires partagées*/
	
	pthread_t t_carton, t_palette, t_cariste, t_erreur, t_log_disque, 
		t_log_windows, t_commande_windows;
	
	/*Initialisation*/
	
	/*Boîtes aux lettres*/
	bal_erreur = msgget( ftok(argv[0], BALERR), IPC_CREAT | MODERW );
	bal_log_disque = msgget( ftok(argv[0], BALDIS), IPC_CREAT | MODERW );
	bal_log_windows = msgget( ftok(argv[0], BALWIN), IPC_CREAT | MODERW );
	
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
	/*pthread_create( &t_carton, NULL, &carton, ? );
	pthread_create( &t_log_disque, NULL, &log_disque, ? );
	pthread_create( &t_log_windows, NULL, &log_windows, ? );
	pthread_create( &t_palette, NULL, &palette, ? );
	pthread_create( &t_cariste, NULL, &cariste, ? );
	pthread_create( &t_erreur, NULL, &erreur, ? );
	pthread_create( &t_commande_windows, NULL, &commande_windows, ? );*/
	
	/*Moteur*/
	sleep( 5 );
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
	msgctl( bal_log_windows, IPC_RMID, NULL );
	msgctl( bal_log_disque, IPC_RMID, NULL );
	msgctl( bal_erreur, IPC_RMID, NULL );
	
	return 0;
}
