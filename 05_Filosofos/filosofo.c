#include "filosofo.h"

void *filosofo(void *num)
{
    while(1)
    {
        int *i = num;
        
        pegaTalher(*i);
        largaTalher(*i);
        
        Sleep(DELAY);
    }
}

void pegaTalher(int nfilosofo)
{
    sem_wait(&sem_mutex);

    status[nfilosofo] = 1;

    printf("Filosofo com fome: %d \n", nfilosofo+1);

    checar(nfilosofo);

    sem_post(&sem_mutex);
    sem_wait(&S[nfilosofo]);
}

void largaTalher(int nfilosofo)
{
    sem_wait(&sem_mutex);
    
    status[nfilosofo]=0;
    
    printf("Filosofo n. %d largou os Talhers %d e %d.\n", nfilosofo+1, ESQ+1, nfilosofo+1);
    printf("Filosofo pensando :%d \n", nfilosofo+1);
    
    checar(ESQ);
    checar(DIR);
    
    sem_post(&sem_mutex);
}

void checar(int nfilosofo)
{
    if(status[nfilosofo] == 1 && status[ESQ] != 2 && status[DIR] != 2)
    {
        status[nfilosofo] = 2;
        
        Sleep(2 * DELAY);
        
        printf("Filosofo n. %d pegou os Talhers %d e %d.\n", nfilosofo+1, ESQ+1, nfilosofo+1);
        printf("Filosofo comendo: %d \n", nfilosofo+1);
        
        sem_post(&S[nfilosofo]);
    }
}