#ifndef ERREUR_H
#define ERREUR_H

#include "config.h"

void error(const char* msg);
int server(int portno);
void wait_order(int newsockfd);

#endif

