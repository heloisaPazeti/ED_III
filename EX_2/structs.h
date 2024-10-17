#ifndef STRUCTS_H
#define STRUCTS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////// TAMANHOS REGISTROS
#define tamRegistro 160
#define cabecalhoFixo 21

//////////////////////////////////////////////////////// TAMANHOS ARVORE
#define tamCabecalho 93
#define tamNo 93

#define tamP 5
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

typedef struct _cabecalhoArvBin
{
    char status;
    int  noRaiz;
    int  RRNproxNo;
    char lixo[83];

} CabecalhoArvBin;

//////////////////////////////////////////////////////// STRUCTS ARVORE

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

//////////////////////////////////////////////////////// INICIALIZADORES REGISTROS

RegCabecalho IniciarCabecalho(void);
RegDados IniciarRegistroDados(void);

//////////////////////////////////////////////////////// INICIALIZADORES ARVORE

CabecalhoArvBin CriarCabecalhoArvBin();
NoArvBin CriarNo();

#endif

