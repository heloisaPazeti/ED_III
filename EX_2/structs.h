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
    char lixo[83];

} CabecalhoArvBin;

typedef struct _noArvBin
{
    char folha;
    int nroChavesNo;
    int RRNdoNo;

    int *P;
    RegistroInfo *info;

    //long int *C;
    //long int *PR;

} NoArvBin;

typedef struct _registroInfo
{
    long int C;
    long int PR;

} RegistroInfo;

typedef struct _noPos
{
    NoArvBin no;        // No em que foi encontrado / deveria estar
    int pos;            // Posição i do no em que foi encontrado
    int posInsercao;    // Posição i do no que deveria estar

} NoPos;

//////////////////////////////////////////////////////// INICIALIZADORES REGISTROS

RegCabecalho IniciarCabecalho(void);
RegDados IniciarRegistroDados(void);

//////////////////////////////////////////////////////// INICIALIZADORES ARVORE

CabecalhoArvBin CriarCabecalhoArvBin();
NoArvBin CriarNo();

#endif

