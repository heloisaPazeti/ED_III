#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H

#include "structs.h"
#include "funcoesFornecidas.h"

//////////////////////////////////////////////////////// FUNCOES DE LEITURA

CabecalhoArvBin LerCabecalhoArvore(char *arquivo);
NoArvBin LerNoArvore(char *arquivo, int rrn);

//////////////////////////////////////////////////////// FUNCOES DE BUSCA

NoPos BuscarNoArvore(char *arquivo, char *chave);

//////////////////////////////////////////////////////// FUNCOES TRABALHO 1

RegDados lerRegistro(FILE *arquivo);
void imprimirRegistro(RegDados registro);

#endif