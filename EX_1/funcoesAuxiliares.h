#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H

#include "structs.h"

///////////////////////////////////////////////////////////////// ESCREVER NO ARQUIVO (1)
// Supõe-se que o arquivo já está aberto
// Supõe-se que será fechado após o retorno da função

RegCabecalho EscreverCabecalho(FILE *arqBin);
int EscreverRegistro(FILE *arqBin, RegDados novoRegisto, int quantReg);

///////////////////////////////////////////////////////////////// PRINTAR REGISTROS (2)

RegCabecalho LerCabecalho(FILE *arqBin);

#endif