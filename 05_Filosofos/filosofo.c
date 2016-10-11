#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "filosofo.h"

#define DELAY 5

int status[5];
int nfilosofo[5] = {0, 1, 2, 3, 4};

sem_t sem_mutex;
sem_t S[5];

void *filosofo(void *num)
{
    for (;;)
    {
        int *i = num;

        pegaTalher(*i);
        largaTalher(*i);

        sleep(DELAY);
    }
}

void pegaTalher(int nfilosofo)
{
    sem_wait(&sem_mutex);

    status[nfilosofo] = 1;

    printf("Filosofo com fome: %d \n", nfilosofo + 1);

    checar(nfilosofo);

    sem_post(&sem_mutex);

    sem_wait(&S[nfilosofo]);
}

void largaTalher(int nfilosofo)
{
    sem_wait(&sem_mutex);

    status[nfilosofo] = 0;

    printf("Filosofo %d largou os talheres %d e %d.\n", nfilosofo + 1, ESQUERDA + 1, nfilosofo + 1);

    printf("Filosofo %d pensando...\n", nfilosofo + 1);

    checar(ESQUERDA);

    checar(DIREITA);

    sem_post(&sem_mutex);
}

void checar(int nfilosofo)
{
    if ((status[nfilosofo] == 1) && (status[ESQUERDA] != 2) && (status[DIREITA] != 2))
    {
        status[nfilosofo] = 2;

        sleep(2 * DELAY);

        printf("Filosofo %d pegou os talheres %d e %d.\n", nfilosofo + 1, ESQUERDA + 1, nfilosofo + 1);

        printf("Filosofo %d comendo...\n", nfilosofo + 1);

        sem_post(&S[nfilosofo]);
    }
}