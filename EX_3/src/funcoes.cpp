#include "funcoes.h"
#include "structs.h"
#include "funcoesAuxiliares.h"

// ========================================================================
// ===================== FUNCOES CRIAÇÃO GRAFO (10) =======================
// ========================================================================

bool Comparar(Vertice a, Vertice b) 
{
    return a.Nome() < b.Nome();
}

int CriarGrafo(std::string nomeArq)
{
    FILE *arquivo;
    RegDados dado;
    std::set<Vertice> vetorVertices;
    std::set<Vertice>::iterator it;
    int tam;

    arquivo = fopen(nomeArq.c_str(), "rb");

    dado = InicializarRegistro(dado);

    fseek(arquivo, 1600, 0);

    while(dado.removido != '2')
    {
        dado = LerRegistro(arquivo);
        
        Vertice novoVertice(dado.nome, dado.especie, dado.habitat, dado.dieta, dado.tipo);
        novoVertice.MostrarVertice();
        it = vetorVertices.find(novoVertice);

        if(it!=vetorVertices.end())   // Se o vértice já existe
        {
            Vertice aux = *it;
            aux.InserirPresa(dado.alimento, dado.populacao);
            aux.AumentarGrauSaida();

            vetorVertices.erase(*it);
            vetorVertices.insert(aux);
        }
        else                            // Se o vértice é novo, deve-se criar um novo set de presas
        {
            novoVertice.InserirPresa(dado.alimento, dado.populacao);
            vetorVertices.insert(novoVertice);
        }
    } 

    fseek(arquivo, 1600, 0);

    while(dado.removido != '2')
    {
        dado = LerRegistro(arquivo);
        
        Vertice novoVertice(dado.nome, dado.especie, dado.habitat, dado.dieta, dado.tipo);
        Vertice alimento(dado.alimento, dado.especie, dado.habitat, dado.dieta, dado.tipo);

        novoVertice.MostrarVertice();
        it = vetorVertices.find(alimento);

        if(it!=vetorVertices.end())   // Se o vértice já existe
        {
            Vertice aux = *it;
            aux.InserirPresa(dado.especie, dado.populacao);
            aux.AumentarGrauSaida();

            vetorVertices.erase(*it);
            vetorVertices.insert(aux);
        }
    } 

    fclose(arquivo);

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