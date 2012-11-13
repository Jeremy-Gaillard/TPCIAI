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

void reprise(int erreur_id)
{

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
	for ( i = 0; i < 20; i++)
	{
		if( shm_entrepot->palettes[i].id != NO_PALETTE)
		{
			if( shm_entrepot->palettes[i].type == LOT_A)
			{
				shm_entrepot->palettes[i].id = NO_PALETTE;
				nb_A--;
				if( nb_A == 0)
				break;	
			}
			else if( shm_entrepot->palettes[i].type == LOT_B)
			{
				shm_entrepot->palettes[i].id = NO_PALETTE;
				nb_B--;
				if( nb_B == 0)
				break;		
			} 
		}
	}	
}

void commande_windows(statut_t* shm_statut, lot_t* shm_lot, entrepot_t* shm_entrepot)
{
	statut = shm_statut;
	lot = shm_lot;
	entrepot = shm_entrepot;

}
