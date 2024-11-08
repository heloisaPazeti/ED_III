#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H

#include "structs.h"

// ========================================================================
// ========================= FUNCOES DE LEITURA ===========================
// ========================================================================

RegDados LerRegistro(FILE *arquivo);
RegCabecalho LerCabecalho(FILE *arquivo);

// ========================================================================
// ====================== FUNCOES DE INICIALIZAÇAO ========================
// ========================================================================
RegDados InicializarRegistro(RegDados registro);

// bool Comparar(Vertice a, Vertice b);

#endif