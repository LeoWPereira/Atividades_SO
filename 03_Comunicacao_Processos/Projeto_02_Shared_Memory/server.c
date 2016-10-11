/*  ***************************************************************************
**
**  Sistemas Operacionais
**  Alunos: - Leonardo Winter Pereira
**          - Lucas Zimmermann Cordeiro
**
**  Referencia: http://www.csl.mtu.edu/
**
**  Esse programa precisa ser inicializado antes do cliente.
**
**  O espaco de memoria possui 3 estados possiveis:
**      o NOT_READY: Quando o servidor ainda nao criou a memoria
**                   ou quando ainda esta a utilizando
**      o FILLED: Quando o servidor terminou de preencher a memoria
**                com 4 (definidos por força bruta) valores
**      o TAKEN: Quando o cliente conseguiu ler todos os valores da memoria
**
** ***************************************************************************/

#include  <stdio.h>
#include  <stdlib.h>

#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>

#include  "shm-02.h"

void  main(int  argc, char *argv[])
{
     key_t          ShmKEY;

     int            ShmID;

     struct Memory  *ShmPTR;
     
     if (argc != 5)
     {
          printf("Use: %s #1 #2 #3 #4\n", argv[0]);
          
          exit(1);
     }
     
     ShmKEY = ftok(".", 'x'); // ftok(const char *path, int id) \
                                 The ftok() function shall return a key based \
                                 on path and id that is usable in subsequent calls \
                                 to msgget(), semget(), and shmget(). The application \
                                 shall ensure that the path argument is the pathname of \
                                 an existing file that the process is able to stat().
     
     ShmID = shmget(ShmKEY, sizeof(struct Memory), IPC_CREAT | 0666);
     if (ShmID < 0)
     {
          printf("*** shmget error (server) ***\n");
          
          exit(1);
     }

     printf("Server has received a shared memory of four integers...\n");
     
     ShmPTR = (struct Memory *) shmat(ShmID, NULL, 0);
     
     if ((int) ShmPTR == -1)
     {
          printf("*** shmat error (server) ***\n");
     
          exit(1);
     }
     printf("Server has attached the shared memory...\n");
     
     ShmPTR->status  = NOT_READY;
     ShmPTR->data[0] = atoi(argv[1]);
     ShmPTR->data[1] = atoi(argv[2]);
     ShmPTR->data[2] = atoi(argv[3]);
     ShmPTR->data[3] = atoi(argv[4]);

     printf("Server has filled %d %d %d %d to shared memory...\n",
            ShmPTR->data[0], ShmPTR->data[1], 
            ShmPTR->data[2], ShmPTR->data[3]);
     
     ShmPTR->status = FILLED;
     
     printf("Please start the client in another window...\n");
                 
     while (ShmPTR->status != TAKEN)
          sleep(1);
          
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR); // shmdt() detaches the shared memory segment \
                                located at the address specified by shmaddr \
                                from the address space of the calling process. \
                                The to-be-detached segment must be currently attached \
                                with shmaddr equal to the value returned by the attaching \
                                shmat() call.
     
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL); // shmctl() performs the control operation specified by \
                                       cmd on the System V shared memory segment whose \
                                       identifier is given in shmid.
     
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     
     exit(0);
}