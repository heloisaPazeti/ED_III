#include "funcoes.h"
#include "structs.h"
#include "funcoesAuxiliares.h"
#include <string.h>
#include <map>
#include <list>
#include <queue>
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
    char status;

    arquivo = fopen(nomeArq.c_str(), "rb");         // Abre o arquivo

    if(arquivo == NULL)
    {
        std::cout << "Falha no processamento do arquivo." << std::endl;
        return vetorVertices;
    }

    if(fread(&status, sizeof(char),1,arquivo) == '0')
    {
        std::cout << "Falha no processamento do arquivo." << std::endl;
        return vetorVertices;
    }

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

    if(vetorVertices.empty()) return -1;

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
 /* == Funcionamento ==
 
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

/* Calcula quantos ciclos existem dentro do grafo.*/
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

    if(vetorVertices.empty()) return -1;

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
// Tarjan's Algorithm -> github.com/williamfiset/Algorithms/blob/master/src/main/java/com/williamfiset/algorithms/graphtheory/TarjanSccSolverAdjacencyList.java
/* == Funcionamento ==

    -> Para todo vertice no grafo aplicar DFS
    -> Mater um id para o vertice (nome) + low (menor vertice que ele alcança)
    -> Manter pilha de cinzas -> analisando ainda
    -> Manter lista de pretos -> ja visitados
    -> Apos cada DFS recursivo -> vertice adjacente esta nos cinzas ?
        -> Se sim -> verificar o menor valor entre os lows
        -> Se nao -> nao faz nada
    -> Ao encontrar o topo de uma subarvore (id == low)
    -> Desempilhar vertices que fazem parte do componente
    -> Somar um componente

    -> OBS: cuidar para vertices "fantasmas" -> que nao estao realmente no grafo 
*/

/* Buscar por componentes conexos no grafo. Verificar se o grafo eh fortemente conexo ou nao.*/
int BuscarComponentes(std::string nomeArq) 
{
    int componentes = 0;
    std::list<Vertice> pilha;
    std::set<Vertice> visitados;
    std::map<std::string, std::string> low;
    std::set<Vertice> vetorVertices = CriarGrafo(nomeArq);

    if(vetorVertices.empty()) return -1;

    for(Vertice v : vetorVertices)                                              // Para todos os vertices do grafo
    {
        if(visitados.find(v) == visitados.end())                                // Se ainda nao foi visitado
            DFS(v, low, pilha, visitados, vetorVertices, componentes);          // Fazer uma busca em profundidade
    }

     if(componentes == 1)                                                       // Grafo fortemente conexo
        std::cout << "Sim, o grafo é fortemente conexo e possui " << componentes << " componente." << std::endl;
    else                                                                        // Grafo nao conexo
        std::cout << "Não, o grafo não é fortemente conexo e possui " << componentes << " componentes." << std::endl;
    return componentes;
}

// ========================================================================
// =================== FUNCOES DE RELAÇÃO GRAFO (14) ======================
// ========================================================================
// Algoritmo de Dijkstra -> https://gist.github.com/marcoscastro/d4e0df5b134c2cd63cf2 s2

/* == Funcionamento ==

    -> Inicialmente tem-se todas as distancias como infinito.
    -> Sabe-se que a distancia do vertice origem para ele mesmo eh zero.
    -> Enquanto a fila de prioridade, ordenada pelo menor custo, conter possibilidades.
    -> Pega-se o proximo com menor custo e recalcula-se o novo custo desse vertice.
    -> Se esse valor for menor do que o anteriormente salvo, atualiza-se para o novo.
    -> Coloca-se o vertice na fila com o novo valor.
    -> Se a fila ficar vazia, quer dizer que todos os possiveis caminhos foram percorridos, logo tem-se o menor caminho.
*/
int RelacaoPresaPredador(std::string nomeArq)
{
    int i, n;
    char nomePredador[200];                                                     // Nomes predadores
    char nomePresa[200];                                                        // Nomes Presas
    std::map<Vertice, int> dist;                                                // Guarda as distancias finais
    std::set<Vertice>::iterator itPredador;                                     // Iterador
    std::set<Vertice> vetorVertices = CriarGrafo(nomeArq);                      // Grafo
    std::priority_queue <std::pair<int, Vertice>, std::vector<std::pair<int, Vertice>>, std::greater<std::pair<int, Vertice> >> pq;
    // ^ Fila de prioridade <distancia, vertice> -> ordena pelo menor vertice

    if(vetorVertices.empty()) return -1;

    std::cin >> n;
    for(i = 0; i < n; i++)
    {
        scan_quote_string(nomePredador);                                        // Pega os nomes
        scan_quote_string(nomePresa);                                           // Pega os nomes
        std::string predador(nomePredador);
        std::string presa(nomePresa);

        itPredador = vetorVertices.find(predador);
        if(itPredador == vetorVertices.end())                                   // Caso não encontre o predador -> caminho nao existe
        {
            std::cout << predador << " " << presa << ": " << "CAMINHO INEXISTENTE" << std::endl;
            continue;
        }

        dist.clear();                                                           // Limpamos as distancias
        Vertice vOrigem = *itPredador;                                          // Vertice de origem
        Vertice vDestino(presa);                                                // Vertice de destino

        for (Vertice vI : vetorVertices) dist[vI] = __INT_MAX__;                // Distancias inicialmente infinitas
        if(dist.find(presa) == dist.end()) dist[presa] = __INT_MAX__;           // Caso a presa nao esteja no grafo em si

        dist[vOrigem] = 0;                                                      // Da origem para origem eh zero
        pq.push(std::make_pair(dist[vOrigem], vOrigem));                        // Colocamos na fila de prioridade

        while(!pq.empty())                                                      // Enquanto tiver caminhos para testar
        {
            std::pair<int, Vertice> p = pq.top();                               // Pegamos o proximo com menor distancia
            Vertice vTemp = p.second;                                           // Vertice de menor distancia
            std::set<Presa> adjacencias = vTemp.Adjacencias();                  // Seus adjacentes
            pq.pop();                                                           // Removemos esse vertice da fila

            for(Presa p : adjacencias)                                          // Percorre-se as adjacencias
            {
                auto it = vetorVertices.find(p.Nome());                         // Encontra-se o proximo vertice
                int novoCusto = dist[vTemp] + p.Populacao();                    // Calcula-se o novo custo
                Vertice vAtual("");

                if(it == vetorVertices.end())                                   // Se o vertice atual n estiver no grafo
                    vAtual = Vertice(p.Nome());                                 // Cria-se um vertice fantasma
                else
                    vAtual = (*it);                                             // Se nao, utiliza-se o encontrado

                if(dist[vAtual] > novoCusto)                                    // Se o novo custo for menor
                {
                    dist[vAtual] = novoCusto;                                   // Altera-se para novo custo
                    pq.push(std::make_pair(dist[vAtual], vAtual));              // Coloca-se na fila de prioridade
                }
                    
            }
        }
        
        std::cout << vOrigem.Nome() << " " << vDestino.Nome() << ": ";          // Print de resposta

        if(dist[vDestino] == __INT_MAX__)                                       // Se distancia eh infinita -> nao tem caminho
            std::cout << "CAMINHO INEXISTENTE" << std::endl;
        else                                                                    // Se nao -> printar caminho
            std::cout << dist[vDestino] << std::endl;
    }

    return 0;
}
