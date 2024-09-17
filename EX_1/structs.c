#include "structs.h"

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

    dados.populacao = 0;
    dados.tamanho = 0;
    dados.unidadeMedida = '-';
    dados.velocidade = 0;

    return dados;
}
