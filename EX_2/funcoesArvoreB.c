#include "funcoesArvoreB.h"
#include "funcoesFornecidas.h"
#include "funcoesAuxiliares.h"
#include "funcoes.h"

///////////////////////////////////////////////////////////////// CRIAR ARVORE (7)

int CriarArvore(char *nomeArq, char *nomeArqArvore)
{

}

///////////////////////////////////////////////////////////////// BUSCAR REGISTRO (8)

int BuscarRegistroArvore(char *nomeArq, char *nomeArqArvore)
{
    char *campo, *chave;
    NoArvBin no;
    NoPos result;
    RegDados reg;
    FILE *arq;


    campo = calloc(10, sizeof(char));
    chave = calloc(160, sizeof(char));

    scanf("%s", campo);                                 // Escaneia campo da chave
    scan_quote_string(chave);                           // Escaneia chave de procura

    if(strncmp(campo, "nome", 4) != 0)
    {
        printf("A arvore esta indexada de acordo com o campo nome. Nao com: %s\n", campo);
        return -1;
    }
    
    result = BuscarNoArvore(nomeArqArvore, chave);      // Busca na arvore -> retorna com no e posicao

    if (result.pos == -2 || result.pos == -1)           // Não foi encontrado ou encontrou algum erro
        return -1;

    arq = fopen(nomeArq, "rb");                         // Abre arquivo de registros
    if(ChecarIntegridadeArquivo(arq, nomeArq) < 0) return -1;

    fseek(arq, result.no.PR[result.pos], SEEK_SET);     // Vai para a posicao da chave
    reg = lerRegistro(arq, nomeArq);                             // Lê o registro
    imprimirRegistro(reg);                              // Imprime o registro

    fclose(arq); 
    return 0;
}

///////////////////////////////////////////////////////////////// ADICIONAR REGISTRO (9)

// Situações de Inserção
    // Arvore Vazia
    // Inserir em folha
    // Overflow no raiz
int AdicionarRegistroArvore(char *nomeArq, char *nomeArqArvore)
{
    int n;
    int i;
    int rrn;
    NoPos result;
    char *chave;
    CabecalhoArvBin cabecalho;

    cabecalho = CriarCabecalhoArvBin();

    scanf("%d", &n);

    for(i=0; i<n; i++)
    {
        chave = (char *)calloc(40, sizeof(char));

        scan_quote_string(chave);

        rrn = InserirRegistrosAdap(nomeArq);
        result = BuscarNoArvore(nomeArqArvore, chave);

        if(result.pos != -1) continue;                      // Encontrou já na árvore

        if(ChecarArvoreVazia(cabecalho, 0) == -1)
            InsereArvoreVazia(nomeArqArvore, chave, rrn);
        else
        { 
            if(result.no.nroChavesNo == tamCPR)                             // Ocorre overflow do nó
                InserirNoComOverflow(nomeArqArvore, result, chave, rrn);
            else                                                            // Há espaço no nó
                InserirNoSemOverflow(nomeArqArvore, result, chave, rrn);
        }

        free(chave);
    }
    return 0;
}

