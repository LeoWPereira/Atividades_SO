#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
#define NUM_THREADS 16
 
 //
 // Basicamente, eh o argumento *start_routine passado para a funcao pthread_create
 //
void *threadBody (void *id) // *id eh o mesmo que o (void *) i passado como parametro pela funcao pthread_create()
{
   long tid = (long) id ;
 
   printf ("t%02ld: Olá!\n", tid) ; 
   sleep (3) ;   // 'dorme' por 3 ms
   printf ("t%02ld: Tchau!\n", tid) ;
 
   pthread_exit (NULL) ; // finaliza a thread em questao
}
 
int main (int argc, char *argv[])
{
   pthread_t thread [NUM_THREADS] ; // cria um vetor com 16 threads
   long i, status ;
 
   for (i=0; i<NUM_THREADS; i++) // Percorre o vetor de threads, criando uma thread por vez
   {
      printf ("Main: criando thread %02ld\n", i) ; // imprime a thread que esta sendo criada
 
      status = pthread_create (&thread[i], NULL, threadBody, (void *) i) ; // armazena, na variavel 'status', o valor de retorno da funcao pthread_create (0 se sucesso, ou o valor do erro em caso de falha)
                                                                           // int pthread_create(pthread_t *thread, const pthread_attr_t *attr, \
                                                                                                 void *(*start_routine) (void *), void *arg);
                                                                           // onde:
                                                                           // The pthread_create() function starts a new thread in the calling \
                                                                                process. The new thread starts execution by invoking \
                                                                                start_routine(); arg is passed as the sole argument of \
                                                                                start_routine().
 
      if (status) // caso a criacao da thread termine em falha
      {
         perror ("pthread_create") ; // mostra na tela uma mensagem afirmando que ocorreu erro
         exit (1) ;
      }
   }
   pthread_exit (NULL) ; // finaliza a thread em questao
}

/*
A ordem de criação, ativação e encerramento das threads é a mesma? Por que?

- Nao. Isso ocorre porque o programador nao tem nenhum controle sobre o escalonamento do processador. Isso
significa que, para processos executados em paralelos, voce nao tem como saber a ordem exata de execucao

Desenhe o diagrama de tempo de sua execução.
*/