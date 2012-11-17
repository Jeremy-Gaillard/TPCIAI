#ifndef SIMULATION_H
#define SIMULATION_H

#include <semaphore.h>

typedef struct arg_simulation
{
	statut_t* statut;
	sem_t* clapet;
	sem_t* piece;
	pthread_t t_carton;
	pthread_t t_palette;
} arg_simulation_t;

void envoi_piece(sem_t* sem_piece);
void simulation(arg_simulation_t* ipc);

#endif

