#include "structs.h"

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

//////////////////////////////////////////////////////// INICIALIZAÇÕES

RegCabecalho IniciarCabecalho()
{
    RegCabecalho cabecalho;

    cabecalho.status = '0';
    cabecalho.topo = -1;
    cabecalho.proxRRN = 0;
    cabecalho.nroRegRem = 0;
    cabecalho.nroPagDisco = 0;
    cabecalho.qttCompacta = 0;

    return cabecalho;
}

RegDados IniciarRegistroDados()
{
    RegDados dados;

    dados.população = 0;
    dados.tamanho = 0;
    dados.unidadeMedida = '-';
    dados.velocidade = 0;

    return dados;
}
