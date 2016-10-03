/*  ***************************************************************************
**
**  Sistemas Operacionais
**  Alunos: - Leonardo Winter Pereira
**          - Lucas Zimmermann Cordeiro
**
**  Referencia: http://www.csl.mtu.edu/
**
** ***************************************************************************/

#define  NOT_READY  -1
#define  FILLED     0
#define  TAKEN      1

struct Memory
{
    int  status;
    int  data[4];
};