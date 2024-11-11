#include "funcoes.h"
#include "structs.h"
#include "funcoesAuxiliares.h"
#include <unordered_set>
#include <stack>

// ========================================================================
// ===================== FUNCOES CRIAÇÃO GRAFO (10) =======================
// ========================================================================

// Cria um grafo em forma de lista de adjacências a partir de dados lidos de um arquivo binário
std::set<Vertice> CriarGrafo(std::string nomeArq)
{
    FILE *arquivo;
    RegDados dado;
    std::set<Vertice> vetorVertices;
    std::set<Vertice>::iterator it;
    int tam;

    arquivo = fopen(nomeArq.c_str(), "rb");         // Abre o arquivo

    dado = InicializarRegistro();                   // Inicializa o registro com valores padrão

    fseek(arquivo, 1600, 0);                        // Posiciona o cursor

    while(dado.removido != '2')                     // Percorre o arquivo
    {
        dado = LerRegistro(arquivo);                // Lê um registro
        
        Vertice novoVertice(dado.nome, dado.especie, dado.habitat, dado.dieta, dado.tipo);  // Inicializa um vértice
        it = vetorVertices.find(novoVertice);       // Verifica se o vértice já existe

        if(it!=vetorVertices.end())                 // Se o vértice já existe, atualiza os dados
        {
            Vertice aux = *it;
            aux.InserirPresa(dado.alimento, dado.populacao);
            aux.AumentarGrauSaida();

            vetorVertices.erase(*it);
            vetorVertices.insert(aux);
        }
        else                                        // Se o vértice não existe, ele é criado
        {
            novoVertice.InserirPresa(dado.alimento, dado.populacao);
            vetorVertices.insert(novoVertice);
        }
    } 

    fseek(arquivo, 1600, 0);                        // Reposiciona o cursor ao final do cabeçalho para ma nova leitura

    dado = InicializarRegistro();                   // Inicializa a variável de registro

    while(dado.removido != '2')                     // Percorre o arquivo
    {
        dado = LerRegistro(arquivo);                // Lê um registro
        Vertice alimento(dado.alimento, dado.especie, dado.habitat, dado.dieta, dado.tipo); // Cria um vértice com o nome do alimento do predador (as demais informações são irrelevantes e incorretas)
        it = vetorVertices.find(alimento);          // Verifica se a presa do predador é também um predador       

        if(it!=vetorVertices.end())   // Se a presa for um predador, atualiza o vértice
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

// Busca n nomes em um grafo implementado como lista de adjacências
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
        scan_quote_string(nomePresa);           // Lê o nome a ser buscado
        encontrados = 0;                        // O número de predadores encontrados inicia em 0
        std::string nome = nomePresa;

        // Percorre os vértices da lista
        for(itVertice = vetorVertices.begin(); itVertice!=vetorVertices.end(); itVertice++)
        {
            Vertice ans = *itVertice;
            nomePredador = ans.BuscarPresa(nome);             // Busca o nome da presa entre as presas do predador 

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

        if(encontrados == 0)                                // Não encontrou a presa
            std::cout << "Registro Inexistente.";

        std::cout << std::endl;
        printf("\n");
    }

    return 0;

}
// ========================================================================
// ==================== FUNCOES DE CICLO GRAFO (12) =======================
// ========================================================================

 /* == Procura se ha ciclos ou nao ==
 
    Pegar um vertice x
    Adicionar adjacentes de x na recStack

    Se adjacente -> esta nos visitados (preto) -> tem ciclo
    Se adjacente -> n esta na recStack (branco) -> adiciona na RecStack
    Se adjacente -> esta na reckStack, mas n nos visitados (cinza) -> nao faz nada

    Adicionar x nos visitados
    Remove x da recStack
    Assim que acabar -> topo da pilha -> refaz.
    Se atingiu final da recStack -> nao tem ciclo

*/


int BuscarCiclo(std::string nomeArq) 
{
    int i = 0;
    bool flag=true;
    std::set<Vertice>::iterator it;
    std::unordered_set<Vertice> cinzaSet;
    std::stack<Vertice> cinzaStack;
    std::set<Vertice> preto;
    std::set<Vertice> vetorVertices = CriarGrafo(nomeArq);

    for(it = vetorVertices.begin(); it != vetorVertices.end(); it++)
    {
        Vertice v = *it;
        if(cinzaSet.find(v) != cinzaSet.end() || preto.find(v) != preto.end())
            continue;
        
        while(flag)
        {    
            if(v.Nome() == "") continue;
            std::set<Presa> adjacentes = v.Adjacencias();
            for(std::set<Presa>::iterator itPresa = adjacentes.begin(); itPresa != adjacentes.end(); it++)
            {
                Presa pTemp = *itPresa;
                Vertice vTemp(pTemp.Nome());

                if(cinzaSet.find(vTemp) == cinzaSet.end() && preto.find(vTemp) == preto.end())
                {
                    cinzaSet.insert(vTemp);
                    cinzaStack.push(vTemp);,
                    v = cinzaStack.top();
                    break;
                }
                else    flag = false;
            }
            
        }

        Vertice v = cinzaStack.top();
        preto.insert(v);   


    }

    std::cout << "Quantidade de ciclos:" << i << std::endl;
    return i;
}


// ========================================================================
// ==================== FUNCOES DE CONEXO GRAFO (13) ======================
// ========================================================================

// ========================================================================
// =================== FUNCOES DE RELAÇÃO GRAFO (14) ======================
// ========================================================================