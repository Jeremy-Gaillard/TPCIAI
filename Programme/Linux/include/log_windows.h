#ifndef LOG_WINDOWS_H
#define LOG_WINDOWS_H

#include "config.h"
#include <semaphore.h>

void envoyer(log_t message);
void log_windows(sem_t windows);

#endif

