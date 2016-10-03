#ifndef __FILOSOFO__
#define __FILOSOFO__

#include <semaphore.h>

#define ESQUERDA (nfilosofo + 4) % 5
#define DIREITA  (nfilosofo + 1) % 5

extern int status[5];
extern int nfilosofo[5];

extern sem_t sem_mutex;
extern sem_t S[5];

void *filosofo(void *num);

void pegaTalher(int nfilosofo);

void largaTalher(int nfilosofo);

void checar(int nfilosofo);

#endif