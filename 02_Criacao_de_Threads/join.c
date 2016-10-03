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
   pthread_attr_t attr ; // cirar uma variavel do tipo pthread_attr_t
   long i, status ;
 
   pthread_attr_init (&attr) ; // Inicializa atributos da thread com valor default 
   pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE) ; // seta o atributo 'attr' para PTHREAD_CREATE_JOINABLE, que permite que uma thread \
                                                                     chame o metodo pthread_join(). Que faz com que a proxima thread espere que a anterior termine \
                                                                     sa execucao
 
   for(i=0; i<NUM_THREADS; i++) // Percorre o vetor de threads, criando uma thread por vez
   {
      printf ("Main: criando thread %02ld\n", i) ; // imprime a thread que esta sendo criada
 
      status = pthread_create (&thread[i], &attr, threadBody, (void *) i) ; // armazena, na variavel 'status', o valor de retorno da funcao pthread_create (0 se sucesso, ou o valor do erro em caso de falha)
                                                                            // int pthread_create(pthread_t *thread, const pthread_attr_t *attr, \
                                                                                                 void *(*start_routine) (void *), void *arg);
                                                                            // onde:
                                                                            // The pthread_create() function starts a new thread in the calling \
                                                                                process. The new thread starts execution by invoking \
                                                                                start_routine(); arg is passed as the sole argument of \
                                                                                start_routine(). \
                                                                                attr sets the attrbutes for the newly created thread 
      if (status) // caso a criacao da thread termine em falha
      {
         perror ("pthread_create") ; // mostra na tela uma mensagem afirmando que ocorreu erro
         exit (1) ;
      }
   }

   for (i=0; i<NUM_THREADS; i++) // novamente percorre o vetor de threads
   {
      printf ("Main: aguardando thread %02ld\n", i);
      status = pthread_join (thread[i], NULL) ; // The pthread_join() function shall suspend execution of the calling  \
                                                   thread until the target thread terminates, unless the target thread \
                                                   has already terminated.
      if (status) // caso a criacao da thread termine em falha
      {
         perror ("pthread_join") ; // mostra na tela uma mensagem afirmando que ocorreu erro
         exit (1) ;
      }
   }
 
   pthread_attr_destroy (&attr) ; // Quando um atributo nao eh mais necessario, este deve ser destruido atraves deste comando. 
 
   pthread_exit (NULL) ; // finaliza a thread em questao
}

/*
Explique o objetivo do parâmetro attr e da chamada pthread_join.

Desenhe o diagrama de tempo de sua execução.
*/