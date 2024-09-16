#include <stdio.h>
#include <stdlib.h>
//////////////////////////////////////////////////////// TAMANHOS
#define tamRegistro 160

//////////////////////////////////////////////////////// STRUCTS

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
    char removido[1];
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

//////////////////////////////////////////////////////// FUNÇÕES

RegCabecalho IniciarCabecalho(void);
RegDados IniciarRegistroDados(void);