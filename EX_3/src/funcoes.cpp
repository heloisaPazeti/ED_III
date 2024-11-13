#include "funcoes.h"
#include "structs.h"
#include "funcoesAuxiliares.h"
#include <string.h>
#include <list>
#include <algorithm>

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

        if(strcmp(dado.nome, "") == 0) continue;

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
        if(strcmp(dado.nome, "") == 0) continue;
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

 /* == Descobrir quantidades de ciclos ==
 
    -> Enquanto vertice x possuir adjacentes
        -> Pegar um vertice x
        -> Procurar nos adjacentes de x um adjacente "branco"
        -> Avançar para esse adjacente e torna-lo "cinza"
        -> Coloca adjacente numa pilha

        -> Pilha montada
        -> Topo da pilha -> preto
        -> Remove topo
        -> Vertice x = novo topo da pilha
        -> Refazer 
*/
int BuscarCiclo(std::string nomeArq) 
{
    int ciclos = 0;
    bool pilhaAlterada = true;
    Vertice v("");
    Vertice vTemp("");
    std::set<Vertice>::iterator it;
    std::set<Vertice>::iterator itTemp;
    std::list<Vertice> cinzas;
    std::set<Vertice> pretos;
    std::set<Presa> adjacentes;
    std::set<Vertice> vetorVertices = CriarGrafo(nomeArq);

    for(it = vetorVertices.begin(); it != vetorVertices.end(); it++)            // Faz para todos os vertices
    {
        if(cinzas.empty())                                                      // Se pilha vazia -> prox caminho
        {
            v = *it;                                                            // Vertice inicial
            if(!VerticeBranco(cinzas, pretos, v) || v.Nome() == "")             // Se já fez pode pular
                continue;
        }
        else                                                                    // Se ainda tiver caminho pra seguir
        {
            it--;
            v = cinzas.front();                                                 // Certifica de pegar o topo
        }                

        pilhaAlterada = true;
        while(pilhaAlterada)                                                        // Enquanto tiver adjacencias  
        {    
            pilhaAlterada = false;
            adjacentes = v.Adjacencias();
            if(adjacentes.empty()) continue;

            for(Presa pTemp : adjacentes)
            {
                itTemp = vetorVertices.find(pTemp.Nome());
                if(itTemp != vetorVertices.end() && pTemp.Nome() != "") 
                    vTemp = *itTemp;
                else
                    continue;

                if(VerticeCinza(cinzas, vTemp) || vTemp.Nome() == v.Nome())
                {
                    ciclos++;
                    continue;
                }
                else if(VerticeBranco(cinzas, pretos, vTemp))                           // Se adjacente eh branco
                {
                    cinzas.push_front(vTemp);                                   // Adiciona adj na pilha
                    v = cinzas.front();                                         // Passa para o novo topo
                    pilhaAlterada = true;                                           // Permanece no while
                    break;                                                      // Saida do for
                }
            }
        }

        cinzas.remove(v);                                                       // Remove o topo
        pretos.insert(v);                                                       // coloca nas pretas
    }

    std::cout << "Quantidade de ciclos: " << ciclos << std::endl;
    return ciclos;
}

// ========================================================================
// ==================== FUNCOES DE CONEXO GRAFO (13) ======================
// ========================================================================

/* == Se fortemente conexo ou nao e quantidade de componentes ==

    -> Para todo vertice x 

    -> Seguir os caminhos possiveis de forma a percorrer todo o grafo.
    -> Se for possivel para x -> x é um componente conexo.
    -> Se for possivel para todo vertice -> grafo fortemente conexo.

    -> Metodo: Buscar por Profundidade
*/
int BuscarComponentes(std::string nomeArq) 
{
    int componentes = 0;
    bool pilhaAlterada = true;
    bool fortementeConexo = true;
    Vertice v("");
    Vertice vTemp("");
    std::set<Vertice>::iterator it;
    std::set<Vertice>::iterator itTemp;
    std::list<Vertice> pilha;
    std::set<Vertice> visitados;
    std::set<Presa> adjacentes;
    std::set<Vertice> vetorVertices = CriarGrafo(nomeArq);

    it = vetorVertices.begin();
    while(it != vetorVertices.end() || !pilha.empty())
    {
        if(pilha.empty())                                                   // Se pilha vazia -> prox caminho
        {
            v = *it;                                                        // Vertice inicial
            it++;
            if(VerticePreto(visitados, v) || v.Nome() == "")               // Se já fez pode pular
                continue;

            pilha.push_front(v);
        }
        else                                                                // Se ainda tiver caminho pra seguir
            v = pilha.front();                                              // Certifica de pegar o topo

        pilhaAlterada = true;                                               // Saber se avançou ou nao
        while(pilhaAlterada)                                                // Enquanto tiver adjacencias  
        {    
            pilhaAlterada = false;                                          // Assume-se que nao sera alterada
            adjacentes = v.Adjacencias();
            if(adjacentes.empty()) continue;                                // Se nao houver adjacentes acabou

            for(Presa pTemp : adjacentes)                                   // Para todas as adjacencias
            {
                itTemp = vetorVertices.find(pTemp.Nome());                  // Pegamos como vertice
                if(itTemp != vetorVertices.end())                           // Se o vertice existir
                    vTemp = *itTemp;                                        // Pegamos sua referencia
                else
                    continue;

                if(VerticeBranco(pilha, visitados, vTemp))                  // Se nao foi visitado
                {
                    v = vTemp;                                              // Alteramos destino 
                    pilhaAlterada = true;                                   // Alteramos a pilha
                    pilha.push_front(v);
                    break;                                                  // Saida do for
                }
            }
        }



        pilha.remove(v);
        visitados.insert(v);
        
        if(pilha.empty())
        {
            
            if(visitados == vetorVertices)                                  // Se todos estao nos visitados
                componentes++;                                              // O componente eh conexo
            else
                fortementeConexo = false;                                   // Se nao, sabe-se que nao sera fort. conexo

            visitados.clear();                                              // Limpamos visitados   
        }
    }

    if(fortementeConexo)
        std::cout << "Sim, o grafo é fortemente conexo e possui 1 componente." << std::endl;
    else
        std::cout << "Não, o grafo não é fortemente conexo e possui " << componentes << " componentes." << std::endl;
    return componentes;
}

// ========================================================================
// =================== FUNCOES DE RELAÇÃO GRAFO (14) ======================
// ========================================================================