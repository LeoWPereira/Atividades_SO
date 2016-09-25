/*  ***************************************************************************
**
**  Sistemas Operacionais
**  Alunos: - Leonardo Winter Pereira
**          - Lucas Zimmermann Cordeiro
**
**  Referencia: http://www.csl.mtu.edu/
**
**  Esse eh o programa cliente. Somente pode ser inicializado quando o servidor
**  dizer que sim, ou ao menos a partir do momento que uma memoria compartilhada
**  tiver sido criada.
**  O cliente requere o mesmo espaco de memoria que o servidor criou, armazena
**  esse espaco em seu proprio espaco de memoria (quando voce cria processos
**  compartilhados, cada processo tem seu proprio espaco de memoria. Portanto,
**  ponteiros e variaveis sao validas somente no contexto deles), recupera os
**  valores, altera o Memory->status para TAKEN, desconecta essa memoria compartilhada
**  e por fim finaliza!
**
** ***************************************************************************/

#include  <stdio.h>
#include  <stdlib.h>

#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>

#include  "shm-02.h"

void  main()
{
     key_t          ShmKEY;

     int            ShmID;
     
     struct Memory  *ShmPTR;
     
     ShmKEY = ftok(".", 'x'); // ftok(const char *path, int id) \
                                 The ftok() function shall return a key based \
                                 on path and id that is usable in subsequent calls \
                                 to msgget(), semget(), and shmget(). The application \
                                 shall ensure that the path argument is the pathname of \
                                 an existing file that the process is able to stat().
     
     ShmID = shmget(ShmKEY, sizeof(struct Memory), 0666);
     
     if (ShmID < 0)
     {
          printf("*** shmget error (client) ***\n");
          
          exit(1);
     }
     printf("   Client has received a shared memory of four integers...\n");
     
     ShmPTR = (struct Memory *) shmat(ShmID, NULL, 0);

     if ((int) ShmPTR == -1)
     {
          printf("*** shmat error (client) ***\n");
     
          exit(1);
     }
     printf("   Client has attached the shared memory...\n");
     
     while (ShmPTR->status != FILLED);

     printf("   Client found the data is ready...\n");
     printf("   Client found %d %d %d %d in shared memory...\n",
                ShmPTR->data[0], ShmPTR->data[1], 
                ShmPTR->data[2], ShmPTR->data[3]);

     ShmPTR->status = TAKEN;
     printf("   Client has informed server data have been taken...\n");
     
     shmdt((void *) ShmPTR); // shmdt() detaches the shared memory segment \
                                located at the address specified by shmaddr \
                                from the address space of the calling process. \
                                The to-be-detached segment must be currently attached \
                                with shmaddr equal to the value returned by the attaching \
                                shmat() call.
     
     printf("   Client has detached its shared memory...\n");
     printf("   Client exits...\n");
     
     exit(0);
}
