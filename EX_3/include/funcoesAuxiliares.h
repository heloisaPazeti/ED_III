#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H

#include "structs.h"

// ========================================================================
// ========================= FUNCOES DE LEITURA ===========================
// ========================================================================

RegDados LerRegistro(FILE *arquivo);
RegCabecalho LerCabecalho(FILE *arquivo);

RegDados InicializarRegistro(RegDados registro);
