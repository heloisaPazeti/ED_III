#include "funcoes.h"
#include "structs.h"
#include "funcoesAuxiliares.h"
#include <iostream>
#include <string>
#include <set>

// ========================================================================
// ===================== FUNCOES CRIAÇÃO GRAFO (10) =======================
// ========================================================================

int CriarGrafo(char* nomeArq)
{
    FILE *arquivo;
    RegDados dado;
    std::set<Vertice> vetorVertices;
    std::set<Vertice>::iterator it;
    int tam;

    arquivo = fopen(nomeArq, "rb");
    dado = InicializarRegistro(dado);

    fseek(arquivo, 1600, SEEK_SET);
    while(dado.removido != '2')
    {
        tam = vetorVertices.size();
        dado = LerRegistro(arquivo);
        Vertice novoVertice(dado.nome, dado.especie, dado.habitat, dado.dieta, dado.tipo);
        vetorVertices.insert(novoVertice);

        if(tam==vetorVertices.size())   // Se o tamanho continua o mesmo, o vértice já existe
        {
            it = vetorVertices.find(novoVertice);
            Vertice aux = *it;
            aux.InserirPresa(dado.alimento);
            aux.AumentarGrauSaida();

            aux.MostrarVertice();
            // procurar presa no vetor de predadores p/ aumentar grau de entrada
        }
        else                            // Se o tamanho muda, o vértice é novo e deve-se criar um novo set de presas
        {

        }
    } 
    return 0;
}

// ========================================================================
// ==================== FUNCOES DE BUSCA GRAFO (11) =======================
// ========================================================================

// ========================================================================
// ==================== FUNCOES DE CICLO GRAFO (12) =======================
// ========================================================================

// ========================================================================
// ==================== FUNCOES DE CONEXO GRAFO (13) ======================
// ========================================================================

// ========================================================================
// =================== FUNCOES DE RELAÇÃO GRAFO (14) ======================
// ========================================================================