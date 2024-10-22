#ifndef FUNCOESINSEREARVORE_H
#define FUNCOESINSEREARVORE_H

#include "structs.h"
#include "funcoesAuxiliares.h"

//////////////////////////////////////////////////////// FUNCOES DE INSERCAO

int InserirArvoreVazia(char *nomeArqArvore, char *chave, int pr);
int InserirNoSemOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info);
int InserirNoComOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info);

int PromoveChave(char *nomeArqArvore, RegistroInfo promovido, NoArvBin noPromocao);

#endif
