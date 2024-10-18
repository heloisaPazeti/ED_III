#ifndef FUNCOES_BUSCA_H
#define FUNCOES_BUSCA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "funcoesAuxiliares.h"

RegDados lerRegistro(FILE *arquivo);
int definirTipo(char *nomeCampo);
void imprimirRegistro(RegDados registro);
int numPagDisco(FILE *arquivo);
void eliminarRegistro(FILE* arquivo, int encadeamento);
int buscarRRN(FILE *arquivo);
void substituirRegistro(FILE *arquivo, RegDados registro);
int escreverRegistro(FILE *arqBin, RegDados novoRegisto, int quantReg);


#endif


