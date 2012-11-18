#include "commande_windows.h"
#include "config.h"
#include "server.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#include <stdlib.h>
#include <string.h>

static statut_t* statut;
static lot_t* lot;
static entrepot_t* entrepot;
static pthread_mutex_t* mtx_entrepot;
static sem_t* sem_erreur_palette;
static sem_t* sem_erreur_carton;
static sem_t* sem_AU;
static sem_t* sem_clapet;

static int my_socket;

void init_commande_windows(int asocket)
{
	my_socket = asocket;
}

void reprise(int erreur_id)
{
	switch(erreur_id) 
	{
		case ERR_AU :
			sem_post(sem_AU);
			sem_post(sem_AU);
			break;
		case ERR_TROP_DE_REBUS :
		case ERR_PAS_DE_CARTON :
		case ERR_IMPRIMANTE_KO :
		case ERR_FILE_D_ATTENTE :
			sem_post(sem_erreur_carton);
			break;
		case ERR_PAS_DE_PALETTE :
		case ERR_FILM_KO :
			sem_post(sem_erreur_palette);
			break;
		default :
			break;
	}
	
	int i;
	for ( i = 0; i < STATUT_SIZE; i++) /*on verifie que tout le matériel est OK*/
	{
		if((*statut)[i] == 0 && i != ST_CLAPET_OUVERT && i != ST_PIECE)
			break;
	}
	if(i == STATUT_SIZE)
	{
		(*statut)[ST_CLAPET_OUVERT] = 1;
		sem_post(sem_clapet);
	}
	
}

void terminaison()
{
	pthread_exit(0);
}

void commander_lot(int nb_A, int nb_B, int prc_max_rebus)
{
	*lot[0] = nb_A;
	*lot[1] = nb_B;
	*lot[2] = prc_max_rebus;
	(*statut)[ST_CLAPET_OUVERT] = 1;
	sem_post(sem_clapet);
}

void expedier_lot(int nb_A, int nb_B)
{
	int i;
	pthread_mutex_lock(mtx_entrepot);
	for ( i = 0; i < 20; i++)
	{
		if( entrepot->palettes[i].id != NO_PALETTE)
		{
			if( entrepot->palettes[i].type == LOT_A && nb_A > 0)
			{
				entrepot->palettes[i].id = NO_PALETTE;
				nb_A--;
				if( nb_A == 0 && nb_B == 0)
					break;	
			}
			else if( entrepot->palettes[i].type == LOT_B && nb_B > 0)
			{
				entrepot->palettes[i].id = NO_PALETTE;
				nb_B--;
				if( nb_B == 0 && nb_A == 0)
					break;		
			} 
		}
	}
	pthread_mutex_unlock(mtx_entrepot);	
}

void commande_windows(arg_commande_windows_t* ipc)
{	
	statut = ipc->shm_statut;
	lot = ipc->shm_lot;
	entrepot = ipc->shm_entrepot;
	mtx_entrepot = ipc->entrepot;
	sem_erreur_palette = ipc->palette;
	sem_erreur_carton = ipc->carton;
	sem_AU = ipc->AU;
	sem_clapet = ipc->clapet;
	//sleep(10);
	wait_order(my_socket);
}



