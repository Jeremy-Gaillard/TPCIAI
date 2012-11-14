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

void envoi_piece(sem_t* sem_piece);
void simulation(arg_simulation_t* ipc);

#endif

