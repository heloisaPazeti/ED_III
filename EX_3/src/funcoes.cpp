#include "funcoes.h"
#include "structs.h"
#include "funcoesAuxiliares.h"

// ========================================================================
// ===================== FUNCOES CRIAÇÃO GRAFO (10) =======================
// ========================================================================

std::set<Vertice> CriarGrafo(std::string nomeArq)
{
    FILE *arquivo;
    RegDados dado;
    std::set<Vertice> vetorVertices;
    std::set<Vertice>::iterator it;
    int tam;

    arquivo = fopen(nomeArq.c_str(), "rb");

    dado = InicializarRegistro();

    fseek(arquivo, 1600, 0);

    while(dado.removido != '2')
    {
        dado = LerRegistro(arquivo);
        
        Vertice novoVertice(dado.nome, dado.especie, dado.habitat, dado.dieta, dado.tipo);
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

    dado = InicializarRegistro();

    while(dado.removido != '2')
    {
        dado = LerRegistro(arquivo);
        Vertice alimento(dado.alimento, dado.especie, dado.habitat, dado.dieta, dado.tipo);
        it = vetorVertices.find(alimento);

        if(it!=vetorVertices.end())   // Se o vértice já existe
        {   
            Vertice aux = *it;
            aux.AumentarGrauEntrada();

            vetorVertices.erase(*it);
            vetorVertices.insert(aux);
        }
    } 

    fclose(arquivo);

    return vetorVertices;
}

// ========================================================================
// ==================== FUNCOES DE BUSCA GRAFO (11) =======================
// ========================================================================

int BuscarGrafo(std::string nomeArq)
{
    int i, n;
    int encontrados=0;
    char nomePresa[60];
    std::string nomePredador;
    std::set<Vertice>::iterator itVertice;
    std::set<Presa>::iterator itPresa;
    std::set<Vertice> vetorVertices = CriarGrafo(nomeArq);

    std::cin >> n;
    

    for(i=0; i<n; i++)
    {
        scan_quote_string(nomePresa);
        encontrados = 0;
        std::string nome = nomePresa;

        // Percorre os vértices
        for(itVertice = vetorVertices.begin(); itVertice!=vetorVertices.end(); itVertice++)
        {
            Vertice ans = *itVertice;
            nomePredador = ans.BuscarPresa(nome);

            if(nomePredador != "-1" && encontrados == 0)      // Encontrou a presa
            {
                std::cout << nomePresa << ": ";
                std::cout << nomePredador;
                encontrados++;
                continue;
            }
            else if(nomePredador != "-1" && encontrados != 0)
            {
                std::cout <<  ", " << nomePredador;
                encontrados++;
                continue;
            }
            else
                continue;
            
        }

        if(encontrados == 0)
            std::cout << "Registro Inexistente.";

        std::cout << std::endl;
        printf("\n");
    }

    return 0;

}
// ========================================================================
// ==================== FUNCOES DE CICLO GRAFO (12) =======================
// ========================================================================

// ========================================================================
// ==================== FUNCOES DE CONEXO GRAFO (13) ======================
// ========================================================================

// ========================================================================
// =================== FUNCOES DE RELAÇÃO GRAFO (14) ======================
// ========================================================================