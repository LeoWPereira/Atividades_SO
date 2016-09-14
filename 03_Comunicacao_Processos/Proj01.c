/*  ***************************************************************************
**
**  Sistemas Operacionais
**  Alunos: - Leonardo Winter Pereira
**          - Lucas Zimmermann Cordeiro
**
**  Referencia: USP
**
**  Este programa eh bem simples:
**
**    o O pai cria a fila de mensagens
**    o o pai cria dois filhos
**    o O primeiro filho faz:
**      - Recebe a mensagem da fila
**      - Chama gettimeofday()
**      - Usando o tempo da mensagem, calcula a diferenca e armazena esse valor em um array
**      - Faz um Loop X vezes
**      - Retorna os resultados
**
**    o O segundo filho faz:
**      - Chama gettimeofday()
**      - Coloca o tempo adquirido acima em uma mensagem
**      - Adiciona a mensagem na fila
**      - Pausa para permitir que o outro filho execute
**      - Faz um loop X vezes
**    o O pai aguarda os filhos terminarem
**
** ***************************************************************************/

/*
** Includes Necessarios
*/
#include <stdio.h>	            
#include <stdlib.h>

#include <sys/time.h>			/* gettimeofday() */
#include <sys/ipc.h>			/* msgget(), msgctl() */
#include <sys/msg.h>			/* msgget(), msgctl() */
#include <sys/types.h>	        /* wait(), msgget(), msgctl() */

#include <unistd.h>	            /* gettimeofday() */
#include <unistd.h>	           	/* fork() */

#include <wait.h>		    	/* wait() */


/*
** NO_OF_ITERATIONS corresponde ao numero de mensagens que serao enviadas.
** Se este numero cresce, o tempo de execucao tambem cresce.
*/
#define NO_OF_ITERATIONS	1000

/*
** MICRO_PER_SECOND define o numero de microsegundos em um segundo
*/
#define MICRO_PER_SECOND	1000000

/*
** MESSAGE_QUEUE_ID, e uma chave arbitraria, foi escolhido um numero qualquer,
** que deve ser unico. Se outra pessoa estiver executando este mesmo programa
** ao mesmo tempo, o numero tem que ser mudado!
*/
#define MESSAGE_QUEUE_ID	1234

/*
** Constantes
*/
#define	SENDER_DELAY_TIME	20
#define MESSAGE_MTYPE		1

/*
** Filhos
*/
void Receiver(int queue_id);
void Sender(int queue_id);

/*
** Programa principal
*/
int main( int argc, char *argv[] )
{
  /*
  ** Algumas variaveis necessarias
  */
  int rtn;
  int count;

  /* 
  ** Variaveis relativas a fila, id e key
  */
  int queue_id;
  key_t key = MESSAGE_QUEUE_ID;

  /*
  ** Cria a fila de mensagens, 0666 e o modo que significa que 
  ** qualquer um pode ler e escrever nessa fila!
  */
  if( (queue_id = msgget(key, IPC_CREAT | 0666)) == -1 ) {
    fprintf(stderr,"Impossivel criar a fila de mensagens!\n");
    exit(1);
  }

  /*
  ** Segundo, inicializa dois filhos
  **
  ** OBS:  Lembre-se, os filhos herdarao a informacao sobre a fila
  **       estabelecida no pai!
  */
  rtn = 1;
  for( count = 0; count < 2; count++ ) {
    if( rtn != 0 ) {
      rtn = fork();
    } else {
      break;
    }
  }

  /*
  ** Verifica o valor retornado para determinar se o processo e pai ou filho
  **
  ** OBS:  o valor de count no loop anterior indicara cada um dos filhos
  **       count = 1 para o primeiro filho, 2 para o segundo, etc.
  */
  if( rtn == 0 && count == 1 ) {

    /*
    ** Sou o primeiro filho me preparando para receber uma mensagem
    */
    printf("Receptor iniciado ...\n");
    Receiver(queue_id);
    exit(0);

  } else if( rtn == 0 && count == 2 ) {
    /*
    ** Sou o segundo filho me preparando para enviar uma mensagem
    */
    printf("Enviador iniciado ...\n");
    Sender(queue_id);
    exit(0);

  } else {
    /*
    ** Sou o pai aguardando meus filhos terminarem
    */
    wait(NULL);
    wait(NULL);

    /*
    ** Removendo a fila de mensagens
    */
    if( msgctl(queue_id,IPC_RMID,NULL) != 0 ) {
      fprintf(stderr,"Impossivel remover a fila!\n");
      exit(1);
    }

    exit(0);
  }
}


/*
** A estrutura de dados seguinte contera os dados que serao transferidos  
** pela fila.  Pode conter um numero de mensagem (msg_no) e o tempo de envio
** (send_time).  Para filas de mensagens, esta estrutura pode conter
** qualquer dado que seja necessario.
*/
typedef struct {
  unsigned int msg_no;
  struct timeval send_time;
} data_t; 


/* 
** O conteudo da estrutura de dados seguinte sera enviado atraves da fila de
** mensagens. A estrutura contem dois dados.  O primeiro e o tipo da
** mensagem (mtype) que sera uma identificacao de mensagem. Para este
** experimento este sera sempre o mesmo. O segundo e uma matriz com tamanho
** igual a estrutura declarada anteriormente.  Nessa matriz e que o dados
** a serem transferidos serao colocados.  Sob a maioria das circunstancias,
** esta estrutura nao necessita mudar.
*/
typedef struct {
  long mtype;
  char mtext[sizeof(data_t)];
} msgbuf_t;


/*
** Esta funcao recebe as mensagens
*/
void Receiver(int queue_id)
{
  /*
  ** Variaveis locais
  */
  int count;
  struct timeval receive_time;
  float delta;
  float max = 0;
  float total = 0;

  /*
  ** Este e o buffer para receber a mensagem
  */
  msgbuf_t message_buffer;

  /*
  ** Este e o ponteiro para os dados no buffer.  Note
  ** como e setado para apontar para o mtext no buffer
  */
  data_t *data_ptr = (data_t *)(message_buffer.mtext);

  /*
  ** Inicia o loop
  */
  for( count = 0; count < NO_OF_ITERATIONS; count++ ) {

    /*
    ** Recebe qualquer mensagem do tipo MESSAGE_MTYPE
    */
    if( msgrcv( queue_id,(struct msgbuf *) &message_buffer,
                sizeof(data_t),MESSAGE_MTYPE,0) == -1 ) {
      fprintf(stderr, "Impossivel receber mensagem!\n");
      exit(1);
    }

    /*
    ** Chama gettimeofday()
    */
    gettimeofday(&receive_time,NULL);

    /*
    ** Calcula a diferenca
    */
    delta  = receive_time.tv_sec  - data_ptr->send_time.tv_sec;
    delta += (receive_time.tv_usec - data_ptr->send_time.tv_usec) /
      (float)MICRO_PER_SECOND;
    total += delta;

    /*
    ** Salva o tempo maximo
    */
    if( delta > max ) {
      max = delta;
    }
  }

  /*
  ** Exibe os resultados
  */
  printf( "O tempo medio de transferencia: %.6f\n", total / NO_OF_ITERATIONS );
  printf( "O tempo maximo de transferencia: %.6f\n", max );

  return;
}


/*
** Esta funcao envia mensagens
*/
void Sender(int queue_id)
{
  /*
  ** Variaveis locais
  */
  int count;
  struct timeval send_time;

  /*
  ** Este e o buffer para as mensagens enviadas
  */
  msgbuf_t message_buffer;

  /*
  ** Este e o ponteiro para os dados no buffer.  Note
  ** como e setado para apontar para mtext no buffer
  */
  data_t *data_ptr = (data_t *)(message_buffer.mtext);

  /*
  ** Inicia o loop
  */
  for( count = 0; count < NO_OF_ITERATIONS; count++ ) {

    /*
    ** Chama gettimeofday()
    */
    gettimeofday(&send_time,NULL);

    /*
    ** Apronta os dados
    */
    message_buffer.mtype = MESSAGE_MTYPE;
    data_ptr->msg_no = count;
    data_ptr->send_time = send_time;

    /*
    ** Envia a mensagem... usa a identificacao da fila, um ponteiro
    ** para o buffer, e o tamanho dos dados enviados
    */
    if( msgsnd( queue_id,(struct msgbuf *) &message_buffer,
                sizeof(data_t),0) == -1 ) {
      fprintf(stderr, "Impossivel enviar mensagem!\n");
      exit(1);
    }

    /*
    ** Dorme por um curto espaco de tempo para permitir ao receptor ser
    ** executado
    */
    usleep(SENDER_DELAY_TIME);
  }

  return;
}
