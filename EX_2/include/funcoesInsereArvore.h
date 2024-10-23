#ifndef FUNCOESINSEREARVORE_H
#define FUNCOESINSEREARVORE_H

#include "structs.h"
#include "funcoesAuxiliares.h"

//////////////////////////////////////////////////////// FUNCOES DE INSERCAO

int InserirArvoreVazia(char *nomeArqArvore, char *chave, int pr);
NoPos InserirNoSemOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info);
//int InserirNoSemOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info);
//int InserirNoComOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info, int tamListaRRN);
int InserirNoComOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info);
NoPos PromoveChave(char *nomeArqArvore, int posInsercao, RegistroInfo promovido, NoArvBin noPromocao);

#endif
