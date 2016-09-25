#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h> 

#define NUM_THREADS  100
#define NUM_STEPS 100000

int sum = 0 ;
pthread_mutex_t mutex ;

void *threadBody(void *id)
{
   int i ;

   for (i=0; i< NUM_STEPS; i++)
   {
      pthread_mutex_lock (&mutex) ;
      sum += 1 ;   // critical section
      pthread_mutex_unlock (&mutex) ;
   }

   pthread_exit (NULL) ;
}

int main (int argc, char *argv[])
{
    struct timeval t1, t2;
    double elapsedTime;

    // start timer
    gettimeofday(&t1, NULL);

   pthread_t thread [NUM_THREADS] ;
   pthread_attr_t attr ;
   long i, status ;

   pthread_mutex_init (&mutex, NULL) ;

   pthread_attr_init (&attr) ;
   pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE) ;

   // create threads
   for(i=0; i<NUM_THREADS; i++)
   {
      status = pthread_create (&thread[i], &attr, threadBody, (void *) i) ;
      if (status)
      {
         perror ("pthread_create") ;
         exit (1) ;
      }
   }

   // wait all threads to finish   
   for (i=0; i<NUM_THREADS; i++)
   {
      status = pthread_join (thread[i], NULL) ;
      if (status)
      {
         perror ("pthread_join") ;
         exit (1) ;
      }
   }

   printf ("Sum should be %d and is %d\n", NUM_THREADS*NUM_STEPS, sum) ;

printf("Time Elapsed: ");

   // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("%f ms.\n", elapsedTime);

   pthread_attr_destroy (&attr) ;
   pthread_exit (NULL) ;
}