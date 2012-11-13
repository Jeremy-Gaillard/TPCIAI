#ifndef COMMANDE_WINDOWS_H
#define COMMANDE_WINDOWS_H

#include "config.h"

void reprise(int erreur_id);
void terminaison();
void commander_lot(int nb_A, int nb_B);
void expedier_lot(int nb_A, int nb_B);
void commande_windows(statut_t* shm_statut, lot_t* shm_lot, entrepot_t* shm_entrepot);

#endif

