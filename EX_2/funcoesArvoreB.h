#ifndef FUNCOESARVOREB_H
#define FUNCOESARVOREB_H

#include "structs.h"
#include "funcoesFornecidas.h"

//////////////////////////////////////////////////////// TAMANHOS
#define tamCabecalho 93
#define tamNo 93

#define tamP 5
#define tamCPR 4
//////////////////////////////////////////////////////// STRUCTS

typedef struct _cabecalhoArvBin
{
    char status;
    int  noRaiz;
    int  RRNproxNo;
    char lixo[83];

} CabecalhoArvBin;

typedef struct _noArvBin
{
    char folha;
    int nroChavesNo;
    int RRNdoNo;

    int *P;
    long int *C;
    long int *PR;

} NoArvBin;

typedef struct _noPos
{
    NoArvBin no;
    int pos;

} NoPos;

//////////////////////////////////////////////////////// INICIALIZADORES

CabecalhoArvBin CriarCabecalhoArvBin();
NoArvBin CriarArvBin();

///////////////////////////////////////////////////////////////// CRIAR ARVORE (7)

int CriarArvore(char *nomeArq, char *nomeArqArvore);
int BuscarRegistroArvore(char *nomeArq, char *nomeArqArvore);
int AdicionarRegistroARvore(char *nomeArq, char *nomeArqArvore);

///////////////////////////////////////////////////////////////// OUTROS 

NoPos BuscarNoArvore(char *arquivo, char *nomeRegistro);
NoArvBin LerNoArvore(char *arquivo, int rrn);
CabecalhoArvBin LerCabecalhoArvore(char *arquivo);

#endif