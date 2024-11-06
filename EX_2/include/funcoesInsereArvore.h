#ifndef FUNCOESINSEREARVORE_H
#define FUNCOESINSEREARVORE_H

#include "structs.h"
#include "funcoesAuxiliares.h"

//////////////////////////////////////////////////////// FUNCOES DE INSERCAO

void InserirArvoreVazia(char *nomeArqArvore, RegistroInfo info);
int InserirNoSemOverflow(char *nomeArqArvore, NoArvBin no, int P, int posInsercao, RegistroInfo info);
//int InserirNoSemOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info);
//int InserirNoComOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info, int tamListaRRN);
int InserirNoComOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info, int noDireita);

#endif
