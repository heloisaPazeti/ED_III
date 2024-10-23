#ifndef STRUCTS_H
#define STRUCTS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////// TAMANHOS REGISTROS
#define tamRegistro 160
#define tamTotalCabecalho 1600
#define cabecalhoFixo 21

//////////////////////////////////////////////////////// TAMANHOS ARVORE
#define tamCabecalhoArvore 93
#define tamNo 93

#define ordemArvore 5
#define tamCPR 4
//////////////////////////////////////////////////////// STRUCTS REGISTROS

typedef struct _cabecalho
{
    char status;
    int topo;
    int proxRRN;
    int nroRegRem;
    int nroPagDisco;
    int qttCompacta;

} RegCabecalho;

typedef struct _dados
{
    char removido;
    int encadeamento;
    int populacao;
    float tamanho;
    char unidadeMedida;
    int velocidade;

    char *nome;
    char *especie;
    char *habitat;
    char *tipo;
    char *dieta;
    char *alimento;

} RegDados;

//////////////////////////////////////////////////////// STRUCTS ARVORE

typedef struct _cabecalhoArvBin
{
    char status;
    int  noRaiz;
    int  RRNproxNo;
    char lixo[84];

} CabecalhoArvBin;

typedef struct _registroInfo
{
    long int C;
    long int PR;

} RegistroInfo;

typedef struct _noArvBin
{
    char folha;
    int nroChavesNo;
    int RRNdoNo;

    int *P;
    RegistroInfo *info;
    
} NoArvBin;

typedef struct _noPos
{
    NoArvBin no;        // No em que foi encontrado / deveria estar
    NoArvBin noAnt;     // No anterior para insercao com overflow
    int pos;            // Posição i do no em que foi encontrado
    int posInsercao;    // Posição i do no que deveria estar

    int *listaRRN;
    int tamListaRRN;

} NoPos;

//////////////////////////////////////////////////////// INICIALIZADORES REGISTROS

RegCabecalho IniciarCabecalho(void);
RegDados IniciarRegistroDados(void);

//////////////////////////////////////////////////////// INICIALIZADORES ARVORE

CabecalhoArvBin CriarCabecalhoArvBin();
NoArvBin CriarNo();
NoPos CriarNoPos();

#endif

//////////////////////////////////////////////////////// DESTRUIDORES ARVORE

void liberaNo(NoArvBin no);
void liberaNoPos(NoPos *noPos);