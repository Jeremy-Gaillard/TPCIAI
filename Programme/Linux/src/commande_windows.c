#include "commande_windows.h"
#include "config.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <pthread.h>

#include <stdlib.h>
#include <string.h>

static statut_t* statut;
static lot_t* lot;
static entrepot_t entrepot;
static pthread_mutex_t mtx_entrepot;
static sem_t sem_erreur_palette;
static sem_t sem_erreur_carton;
static sem_t sem_AU;
static sem_t sem_clapet;

void reprise(int erreur_id) 
{
	switch(erreur_id) 
	{
		case ERR_AU :
			sem_post(&sem_AU);
			sem_post(&sem_AU);
			break;
		case ERR_TROP_DE_REBUS :
		case ERR_PAS_DE_CARTON :
		case ERR_IMPRIMANTE :
		case ERR_FILE_D_ATTENTE :
			sem_post(&sem_erreur_carton);
			break;
		case ERR_PAS_DE_PALETTE :
		case ERR_FILM_KO :
			sem_post(&sem_erreur_palette);
			break;
		default :
			break;
	}
	
	int i;
	for ( i = 0; i < STATUT_SIZE - 1; i++) /*dernière case : clapet ouvert -> 0, on verifie que tout le matériel est OK*/
	{
		if((*shm_statut)[i] == 0)
			break;
	}
	if(i == STATUT_SIZE - 2)
	{
		(*shm_statut)[STATUT_SIZE - 1] = 1;
		sem_post(&sem_clapet);
	}
	
}

void terminaison()
{
	pthread_exit(0);
}

void commander_lot(int nb_A, int nb_B)
{
	*shm_lot[0] = nb_A;
	*shm_lot[1] = nb_B;
}

void expedier_lot(int nb_A, int nb_B)
{
	pthread_mutex_lock(&mtx_entrepot);
	for ( i = 0; i < 20; i++)
	{
		if( shm_entrepot->palettes[i].id != NO_PALETTE)
		{
			if( shm_entrepot->palettes[i].type == LOT_A && nb_A > 0)
			{
				shm_entrepot->palettes[i].id = NO_PALETTE;
				nb_A--;
				if( nb_A == 0 && nb_B == 0)
					break;	
			}
			else if( shm_entrepot->palettes[i].type == LOT_B && nb_B > 0)
			{
				shm_entrepot->palettes[i].id = NO_PALETTE;
				nb_B--;
				if( nb_B == 0 && nb_A == 0)
					break;		
			} 
		}
	}
	pthread_mutex_unlock(&mtx_entrepot);	
}

void commande_windows(statut_t* shm_statut, lot_t* shm_lot, entrepot_t* shm_entrepot)
{
	statut = shm_statut;
	lot = shm_lot;
	entrepot = shm_entrepot;

}
