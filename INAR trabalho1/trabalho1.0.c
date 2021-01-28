
/*
    Autor: Beans
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_Q_SIZE 100
struct atomo
{
    bool atomoObjectivo;
    int informacao;
    struct atomo* left;
    struct atomo* right;
};
/*------------------------------------------Prototipos de Funcoes------------------------------------*/
struct atomo** criarFila(int *, int *);
void enfileirar(struct atomo **, int *, struct atomo *);
struct atomo *deSenfileirar(struct atomo **, int *);
struct atomo* novoNo(int informacao, bool objectivo);
/*------------------------------------------------------------------------*/
struct atomo** criarPilha(int *);
void empilhar(struct atomo **, int *, struct atomo *);
struct atomo* desempilhar(struct atomo **, int *);
/*------------------------------------------------------------------------*/
//void posOrdemIterativo(struct atomo* raiz);
void calcularCustoBFS(struct atomo* raiz, int *contador);
void calcularPreOrdemIterativo(struct atomo* raiz, int *contador);
void calcularInOrdemIterativo(struct atomo* raiz, int *contador);
/*------------------------------------------------------------------------*/
void ordemSimetrica(struct atomo* raiz);
void inOrdem(struct atomo* raiz);
void posOrdem(struct atomo* raiz);
void printLevelOrder(struct atomo* raiz);
/*--------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
--------------------------------------------------------------------------- */
struct atomo ** criarFila(int *frente, int *fundo)
{
  struct atomo **queue = (struct atomo **)malloc(sizeof(struct atomo*)*MAX_Q_SIZE);
  *frente = *fundo = 0;
  return queue;
}
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
void enfileirar(struct atomo **queue, int *fundo, struct atomo *new_atomo)
{
  queue[*fundo] = new_atomo;
  (*fundo)++;
}
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
struct atomo *deSenfileirar(struct atomo **queue, int *frente)
{
  (*frente)++;
  return queue[*frente - 1];
}
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
struct atomo* novoNo(int informacao, bool objectivo)
{
  struct atomo* no = (struct atomo*) malloc(sizeof(struct atomo));
  no->informacao = informacao;
  no->atomoObjectivo = objectivo;
  no->left = NULL;
  no->right = NULL;

  return(no);
}
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
struct atomo** criarPilha(int *topo)
{
    struct atomo **stack = (struct atomo **)malloc(sizeof(struct atomo*)*MAX_Q_SIZE);
    *topo = -1;
    return stack;
}
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
void empilhar(struct atomo **stack, int *topo, struct atomo *new_atomo)
{
    (*topo)++;
    stack[*topo] = new_atomo;
}
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
struct atomo* desempilhar(struct atomo **stack, int *topo)
{
    //(*topo);
    return stack[*topo];
}
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
void main()
{
  struct atomo *raiz = novoNo(1,false);
  raiz->left = novoNo(2,false);
  raiz->right = novoNo(3,false);
  raiz->left->left = novoNo(4,false);
  raiz->left->right = novoNo(5,false);
  raiz->right->left = novoNo(6,true);
  raiz->right->right = novoNo(7,false);
  int contador1 = 0,contador2 = 0,contador3 = 0,contador4 = 0;
  printf("\n BFS:             ");
  printLevelOrder(raiz);
  printf("\n DFS (PRE Ordem): ");
  ordemSimetrica(raiz);
  printf("\n DFS (IN Ordem):  ");
  inOrdem(raiz);
  printf("\n DFS (POS Ordem): ");
  posOrdem(raiz);
  printf("\n OS CUSTOS\n");
  printf("\n No objectivo = 6\n");
  calcularPreOrdemIterativo(raiz,&contador1);
  printf(" Pre Ordem custo = %d\n", contador1);
  calcularInOrdemIterativo(raiz,&contador2);
  printf(" In Ordem custo = %d\n", contador2);
  //calcularCustoPosOrdem(raiz,&contador3);
  //printf(" POS Ordem custo = %d\n", contador3);
  calcularCustoBFS(raiz,&contador4);
  printf(" BFS custo = %d\n", contador4);
  exit(0);
}
/*---------------------------------------Algoritmos de Buscas-----------------------------------------*/
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
void printLevelOrder(struct atomo* raiz)
{
  int fundo, frente;
  struct atomo **queue = criarFila(&frente, &fundo);
  struct atomo *no_temp = raiz;
  while (no_temp)
  {
    printf("%d ", no_temp->informacao);
    if (no_temp->left) enfileirar(queue, &fundo, no_temp->left);
    if (no_temp->right) enfileirar(queue, &fundo, no_temp->right);
    no_temp = deSenfileirar(queue, &frente);
  }
}
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
void ordemSimetrica(struct atomo* raiz)
{
    if (raiz != NULL)
    {
      printf("%d ", raiz->informacao);
      ordemSimetrica(raiz->left);
      ordemSimetrica(raiz->right);
    }
}

/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
void inOrdem(struct atomo* raiz)
{
  if (raiz != NULL)
  {
    inOrdem(raiz->left);
    printf("%d ", raiz->informacao);
    inOrdem(raiz->right);
  }
}
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
void posOrdem(struct atomo* raiz)
{
  if (raiz != NULL)
  {
    posOrdem(raiz->left);
    posOrdem(raiz->right);
    printf("%d ", raiz->informacao);
  }
}
/*---------------------------------------------------------------------------------------------
                              Algoritmos de Buscas / TESTE OBJECTIVO
---------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
void calcularCustoPosOrdem(struct atomo* raiz, int *contador)
{
  if (raiz != NULL)
  {
    calcularCustoPosOrdem(raiz->left, contador);
    calcularCustoPosOrdem(raiz->right, contador);
    if (raiz->atomoObjectivo == true)
    {
      printf(" Achou -> %d ", raiz->informacao);
      //(*contador)++;
      return;
    }
    (*contador)++;
  }
}
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
void calcularCustoBFS(struct atomo* raiz, int *contador)
{
  int fundo, frente;
  struct atomo **queue = criarFila(&frente, &fundo);
  struct atomo *no_temp = raiz;

  while (no_temp)
  {
    (*contador)++;
    if (no_temp->atomoObjectivo == true)
    {
      printf(" Achou -> %d ", no_temp->informacao);
      return;
    }
    if (no_temp->left) enfileirar(queue, &fundo, no_temp->left);
    if (no_temp->right) enfileirar(queue, &fundo, no_temp->right);
    no_temp = deSenfileirar(queue, &frente);
  }
}
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
void calcularPreOrdemIterativo(struct atomo* raiz, int *contador)
{
    int topo;
    struct atomo **stack = criarPilha(&topo);
    struct atomo *no_temp = raiz;
    do
    {
        while(no_temp != NULL)
        {
            //printf("%d ", no_temp->informacao);
            (*contador)++;
            if (no_temp->atomoObjectivo == true)
            {
              printf(" Achou -> %d ", no_temp->informacao);
              return;
            }
            empilhar(stack, &topo, no_temp);
            no_temp = no_temp->left;
        }
        no_temp = desempilhar(stack, &topo);
        topo = topo - 1;
        no_temp = no_temp->right;
    }
    while ((stack[topo-1] != NULL) || (no_temp != NULL));
}
/*-------------------------------------------------------------------------------------------------------
Objectivo:
Parâmetro Entrada:
Parâmetro de Saída:
------------------------------------------------------------------------------------------------------ */
void calcularInOrdemIterativo(struct atomo* raiz, int *contador)
{
    int topo;
    struct atomo **stack = criarPilha(&topo);
    struct atomo *no_temp = raiz;
    do
    {
        while(no_temp != NULL)
        {
            empilhar(stack, &topo, no_temp);
            no_temp = no_temp->left;
        }
        no_temp = desempilhar(stack, &topo);
        topo = topo - 1;
        (*contador)++;
        if (no_temp->atomoObjectivo == true)
        {
          printf(" Achou -> %d ", no_temp->informacao);
          return;
        }
        //printf("%d ", no_temp->informacao);
        no_temp = no_temp->right;
    }
    while ((stack[topo-1] != NULL) || (no_temp != NULL));
}
