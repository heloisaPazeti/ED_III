#ifndef FUNCOESARVOREB_H
#define FUNCOESARVOREB_H

#include "structs.h"
#include "funcoesFornecidas.h"

///////////////////////////////////////////////////////////////// CRIAR ARVORE (7)

int CriarArvore(char *nomeArq, char *nomeArqArvore);
int BuscarRegistroArvore(char *nomeArq, char *nomeArqArvore);
int AdicionarRegistroARvore(char *nomeArq, char *nomeArqArvore);

///////////////////////////////////////////////////////////////// OUTROS 

NoPos BuscarNoArvore(char *arquivo, char *nomeRegistro);
NoArvBin LerNoArvore(char *arquivo, int rrn);
CabecalhoArvBin LerCabecalhoArvore(char *arquivo);

#endif