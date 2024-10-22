#ifndef FUNCOESTRABALHO1_H
#define FUNCOESTRABALHO1_H

#include "funcoesAuxiliares.h"

//////////////////////////////////////////////////////// LEITURA

RegDados lerRegistro(FILE *arqBin, char *arquivo);
RegDados lerTeclado();
RegCabecalho LerCabecalho(FILE *arqBin);

//////////////////////////////////////////////////////// ESCRITA

int EscreverCabecalho(FILE *arqBin, RegCabecalho cabecalho);
int EscreverRegistro(FILE *arqBin, RegDados novoRegisto, int quantReg);

//////////////////////////////////////////////////////// INSERÇÃO

void adicionarRegistro(RegDados registro, FILE *arquivo);
int InserirRegistrosAdap(char *nomeArq, RegDados registro);

//////////////////////////////////////////////////////// IMPRESSÃO

void imprimeRegistro(RegDados registro);

//////////////////////////////////////////////////////// HELPERS

int tamanhoString(char *string);
void substituirRegistro(FILE *arquivo, RegDados registro);

#endif
