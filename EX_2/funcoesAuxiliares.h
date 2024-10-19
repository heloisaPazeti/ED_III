#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H

#include "structs.h"
#include "funcoesFornecidas.h"

//////////////////////////////////////////////////////// FUNCOES DE LEITURA

CabecalhoArvBin LerCabecalhoArvore(char *arquivo);
NoArvBin LerNoArvore(char *arquivo, int rrn);
int EscreverCabecalho(FILE *arqBin, RegCabecalho cabecalho);
int EscreverRegistro(FILE *arqBin, RegDados novoRegisto, int quantReg);
RegCabecalho LerCabecalho(FILE *arqBin);

//////////////////////////////////////////////////////// FUNCOES DE BUSCA

NoPos BuscarNoArvore(char *arquivo, char *chave);

//////////////////////////////////////////////////////// FUNCOES DE INSERCAO

int InserirArvoreVazia(char *nomeArqArvore, char *chave, int rrn);
int InserirNoSemOverflow(char *nomeArqArvore, NoPos result, char *chave, int pr);
int InserirNoComOverflow(char *nomeArqArvore, NoPos result, char *chave, int rrn);

//////////////////////////////////////////////////////// CHECAGENS

int ChecarCabecalho(CabecalhoArvBin cabecalho);
int ChecarArvoreVazia(CabecalhoArvBin cabecalho, int printError);
int ChecarIntegridadeArquivo(FILE *arquivo, char *nomeArq);

//////////////////////////////////////////////////////// HELPERS

int AlterarCabecalho(char *nomeArq, char status, int noRaiz, int rrnProxNo);
NoArvBin OrdenaNo(NoPos noPos);
RegistroInfo* OrdenaInfos(int size, NoPos resultado, RegistroInfo info);

//////////////////////////////////////////////////////// FUNCOES TRABALHO 1

RegDados lerRegistro(FILE *arqBin, char *arquivo);
void imprimirRegistro(RegDados registro);
RegDados lerDadosDoTeclado();
int InserirRegistrosAdap(char *nomeArq, RegDados registro);

///////////////////////////////////////////////////////////////// ADICIONAR REGISTROS (5)
void adicionarRegistro(RegDados registro, FILE *arquivo);
RegDados lerDadosDoTeclado();
int tamanhoString(char *string);

#endif
