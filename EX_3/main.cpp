#include <iostream>
#include <string>
#include <stdio.h>
#include <set>
#include "funcoesAuxiliares.h"
#include "funcoes.h"
#include "structs.h"

int main()
{
    int n;
    std::string nomeArq;
    std::set<Vertice> vetorVertices;
    
    std::cin >> n >> nomeArq;

    switch(n)
    {
        case 10:
            vetorVertices =  CriarGrafo(nomeArq);
            MostrarGrafo(vetorVertices);
            break;
        case 11:
            BuscarGrafo(nomeArq);
            break;

        case 12:
            BuscarCiclo(nomeArq);
            break;

        case 13:
            BuscarComponentes(nomeArq);
            break;

        case 14: 
            RelacaoPresaPredador(nomeArq);
            break;

        default:
            std::cout << "Opcao invalida!" << std::endl;
    }

    return 0;
}