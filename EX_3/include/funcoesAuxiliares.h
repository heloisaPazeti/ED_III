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
// ======================== FUNCOES DE IMPRESSAO ==========================
// ========================================================================

void MostrarGrafo(std::set<Vertice> vetorVertices);


bool VerticePreto(std::set<Vertice> pretos, Vertice v);
bool VerticeCinza(std::list<Vertice> cinzas, Vertice v);
bool VerticeBranco(std::list<Vertice> cinzas, std::set<Vertice> pretos, Vertice v);


#endif