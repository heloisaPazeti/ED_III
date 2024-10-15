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
    int P[tamP];
    long int C[tamCPR];
    long int PR[tamCPR];

} NoArvBin;

//////////////////////////////////////////////////////// INICIALIZADORES

CabecalhoArvBin CriarCabecalhoArvBin();
NoArvBin CriarArvBin();

///////////////////////////////////////////////////////////////// CRIAR ARVORE (7)

int CriarArvore(char *nomeArq, char *nomeArqArvore);
int BuscarRegistroArvore(char *nomeArq, char *nomeArqArvore);
int AdicionarRegistroARvore(char *nomeArq, char *nomeArqArvore);

///////////////////////////////////////////////////////////////// OUTROS 

NoArvBin BuscarNoArvore(char *arquivo, char *nomeRegistro, int *index);
NoArvBin LerNoArvore(char *arquivo, int rrn);
CabecalhoArvBin LerCabecalhoArvore(char *arquivo);

#endif