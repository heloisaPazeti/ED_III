#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H

#include "structs.h"

// ========================================================================
// ========================= FUNCOES DE LEITURA ===========================
// ========================================================================

void scan_quote_string(char *str);
RegDados LerRegistro(FILE *arquivo);
RegCabecalho LerCabecalho(FILE *arquivo);

// ========================================================================
// ====================== FUNCOES DE INICIALIZAÇAO ========================
// ========================================================================

RegDados InicializarRegistro();

// ========================================================================
// ======================== FUNCOES DE IMPRESSAO ==========================
// ========================================================================

void MostrarGrafo(std::set<Vertice> vetorVertices);


#endif