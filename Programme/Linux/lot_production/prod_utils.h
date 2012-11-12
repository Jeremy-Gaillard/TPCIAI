#ifndef PROD_UTILS_H
#define PROD_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include "config.h"


int carton( mqd_t bal_erreur, mdq_t bal_log_disque, mdq_t bal_log_windows,
            sem_t sem_piece, sem_t sem_carton, sem_t sem_erreur_carton,
            statut_t shm_statut );

int palette();

int cariste();

void gerer_erreur( int erreur_id, mqd_t bal_erreur );

#endif
