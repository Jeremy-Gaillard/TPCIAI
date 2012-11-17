#ifndef ERREUR_H
#define ERREUR_H

#include "config.h"

typedef struct arg_erreur
{
	statut_t* statut;
} arg_erreur_t;
	
void erreur(arg_erreur_t* ipc);

#endif

