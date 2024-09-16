#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"

RegDados lerRegistro(FILE *arquivo);
int definirTipo(char *nomeCampo);
void imprimirRegistro(RegDados registro);