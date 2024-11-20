#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H

#include "structs.h"
#include <map>

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

// ========================================================================
// ====================== FUNCOES DE VERIFICACAO ==========================
// ========================================================================

bool VerticePreto(std::set<Vertice> pretos, Vertice v);
bool VerticeCinza(std::list<Vertice> cinzas, Vertice v);
bool VerticeBranco(std::list<Vertice> cinzas, std::set<Vertice> pretos, Vertice v);

Vertice distMin(std::map<Vertice, int> dists, std::list<Vertice> visitados);
// ========================================================================
// ======================== FUNCOES DE BUSCA ==============================
// ========================================================================

int DFS(Vertice v, std::map<std::string, std::string> &low, std::list<Vertice> &pilha, std::set<Vertice> &visitados, std::set<Vertice> vetorVertices, int &componentes);


#endif