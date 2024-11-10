#include "funcoes.h"
#include "structs.h"
#include "funcoesAuxiliares.h"
#include <stack>
#include <vector>

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

bool BuscarCiclo(std::string nomeArq) 
{
    std::stack<Vertice> recStack;
    std::vector<Vertice> vitados;
    std::set<Vertice>::iterator itVertice;
    std::set<Vertice> vetorVertices = CriarGrafo(nomeArq);

    // Pegar um vertice x
        // Adicionar x na recStack (pilha)
        // Trocar a cor de x para preto
        // Adicionar adjacente na recStack

        // Se adjacente -> cor = branco -> troca pra cinza, bota na recStack
        // Se adjacente -> cor = cinza  -> nao faz nada
        // Se adjacente -> cor = preta  -> adjacente esta na recStack?
            // Sim -> tem ciclo

        // Assim que acabar -> topo da pilha refaz.
        // Se atingiu final da recStack -> nao tem ciclo
        


    // Supondo o grafo com as conexoes - sem ciclos

    // A -> B
    // A -> C
    // B -> C

    // A -> cor preta
    // B -> cor cinza
    // C -> cor cinza

    // RecStack (pilha) -> [A][B][C]
    // Prox -> [C]

    // C -> cor preta
   
    // RecStack (pilha) -> [B]

    // B -> cor preta
    // C -> ja eh preto, mas n esta na rec stack

    // fim da reck stack -> sem ciclo
    // -------------------------------------------------

    // Supondo o grafo com as conexoes - com ciclos

    // A -> B
    // A -> C
    // B -> C
    // C -> B

    // Visitados (lista) -> [A]
    // RecStack (pilha)  -> [B][C]
    // Prox -> [C]
   
    // C -> cor preta
    // B -> cinza -> n faz nada

    // Visitados (lista) -> [A][C]
    // RecStack (pilha)  -> [B]
    // Prox -> [B]

    // B -> cor preta
    // C -> ja eh preto, ja foi visitado -> tem ciclo



    return false;
}

// ========================================================================
// ==================== FUNCOES DE CONEXO GRAFO (13) ======================
// ========================================================================

// ========================================================================
// =================== FUNCOES DE RELAÇÃO GRAFO (14) ======================
// ========================================================================