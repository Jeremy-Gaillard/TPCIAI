#ifndef SIMULATION_H
#define SIMULATION_H

#include <semaphore.h>

typedef struct arg_simulation
{
	statut_t* statut;
	sem_t* clapet;
	pthread_t t_carton;
	pthread_t t_palette;
} arg_simulation_t;

typedef struct arg_envoi_piece
{
	sem_t* piece;
	sem_t* clapet;
	statut_t* statut;
} arg_envoi_piece_t;

void envoi_piece(arg_envoi_piece_t* ipc);
void simulation(arg_simulation_t* ipc);

#endif

