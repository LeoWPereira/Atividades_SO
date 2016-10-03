#include <pthread.h>
#include <stdio.h>

#include "filosofo.h"

#define NUM_FILOSOFOS sizeof(nfilosofo) / sizeof(nfilosofo[0])

void main (int argc, char* argv[])
{
    int i;

    pthread_t thread_id[NUM_FILOSOFOS];

    sem_init(&sem_mutex, 0, 1);

    for (i = 0; i < NUM_FILOSOFOS; i++)
        sem_init(&S[i], 0, 0);

    for (i = 0; i < NUM_FILOSOFOS; i++)
    {
        pthread_create(&thread_id[i],
                       NULL,
                       filosofo,
                       &nfilosofo[i]);

        printf("Filosofo criado n: %d \n", i + 1);
    }

    for (i = 0; i < NUM_FILOSOFOS; i++)
        pthread_join(thread_id[i], NULL);
}