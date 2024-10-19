#include "funcoesArvoreB.h"
#include "funcoesFornecidas.h"
#include "funcoesAuxiliares.h"
//#include "funcoes.h"

///////////////////////////////////////////////////////////////// CRIAR ARVORE (7)

int CriarArvore(char *nomeArq, char *nomeArqArvore)
{

}

///////////////////////////////////////////////////////////////// BUSCAR REGISTRO (8)

int BuscarRegistroArvore(char *nomeArq, char *nomeArqArvore)
{
    char *campo, *chave;
    NoArvBin no;
    NoPos resultado;
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
    
    resultado = BuscarNoArvore(nomeArqArvore, chave);      // Busca na arvore -> retorna com no e posicao

    if (resultado.pos == -2 || resultado.pos == -1)           // Não foi encontrado ou encontrou algum erro
        return -1;

    arq = fopen(nomeArq, "rb");                         // Abre arquivo de registros
    if(ChecarIntegridadeArquivo(arq, nomeArq) < 0) return -1;

    fseek(arq, resultado.no.PR[resultado.pos], SEEK_SET);     // Vai para a posicao da chave
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
    int n, i, retorno;
    int rrn;
    NoPos resultado;
    char *chave;
    CabecalhoArvBin cabecalho;

    //cabecalho = CriarCabecalhoArvBin();

    cabecalho = LerCabecalhoArvore(nomeArqArvore);
    if(cabecalho.status != 1) return -1;

    if(AlterarCabecalho(nomeArqArvore, '0', cabecalho.noRaiz, cabecalho.RRNproxNo) == -1) return -1;

    scanf("%d", &n);

    for(i=0; i<n; i++)
    {
        RegDados registro = lerDadosDoTeclado();

        //chave = (char *)calloc(40, sizeof(char));
        //scan_quote_string(chave);

        rrn = InserirRegistrosAdap(nomeArq, registro);
        resultado = BuscarNoArvore(nomeArqArvore, registro.nome);

        if(resultado.pos != -1) continue;                      // Encontrou já na árvore

        if(ChecarArvoreVazia(cabecalho, 0) == -1)
            retorno = InsereArvoreVazia(nomeArqArvore, registro.nome, rrn);
        else
        { 
            if(resultado.no.nroChavesNo == tamCPR)                             // Ocorre overflow do nó
                retorno = InserirNoComOverflow(nomeArqArvore, resultado, registro.nome, rrn);
            else                                                            // Há espaço no nó
                retorno = InserirNoSemOverflow(nomeArqArvore, resultado, registro.nome, rrn);
        }

        //free(chave);
    }

    if (AlterarCabecalho(nomeArqArvore, '1', 0, 1) == -1) return -1;
    return retorno;
}

