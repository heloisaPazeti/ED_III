#include <stdio.h>
#include <stdlib.h>

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
    int população;
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