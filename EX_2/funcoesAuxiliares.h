#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H

#include "structs.h"
#include "funcoesFornecidas.h"

//////////////////////////////////////////////////////// FUNCOES DE LEITURA

CabecalhoArvBin LerCabecalhoArvore(char *arquivo);
NoArvBin LerNoArvore(char *arquivo, int rrn);

//////////////////////////////////////////////////////// FUNCOES DE BUSCA

NoPos BuscarNoArvore(char *arquivo, char *chave);

//////////////////////////////////////////////////////// CHECAGENS

int ChecarCabecalho(CabecalhoArvBin cabecalho);
int ChecarArvoreVazia(CabecalhoArvBin cabecalho);
int ChecarIntegridadeArquivo(FILE *arquivo, char *nomeArq);

//////////////////////////////////////////////////////// FUNCOES TRABALHO 1

RegDados lerRegistro(FILE *arqBin, char *arquivo);
void imprimirRegistro(RegDados registro);

#endif