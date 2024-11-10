#include "funcoes.h"
#include "structs.h"
#include "funcoesAuxiliares.h"

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
    std::set<Vertice> recStack;
    std::set<Vertice> visitados;
    std::set<Vertice> vetorVertices = CriarGrafo(nomeArq);



    Vertice v = *vetorVertices.begin();
    std::cout << "NOME: " << v.Nome();
    recStack.insert(v);
    std::cout << recStack << std::endl;

    while(!recStack.empty())
    {
        Presa presa;
        std::set<Presa>::iterator p;
        std::set<Presa> presas = v.Adjacencias();  

        for(p = presas.begin(); p != presas.end(); p++)                 // Para cada presa na lista de adjacencias
        {
            presa = *p;                                                 // Pega-se referência
            Vertice vTemp(presa.Nome());                                // Cria-se um vertice temporario

            if(visitados.find(vTemp) != visitados.end()) 
                i++;                                                    // Se adjacencia já foi visitada -> tem ciclo
            else
                recStack.insert(recStack.begin(), vTemp);                   // Adiciona no inicio da rec stack se não estiver
        }

        auto pair = visitados.insert(v);                                            // Coloca que visitou esse vertice
        recStack.erase(v);                                              // Apaga esse vertice da reckStack
        if(!recStack.empty())
            v = *recStack.begin();                                      // O vertice eh o topo da pilha

        std::cout << visitados << std::endl;
        std::cout << recStack << std::endl;
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