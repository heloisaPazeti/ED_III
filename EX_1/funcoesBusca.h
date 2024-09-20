#ifndef FUNCOES_BUSCA_H
#define FUNCOES_BUSCA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"

RegDados lerRegistro(FILE *arquivo);
int definirTipo(char *nomeCampo);
void imprimirRegistro(RegDados registro);
int numPagDisco(FILE *arquivo);
int buscarRRN(FILE *arquivo);
void eliminarRegistro(FILE* arquivo, int proxRRN);

#endif


