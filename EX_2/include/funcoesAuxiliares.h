#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H

#include "structs.h"
#include "funcoesBuscaArvore.h"
#include "funcoesFornecidas.h"
#include "funcoesTrabalho1.h"

//////////////////////////////////////////////////////// FUNCOES DE LEITURA

CabecalhoArvBin LerCabecalhoArvore(char *arquivo);
NoArvBin LerNoArvore(char *arquivo, int rrn);

//////////////////////////////////////////////////////// FUNCOES DE ESCRITA

int EscreveNo(char *nomeArq, NoArvBin no, int rrn);

//////////////////////////////////////////////////////// CHECAGENS

int ChecarCabecalho(CabecalhoArvBin cabecalho);
int ChecarArvoreVazia(char *nomeArquivo, int printError);
int ChecarIntegridadeArquivo(FILE *arquivo, char *nomeArq);

//////////////////////////////////////////////////////// ALTERAÇÕES

int AlterarCabecalho(char *nomeArq, char status, int noRaiz, int rrnProxNo);
NoArvBin AlterarNo(NoArvBin no, char folha, int nroChavesNo, int rrnNo);

//////////////////////////////////////////////////////// ORDENAÇÕES

NoArvBin OrdenaNo(NoArvBin noOriginal, int posInsercao, RegistroInfo info);
RegistroInfo* OrdenaInfos(int size, NoArvBin no, int posInsercao, RegistroInfo info);

//////////////////////////////////////////////////////// POSIÇÃO

int EncontraPosicao(NoArvBin no, RegistroInfo info);

void LiberaNo(NoArvBin no);


#endif
