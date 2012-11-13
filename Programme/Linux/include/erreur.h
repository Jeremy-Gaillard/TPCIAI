#ifndef ERREUR_H
#define ERREUR_H

#include "config.h"
#include <semaphore.h>

void erreur(statut_t* shm_statut, sem_t erreur, sem_t windows, sem_t disque);

#endif

