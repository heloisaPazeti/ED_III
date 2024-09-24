#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H

#include "structs.h"
#include "funcoesFornecidas.h"

///////////////////////////////////////////////////////////////// ESCREVER NO ARQUIVO (1)
// Supõe-se que o arquivo já está aberto
// Supõe-se que será fechado após o retorno da função

int EscreverCabecalho(FILE *arqBin, RegCabecalho cabecalho);
int EscreverRegistro(FILE *arqBin, RegDados novoRegisto, int quantReg);

///////////////////////////////////////////////////////////////// PRINTAR REGISTROS (2)

RegCabecalho LerCabecalho(FILE *arqBin);

///////////////////////////////////////////////////////////////// ADICIONAR REGISTROS (5)
void adicionarRegistro(RegDados registro, FILE *arquivo);
RegDados lerDadosDoTeclado();
void SubstituirRegistro(FILE *arquivo, RegDados registro);

#endif