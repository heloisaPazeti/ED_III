#include "funcoesArvoreB.h"
#include "funcoesFornecidas.h"
#include "funcoesAuxiliares.h"

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

int AdicionarRegistroARvore(char *nomeArq, char *nomeArqArvore)
{
    // Lembrar de chamar a função 5
}

///////////////////////////////////////////////////////////////// OUTROS

