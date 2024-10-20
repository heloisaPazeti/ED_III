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

NoPos BuscarNoArvore(char *arquivo, long int chave);

//////////////////////////////////////////////////////// FUNCOES DE INSERCAO

int InserirArvoreVazia(char *nomeArqArvore, char *chave, int pr);
int InserirNoSemOverflow(char *nomeArqArvore, int posInsercao, RegistroInfo info);
int InserirNoComOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info);

int PromoveChave(char *nomeArqArvore, RegistroInfo promovido, NoArvBin noPromocao);

//////////////////////////////////////////////////////// CHECAGENS

int ChecarCabecalho(CabecalhoArvBin cabecalho);
int ChecarArvoreVazia(CabecalhoArvBin cabecalho, int printError);
int ChecarIntegridadeArquivo(FILE *arquivo, char *nomeArq);

//////////////////////////////////////////////////////// HELPERS

int AlterarCabecalho(char *nomeArq, char status, int noRaiz, int rrnProxNo);
NoArvBin AlterarNo(NoArvBin no, char folha, int nroChavesNo, int rrnNo);
NoArvBin OrdenaNo(NoArvBin noOriginal, int posInsercao, RegistroInfo info);
RegistroInfo* OrdenaInfos(int size, NoArvBin no, int posInsercao, RegistroInfo info);
int EncontraPosicao(NoArvBin no, RegistroInfo info);

//////////////////////////////////////////////////////// FUNCOES TRABALHO 1

RegDados lerRegistro(FILE *arqBin, char *arquivo);
int escreverRegistro(FILE *arqBin, RegDados novoRegisto, int quantReg);
void imprimirRegistro(RegDados registro);
RegDados lerDadosDoTeclado();
int InserirRegistrosAdap(char *nomeArq, RegDados registro);

///////////////////////////////////////////////////////////////// ADICIONAR REGISTROS (5)
void adicionarRegistro(RegDados registro, FILE *arquivo);
RegDados lerDadosDoTeclado();
int tamanhoString(char *string);

#endif
