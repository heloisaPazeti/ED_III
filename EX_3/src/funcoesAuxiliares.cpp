#include "structs.h"
#include "funcoesAuxiliares.h"
#include <string.h>
#include <map>

// ========================================================================
// ========================= FUNCOES DE LEITURA ===========================
// ========================================================================

RegDados LerRegistro(FILE *arquivo)
{
    RegDados temp;
    char dado[142], *linha;
    
    temp = InicializarRegistro();
    
    if(fread(&temp.removido, sizeof(char),1,arquivo)==0)    // Caso a leitura falhe, o campo de remoção recebe um valor logicamente inválido
    {
        temp.removido = '2';
    }    
    if(temp.removido == '1')                                // Caso o registro tenha sido removido, não completa a leitura e retorna 
    {
        fread(&temp.encadeamento, sizeof(int),1,arquivo);
    }
    if(temp.removido != '1')                                // Caso o campo não tenha sido removido, a leitura dos campos é finalizada
    {
        fread(&temp.encadeamento, sizeof(int),1,arquivo);
        fread(&temp.populacao, sizeof(int),1,arquivo);
        fread(&temp.tamanho, sizeof(float),1,arquivo);
        fread(&temp.unidadeMedida, sizeof(char),1,arquivo);
        fread(&temp.velocidade, sizeof(int), 1,arquivo);
        if(fread(dado, sizeof(char), 142, arquivo)==0)      // Caso a leitura falhe, o campo de remoção recebe um valor logicamente inválido
        {
            temp.removido = '2';
            return temp;
        }

        linha = strdup(dado);                               // A string de dados (armazena os campos de tamanho variável) é duplicada
        temp.nome = strsep(&linha, "#");                    // Separação dos dados de acordo com o separador '#'
        temp.especie = strsep(&linha, "#");
        temp.habitat = strsep(&linha, "#");
        temp.tipo = strsep(&linha, "#");
        temp.dieta = strsep(&linha, "#");
        temp.alimento = strsep(&linha, "#");


    }

    return temp;
}

RegCabecalho LerCabecalho(FILE *arquivo) 
{
    RegCabecalho cabecalho;

    if (arquivo == NULL)
    {
        printf("Falha no processamento do arquivo [Arq. CSV == NULL]\n");
        cabecalho.status = '2';
        return cabecalho;
    }

    fseek(arquivo, 0, SEEK_SET);

    fread(&cabecalho.status, sizeof(char),1,arquivo);
    fread(&cabecalho.topo, sizeof(int),1,arquivo);
    fread(&cabecalho.proxRRN, sizeof(int),1,arquivo);
    fread(&cabecalho.nroRegRem, sizeof(int),1,arquivo);
    fread(&cabecalho.nroPagDisco, sizeof(int),1,arquivo);
    fread(&cabecalho.qttCompacta, sizeof(int),1,arquivo);

    fseek(arquivo, 1600, SEEK_SET);

    return cabecalho;
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

// ========================================================================
// ====================== FUNCOES DE IMPRESSAO ============================
// ========================================================================

void MostrarGrafo(std::set<Vertice> vetorVertices)
{
    std::set<Vertice>::iterator it;
    
    for(it = vetorVertices.begin(); it!=vetorVertices.end(); it++)
    {
        Vertice ans = *it;
        if(ans.Nome() != "")
            ans.MostrarVertice();
    }
}

// ========================================================================
// ====================== FUNCOES DE VERIFICACAO ==========================
// ========================================================================

bool VerticePreto(std::set<Vertice> pretos, Vertice v)  { return pretos.find(v) != pretos.end(); }
bool VerticeCinza(std::list<Vertice> cinzas, Vertice v) { return std::find(cinzas.begin(), cinzas.end(), v) != cinzas.end(); }
bool VerticeBranco(std::list<Vertice> cinzas, std::set<Vertice> pretos, Vertice v) { return ((!VerticeCinza(cinzas, v)) && (!VerticePreto(pretos, v))); }

Vertice distMin(std::map<Vertice, int> dists, std::list<Vertice> visitados)
{
    int min = __INT_MAX__;
    Vertice minVertice("");
    Vertice vTemp("");
    std::map<Vertice, int>::iterator it;

    for (it = dists.begin(); it != dists.end(); it++) 
    {
        vTemp = (*it).first;
        if (min > dists[vTemp] && (!VerticeCinza(visitados, vTemp))) 
        {
            min = dists[vTemp];
            minVertice = vTemp;
        }
    }

    return minVertice;
}


// ========================================================================
// ======================== FUNCOES DE BUSCA ==============================
// ========================================================================

/* Dado um vertice v faz uma busca em profundidade por ele calculando os componentes e retorna esse valor (recursivo).*/
int DFS(Vertice v, std::map<std::string, std::string> &low, std::list<Vertice> &pilha, std::set<Vertice> &visitados, std::set<Vertice> vetorVertices, int &componentes)
{
    Vertice vTemp("");
    std::set<Vertice>::iterator itV;
    std::set<Presa> adjacencias = v.Adjacencias();

    low[v.Nome()] = v.Nome();                                                   // Coloca-se no id nome o seu valor inicialmente
    pilha.push_front(v);                                                        // Adiciona-se vertice na pilha
    visitados.insert(v);                                                        // Adiciona-se vertice nos visitados

    for(Presa presa :  adjacencias)                                             // Para todas suas adjacencias
    {
        itV = vetorVertices.find(presa.Nome());                                 // Procura-se a presa

        if(itV != vetorVertices.end())                                          // Se a busca encontrar algo
            vTemp = *itV;                                                       // Vertice temporario assume seu valor
        else                                                                    // Caso nao, pode ser final das adjacencias ou o vertice fantasma
        {
            Vertice vPresa(presa.Nome());                                       // Cria-se um vertice fantasma
            if(presa.Nome() != "" && !VerticePreto(visitados, vPresa))          // Verifica-se se nao eh fim da lista e se ja nao foi visitado
            {
                visitados.insert(vPresa);                                       // Insere-se essa presa aos visitados
                componentes++;                                                  // Soma-se mais um componente
            }
            continue;
        }

        if(visitados.find(vTemp) == visitados.end())                            // Se vertice nao foi visitado
            DFS(vTemp, low, pilha, visitados, vetorVertices, componentes);      // Faz-se outra DFS
        if(VerticeCinza(pilha, vTemp))                                          // Se vertice adjacente esta sendo analisado
            low[v.Nome()] = min(low[v.Nome()], low[vTemp.Nome()]);              // Calculam-se um possivel novo low
    }

    if(v.Nome() == low[v.Nome()])                                               // Caso esteja-se no topo da subarvore
    {
        for(Vertice vI = pilha.front(); ; vI = pilha.front())                   // Desempilha-se todo o caminho ate esse topo
        {
            pilha.remove(vI);                                                   // Remove da pilha
            if(vI.Nome() == v.Nome()) break;                                    // Se chega-se ate o topo, para
        }
        componentes++;                                                          // Soma-se um componente
    }

    return componentes;                                                         // Retorna-se o total de componentes
}