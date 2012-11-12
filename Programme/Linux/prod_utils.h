#ifndef PROD_UTILS_H
#define PROD_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include "config.h"

void gerer_erreur( int erreur_id, mqd_t bal_erreur );

#endif
