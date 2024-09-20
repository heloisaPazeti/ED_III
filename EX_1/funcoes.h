#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

RegDados LerCSV();
int EscreverArquivo(char *nomeCSV);
int EscreverRegistros(char *nomeArq);
int BuscarRegistros(char *nomeArq);
int RemoverRegistros();
int InserirRegistros();
int Compactador(char *nomeArq);

#endif


