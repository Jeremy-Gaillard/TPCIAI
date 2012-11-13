#ifndef SIMULATION_H
#define SIMULATION_H

#include <semaphore.h>

void simulation(statut_t* shm_statut, sem_t erreur, sem_t windows, sem_t disque, sem_t clapet);

#endif

