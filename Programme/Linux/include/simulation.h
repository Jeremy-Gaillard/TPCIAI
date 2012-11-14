#ifndef SIMULATION_H
#define SIMULATION_H

#include <semaphore.h>

typedef struct arg_simulation
{
	statut_t* statut;
	sem_t* erreur;
	sem_t* windows;
	sem_t* disque;
	sem_t* clapet;
	sem_t* piece;
} arg_simulation_t;

void simulation(arg_simulation_t* ipc);

#endif

