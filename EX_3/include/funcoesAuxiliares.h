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
RegDados InicializarRegistro();

void MostrarGrafo(std::set<Vertice> vetorVertices);

void scan_quote_string(char *str);


#endif